/*=======================================================
Name : 11.c
Author : Athira Lonappan
Description :Write a program to open a file, duplicate the file descriptor and append the file with both the
descriptors and check whether the file is updated properly or not.
a. use dup
b. use dup2
c. use fcntl
Date : 24th August,2025
=========================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	int fd;
	int new_fd;
	fd=open("target.txt",O_RDONLY|O_CREAT,0644);
	new_fd=dup(fd);
	printf("New fd via dup (smallest available fd) : %d\n",new_fd);
	new_fd=dup2(fd,100);
	printf("New fd via dup2 (duplicates fd into 100 new_fd) : %d\n",new_fd);
	new_fd=fcntl(fd,F_DUPFD,100);
	printf("New fd via fcntl (new_fd will be available fd>=100) : %d\n",new_fd);
	close(fd);
	close(new_fd);
}

/*
==============================================================
OUTPUT:

New fd via dup (smallest available fd) : 4
New fd via dup2 (duplicates fd into 100 new_fd) : 100
New fd via fcntl (new_fd will be available fd>=100) : 101
==============================================================
*/
