/*
=======================================================
Name : 26a.c
Author : Athira Lonappan
Description : Write a program to execute an executable program.
a. use some executable program
b. pass some input to an executable program. (for example execute an executable of $./a.out name)
Date : 6th September,2025
=========================================================
*/

#include<stdio.h>
#include<unistd.h>

int main(int argc,char *argv[]){
	if (argc>1){
		printf("\nHello %s, you have executed me through another program!",argv[1]);
	}
	else{
		printf("No name passed.");
	}
	return 0;
}

/*
==============================================================
OUTPUT:

Hello Athira, you have executed me through another program!
==============================================================
*/
