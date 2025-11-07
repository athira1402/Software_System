/*=======================================================
Name : 13b.c
Author : Athira Lonappan
Description : Write two programs: 
first program is waiting to catch SIGSTOP signal, 
the second program will send the signal (using kill system call). 
Find out whether the first program is able to catch the signal or not.
Date : 15th September,2025
=========================================================
*/

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid;
    printf("Enter PID to send SIGSTOP: ");
    scanf("%d", &pid);

    if (kill(pid, SIGSTOP) == 0) {
        printf("Sent SIGSTOP to %d\n", pid);
    } else {
        perror("kill");
    }

    return 0;
}


/*
=========================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./13b.exe
Enter PID to send SIGSTOP: 9537
Sent SIGSTOP to 9537
=========================================================
*/
