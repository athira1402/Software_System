/*
=======================================================
Name : 21.c
Author : Athira Lonappan
Description : Write a program, call fork and print the parent and child process id.
Date : 2nd September,2025
=========================================================
*/

#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
	pid_t pid=fork();
	if (pid<0){
		perror("fork");
		exit(1);
	}
	else if (pid==0){
		printf("\nChild process PID: %d \nParent process PID: %d",getpid(),getppid());
	}
	else{
		printf("\nParent process PID: %d \nChild process PID: %d",getpid(),pid);
	}
return 0;
}

/*
=======================================================================
OUTPUT:

Parent process PID: 1316
Child process PID: 1317
Child process PID: 1317
Parent process PID: 1316
=======================================================================
*/
