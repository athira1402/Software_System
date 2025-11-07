/*=======================================================
Name : 13a.c
Author : Athira Lonappan
Description : Write two programs: 
first program is waiting to catch SIGSTOP signal, 
the second program will send the signal (using kill system call). 
Find out whether the first program is able to catch the signal or not.
Date : 15th September,2025
=========================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum) {
    printf("Caught signal %d\n", signum);
}

int main() {

    signal(SIGSTOP, handler);

    printf("Process PID: %d\n", getpid());
    printf("Waiting for SIGSTOP...\n");

    while (1) {
        pause();   
    }
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./13a.exe
Process PID: 9537
Waiting for SIGSTOP...

[17]+  Stopped                 ./13a.exe
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$
=========================================================
*/
