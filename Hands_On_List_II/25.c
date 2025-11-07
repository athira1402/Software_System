/*=======================================================
Name : 25.c
Author : Athira Lonappan
Description : Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
a. access permission
b. uid, gid
c. time of last message sent and received
d. time of last change in the message queue
d. size of the queue
f. number of messages in the queue
g. maximum number of bytes allowed
h. pid of the msgsnd and msgrcv
Date : 15th September,2025
=========================================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds info;

    key = ftok(".", 'A');

    msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgctl(msgid, IPC_STAT, &info) == -1) {
        perror("msgctl");
        return 1;
    }

    printf("Access Permissions: %o\n", info.msg_perm.mode & 0777);
    printf("Owner UID: %d\n", info.msg_perm.uid);
    printf("Owner GID: %d\n", info.msg_perm.gid);

    printf("Last message sent: %s", ctime(&info.msg_stime));
    printf("Last message received: %s", ctime(&info.msg_rtime));
    printf("Last change: %s", ctime(&info.msg_ctime));

    printf("Size of queue in bytes: %ld\n", info.__msg_cbytes);
    printf("Number of messages in queue: %ld\n", info.msg_qnum);
    printf("Maximum bytes allowed in queue: %ld\n", info.msg_qbytes);

    printf("PID of last msgsnd: %d\n", info.msg_lspid);
    printf("PID of last msgrcv: %d\n", info.msg_lrpid);

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./25.exe
Access Permissions: 666
Owner UID: 1000
Owner GID: 1000
Last message sent: Thu Jan  1 00:00:00 1970
Last message received: Thu Jan  1 00:00:00 1970
Last change: Sun Sep 21 04:06:18 2025
Size of queue in bytes: 0
Number of messages in queue: 0
Maximum bytes allowed in queue: 16384
PID of last msgsnd: 0
PID of last msgrcv: 0
=========================================================
*/
