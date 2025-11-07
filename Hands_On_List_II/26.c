/*=======================================================
Name : 26.c
Author : Athira Lonappan
Description : Write a program to send messages to the message queue. Check $ipcs -q
Date : 21st September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
    long mtype;
    char mtext[100];
};

int main() {
    key_t key;
    int msgid;
    struct msgbuf message;

    key = ftok(".", 'A');
    msgid = msgget(key, 0666 | IPC_CREAT);

    message.mtype = 1;
    printf("Enter message to send: ");
    fgets(message.mtext, sizeof(message.mtext),stdin);

    if (msgsnd(msgid, &message, strlen(message.mtext)+1, 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    printf("Message sent successfully!\n");

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./26.exe
Enter message to send: Hello Athira
Message sent successfully!
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages
0x41302cdb 0          athira     666        14           1
=========================================================
*/
