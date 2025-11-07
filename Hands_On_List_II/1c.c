/*
=======================================================
Name : 1c.c
Author : Athira Lonappan
Description : Write a separate program (for each time domain) to set a interval timer in 10sec and 10micro second
a. ITIMER_PROF
Date : 9th September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handler(int signum) {
    printf("ITIMER_PROF expired........\n");
}

int main() {
    struct itimerval timer;


    signal(SIGPROF, handler);
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    setitimer(ITIMER_PROF, &timer, NULL);

    while (1) {
	long i=0;
	i++;
    }

    return 0;
}


/*
===============================================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./1c.exe
ITIMER_PROF expired........
ITIMER_PROF expired........
ITIMER_PROF expired........
ITIMER_PROF expired........
===============================================================================
*/
