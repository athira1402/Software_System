/*=======================================================
Name : 18.c
Author : Athira Lonappan
Description : Write a program to find out total number of directories on the pwd.
execute ls -l | grep ^d | wc ? Use only dup2.
Date : 18th September,2025
=========================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int pipe1[2], pipe2[2];
    pipe(pipe1);
    pipe(pipe2);

    if (fork() == 0) {
        close(pipe1[0]);
        close(pipe2[0]);
	close(pipe2[1]);
        dup2(pipe1[1], STDOUT_FILENO);
	execlp("ls", "ls", "-l", NULL);
    }

    if (fork() == 0) {
	close(pipe1[1]);
        close(pipe2[0]);
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);
	execlp("grep", "grep", "^-r", NULL);
    }

    if (fork() == 0) {
        close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[1]);
        dup2(pipe2[0], STDIN_FILENO);
	execlp("wc", "wc", NULL);
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./18.exe
     63     567    3279
=========================================================
*/
