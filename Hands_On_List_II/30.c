/*=======================================================
Name : 30.c
Author : Athira Lonappan
Description : Write a program to create a shared memory.
a. write some data to the shared memory
b. attach with O_RDONLY and check whether you are able to overwrite.
c. detach the shared memory
d. remove the shared memory
Date : 21st September,2025
=========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

int main() {
    key_t shm_key = ftok(".", 'S');
    int shm_id;

    shm_id = shmget(shm_key, 1024, 0666 | IPC_CREAT);
    printf("Shared memory created with ID: %d\n", shm_id);

    char *rw_ptr = (char*) shmat(shm_id, NULL, 0);
    if (rw_ptr == (char*) -1) {
        perror("shmat");
        exit(1);
    }

    strcpy(rw_ptr, "Hello Shared Memory!");
    printf("Data written: %s\n", rw_ptr);

    char *ro_ptr = (char*) shmat(shm_id, NULL, SHM_RDONLY);
    if (ro_ptr == (char*) -1) {
        perror("shmat readonly");
        exit(1);
    }

    printf("Read-only attachment: %s\n", ro_ptr);
    ro_ptr[0] = 'h';

    shmdt(rw_ptr);
    shmdt(ro_ptr);
    printf("Sharmed memory detatched.\n");

    shmctl(shm_id, IPC_RMID, NULL);
    printf("Shared memory removed.\n");

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./30.exe
Shared memory created with ID: 1
Data written: Hello Shared Memory!
Read-only attachment: Hello Shared Memory!
Segmentation fault (core dumped)

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./30.exe
Shared memory created with ID: 0
Data written: Hello Shared Memory!
Read-only attachment: Hello Shared Memory!
Sharmed memory detatched.
Shared memory removed.
=========================================================
*/
