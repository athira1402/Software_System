#!/bin/bash

#/*=======================================================
#Name : 19b.sh
#Author : Athira Lonappan
#Description : Create a FIFO file by a. mkfifo command
#Date : 18th September,2025
#=========================================================

mkfifo myfifo1

<<'COMMENT_BLOCK'

OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./19b.sh
athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ls -l | grep ^p
prw-r--r-- 1 athira athira     0 Sep 20 13:51 myfifo
prw-r--r-- 1 athira athira     0 Sep 20 13:56 myfifo1

COMMENT_BLOCK
