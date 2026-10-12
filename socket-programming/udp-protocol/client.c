#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "telemetry.h"

#define PORT 8080
#define SERVER_IP "35.195.27.206"

int main() {
    // 1. UDP Soketi oluşturma (SOCK_DGRAM)
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("socket creation failed");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);

    TelemetriPaketi paket;
    paket.paket_id = 1;
    paket.rakim = 3500.0f;
    paket.hiz = 220.0f;
    paket.pitch = 5;
    paket.roll = 0;
    paket.batarya = 100;
    paket.sistem_durumu = 1;

    while(1){
        ssize_t byte = sendto(sockfd, &paket, sizeof(paket) , 0 , (struct sockaddr*)&server_address, sizeof(server_address));
        if(byte < 0){
            perror("sending failed");
            return -1;
        }
        printf("Paket %d gönderildi, %zu byte, Gönderilen IP: %s\n", paket.paket_id, byte, inet_ntoa(server_address.sin_addr));
        paket.paket_id++;
        paket.rakim += 10.0f;
        paket.hiz += 5.0f;
        paket.pitch += 1;
        paket.roll += 1;
        paket.batarya -= 1;
        paket.sistem_durumu = paket.batarya > 20 ? 1 : 0;
        usleep(50000);
        
    }
    

    return 0;
}
