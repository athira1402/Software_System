/*
========================================================
Name : 1b_systemcall.c
Author : Athira Lonappan
Description : Create the following types of a files using system call
b. hard link (link system call)
Date : 10th August,2025
=========================================================
*/

#include <unistd.h>   
#include <fcntl.h>    
#include <stdio.h>    
#include <string.h>   
#include <stdlib.h>   

int main() {
    
    const char *target = "target.txt";
    const char *linkname = "hard_link.txt";
    int fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    const char *msg = "Target for hard link\n";
    write(fd, msg, strlen(msg));
    close(fd);
    link(target, linkname);
    printf("Hard link created: %s -> %s\n", linkname, target);  
    printf("\nListing with inode numbers:\n");
    system("ls -li target.txt hard_link.txt");
    return 0;
}

/*
=====================================================
OUTPUT:

Hard link created: hard_link.txt -> target.txt

Listing with inode numbers:
10861 -rw-r--r-- 2 athira athira 21 Sep  6 03:14 hard_link.txt
10861 -rw-r--r-- 2 athira athira 21 Sep  6 03:14 target.txt
=====================================================
*/
