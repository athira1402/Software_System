/*=======================================================
Name : 10c.c
Author : Athira Lonappan
Description : 10. Write a separate program using sigaction system call to catch the following signals.
a. SIGFPE
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handler(int signum) {
    printf("Caught SIGFPE (Floating Point Exception) using sigaction. \n");
    exit(1);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGFPE, &sa, NULL);

    int x = 1;
    int y = 0;
    int z = x/y;
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./10c.exe
Caught SIGFPE (Floating Point Exception) using sigaction.
=========================================================
*/
