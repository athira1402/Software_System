/*=======================================================
Name : 4.c
Author : Athira Lonappan
Description : Write a program to measure how much time is taken to execute 100 getppid ( ) system call. Use time stamp counter.
Date : 9th September,2025
=========================================================
*/

#include<stdio.h>
#include<x86intrin.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<unistd.h>
#include<inttypes.h>

int main(){

int pid;
uint64_t start=__rdtsc();
for(int i=0;i<100;i++){
	pid=getppid();
}

uint64_t end=__rdtsc();

FILE *file=popen("grep 'cpu MHz' /proc/cpuinfo | head -1 | awk '{print $4}'","r");
if (file==NULL) {
	perror("popen");
	exit(1);
}

char buf[128];
fgets(buf,sizeof(buf),file);
double cpu_freq=atof(buf);
pclose(file);

uint64_t cycles=end-start;
double time=cycles/cpu_freq;
printf("\nPID : %d",pid);
printf("\nCycles : %"PRIu64"",cycles);
printf("\nCPU Freqency : %f",cpu_freq);
printf("\nTime taken to execute 100 getppid() systemcall : %f\n",time);

return 0;
}

/*
================================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./4.exe

PID : 8523
Cycles : 46229
CPU Freqency : 2496.000000
Time taken to execute 100 getppid() systemcall : 18.521234
================================================================
*/
