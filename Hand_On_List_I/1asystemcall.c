/*
========================================================
Name : 1asystemcall.c
Author : Athira Lonappan
Description : Create the following types of a files using system call
a. soft link (symlink system call)
Date : 10th August,2025
=========================================================
*/

#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main( ) {
const char *target="target.txt";
const char *link="soft_link.txt";


int fd=open(target,O_WRONLY|O_CREAT|O_TRUNC,0644);
const char *msg="Target for soft link\n";
write(fd,msg,strlen(msg));
close(fd);

symlink(target,link);
printf("Symlink created: %s->%s\n",link,target);
printf("\nListing with inode numbers:\n");
system("ls -li target.txt soft_link.txt");
return 0;
}

/*
=======================================================
OUTPUT:

Symlink created: soft_link.txt->target.txt

Listing with inode numbers:
12466 lrwxrwxrwx 1 athira athira 10 Sep  6 03:10 soft_link.txt -> target.txt
10861 -rw-r--r-- 2 athira athira 21 Sep  6 03:11 target.txt
=======================================================
*/
