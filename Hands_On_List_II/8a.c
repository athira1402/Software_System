/*=======================================================
Name : 8a.c
Author : Athira Lonappan
Description : Write a separate program using signal system call to catch the following signals.
a. SIGSEGV
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <signal.h>

void handler(int signum) {
    printf("Caught SIGSEGV (Segmentation Fault)\n");
}

int main() {
    signal(SIGSEGV, handler);
    int *p = NULL;   
    *p = 10;         
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./8a.exe
Caught SIGSEGV (Segmentation Fault)
Caught SIGSEGV (Segmentation Fault)
Caught SIGSEGV (Segmentation Fault)
=========================================================
*/
