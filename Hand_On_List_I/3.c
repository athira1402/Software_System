/*
========================================================
Name : 3.c
Author : Athira Lonappan
Description : Write a program to create a file and print the file descriptor value. Use creat ( ) system call
Date : 10th August,2025
=========================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	int fd=creat("new.txt",0644);
	printf("File description: %d\n",fd);
	close(fd);
	return 0;
}

/*
==============================================================
OUTPUT:

File description: 3
==============================================================
*/
