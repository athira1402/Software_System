/*=======================================================
Name : 8e.c
Author : Athira Lonappan
Description : Write a separate program using signal system call to catch the following signals.
e. SIGALRM (use setitimer system call)
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handler(int signum) {
    printf("Caught SIGALRM (setitimer) \n");
}

int main() {
    struct itimerval timer;


    signal(SIGALRM, handler);

    timer.it_value.tv_sec = 5;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);

    while (1) {
        pause();
    }
    return 0;
}

/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./8e.exe
Caught SIGALRM (setitimer)
=========================================================
*/
