/*=======================================================
Name : 27.c
Author : Athira Lonappan
Description : Write a program to receive messages from the message queue.
a. with 0 as a flag
Date : 21st September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>

struct msg_buffer {
    long mtype;
    char mtext[100];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;
    key = ftok(".", 'A');
    msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgrcv(msgid, &message, sizeof(message.mtext), 0, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }
    printf("Received msg with 0 flag : %s\n", message.mtext);
    return 0;
}

/*
==========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./27a.exe
Received msg with 0 flag : Hello Athira
==========================================================
*/
