/*=======================================================
Name : 18.c
Author : Athira Lonappan
Description :Write a program to perform Record locking.
a. Implement write lock
b. Implement read lock
Create three records in a file. Whenever you access a particular record, first lock it then modify/access
to avoid race condition.
Date : 31st August,2025
=========================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

#define RECORD_SIZE 100
#define NUM_OF_RECORDS 3


void set_lock(int fd,short type,int recno){
        struct flock lock;
        lock.l_type=type;
        lock.l_whence=SEEK_SET;
        lock.l_start=recno*RECORD_SIZE;
        lock.l_len=RECORD_SIZE;

        if (fcntl(fd,F_SETLKW,&lock)==-1){
                perror("fcntl");
                exit(1);
        }

        if (type==F_RDLCK)
                printf("\nRead lock acquired");
        else if (type==F_WRLCK)
                printf("\nWrite lock acquired");

}

void release_lock(int fd,int recno){
        struct flock lock;
        lock.l_type=F_UNLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=recno*RECORD_SIZE;
        lock.l_len=RECORD_SIZE;

        if (fcntl(fd,F_SETLK,&lock)==-1){
                perror("fcntl");
                exit(1);
        }

        printf("\nLock Released!");
}


int main(){
        int fd;
        char buf[RECORD_SIZE]={0};
	int recno;

        fd=open("record_lock.txt",O_RDWR|O_CREAT,0660);
        if (fd==-1){
                perror("open");
                exit(1);
        }

	for(int i=0;i<NUM_OF_RECORDS;i++){
		lseek(fd,i*RECORD_SIZE,SEEK_SET);
		write(fd,"EMPTY\n",6);
	}

	while (1){
	printf("\nEnter record no to access (0-2):");
	scanf("%d",&recno);

	if (recno<0 || recno>=NUM_OF_RECORDS){
		printf("Invalid record access!");
		exit(1);
	}

	int choice;
	printf("1. Write Lock \n 2. Read Lock \n 3. Exit \n Enter your choice:");
	scanf("%d",&choice);
	getchar();
	
	if (choice==1){
                set_lock(fd,F_WRLCK,recno);
                printf("\nEnter text to write in record %d:",recno);
                fgets(buf,sizeof(buf),stdin);
		lseek(fd,recno*RECORD_SIZE,SEEK_SET);
                write(fd,buf,RECORD_SIZE);
                sleep(10);
                release_lock(fd,recno);
        }
        else if (choice==2){
                set_lock(fd,F_RDLCK,recno);
                lseek(fd,recno*RECORD_SIZE,SEEK_SET);
                int n=read(fd,buf,sizeof(buf));
                if (n>0) buf[n]='\0';
                printf("\nRecord contents: %s",buf);
                sleep(10);
		release_lock(fd,recno);
        }
	else if (choice==3){
		printf("Exiting.....");
		break;
	}
        else{
                printf("Invalid Choice! ;-(");
        }
	}	
        close(fd);
        return 0;
}

/*
=====================================================================
OUTPUT:


Enter record no to access (0-2):1
1. Write Lock
 2. Read Lock
 3. Exit
 Enter your choice:1

Write lock acquired
Enter text to write in record 1:Accessing Record 1

Lock Released!
Enter record no to access (0-2):1
1. Write Lock
 2. Read Lock
 3. Exit
 Enter your choice:2

Read lock acquired
Record contents: Accessing Record 1

Lock Released!
Enter record no to access (0-2):2
1. Write Lock
 2. Read Lock
 3. Exit
 Enter your choice:3
Exiting.....
=================================================================
*/
