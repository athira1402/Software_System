/*
=======================================================
Name : 26b.c
Author : Athira Lonappan
Description : Write a program to execute an executable program.
a. use some executable program
b. pass some input to an executable program. (for example execute an executable of $./a.out name)
Date : 6th September,2025
=========================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
	char *args[]={"./26a.exe","Athira",NULL};
	execvp(args[0],args);
	
	perror("execvp failed");
	return 1;
}

/*
==============================================================
OUTPUT:

Hello Athira, you have executed me through another program!
==============================================================
*/
