/*=======================================================
Name : 9.c
Author : Athira Lonappan
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - Use signal system call.
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {

    signal(SIGINT, SIG_IGN);
    printf("SIGINT is ignored on pressing Ctrl+C.\n");
    sleep(5);

    signal(SIGINT, SIG_DFL);
    printf("SIGINT reset to default. Press Ctrl+C now to terminate.\n");

    while (1) {
        
    }

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./9.exe
SIGINT is ignored on pressing Ctrl+C.
^CSIGINT reset to default. Press Ctrl+C now to terminate.
^C
=========================================================
*/
