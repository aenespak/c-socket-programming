#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;        // Sunucunun kendi adresi
    struct sockaddr_in client_address; // İstemcinin adresi
    socklen_t client_addrlen = sizeof(client_address);
    char buffer[1024] = {0};

    // 1. Soket Oluştur (IPv4, TCP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Adres ve Port Ayarla
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 3. Soketi Bağla (Bind)
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // 4. Dinlemeye Başla (Listen)
    listen(server_fd, 3);
    printf("Sunucu dinliyor (Port: %d)...\n", PORT);

    // 5. Bağlantıyı Kabul Et (Accept)
    new_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_addrlen);

    // İstemcinin IP adresini ve Port numarasını okuma
    char *client_ip = inet_ntoa(client_address.sin_addr);
    int client_port = ntohs(client_address.sin_port);

    printf("İstemci bağlandı! IP Adresi: %s, Port: %d\n", client_ip, client_port);

    // 6. Mesaj Al ve Yanıt Ver
    read(new_socket, buffer, 1024);
    printf("İstemciden gelen mesaj: %s\n", buffer);

    send(new_socket, "Merhaba!", 8, 0);

    // 7. Kapat
    close(new_socket);
    close(server_fd);

    return 0;
}
