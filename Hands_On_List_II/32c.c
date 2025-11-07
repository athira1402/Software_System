/*=======================================================
Name : 32c.c
Author : Athira Lonappan
Description : Write a program to implement semaphore to protect any critical section.
c.protect multiple pseudo resources ( may be two) using counting semaphore
Date : 23rd September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main() {
    key_t key = ftok("semfile", 65);
    int semid = semget(key, 1, 0660 | IPC_CREAT);

    struct sembuf acquire = {0, -1, 0};
    struct sembuf release = {0,  1, 0};

    printf("Trying to access resource...\n");

    semop(semid, &acquire, 1);
    printf("Resource acquired!\n");

    sleep(5);

    printf("Releasing resource...\n");
    semop(semid, &release, 1);

    return 0;
}


/*
===========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./32c.exe
Trying to access resource...
Resource acquired!
Releasing resource...
===========================================================
*/


/*
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    sem_t *sem = sem_open("/resource_sem", O_CREAT, 0660, 2);

    printf("Trying to access resource...\n");
    sem_wait(sem);
    printf("Resource acquired!\n");

    sleep(2); 

    printf("Releasing resource...\n");
    sem_post(sem);

    sem_close(sem);
    return 0;
}
*/
