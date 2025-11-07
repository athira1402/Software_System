/*=======================================================
Name : 32a.c
Author : Athira Lonappan
Description : Write a program to implement semaphore to protect any critical section.
a.rewrite the ticket number creation program using semaphore
Date : 23rd September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string.h>

int main() {
    key_t key = ftok(".", 'A');
    int semid = semget(key, 1, 0666 | IPC_CREAT);

    semctl(semid, 0, SETVAL, 1);

    struct sembuf wait_op = {0, -1, 0};    // P()
    struct sembuf post_op = {0, 1, 0};     // V()

    int fd = open("ticket.txt", O_RDWR | O_CREAT, 0660);

    semop(semid, &wait_op, 1); //lock

    char buf[100];
    lseek(fd, 0, SEEK_SET);
    int n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0) buf[n] = '\0';
    else strcpy(buf, "0");

    int num = atoi(buf);
    num += 1;

    sprintf(buf, "%d", num);
    lseek(fd, 0, SEEK_SET);
    write(fd, buf, strlen(buf));

    printf("Ticket number: %s\n", buf);

    semop(semid, &post_op, 1); //unlock

    close(fd);
    return 0;
}


/*
===========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./32a.exe
Ticket number: 263
===========================================================
*/




/*
sem_t *sem = sem_open("/ticket_sem", O_CREAT, 0660, 1);
    int fd = open("ticket.txt", O_RDWR | O_CREAT, 0660);

    sem_wait(sem);

    char buf[100];
    lseek(fd, 0, SEEK_SET);
    int n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0) buf[n] = '\0';
    int num = atoi(buf);
    num += 1;

    sprintf(buf, "%d", num);
    lseek(fd, 0, SEEK_SET);
    write(fd, buf, strlen(buf));

    printf("Ticket number: %s\n", buf);

    sem_post(sem);

    close(fd);
    sem_close(sem);
    return 0;

*/
