/*=======================================================
Name : 23.c
Author : Athira Lonappan
Description : Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer).
Date : 20th September,2025
=========================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {

    long open_max;
    struct rlimit rl;

    open_max = sysconf(_SC_OPEN_MAX);
    printf("Maximum number of open files (sysconf): %ld\n", open_max);

    if (getrlimit(RLIMIT_NOFILE, &rl) == 0) {
        printf("Maximum number of open files (getrlimit): %ld\n", (long)rl.rlim_cur);
    }

    int fd[2];
    pipe(fd);

    long pipe_size = fpathconf(fd[0], _PC_PIPE_BUF);
    printf("Size of pipe (circular buffer): %ld bytes\n", pipe_size);

    close(fd[0]);
    close(fd[1]);

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./23.exe
Maximum number of open files (sysconf): 10240
Maximum number of open files (getrlimit): 10240
Size of pipe (circular buffer): 4096 bytes
=========================================================
*/
