/*=======================================================
Name : 34a.c
Author : Athira Lonappan
Description : Write a program to create a concurrent server.
a. use fork
Date : 21st September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#define PORT 8080

void handle_client(int client_fd) {
    char buf[100];
    int n = read(client_fd, buf, sizeof(buf)-1);
    if (n > 0) {
        buf[n] = '\0';
        printf("Client says: %s\n", buf);
        write(client_fd, "Hello from server\n", 18);
    }
    sleep(10);
    close(client_fd);
    exit(0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;

    signal(SIGCHLD, SIG_IGN); 

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 2);

    printf("Server listening on port %d\n", PORT);

    while (1) {
        client_fd = accept(server_fd, NULL, NULL);
        if (fork() == 0) {
            close(server_fd);
            handle_client(client_fd);
        }
        close(client_fd);
    }

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./34a.exe
Server listening on port 8080
Client says: Hello!

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ telnet 127.0.0.1 8080
Trying 127.0.0.1...
Connected to 127.0.0.1.
Escape character is '^]'.
Hello!
Hello from server
Connection closed by foreign host.
=========================================================
*/
