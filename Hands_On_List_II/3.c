/*
=======================================================
Name : 3.c
Author : Athira Lonappan
Description : Write a program to set (any one) system resource limit. Use setrlimit system call.
Date : 9th September,2025
=========================================================
*/

#include <stdio.h>
#include <sys/resource.h>

int main() {
    struct rlimit rl;

    getrlimit(RLIMIT_NOFILE, &rl);
    printf("Before: Soft = %lu, Hard = %lu\n", rl.rlim_cur, rl.rlim_max);

    rl.rlim_cur = 512;  
    rl.rlim_max = 1024; 
    setrlimit(RLIMIT_NOFILE, &rl);

    getrlimit(RLIMIT_NOFILE, &rl);
    printf("After:  Soft = %lu, Hard = %lu\n", rl.rlim_cur, rl.rlim_max);

    return 0;
}

/*
======================================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./3.exe
Before: Soft = 10240, Hard = 1048576
After:  Soft = 512, Hard = 1024
======================================================================
*/
