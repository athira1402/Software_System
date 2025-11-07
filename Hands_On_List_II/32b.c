/*=======================================================
Name : 32b.c
Author : Athira Lonappan
Description : Write a program to implement semaphore to protect any critical section.
b.protect shared memory from concurrent write access
Date : 23rd September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

int main() {
    key_t key_sem = ftok(".", 'A');
    key_t key_shm = ftok(".", 'B');

    int shmid = shmget(key_shm, sizeof(int), IPC_CREAT | 0660);
    int *ticket = (int*) shmat(shmid, NULL, 0);

    int semid = semget(key_sem, 1, IPC_CREAT | 0660);

    semctl(semid, 0, SETVAL, 1);

    struct sembuf lock = {0, -1, 0};     // P()
    struct sembuf unlock = {0, 1, 0};    // V()

    semop(semid, &lock, 1);  // LOCK

    int fd = open("ticket.txt", O_RDONLY);
    char buf[100];
    int n = read(fd, buf, sizeof(buf) - 1);
    buf[n] = '\0';

    int current = atoi(buf);
    current++;
    *ticket = current;

    printf("Updated ticket number: %d\n", current);

    sprintf(buf, "%d", current);
    write(fd, buf, strlen(buf));
    close(fd);

    semop(semid, &unlock, 1); // UNLOCK

    shmdt(ticket);
    return 0;
}

/*
===========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./32b.exe
Updated ticket number: 264
===========================================================
*/

/*

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/ticket_shm"

int main() {
    sem_t *sem = sem_open("/shm_sem", O_CREAT, 0660, 1);
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0660);
    ftruncate(shm_fd, sizeof(int));
    int *ticket = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_wait(sem);

    int fd = open("ticket.txt", O_RDONLY);
    char buf[100];
    int n = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    buf[n] = '\0';
    *ticket = atoi(buf);

    (*ticket)++;
    printf("Updated ticket number: %d\n", *ticket);

    fd = open("ticket.txt", O_WRONLY | O_TRUNC | O_CREAT, 0660);
    sprintf(buf, "%d", *ticket);
    write(fd, buf, strlen(buf));
    close(fd);

    sem_post(sem);
    munmap(ticket, sizeof(int));
    close(shm_fd);
    sem_close(sem);
    return 0;
}
*/
