/*=======================================================
Name : 6.c
Author : Athira Lonappan
Description : Write a simple program to create three threads.
Date : 12th September,2025
=========================================================
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_function(void* arg) {
    sleep(1);
    return NULL;
}

int main() {
    pthread_t threads[3];
    int ids[3];

    for(int i = 0; i < 3; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    for(int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    printf("All threads finished.\n");
    return 0;
}

/*
=================================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./6.exe
All threads finished.
=================================================================
*/
