/*
========================================================
Name : 1c_systemcall.c
Author : Athira Lonappan
Description : Create the following types of a files using system call
c. FIFO (mknod system call)
Date : 10th August,2025
=========================================================
*/

#include <sys/types.h>  
#include <sys/stat.h>   
#include <stdio.h>      
#include <stdlib.h>     
int main() {
    const char *fifo_name = "myfifo";
    mkfifo(fifo_name, 0644);
    printf("FIFO created: %s\n", fifo_name);
    printf("\nListing FIFO details:\n");
    system("ls -l myfifo");
    return 0;
}

/*
=========================================================
OUTPUT:

FIFO created: myfifo

Listing FIFO details:
prw-r--r-- 1 athira athira 0 Aug 13 14:40 myfifo
=========================================================
*/
