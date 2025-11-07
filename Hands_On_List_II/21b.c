/*=======================================================
Name : 21b.c
Author : Athira Lonappan
Description : Write two programs so that both can communicate by FIFO -Use two way communications.
b.sender.c
Date : 18h September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    int fd1,fd2;
    char w_msg[100];
    char r_msg[]="Hello from reader!";
    mkfifo("fifo2",0666);

    fd1 = open("fifo1", O_RDONLY);
    read(fd1, w_msg, sizeof(w_msg));
    printf("%s",w_msg);

    fd2 = open("fifo2",O_WRONLY);                                                                                    read(fd2,r_msg,sizeof(r_msg));
    write(fd2, r_msg,sizeof(r_msg));

    close(fd1);
    close(fd2);
    return 0;
}

/*
============================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./21b.exe
Hello from writer!
=============================================================
*/
