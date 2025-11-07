/*=======================================================
Name : 28.c
Author : Athira Lonappan
Description : Write a program to change the exiting message queue permission. (use msqid_ds structure)
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
    struct msqid_ds info;

    key = ftok(".", 'A');
    msgid = msgget(key, 0666);

    if (msgctl(msgid, IPC_STAT, &info) == -1) {
        perror("msgctl IPC_STAT");
        return 1;
    }

    printf("Old permission: %o\n", info.msg_perm.mode & 0777);

    info.msg_perm.mode = 0644;

    if (msgctl(msgid, IPC_SET, &info) == -1) {
        perror("msgctl IPC_SET");
        return 1;
    }

    printf("New permission: %o\n", info.msg_perm.mode & 0777);

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./28.exe
Old permission: 666
New permission: 644
=========================================================
*/
