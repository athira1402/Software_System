/*=======================================================
Name : 19e.c
Author : Athira Lonappan
Description : Create a FIFO file by d. mkfifo library function
Date : 18th September,2025
=========================================================
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    mkfifo("fifo_mkfifo", 0666);
    printf("FIFO created using mkfifo system call.\n");

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./19e.exe
FIFO created using mkfifo system call.
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ls -l | grep ^p
prw-r--r-- 1 athira athira     0 Sep 20 15:05 fifo_mkfifo
=========================================================
*/
