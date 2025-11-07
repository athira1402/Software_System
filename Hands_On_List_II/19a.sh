#!/bin/bash

#/*=======================================================
#Name : 19a.sh
#Author : Athira Lonappan
#Description : Create a FIFO file by a. mknod command
#Date : 18th September,2025
#=========================================================

mknod myfifo p

<<'COMMENT_BLOCK'

OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./19a.sh
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ls -l | grep ^p
prw-r--r-- 1 athira athira     0 Sep 20 13:51 myfifo

COMMENT_BLOCK
