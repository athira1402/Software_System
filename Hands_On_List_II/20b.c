/*=======================================================
Name : 20a.c
Author : Athira Lonappan
Description : Write two programs so that both can communicate by FIFO -Use one way communication.
a.reader.c
Date : 18h September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    int fd;
    char msg[100];
    fd = open("new_myfifo", O_RDONLY);

    read(fd, msg, sizeof(msg));
    
    printf("%s",msg);

    close(fd);
    return 0;
}

/*
==========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./20b.exe
Hello from Writer via FIFO!
============================================================
*/
