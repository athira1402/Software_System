/*=======================================================
Name : 15.c
Author : Athira Lonappan
Description : Write a simple program to send some data from parent to the child process.
Write a simple program to send some data from parent to the child process.
Date : 15th September,2025
=========================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];            
    char write_txt[] = "Parent wrote to Child succesfully!";
    char read_txt[100];

    pipe(fd);  

    pid_t pid = fork();     

    if (pid > 0) {  
        close(fd[0]);     
        write(fd[1], write_txt, strlen(write_txt)+1);  
        close(fd[1]);     
    }
    else if (pid == 0) {  
        close(fd[1]);     
        read(fd[0], read_txt, sizeof(read_txt));    
        printf("Child received: %s\n", read_txt);
        close(fd[0]);     
    }

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./15.exe
Child received: Parent wrote to Child succesfully!
=========================================================
*/
