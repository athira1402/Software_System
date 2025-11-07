/*=======================================================
Name : 24.c
Author : Athira Lonappan
Description : Write a program to create a message queue and print the key and message queue id.
Date : 20th September,2025
=========================================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;

    key = ftok(".", 'A');
    msgid = msgget(key, 0666 | IPC_CREAT);

    printf("Key: %d\n", key);
    printf("Message Queue ID: %d\n", msgid);

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./24.exe
Key: 1093676251
Message Queue ID: 0
=========================================================
*/
