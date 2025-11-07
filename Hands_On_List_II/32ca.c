#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    key_t key = ftok("semfile", 65);
    int semid = semget(key, 1, IPC_CREAT | 0666);

    if (semid < 0) {
        perror("semget");
        exit(1);
    }

    semctl(semid, 0, SETVAL, 2);

    printf("Semaphore created and initialized to 2.\n");
    return 0;
}
