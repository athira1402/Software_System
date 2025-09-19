/*
=======================================================
Name : 23.c
Author : Athira Lonappan
Description : Write a program to create a Zombie state of the running program.
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
                printf("\nChild process PID %d exiting...",getpid());
		sleep(5);
		exit(0);
        }
        else{
                printf("\nParent process PID %d sleeping....",getpid());
		sleep(20);
		printf("\nParent's sleep time over.");
        }
return 0;
}


/*
============================================================
OUTPUT:

Child process PID 1277 exiting...Parent process PID 1276 sleeping....
Parent's sleep time over.

ps -eo pid,ppid,state,cmd | grep '[zZ]'
   1277    1276 Z [a.out] <defunct>
   1291    1108 S grep --color=auto [zZ]
============================================================
*/
