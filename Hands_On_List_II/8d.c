/*=======================================================
Name : 8d.c
Author : Athira Lonappan
Description : Write a separate program using signal system call to catch the following signals.
d. SIGALRM (use alarm system call)
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum) {
    printf("Caught SIGALRM (alarm)\n");
}

int main() {
    signal(SIGALRM, handler);
    alarm(3);       
    pause();        
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./8d.exe
Caught SIGALRM (alarm)
=========================================================
*/
