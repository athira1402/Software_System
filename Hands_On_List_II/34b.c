/*=======================================================
Name : 34b.c
Author : Athira Lonappan
Description : Write a program to create a concurrent server.
b. use pthread_create
Date : 21st September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

void* handle_client(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);

    char buffer[1024];
    int n = read(client_fd, buffer, sizeof(buffer)-1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Client says: %s\n", buffer);
        write(client_fd, "Hello from pthread server\n", 27);
    }
    close(client_fd);
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        int* client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, NULL, NULL);

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_fd);
        pthread_detach(tid);
    }

    return 0;
}




/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./34b.exe
Server listening on port 8080...
Client says: Hello!

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ telnet 127.0.0.1 8080
Trying 127.0.0.1...
Connected to 127.0.0.1.
Escape character is '^]'.
Hello!
Hello from pthread server
Connection closed by foreign host.
=========================================================
*/
