//TCP Protocol Message Application

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(){
    int sockfd;
    struct sockaddr_in server_address;

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
    pid_t pid = fork();
    if(pid < 0){
        perror("fork failed");
        exit(-1);
    }
    if(pid == 0){
        char msg[1024] = {0};
        while(1){
            memset(msg, 0 , sizeof(msg));
            ssize_t bytes_read = read(sockfd, msg, sizeof(msg));
            if(bytes_read == 0){
                printf("server closed the connection\n");
                break;
            }
            msg[bytes_read] = '\0';
            msg[strcspn(msg, "\r\n")] = '\0';
            printf("Server: %s\n", msg);
            if(strncmp(msg, "exit", 4) == 0){
                break;
            }
        }
    }
    else{
        char msg[1024] = {0};
        while(1){
            memset(msg, 0, sizeof(msg));
            fgets(msg, sizeof(msg), stdin);
            send(sockfd, msg, strlen(msg), 0);
            if(strncmp(msg, "exit", 4) == 0){
                break;
            }
        }
        kill(pid, SIGTERM);
    }
    close(sockfd);

    return 0;


}