/*
========================================================
Name : 4.c
Author : Athira Lonappan
Description :Write a program to open an existing file with read write mode. Try O_EXCL flag also.
Date : 10th August,2025
=========================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	const char *file="new.txt";
	int fd=open(file,O_RDWR);
	if (fd==-1){
	perror("open without O_EXCL");
	} else{
	printf("\nFile Descriptor:%d",fd);
	}
	close(fd);

	fd=open(file,O_RDWR,O_CREAT,O_EXCL);
	if (fd==-1){
		perror("open with O_EXCL");
	} else{
		printf("\nCreated and opened with O_EXCL");
	}
	close(fd);
	return 0;
}

/*
============================================================
OUTPUT:

File Descriptor:3
Created and opened with O_EXCL
============================================================
*/
