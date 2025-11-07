/*=======================================================
Name : 10a.c
Author : Athira Lonappan
Description : Write a separate program using sigaction system call to catch the following signals.
a. SIGSEGV
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handler(int signum) {
    printf("Caught SIGSEGV (Segmentation Fault) using sigaction. \n");
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGSEGV, &sa, NULL);

    int *p = NULL;
    *p = 10;  
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./10a.exe
Caught SIGSEGV (Segmentation Fault) using sigaction.
Caught SIGSEGV (Segmentation Fault) using sigaction.
Caught SIGSEGV (Segmentation Fault) using sigaction.
Caught SIGSEGV (Segmentation Fault) using sigaction.
=========================================================
*/
