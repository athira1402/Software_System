/*=======================================================
Name : 17c.c
Author : Athira Lonappan
Description : Write a program to execute ls -l | wc.
c. use fcntl
Date : 18th September,2025
=========================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) { 
        close(fd[0]);
	close(1);
        fcntl(fd[1], F_DUPFD, 1); 
        execlp("ls", "ls", "-l", NULL);
    }

    if (fork() == 0) { 
        close(fd[1]);
	close(0);
        fcntl(fd[0], F_DUPFD, 0); 
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

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./17c.exe
     60     533    3079
=========================================================
*/
