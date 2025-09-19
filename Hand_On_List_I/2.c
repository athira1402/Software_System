/*
========================================================
Name : 2.c
Author : Athira Lonappan
Description : Write a simple program to execute in an infinite loop at the background. Go to /proc directory and
identify all the process related information in the corresponding proc directory.
Date : 10th August,2025
=========================================================
*/

#include<stdio.h>

int main(){
	for(;;){
	}
return 0;
}

/*
================================================================
OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_I$ ls -l /proc/1876
total 0
-r--r--r--  1 athira athira 0 Sep  6 03:05 arch_status
dr-xr-xr-x  2 athira athira 0 Sep  6 03:05 attr
-r--------  1 athira athira 0 Sep  6 03:05 auxv
-r--r--r--  1 athira athira 0 Sep  6 03:05 cgroup
--w-------  1 athira athira 0 Sep  6 03:05 clear_refs
-r--r--r--  1 athira athira 0 Sep  6 03:05 cmdline
-rw-r--r--  1 athira athira 0 Sep  6 03:05 comm
-rw-r--r--  1 athira athira 0 Sep  6 03:05 coredump_filter
-r--r--r--  1 athira athira 0 Sep  6 03:05 cpuset
lrwxrwxrwx  1 athira athira 0 Sep  6 03:05 cwd -> /home/athira/Software_System/Hands_on_List_I
-r--------  1 athira athira 0 Sep  6 03:05 environ
lrwxrwxrwx  1 athira athira 0 Sep  6 03:05 exe -> /home/athira/Software_System/Hands_on_List_I/a.out
dr-x------  2 athira athira 3 Sep  6 03:05 fd
dr-xr-xr-x  2 athira athira 0 Sep  6 03:05 fdinfo
-rw-r--r--  1 athira athira 0 Sep  6 03:05 gid_map
-r--------  1 athira athira 0 Sep  6 03:05 io
-r--------  1 athira athira 0 Sep  6 03:05 ksm_merging_pages
-r--------  1 athira athira 0 Sep  6 03:05 ksm_stat
-r--r--r--  1 athira athira 0 Sep  6 03:05 latency
-r--r--r--  1 athira athira 0 Sep  6 03:05 limits
-rw-r--r--  1 athira athira 0 Sep  6 03:05 loginuid
dr-x------  2 athira athira 0 Sep  6 03:05 map_files
-r--r--r--  1 athira athira 0 Sep  6 03:05 maps
-rw-------  1 athira athira 0 Sep  6 03:05 mem
-r--r--r--  1 athira athira 0 Sep  6 03:05 mountinfo
-r--r--r--  1 athira athira 0 Sep  6 03:05 mounts
-r--------  1 athira athira 0 Sep  6 03:05 mountstats
dr-xr-xr-x 57 athira athira 0 Sep  6 03:05 net
dr-x--x--x  2 athira athira 0 Sep  6 03:05 ns
-r--r--r--  1 athira athira 0 Sep  6 03:05 numa_maps
-rw-r--r--  1 athira athira 0 Sep  6 03:05 oom_adj
-r--r--r--  1 athira athira 0 Sep  6 03:05 oom_score
-rw-r--r--  1 athira athira 0 Sep  6 03:05 oom_score_adj
-r--------  1 athira athira 0 Sep  6 03:05 pagemap
-r--------  1 athira athira 0 Sep  6 03:05 patch_state
-r--------  1 athira athira 0 Sep  6 03:05 personality
-rw-r--r--  1 athira athira 0 Sep  6 03:05 projid_map
lrwxrwxrwx  1 athira athira 0 Sep  6 03:05 root -> /
-rw-r--r--  1 athira athira 0 Sep  6 03:05 sched
-r--r--r--  1 athira athira 0 Sep  6 03:05 schedstat
-r--r--r--  1 athira athira 0 Sep  6 03:05 sessionid
-rw-r--r--  1 athira athira 0 Sep  6 03:05 setgroups
-r--r--r--  1 athira athira 0 Sep  6 03:05 smaps
-r--r--r--  1 athira athira 0 Sep  6 03:05 smaps_rollup
-r--------  1 athira athira 0 Sep  6 03:05 stack
-r--r--r--  1 athira athira 0 Sep  6 03:05 stat
-r--r--r--  1 athira athira 0 Sep  6 03:05 statm
-r--r--r--  1 athira athira 0 Sep  6 03:05 status
-r--------  1 athira athira 0 Sep  6 03:05 syscall
dr-xr-xr-x  3 athira athira 0 Sep  6 03:05 task
-rw-r--r--  1 athira athira 0 Sep  6 03:05 timens_offsets
-r--r--r--  1 athira athira 0 Sep  6 03:05 timers
-rw-rw-rw-  1 athira athira 0 Sep  6 03:05 timerslack_ns
-rw-r--r--  1 athira athira 0 Sep  6 03:05 uid_map
-r--r--r--  1 athira athira 0 Sep  6 03:05 wchan
================================================================
*/

