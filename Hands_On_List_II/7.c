/*=======================================================
Name : 7.c
Author : Athira Lonappan
Description : Write a simple program to print the created thread ids.
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <pthread.h>

void* thread_function(void* arg) {
    printf("Hello from thread %d\n", *((int*)arg));
    return NULL;
}

int main() {
    pthread_t threads[3];

    int ids[3];

    for(int i = 0; i < 3; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, &ids[i]);
    }

    for(int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    printf("All threads finished.\n");
    return 0;
}

/*
=================================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./7.exe
Hello from thread 1
Hello from thread 2
Hello from thread 3
All threads finished.
=================================================================
*/
