/*
========================================================
Name : 5.c
Author : Athira Lonappan
Description :Write a program to create five new files with infinite loop. Execute the program in the background
and check the file descriptor table at /proc/pid/fd.
Date : 10th August,2025
=========================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	int fd[5];
	char file[10];
	for(int i=0;i<5;i++){
		sprintf(file,"file%d.txt",i+1);
		fd[i]=open(file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
		printf("created %s \n",file);
	}
	for(;;){
	}
	return 0;
}

/*
============================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ ./a.out &
[2] 1826
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ created file1.txt
created file2.txt
created file3.txt
created file4.txt
created file5.txt
ls -l /proc/1826/fd
total 0
lrwx------ 1 athira athira 64 Sep  6 02:59 0 -> /dev/pts/2
lrwx------ 1 athira athira 64 Sep  6 02:59 1 -> /dev/pts/2
lrwx------ 1 athira athira 64 Sep  6 02:59 2 -> /dev/pts/2
l-wx------ 1 athira athira 64 Sep  6 02:59 3 -> /home/athira/Software_System/Hands_on_List_I/file1.txt
l-wx------ 1 athira athira 64 Sep  6 02:59 4 -> /home/athira/Software_System/Hands_on_List_I/file2.txt
l-wx------ 1 athira athira 64 Sep  6 02:59 5 -> /home/athira/Software_System/Hands_on_List_I/file3.txt
l-wx------ 1 athira athira 64 Sep  6 02:59 6 -> /home/athira/Software_System/Hands_on_List_I/file4.txt
l-wx------ 1 athira athira 64 Sep  6 02:59 7 -> /home/athira/Software_System/Hands_on_List_I/file5.txt
============================================================
*/
