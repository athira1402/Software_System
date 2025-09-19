/*=======================================================
Name : 7.c
Author : Athira Lonappan
Description :Write a program to copy file1 into file2 ($cp file1 file2).
Date : 17th August,2025
=========================================================
*/


#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main() {
	char buffer[1024];
	int fd1,fd2;
	fd1=open("target.txt",O_RDONLY);
	fd2=open("copy1.txt",O_WRONLY | O_CREAT | O_TRUNC,0644);
	
	int n;
	while((n=read(fd1,buffer,sizeof(buffer)))>0){
		write(fd2,buffer,n);
	}
	close(fd1);
	close(fd2);
	return 0;

}

/*
==============================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ cat target.txt
Target for hard link
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ cat copy1.txt
Target for hard link
==============================================================
*/
