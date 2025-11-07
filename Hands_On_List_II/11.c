/*=======================================================
Name : 11.c
Author : Athira Lonappan
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - use sigaction system call.
Date : 15th September,2025
=========================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    struct sigaction sa;

    sa.sa_handler = SIG_IGN;     
    sa.sa_flags = 0;             
    sigemptyset(&sa.sa_mask);    
    sigaction(SIGINT, &sa, NULL);

    printf("SIGINT is being ignored for 5 seconds (sigaction).\n");
    sleep(5);

    sa.sa_handler = SIG_DFL;     
    sigaction(SIGINT, &sa, NULL);

    printf("SIGINT reset to default (sigaction).\n");

    while (1) {
        sleep(1);
    }

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./11.exe
SIGINT is being ignored for 5 seconds (sigaction).
^CSIGINT reset to default (sigaction).
^C
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$
=========================================================
*/
