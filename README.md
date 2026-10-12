# C Socket Programming: TCP & UDP Implementations

Bu repo, C dilinde düşük seviyeli (low-level) soket programlama ile geliştirilmiş iki farklı ağ protokolü çalışmasını içerir.
1. **UDP İHA Telemetri Akışı:** Havacılık ve savunma sanayii standartlarında, bellek dolgusuz (`#pragma pack(1)`) binary telemetri paketi yayını.
2. **TCP Çift Yönlü Mesajlaşma:** `fork()` ile çoklu süreç (multi-process) mimarisi kullanılarak geliştirilmiş eşzamanlı mesajlaşma uygulaması.

---

## 📁 Proje Yapısı

```text
socket-programming/
├── tcp-protocol/
│   ├── server.c      # Çift yönlü TCP sunucusu
│   └── client.c      # Çift yönlü TCP istemcisi
└── udp-protocol/
    ├── telemetry.h   # 16-byte paket şablonu
    ├── server.c      # Yer Kontrol İstasyonu
    └── client.c      # İHA uçuş bilgisayarı simülatörü
```

---

## 1. UDP Telemetri Protokolü

Havacılık sistemlerinde telemetri verilerinin düşük gecikmeyle aktarımı için UDP tercih edilir.

* **Sözleşme (`telemetry.h`):**
  * `<stdint.h>` ile platformdan bağımsız sabit boyutlu türler (`uint16_t`, `int16_t`, `uint8_t`).
  * `#pragma pack(push, 1)` ile derleyici padding kapatılarak net **16 bayt** paket boyutu.
  * Uçuş verileri: Paket ID, irtifa, hız, pitch, roll, batarya yüzdesi ve sistem durumu bayrağı.
* **Mimari:**
  * İstemci (`client.c`): `connect()` gerekmeksizin `sendto()` ile periyodik telemetri fırlatır.
  * Sunucu (`server.c`): Tek bir UDP soketi üzerinden `recvfrom()` ile gelen paketleri ve gönderen IP/port bilgisini yakalar.


## 2. TCP Çift Yönlü Mesajlaşma

Bağlantı tabanlı TCP soket mimarisi üzerine kuruludur.

* **Multi-Process Mimari:**
  * `fork()` ile süreç ikiye ayrılır:
    * **Çocuk Süreç:** Soketten gelen verileri `read()` ile dinler.
    * **Ana Süreç:** Kullanıcı girdisini `send()` ile iletir.


## Teknolojiler ve Standartlar
* **Dil:** C 
* **Ağ:** IPv4, TCP , UDP 
* **Sistem:** Linux

---

## Karşılaşılan Problemler 


### 1. Bulut ve Güvenlik Duvarı Engelleri
* **Problem:** İstemci buluttaki VPS'e veri gönderdiğinde `TIMEOUT` hatası aldı, paketler ilk başta sunucuya hiç ulaşmadı.
* **Sebep:** VPS'in güvenlik duvarı, varsayılan port dışındaki tüm portları dış dünyaya kilitlediği için TCP ve UDP için ayrı ayrı güvenlik duvarı kuralları tanımlanması gerekiyordu.
* **Çözüm:** VPS üzerinden TCP ve UDP için ayrı ayrı güvenlik duvarı kuralları tanımlandı ve 8080 portu açıldı.

### 2. Süreç Patlaması / Fork Bomb Riski 
* **Problem:** Çift yönlü TCP mesajlaşmasında `while(1)` içine `fork()` konulduğunda ana süreç durmaksızın çocuk süreçler türetti.
* **Sebep:** `fork()` her döngüde çalıştığında işletim sisteminde binlerce çocuk süreç oluşur ve sistem kilitlenir.
* **Çözüm:** `fork()` döngüden önce **yalnızca 1 kez** çağrıldı; çocuk süreç ve ebeveyn süreç, mesaj alışverişi için birbiriyle haberleşerek bağımsız bir şekilde çalıştırıldı.

### 3. Bellek Dolgusu (Struct Padding) ve Veri Kayması
* **Problem:** Farklı işlemci mimarilerinde veya derleyicilerde `struct` boyutunun tutarsızlaşma ve telemetri verilerinin bozulma riski.
* **Sebep:** 32-bit ve 64-bit derleyiciler farklı boyutlardaki verilerin arasını gereksiz boşluklarla doldurur. Bu durum önemli verilerin bozulmasına sebep olabilir.
* **Çözüm:** `<stdint.h>` ile sabit boyutlu tipler (`uint16_t`, `uint8_t`) kullanıldı ve `#pragma pack(push, 1)` ile derleyicinin araya boşluk koyması engellenerek tam 16 baytlık bir paket sağlandı.

## Bilinen Kısıtlar

### Yerel Bilgisayarın Sunucu Olarak Kullanılamaması
* **Problem:** Kişisel bilgisayarın doğrudan bir sunucu olarak kullanılamaması ve dış ağdaki istemcilerin bu bilgisayara doğrudan bağlanamaması.
* **Sebep:** Bilgisayarın yerel ağda özel bir IP alması, modem üzerinde port yönlendirme yapılamaması ve kurumsal ağlardaki güvenlik duvarı engelleri.
