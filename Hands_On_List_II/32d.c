/*=======================================================
Name : 32d.c
Author : Athira Lonappan
Description : Write a program to implement semaphore to protect any critical section.
d.remove the created semaphore
Date : 23rd September,2025
=========================================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

int main() {
    key_t key = ftok("semfile", 65);
    int semid = semget(key, 1, 0666);

    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
    } else {
        printf("Semaphore removed successfully.\n");
    }
}

/*
===========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./32d.exe
Semaphores removed.
===========================================================
*/

/*
sem_unlink("/ticket_sem");
    sem_unlink("/shm_sem");
    sem_unlink("/resource_sem");
    printf("Semaphores removed.\n");
    return 0;
*/
