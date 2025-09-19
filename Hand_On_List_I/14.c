/*=======================================================
Name : 14.c
Author : Athira Lonappan
Description :Write a program to find the type of a file.
a. Input should be taken from command line.
b. program should be able to identify any type of a file.
Date : 24th August,2025
=========================================================
*/

#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>

int main(int argc,char *agrv[]){
	struct stat file;
	if (argc!=2){
		printf("No 2 arguments were passed\n");
		exit(1);
	}
	if (stat(agrv[1],&file)<0){
		perror("stat");
		exit(1);
	}
	if (S_ISREG(file.st_mode)) printf("Regular file\n");
	else if (S_ISDIR(file.st_mode)) printf("Directory \n");
	else if (S_ISBLK(file.st_mode)) printf("Block device file.\n");
	else if (S_ISCHR(file.st_mode)) printf("Character device file.\n");
	else if (S_ISFIFO(file.st_mode)) printf("Named pipe FIFO.\n");
	else if (S_ISLNK(file.st_mode)) printf("Symbolic Link\n");
	else if (S_ISSOCK(file.st_mode)) printf("Socket\n");
	else printf("Unknown file type\n");
	return 0;
}

/*
==================================================================
OUTPUT : 

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ ./a.out /home/athira
Directory
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ ./a.out /dev/null
Character device file.
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ ./a.out fork.txt
Regular file
==================================================================
*/
