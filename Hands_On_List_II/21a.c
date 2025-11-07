/*=======================================================
Name : 21a.c
Author : Athira Lonappan
Description : Write two programs so that both can communicate by FIFO -Use two way communications.
a.writer.c
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
    char r_msg[100];
    char w_msg[]="Hello from writer!";
    mkfifo("fifo1",0666);

    fd1 = open("fifo1", O_WRONLY);
    write(fd1, w_msg, sizeof(w_msg));

    fd2 = open("fifo2",O_RDONLY);
    read(fd2,r_msg,sizeof(r_msg));
    printf("%s",r_msg);

    close(fd1);
    close(fd2);
    return 0;
}

/*
===============================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./21a.exe
Hello from reader!
===============================================================
*/
