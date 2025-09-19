/*=======================================================
Name : 16.c
Author : Athira Lonappan
Description : Write a program to perform mandatory locking.
a. Implement write lock
b. Implement read lock
Date : 31st August,2025
=========================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

void set_lock(int fd,short type){
	struct flock lock;
	lock.l_type=type;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;

	if (fcntl(fd,F_SETLKW,&lock)==-1){
		perror("fcntl");
		exit(1);
	}

	if (type==F_RDLCK)
		printf("\nRead lock acquired");
	else if (type==F_WRLCK)
		printf("\nWrite lock acquired");

}

void release_lock(int fd){
	struct flock lock;
	lock.l_type=F_UNLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;

	if (fcntl(fd,F_SETLK,&lock)==-1){
		perror("fcntl");
		exit(1);
	}

	printf("\nLock Released!");
}

int main(){
	int fd;
	char buf[100];
	fd=open("file_lock.txt",O_RDWR|O_CREAT,0660);
	if (fd==-1){
		perror("open");
		exit(1);
	}
	
	int choice;
	printf("1. Write lock\n 2. Read lock\n Enter choice:");
	scanf("%d",&choice);

	if (choice==1){
		set_lock(fd,F_WRLCK);
		printf("\nEnter text to write in file:");
		getchar();
		fgets(buf,sizeof(buf),stdin);
		write(fd,buf,strlen(buf));
		sleep(10);
		release_lock(fd);
	}
	else if (choice==2){
		set_lock(fd,F_RDLCK);
		lseek(fd,0,SEEK_SET);
		int n=read(fd,buf,sizeof(buf));
		if (n>0) buf[n]='\0';
		printf("\nFile contents: %s",buf);
		sleep(10);
		release_lock(fd);
	}
	else{
		printf("Invalid Choice! ;-(");
	}
	close(fd);
	return 0;

}

/*
====================================================================
OUTPUT: 

 gcc 16.c
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ ./a.out
1. Write lock
 2. Read lock
 Enter choice:1

Write lock acquired
Enter text to write in file: Writing here.......

Lock Released!athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ gcc 16.c
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ ./a.out
1. Write lock
 2. Read lock
 Enter choice:2

Read lock acquired
File contents:  Writing here.......

Lock Released!
=====================================================================
*/
