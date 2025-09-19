#========================================================
#Name : 1a.sh
#Author : Athira Lonappan
#Description : Create the following types of a files using shell command - a. soft link 
#Date : 10th August,2025
#=========================================================


#!/bin/bash
echo "Target of symbolic link" >target.txt
ln -s target.txt soft_link.txt
ls -li soft_link.txt

<< "COMMENT"

OUTPUT: 

12466 lrwxrwxrwx 1 athira athira 10 Sep  6 03:09 soft_link.txt -> target.txt

COMMENT
