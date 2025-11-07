/*=======================================================
Name : 5.c
Author : Athira Lonappan
Description : Write a program to print the system limitation of
a. maximum length of the arguments to the exec family of functions.
b. maximum number of simultaneous process per user id.
c. number of clock ticks (jiffy) per second.
d. maximum number of open files
e. size of a page
f. total number of pages in the physical memory
g. number of currently available pages in the physical memory.
Date : 9th September,2025
=========================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>

int main() {
    struct rlimit rl;

    long arg_max = sysconf(_SC_ARG_MAX);
    printf("Max length of arguments to exec: %ld bytes\n", arg_max);

    getrlimit(RLIMIT_NPROC, &rl);
    printf("Max number of processes per user: soft=%ld, hard=%ld\n", rl.rlim_cur, rl.rlim_max);

    long ticks = sysconf(_SC_CLK_TCK);
    printf("Clock ticks (jiffy) per second: %ld\n", ticks);

    getrlimit(RLIMIT_NOFILE, &rl);
    printf("Max number of open files: soft=%ld, hard=%ld\n", rl.rlim_cur, rl.rlim_max);

    long page_size = sysconf(_SC_PAGESIZE);
    printf("Page size: %ld bytes\n", page_size);

    long phys_pages = sysconf(_SC_PHYS_PAGES);
    long avail_pages = sysconf(_SC_AVPHYS_PAGES);
    printf("Total pages in physical memory: %ld\n", phys_pages);
    printf("Available pages in physical memory: %ld\n", avail_pages);

    return 0;
}

/*
==================================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./5.exe
Max length of arguments to exec: 2097152 bytes
Max number of processes per user: soft=31101, hard=31101
Clock ticks (jiffy) per second: 100
Max number of open files: soft=10240, hard=1048576
Page size: 4096 bytes
Total pages in physical memory: 1993010
Available pages in physical memory: 1850243
==================================================================
*/
