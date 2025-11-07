/*=======================================================
Name : 17a.c
Author : Athira Lonappan
Description : Write a program to execute ls -l | wc.
a. use dup
Date : 18th September,2025
=========================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd); 

    if (fork() == 0) {
        close(fd[0]);

        close(1);
        dup(fd[1]);
        close(fd[1]);

        execlp("ls", "ls", "-l", NULL);
        perror("execlp"); exit(1);
    }

    if (fork() == 0) {
        close(fd[1]);

        close(0);
        dup(fd[0]);
        close(fd[0]);

        execlp("wc", "wc", NULL);
        perror("wc"); exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./17a.exe
     62     551    3185
=========================================================
*/
