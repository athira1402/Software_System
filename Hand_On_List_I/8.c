/*=======================================================
Name : 8.c
Author : Athira Lonappan
Description :Write a program to open a file in read only mode, read line by line and display each line as it is read.
Close the file when end of file is reached.
Date : 17th August,2025
=========================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
	char buffer[1024];
	int fd;
	char line[1024];
	int j=0;
	int n;
	fd=open("linebyline.txt",O_RDONLY);
	
	while((n=read(fd,buffer,sizeof(buffer)))>0){
		for(int i=0;i<n;i++){
			if (buffer[i]=='\n'){
				line[j++]='\0';
				j=0;
				printf("%s\n",line);
			}
			else{
				line[j++]=buffer[i];
			}
		}
	}
close(fd);
return 0;
}

/*
==========================================================
OUTPUT:

Hi there!
It's me!
Okay byee :-)
==========================================================
*/
