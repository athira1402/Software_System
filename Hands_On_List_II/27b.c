/*=======================================================
Name : 27b.c
Author : Athira Lonappan
Description : Write a program to receive messages from the message queue.
b. with IPC_NOWAIT as a flag
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

    if (msgrcv(msgid, &message, sizeof(message.mtext), 0, IPC_NOWAIT) == -1) {
        if (errno == ENOMSG)
            printf("No messages available.\n");
        else {
            perror("msgrcv");
            exit(1);
        }
    } else {
        printf("Received msg with IPC_NOWAIT flag : %s\n", message.mtext);
    }

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./27b.exe
Received msg with IPC_NOWAIT flag : Hello Athira
=========================================================
*/
