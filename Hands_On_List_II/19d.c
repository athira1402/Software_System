/*=======================================================
Name : 19d.c
Author : Athira Lonappan
Description : Create a FIFO file by d. mknod system call
Date : 18th September,2025
=========================================================
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    mknod("fifo_mknod", S_IFIFO | 0666, 0);
    printf("FIFO created using mknod system call.\n");

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./19d.exe
FIFO created using mknod system call.
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ls -l | grep ^p
prw-r--r-- 1 athira athira     0 Sep 20 15:03 fifo_mknod
=========================================================
*/
