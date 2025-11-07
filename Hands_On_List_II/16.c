/*=======================================================
Name : 16.c
Author : Athira Lonappan
Description : Write a program to send and receive data from parent to child vice versa. Use two way communication.
Date : 18th September,2025
=========================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd1[2], fd2[2];  
    char parent_msg[] = "Parent wrote to Child successfully!";
    char child_msg[] = "Child wrote to Parent succesfully!";
    char buf[100];

    pipe(fd1);
    pipe(fd2);

    pid_t pid = fork();

    if (pid > 0) {  
        close(fd1[0]); 
        close(fd2[1]); 

        write(fd1[1], parent_msg, strlen(parent_msg)+1);

        read(fd2[0], buf, sizeof(buf));
        printf("%s\n", buf);

        close(fd1[1]);
        close(fd2[0]);
    }
    else if (pid == 0) {  
        close(fd1[1]); 
        close(fd2[0]); 

        read(fd1[0], buf, sizeof(buf));
        printf("%s\n", buf);

        // Send message to parent
        write(fd2[1], child_msg, strlen(child_msg)+1);

        close(fd1[0]);
        close(fd2[1]);
    }

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./16.exe
Parent wrote to Child successfully!
Child wrote to Parent succesfully!
=========================================================
*/
