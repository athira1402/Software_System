/*=======================================================
Name : 12.c
Author : Athira Lonappan
Description :Write a program to find out the opening mode of a file. Use fcntl.
Date : 24th August,2025
=========================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
	int fd=open("target.txt",O_RDWR|O_CREAT,0644);
	int flag=fcntl(fd,F_GETFL);
	int mode=flag & O_ACCMODE;
	if (mode==O_RDONLY) printf("Read Only mode.\n");
	else if (mode==O_WRONLY) printf("Write Only mode.\n");
	else if (mode==O_RDWR) printf("Read write mode.\n");
	close(fd);
}


/*
=======================================================
OUTPUT:

Read write mode.
=======================================================
*/
