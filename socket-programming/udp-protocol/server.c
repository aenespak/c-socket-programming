#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "telemetry.h"

#define PORT 8080

int main() {
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    TelemetriPaketi paket;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); //UDP protokolünde tek soket yeterlidir.
    if(sockfd < 0){
        perror("socket creation failed");
        return -1;
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
        perror("bind failed");
        return -1;
    }

    printf("Waiting for data..\n");

    while(1){
        ssize_t byte = recvfrom(sockfd, &paket, sizeof(paket), 0, (struct sockaddr*)&client_address, &client_address_len );
        if(byte< 0){
            perror("recvfrom failed");
            return -1;
        }
        printf("Received %zu bytes from %s:%d\n", byte, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        printf("Paket ID: %d\n", paket.paket_id);
        printf("Rakım: %.2f\n", paket.rakim);
        printf("Hız: %.2f\n", paket.hiz);
        printf("Pitch: %d\n", paket.pitch);
        printf("Roll: %d\n", paket.roll);
        printf("Batarya: %d\n", paket.batarya);
        printf("Sistem Durumu: %d\n\n\n", paket.sistem_durumu);
    }


    return 0;
}
