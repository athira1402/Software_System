#=======================================================
#Name : 20.sh
#Author : Athira Lonappan
#Description : Find out the priority of your running program. Modify the priority with nice command.
#Date : 31st August,2025
#=========================================================


echo "Current Shell PID: $$"
echo "Current priority : "
ps -o pid,ni,comm -p $$

echo ""
echo "Starting new process with modified priority..."
nice -n 10 sleep 10 &

NEW_PID=$!
echo ""
echo "New process PID: $NEW_PID"
echo "New process priority before renice:"
ps -o pid,ni,comm -p $NEW_PID

echo ""
echo "Renice process to niceness=5"
sudo renice -n 5 -p $NEW_PID

echo ""
echo "New process priority after renice"
ps -o pid,ni,comm -p $NEW_PID


<< "COMMENT"
OUTPUT: 

Current Shell PID: 1330
Current priority :
    PID  NI COMMAND
   1330   0 bash

Starting new process with modified priority...

New process PID: 1332
New process priority before renice:
    PID  NI COMMAND
   1332   0 nice

Renice process to niceness=5
[sudo] password for athira:
1332 (process ID) old priority 10, new priority 5

New process priority after renice
    PID  NI COMMAND
   1332   5 sleep
COMMENT
