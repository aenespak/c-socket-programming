#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "telemetry.h"

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("socket creation failed");
        exit(-1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    int check = bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    if(check < 0){
        perror("bind failed");
        exit(-1);
    }

    if(listen(server_fd, 3) < 0){
        perror("listen failed");
        exit(-1);
    }
    printf("waiting for connection... PORT: %d\n", PORT);

    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_address_size);
    
    if(new_socket < 0 ){
        perror("accept failed");
        exit(-1);
    }

    printf("connection successful\n");

    char buffer[1024] = {0};
    ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer)-1);
    if(bytes_read > 0){
        buffer[bytes_read] = '\0';
        printf("message received from client: %s\n", buffer);
    }
    char *msg = "Your message has been received. Thank you";
    send(new_socket, msg, strlen(msg), 0);
    printf("message sent\n");
    close(new_socket);
    close(server_fd);
    return 0;


}
