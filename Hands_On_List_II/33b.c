/*=======================================================
Name : 33b.c
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
    int sockfd;
    char buffer[100];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    char msg[] = "Hello from Client";
    write(sockfd, msg, strlen(msg));

    read(sockfd, buffer, sizeof(buffer));
    printf("Server: %s\n", buffer);

    close(sockfd);
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./33b.exe
Server: Hello from Server
=========================================================
*/
