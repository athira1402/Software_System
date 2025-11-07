/*=======================================================
Name : 22.c
Author : Athira Lonappan
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO.
Date : 20th September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    char buf[100];
    int fd;

    mkfifo("myfifo", 0666);

    if (fork() == 0) {

        fd = open("myfifo", O_WRONLY);
        char msg[] = "Hello Athira!\n";
        write(fd, msg, strlen(msg));
        close(fd);
        exit(0);
    } else {

        fd = open("myfifo", O_RDONLY | O_NONBLOCK);

        printf("Waiting for data (10 sec)...\n");

        fd_set readfds;
        struct timeval tv;
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        tv.tv_sec = 10;
        tv.tv_usec = 0;

        int ret = select(fd + 1, &readfds, NULL, NULL, &tv);

        if (ret == -1) {
            perror("select");
        } else if (ret == 0) {
            printf("No data written within 10 seconds.\n");
        } else {
            if (FD_ISSET(fd, &readfds)) {
                int n = read(fd, buf, sizeof(buf) - 1);
                buf[n] = '\0';
                printf("Output: %s\n", buf);
            }
        }

        close(fd);
        wait(NULL);  
    }

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./22.exe
Waiting for data (10 sec)...
Output: Hello Athira!
=========================================================
*/
