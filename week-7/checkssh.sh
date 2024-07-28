#!/bin/bash
previous="/var/tmp/sshPrevious"
current="/var/tmp/sshCurrent"
log="/var/tmp/sshLog"
email="root@localhost"

if [ ! -f $previous]; then
    touch $previous
fi

w -h > $current
newLogins=$(comm -13 $previous $current)

echo "===========" >> $log
echo "Check time $(date) " >> $log

if [ -n "$newLogins" ]; then
    echo "New ssh login detected: " >> $log
    username=$(w -h | awk '{print $1}')
    startDay=$(who -a | grep "pts/1" | awk '{print $2}')
    startTime=$(who -a | grep "pts/1" | awk '{print $3}')
    echo "User $username login in $startTime at $startDay" >> $log
    cat $log
    cat $log | mail -s "login ssh detected" $email
fi

mv $current $previous