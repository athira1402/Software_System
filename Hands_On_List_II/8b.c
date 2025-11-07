/*=======================================================
Name : 8b.c
Author : Athira Lonappan
Description : Write a separate program using signal system call to catch the following signals.
b. SIGINT
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum) {
    printf("Caught SIGINT (Ctrl+C)\n");
}

int main() {
    signal(SIGINT, handler);
    while (1) {
           
    }
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./8b.exe
^CCaught SIGINT (Ctrl+C)
=========================================================
*/
