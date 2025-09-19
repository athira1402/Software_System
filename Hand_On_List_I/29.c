/*
=======================================================
Name : 29.c
Author : Athira Lonappan
Description :Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO,
SCHED_RR)
Date : 6th September,2025
=========================================================
*/

#include<stdio.h>
#include<sched.h>
#include<unistd.h>

void print_policy(int policy){
	if (policy==SCHED_FIFO)  printf("SCHED_FIFO");
	else if (policy==SCHED_RR) printf("SCHED_RR");
	else if (policy==SCHED_OTHER) printf("SCHED_OTHER");
	else printf("Unknown");
}

int main(){
	int pid=getpid();
	
	int current_policy=sched_getscheduler(pid);
	if (sched_getscheduler(pid)==-1) {
		perror("sched_getscheduler failed");
		return 1;
	}
	printf("Current policy: ");
	print_policy(current_policy);
	
	int choice;
	printf("\nModify policy to \n 1. SCHED_FIFO \n 2. SCHED_RR \n Enter your choice:");
	scanf("%d",&choice);

	int new_policy;
	if (choice==1) new_policy=SCHED_FIFO;
	else if (choice==2) new_policy=SCHED_RR;
	else ("\nInvalid choice");

	struct sched_param parameter;
	parameter.sched_priority=50;

	if (sched_setscheduler(pid,new_policy,&parameter)==-1){
		perror("sched_setscheduler failed");
		return 1;	
	}

	printf("Scheduling policy changed to ");
	print_policy(new_policy);
	return 0;
}

/*
==================================================================
OUTPUT: 

Current policy: SCHED_OTHER
Modify policy to
 1. SCHED_FIFO
 2. SCHED_RR
 Enter your choice:2
Scheduling policy changed to SCHED_RR

==================================================================
*/
