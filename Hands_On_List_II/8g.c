/*=======================================================
Name : 8g.c
Author : Athira Lonappan
Description : Write a separate program using signal system call to catch the following signals.
g. SIGPROF (use setitimer system call)
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handler(int signum) {
    printf("Caught SIGPROF (setitimer) \n");
}

int main() {
    struct itimerval timer;


    signal(SIGPROF, handler);
    timer.it_value.tv_sec = 5;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_PROF, &timer, NULL);

    while (1) {
        long i=0;
        i++;
    }
    
    return 0;
}

/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./8g.exe
Caught SIGPROF (setitimer)
=========================================================
*/
