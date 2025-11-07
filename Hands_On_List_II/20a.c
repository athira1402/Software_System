/*=======================================================
Name : 20a.c
Author : Athira Lonappan
Description : Write two programs so that both can communicate by FIFO -Use one way communication.
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
    int fd;
    char msg[] = "Hello from Writer via FIFO!\n";

    mkfifo("new_myfifo", 0666);

    fd = open("new_myfifo", O_WRONLY);

    write(fd, msg, sizeof(msg));

    close(fd);
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./20a.exe
=========================================================
*/
