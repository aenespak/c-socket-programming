#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(){
    int sockfd;
    struct sockaddr_in server_address;
    char *msg = "Hello from secure pc.";
    char buffer[1024] = {0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0 ){
        printf("socket creation failed\n");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0){
        printf("invalid ip address");
        return -1;
    }

    if(connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        printf("connection failed");
        return -1;
    }

    send(sockfd, msg, strlen(msg), 0);
    printf("message sent\n");

    read(sockfd, buffer, 1024);
    printf("message received from server : %s \n", buffer);

    close(sockfd);

    return 0;


}