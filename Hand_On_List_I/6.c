/*=======================================================
Name : 6.c
Author : Athira Lonappan
Description :Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
Date : 17th August,2025
=========================================================
*/

#include<stdio.h>
#include<unistd.h>

int main(){
	char buffer[100];
	int bytes=read(0,buffer,sizeof(buffer));
	if (bytes>0){
		write(1,buffer,bytes);
	}
	return 0;
}


/*
=======================================================
OUTPUT:

Hi, back again!
Hi, back again!
=======================================================
*/
