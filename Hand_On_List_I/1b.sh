#========================================================
#Name : 1b.sh
#Author : Athira Lonappan
#Description : Create the following types of a files using shell command - b. hard link                       
#Date : 10th August,2025
#=========================================================


echo "Target for hard link" > target.txt
ln target.txt hard_link.txt
ls -li target.txt hard_link.txt


#================================================================
#OUTPUT:

#10861 -rw-r--r-- 2 athira athira 21 Sep  6 03:12 hard_link.txt
#10861 -rw-r--r-- 2 athira athira 21 Sep  6 03:12 target.txt

#===============================================================
