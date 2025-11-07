#!/bin/bash

#=======================================================
#Name : 19c.sh
#Author : Athira Lonappan
#Description : Create a FIFO file by c.use strace command to find out, which command (mknod or mkfifo) is better.
#Date : 18th September,2025
#=========================================================

strace mknod myfifo2 p
strace mkfifo myfifo3

<<'COMMENT_BLOCK'

OUTPUT:

athira@DESKTOP-UTGP8IB:~/Software_System/Hands_on_List_II$ ./19c.sh
mknodat(AT_FDCWD, "myfifo2", S_IFIFO|0666) = 0
mknodat(AT_FDCWD, "myfifo3", S_IFIFO|0666) = 0

COMMENT_BLOCK
