/*=======================================================
Name : 19.c
Author : Athira Lonappan
Description : Write a program to find out time taken to execute getpid system call. Use time stamp counter.
Date : 31st August,2025
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

uint64_t start=__rdtsc();
int pid=getpid();
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
printf("\nTime taken to execute getpid() systemcall : %f\n",time);

return 0;
}

/*
================================================================
OUTPUT:

PID : 1383
Cycles : 6884
CPU Freqency : 2496.001000
Time taken to execute getpid() systemcall : 2.758012
================================================================
*/
