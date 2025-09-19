/*=======================================================
Name : 13.c
Author : Athira Lonappan
Description :Write a program to wait for a STDIN for 10 seconds using select. 
Write a proper print statement to verify whether the data is available within 10 seconds or not (check in $man 2 select).
Date : 24th August,2025
=========================================================
*/


#include<stdio.h>
#include<unistd.h>
#include<sys/select.h>

int main(){
	fd_set myreadset;
	int val;
	struct timeval timeout;

	//Initializing timout sec
	timeout.tv_sec=10;
	timeout.tv_usec=0;

	FD_ZERO(&myreadset);
	FD_SET(STDIN_FILENO, &myreadset);

	printf("Waiting for user input.....\n");
	
	val=select(STDIN_FILENO+1,&myreadset,NULL,NULL,&timeout);
	
	if (val<0) perror("select\n");
	else if (val==0) printf("no input\n");
	else if (val>0){
		if (FD_ISSET(STDIN_FILENO, &myreadset)) printf("Data available\n");

		char buffer[1024];
		int n=read(STDIN_FILENO,buffer,sizeof(buffer)-1);
		if (n>0){
			buffer[n]='\0';			
			printf("You entered: %s\n",buffer);
		}}
	else return printf("Data not available in 10 secs.\n");
	
	return 0;
}

/*
=================================================================
OUTPUT:

 ./a.out
Waiting for user input.....
Hi, Athira here
Data available
You entered: Hi, Athira here

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ ./a.out
Waiting for user input.....
no input
=================================================================
*/
