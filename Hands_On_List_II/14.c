/*=======================================================
Name : 14.c
Author : Athira Lonappan
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor.
Date : 15th September,2025
=========================================================
*/


#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];                 
    char write_txt[] = "Hello, Athira!";
    char read_txt[100];

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    write(fd[1], write_txt, strlen(write_txt)+1);  
    read(fd[0], read_txt, sizeof(read_txt));

    printf("Message from pipe: %s\n", read_txt);

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./14.exe
Message from pipe: Hello, Athira!
=========================================================
*/
