/*=======================================================
Name : 29.c
Author : Athira Lonappan
Description : Write a program to remove the message queue.
Date : 21st September,2025
=========================================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

int main() {
    key_t key;
    int msgid;

    key = ftok(".", 'A');
    msgid = msgget(key, 0666);

    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl IPC_RMID");
        return 1;
    }

    printf("Message queue removed successfully.\n");
    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./29.exe
Message queue removed successfully.
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages
=========================================================
*/
