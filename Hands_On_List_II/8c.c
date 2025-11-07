/*=======================================================
Name : 8c.c
Author : Athira Lonappan
Description : Write a separate program using signal system call to catch the following signals.
c. SIGFPE
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <signal.h>

void handler(int signum) {
    printf("Caught SIGFPE (Floating Point Exception)\n");
}

int main() {
    signal(SIGFPE, handler);
    int x = 1;
    int y = 0;
    int z = x / y;
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./8c.exe
Caught SIGFPE (Floating Point Exception)
Caught SIGFPE (Floating Point Exception)
Caught SIGFPE (Floating Point Exception)
=========================================================
*/
