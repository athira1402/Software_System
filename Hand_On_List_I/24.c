/*
=======================================================
Name : 24.c
Author : Athira Lonappan
Description : Write a program to create an orphan process.
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
                printf("\nChild process PID %d PPID %d sleeping...",getpid(),getppid());
                sleep(20);
		printf("\nChild process PID %d PPID %d sleeping...",getpid(),getppid());
                printf("\nChild's sleep over.");
		fflush(stdout);
		exit(0);
        }
        else{
                printf("\nParent process PID %d exiting....",getpid());
		exit(0);
        }
return 0;
}

/*
====================================================================
OUTPUT:

Parent process PID 1265 exiting....athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ Child process PID 1266 PPID 1265 sleeping...
Child process PID 1266 PPID 498 sleeping...
Child's sleep over.
====================================================================
*/
