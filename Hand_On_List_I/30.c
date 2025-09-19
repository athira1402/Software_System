/*
=======================================================
Name : 30.c
Author : Athira Lonappan
Description : Write a program to run a script at a specific time using a Daemon process.
Date : 6th September,2025
=========================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int main(){
	pid_t pid=fork();
	if (pid<0) perror("fork failed");
	if (pid>0) exit(0);
	
	setsid();
	chdir("/");
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	int hour=12;
	int mins=37;

	setenv("TZ", "Asia/Kolkata", 1);
	tzset();


	while(1){
		time_t now=time(NULL);
		struct tm *t=localtime(&now);

		system("sh -c 'echo \"Live at $(date)\" >> /home/athira/Software_System/Hands_on_List_I/30_log.txt'");

		if (t->tm_hour==hour && t->tm_min==mins){
                        printf("Triggering script at %d %d:",t->tm_hour, t->tm_min);
			system("/home/athira/Software_System/Hands_on_List_I/30a.sh");
			sleep(60);
		}
	}
	return 0;
}

/*
===============================================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ ./a.out &
[4] 2581
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ cat deamon.txt
Daemon triggered at Sat Sep  6 12:37:00 IST 2025
===============================================================================
*/
