/*
=======================================================
Name : 22.c
Author : Athira Lonappan
Description : Write a program, open a file, call fork, and then write to the file by both the child as well as the
parent processes. Check output of the file..
Date : 2nd September,2025
=========================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>

int main(){
	int fd;
	fd=open("fork.txt",O_RDWR|O_CREAT|O_APPEND,0664);
	if (fd<0){
		perror("open");
		exit(1);
	}

	pid_t pid=fork();
	if (pid<0){
		perror("fork");
		close(fd);
		exit(1);
	}
	else if (pid==0){
		dprintf(fd,"\nChild process with PID %d writing to file",getpid());
	}
	else{
		dprintf(fd,"\nParents process with PID %d writing to file",getpid());
	}
	close(fd);
	return 0;
}

/*
=======================================================================
OUTPUT:

Parents process with PID 5099 writing to file
Child process with PID 5100 writing to file
=======================================================================

*/
