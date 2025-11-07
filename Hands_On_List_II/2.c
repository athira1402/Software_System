/*
=======================================================
Name : 2.c
Author : Athira Lonappan
Description : Write a program to print the system resource limits. Use getrlimit system call.
Date : 9th September,2025
=========================================================
*/

#include <stdio.h>
#include <sys/resource.h>

int main() {
    struct rlimit rl;

    getrlimit(RLIMIT_CPU, &rl);
    printf("CPU time: Soft = %lu, Hard = %lu seconds\n", rl.rlim_cur, rl.rlim_max);

    getrlimit(RLIMIT_FSIZE, &rl);
    printf("Max file size: Soft = %lu, Hard = %lu bytes\n", rl.rlim_cur, rl.rlim_max);

    getrlimit(RLIMIT_NOFILE, &rl);
    printf("Open files: Soft = %lu, Hard = %lu\n", rl.rlim_cur, rl.rlim_max);

    getrlimit(RLIMIT_CORE, &rl);
    printf("Core file size: Soft = %lu, Hard = %lu bytes\n", rl.rlim_cur, rl.rlim_max);

    return 0;
}

/*
======================================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./2.exe
CPU time: Soft = 18446744073709551615, Hard = 18446744073709551615 seconds
Max file size: Soft = 18446744073709551615, Hard = 18446744073709551615 bytes
Open files: Soft = 10240, Hard = 1048576
Core file size: Soft = 0, Hard = 18446744073709551615 bytes
======================================================================
*/
