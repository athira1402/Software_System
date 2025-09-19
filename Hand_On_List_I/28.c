/*
=======================================================
Name : 28.c
Author : Athira Lonappan
Description : Write a program to get maximum and minimum real time priority.
Date : 6th September,2025
=========================================================
*/

#include<stdio.h>
#include<sched.h>

int main(){
	
	printf("SCHED_RR\n");
	printf("Min priority: %d\n",sched_get_priority_min(SCHED_RR));
	printf("Max priority: %d\n",sched_get_priority_max(SCHED_RR));

	printf("SCHED_FIFO\n");
        printf("Min priority: %d\n",sched_get_priority_min(SCHED_FIFO));
        printf("Max priority: %d\n",sched_get_priority_max(SCHED_FIFO));

	return 0;
}


/*
===============================================================
OUTPUT:

SCHED_RR
Min priority: 1
Max priority: 99
SCHED_FIFO
Min priority: 1
Max priority: 99

===============================================================
*/
