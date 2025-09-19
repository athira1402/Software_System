/*
=======================================================
Name : 25.c
Author : Athira Lonappan
Description : Write a program to create three child processes. The parent should wait for a particular child (use
waitpid system call).
Date : 2nd September,2025
=========================================================
*/

#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
        pid_t child1=fork();
	if (child1==0){
		printf("\nChild 1 PID %d sleeping...\n",getpid());
		sleep(3);
		printf("\nChild 1 finished task.\n");
		exit(11);
	}

	pid_t child2=fork();
        if (child2==0){
                printf("\nChild 2 PID %d sleeping...\n",getpid());
                sleep(10);
                printf("\nChild 2 finished task.\n");
                exit(22);
        }

	pid_t child3=fork();
        if (child3==0){
                printf("\nChild 3 PID %d sleeping...\n",getpid());
                sleep(15);
                printf("\nChild 3 finished task.\n");
                exit(33);
        }

	printf("\nParents PID %d waiting for child 2 PID %d\n",getpid(),child2);
	
	int status;
	pid_t wait=waitpid(child2,&status,0);

	if (wait==child2 && WIFEXITED(status)){
		printf("\nChild 2 exited with code %d",WEXITSTATUS(status));
	}

	return 0;
}

/*
==============================================================
OUTPUT:

Child 1 PID 0 sleeping...

Child 2 PID 0 sleeping...

Child 3 PID 0 sleeping...

Parents PID 1074 waiting for child 2 PID 1076

Child 1 finished task.

Child 2 finished task.

Child 2 exited with code 22
==============================================================
*/
