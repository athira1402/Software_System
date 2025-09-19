/*=======================================================
Name : 9.c
Author : Athira Lonappan
Description :Write a program to print the following information about a given file.
a. inode
b. number of hard links
c. uid
d. gid
e. size
f. block size
g. number of blocks
h. time of last access
i. time of last modification
j. time of last change
Date : 17th August,2025
=========================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<time.h>

int main(){
	char *file="linebyline.txt";
	struct stat st;
	if (stat(file,&st)==-1){
		perror("stat end");
		exit(1);
	}
	
	printf("File : %s\n",file);
	printf("Inode number: %lu\n",(unsigned long)st.st_ino);
	printf("Number of hard links: %lu\n",(unsigned long)st.st_nlink);
	printf("UID: %u\n",st.st_uid);
	printf("GID: %u\n",st.st_gid);
	printf("Size: %lu\n",st.st_size);
	printf("Block size: %lu\n",st.st_blksize);
	printf("Number of blocks: %ld\n",st.st_blocks);
	printf("Last access: %s\n",ctime(&st.st_atime));
	printf("Last modification: %s\n",ctime(&st.st_mtime));
	printf("Last status change: %s\n",ctime(&st.st_ctime));
	return 0;
}

/*
===============================================================
OUTPUT:

File : linebyline.txt
Inode number: 2605
Number of hard links: 1
UID: 1000
GID: 1000
Size: 35
Block size: 4096
Number of blocks: 8
Last access: Thu Aug 14 08:54:18 2025

Last modification: Thu Aug 14 08:54:30 2025

Last status change: Thu Aug 14 08:54:30 2025
===============================================================
*/
