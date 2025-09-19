/*=======================================================
Name : 10.c
Author : Athira Lonappan
Description :Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
bytes (use lseek) and write again 10 bytes.
a. check the return value of lseek
b. open the file with od and check the empty spaces in between the data.
Date : 17th August,2025
=========================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
	int fd;	
	char buf1[]="ABCDEFGHIJ1234567890";
	fd=open("lseek_eg.txt",O_RDWR | O_CREAT,0644);
	write(fd,buf1,10);
	lseek(fd,10,SEEK_CUR);
	write(fd,buf1+10,10);
	close(fd);
	//system("od -c lseek_eg.txt");
	system("od -c --address-radix=d lseek_eg.txt");
}

/*
==============================================================
OUTPUT:

0000000   A   B   C   D   E   F   G   H   I   J  \0  \0  \0  \0  \0  \0
0000016  \0  \0  \0  \0   1   2   3   4   5   6   7   8   9   0
0000030
==============================================================
*/
