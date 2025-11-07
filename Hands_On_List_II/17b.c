/*=======================================================
Name : 17b.c
Author : Athira Lonappan
Description : Write a program to execute ls -l | wc.
b. use dup2
Date : 15th September,2025
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
        dup2(fd[1], STDOUT_FILENO); 
        execlp("ls", "ls", "-l", NULL);
    }

    if (fork() == 0) { 
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO); 
        execlp("wc", "wc", NULL);
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

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./17b.exe
     57     506    2920
=========================================================
*/
