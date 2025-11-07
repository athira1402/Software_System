/*=======================================================
Name : 10b.c
Author : Athira Lonappan
Description : Write a separate program using sigaction system call to catch the following signals.
b. SIGINT
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum) {
    printf("Caught SIGINT using sigaction. \n");
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);

    while (1) {

    }
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./10b.exe
^CCaught SIGINT using sigaction.
=========================================================
*/
