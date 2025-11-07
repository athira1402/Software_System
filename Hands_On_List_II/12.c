/*=======================================================
Name : 12.c
Author : Athira Lonappan
Description : Write a program to create an orphan process. Use kill system call to send SIGKILL signal to the parent process from the child process.
Date : 15th September,2025
=========================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();   

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {  
        printf("Child PID = %d, Parent PID = %d\n", getpid(), getppid());

        kill(getppid(), SIGKILL);

        printf("Parent killed, Child now an orphan.\n");
        sleep(5);
        printf("Child's new Parent PID = %d\n", getppid());
    } 
    else {
        while (1) {
            sleep(1); 
        }
    }

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./12.exe
Child PID = 9475, Parent PID = 9474
Parent killed, Child now an orphan.
Killed
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ Child's new Parent PID = 8521
=========================================================
*/
