/*=======================================================
Name : 31.c
Author : Athira Lonappan
Description : Write a program to create a semaphore and initialize value to the semaphore.
a. create a binary semaphore
b. create a counting semaphore
Date : 21st September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main() {
    key_t key1 = ftok(".", 'A');  
    key_t key2 = ftok(".", 'B');  

    
    int bin_sem = semget(key1, 1, 0666 | IPC_CREAT);
    semctl(bin_sem, 0, SETVAL, 1);  
    printf("Binary semaphore ID: %d, initialized to 1\n", bin_sem);

    int count_sem = semget(key2, 1, 0666 | IPC_CREAT);
    semctl(count_sem, 0, SETVAL, 5); 
    printf("Counting semaphore ID: %d, initialized to 5\n", count_sem);

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./31.exe
Binary semaphore ID: 0, initialized to 1
Counting semaphore ID: 1, initialized to 5
=========================================================
*/
