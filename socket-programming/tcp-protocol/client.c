#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "telemetry.h"

#define PORT 8080

int main(){
    int sockfd;
    struct sockaddr_in server_address;

    TelemetriPaketi paket;
    paket.paket_id = 1;
    paket.rakim = 3500.5f;
    paket.hiz = 210.0f;
    paket.pitch = 5;
    paket.roll = -2;
    paket.batarya = 85;
    paket.sistem_durumu = 1;



    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0 ){
        printf("socket creation failed\n");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "35.195.27.206", &server_address.sin_addr) <= 0){
        printf("invalid ip address");
        return -1;
    }

    if(connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        printf("connection failed");
        return -1;
    }
    //buradan sonrasına devam ederse bağlanmış demektir.
    while(1){
        send(sockfd, &paket, sizeof(paket), 0);
        printf("message sent\n");
        paket.paket_id++;
        paket.rakim++;
        paket.hiz += 2.5;
        paket.batarya -= 0.5;
        paket.pitch = (paket.pitch + 2) % 180;
        paket.roll = (paket.roll + 3) % 180;
        paket.sistem_durumu = 1;
        usleep(500000);
    }

    close(sockfd);

    return 0;


}