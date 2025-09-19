/*
=======================================================
Name : 27.c
Author : Athira Lonappan
Description : Write a program to execute ls -Rl by the following system calls
a. execl
b. execlp
c. execle
d. execv
e. execvp
Date : 6th September,2025
=========================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
	int choice;
	printf("ls -Rl using \n 1. execl \n 2. execlp \n 3. execle \n 4. execv \n 5. execvp \nEnter your choice:");
	scanf("%d",&choice);

	if (choice==1){
		printf("Using execl to run ls-Rl\n");
		execl("/bin/ls","ls","-Rl",NULL);
		perror("execl failed");
	}
	else if (choice==2){
		printf("Using execlp to run ls-Rl\n");
                execlp("ls","ls","-Rl",NULL);
                perror("execlp failed");
	}
	else if (choice==3){
		printf("Using execle to run ls-Rl\n");
		char *env[]={"CUSTOM=Athira",NULL};
                execle("/bin/ls","ls","-Rl",NULL,env);
                perror("execle failed");
	}
	else if (choice==4){
                printf("Using execv to run ls-Rl\n");
                char *args[]={"ls","-Rl",NULL};
		execv("/bin/ls",args);
                perror("execv failed");
        }
        else if (choice==5){
                printf("Using execvp to run ls-Rl\n");
		char *args[]={"ls","-Rl",NULL};
                execvp("ls",args);
                perror("execvp failed");
	}
	else{
		printf("Invalid choice");
	}
return 0;
}

/*
===============================================================

OUTPUT:

ls -Rl using
 1. execl
 2. execlp
 3. execle
 4. execv
 5. execvp
Enter your choice:3
Using execle to run ls-Rl
.:
total 248
-rw-r--r-- 1 athira athira   788 Sep  2 07:30 10.c
-rw-r--r-- 1 athira athira   846 Sep  2 07:29 11.c
-rw-r--r-- 1 athira athira   608 Sep  2 07:28 12.c
-rw-r--r-- 1 athira athira  1126 Sep  2 07:27 13.c
-rw-r--r-- 1 athira athira  1056 Sep  2 07:26 14.c
-rw-r--r-- 1 athira athira   416 Sep  2 07:24 15.c
-rw-r--r-- 1 athira athira  1620 Sep  2 07:22 16.c
-rw-r--r-- 1 athira athira  1668 Sep  2 07:22 17.c
-rw-r--r-- 1 athira athira  1451 Aug 30 17:42 17b.c
-rw-r--r-- 1 athira athira  2748 Sep  2 07:17 18.c
-rw-r--r-- 1 athira athira   968 Sep  2 07:16 19.c
-rwxr-xr-x 1 athira athira   381 Sep  2 08:23 1a.sh
-rwxr-xr-x 1 athira athira   776 Sep  2 08:26 1asystemcall.c
-rwxr-xr-x 1 athira athira   396 Sep  2 08:24 1b.sh
-rw-r--r-- 1 athira athira  2065 Sep  2 08:27 1b_systemcall.c
-rwxr-xr-x 1 athira athira   327 Sep  2 08:25 1c_shell.sh
-rwxr-xr-x 1 athira athira   816 Sep  2 08:28 1c_systemcall.c
-rw-r--r-- 1 athira athira   434 Sep  2 08:21 2.c
-rwxr-xr-x 1 athira athira   731 Sep  2 07:08 20.sh
-rw-r--r-- 1 athira athira   623 Sep  2 07:05 21.c
-rw-r--r-- 1 athira athira   834 Sep  2 07:04 22.c
-rw-r--r-- 1 athira athira   759 Sep  2 07:03 23.c
-rw-r--r-- 1 athira athira   871 Sep  2 07:02 24.c
-rw-r--r-- 1 athira athira  1305 Sep  2 07:01 25.c
-rw-r--r-- 1 athira athira   219 Aug 31 14:41 26a.c
-rwxr-xr-x 1 athira athira 15960 Aug 31 14:41 26a.exe
-rw-r--r-- 1 athira athira   554 Sep  2 07:00 26b.c
-rwxr-xr-x 1 athira athira 16056 Aug 31 14:40 26b.exe
-rw-r--r-- 1 athira athira  1445 Sep  2 06:58 27.c
-rw-r--r-- 1 athira athira   881 Sep  5 17:16 28.c
-rw-r--r-- 1 athira athira  1578 Sep  5 17:13 29.c
-rw-r--r-- 1 athira athira   454 Sep  2 08:20 3.c
-rw-r--r-- 1 athira athira   770 Sep  5 17:15 30.c
-rwxr-xr-x 1 athira athira    50 Sep  5 17:14 30a.sh
-rw-r--r-- 1 athira athira   671 Sep  2 08:19 4.c
-rw-r--r-- 1 athira athira   616 Sep  2 08:19 5.c
-rw-r--r-- 1 athira athira   457 Sep  2 07:36 6.c
-rw-r--r-- 1 athira athira   572 Sep  2 07:35 7.c
-rw-r--r-- 1 athira athira   737 Sep  2 07:32 8.c
-rw-r--r-- 1 athira athira  1148 Sep  2 07:31 9.c
-rwxr-xr-x 1 athira athira 16360 Sep  5 17:17 a.out
-rw-r--r-- 1 athira athira     1 Aug 13 20:12 copy.txt
-rw-r--r-- 1 athira athira    21 Aug 13 21:17 copy1.txt
-rw-r--r-- 1 athira athira     0 Aug 13 18:57 file1.txt
-rw-r--r-- 1 athira athira     0 Aug 13 18:57 file2.txt
-rw-r--r-- 1 athira athira     0 Aug 13 18:57 file3.txt
-rw-r--r-- 1 athira athira     0 Aug 13 18:57 file4.txt
-rw-r--r-- 1 athira athira     0 Aug 13 18:57 file5.txt
-rw-r----- 1 athira athira    26 Aug 30 17:42 file_lock.txt
-rw-r--r-- 1 athira athira   178 Aug 31 09:59 fork.txt
-rw-r--r-- 2 athira athira    21 Aug 13 14:31 hard_link.txt
-rw-r--r-- 1 athira athira    57 Aug 13 15:29 infinite.c
-rw-r--r-- 1 athira athira    35 Aug 14 08:54 linebyline.txt
-rw-r--r-- 1 athira athira    30 Aug 14 17:48 lseek_eg.txt
---x--x--t 1 athira athira    30 Aug 14 17:25 lseek_example.txt
prw-r--r-- 1 athira athira     0 Aug 13 14:40 myfifo
-rw-r--r-- 1 athira athira     0 Aug 13 17:58 new.txt
-rw-r----- 1 athira athira   146 Aug 31 04:00 record_lock.txt
lrwxrwxrwx 1 athira athira    10 Aug 13 13:28 soft_link.txt -> target.txt
-rw-r--r-- 2 athira athira    21 Aug 13 14:31 target.txt
-rw-r----- 1 athira athira     7 Aug 31 06:49 ticket.txt
===============================================================
*/
