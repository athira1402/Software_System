/*=======================================================
Name : 17.c
Author : Athira Lonappan
Description : Write a program to simulate online ticket reservation. Implement write lock
Write a program to open a file, store a ticket number and exit. Write a separate program, to
open the file, implement write lock, read the ticket number, increment the number and print
the new ticket number then close the file.
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
        fd=open("ticket.txt",O_RDWR|O_CREAT,0660);
        if (fd==-1){
                perror("open");
                exit(1);
        }

        printf("Enter ticket number:");
        fgets(buf,sizeof(buf),stdin);
	write(fd,buf,strlen(buf));
	close(fd);
	return 0;
}
	
/*
=========================================================
OUTPUT:

 gcc 17.c
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ ./a.out
Enter ticket number:6219
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ gcc 17b.c
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ ./a.out

Write lock acquired
Ticket number: 6220

Lock Released!
=========================================================
*/
