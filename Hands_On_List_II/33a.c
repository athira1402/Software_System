/*=======================================================
Name : 33a.c
Author : Athira Lonappan
Description : Write a program to communicate between two machines using socket
Date : 21st September,2025
=========================================================
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd, clientfd;
    char buffer[100];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 1);

    clientfd = accept(sockfd, NULL, NULL);

    read(clientfd, buffer, sizeof(buffer));
    printf("Client: %s\n", buffer);

    char reply[] = "Hello from Server";
    write(clientfd, reply, strlen(reply));

    close(clientfd);
    close(sockfd);
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./33a.exe
Client: Hello from Client
=========================================================
*/
