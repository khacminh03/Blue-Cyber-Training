#!/bin/bash
previous="/var/tmp/etc_files_prev"
current="/var/tmp/etc_files_curr"
log="/var/log/checketc.log"
email="root@localhost"

# Kiểm tra lần chạy đầu tiên
if [ ! -f $previous ]; then
    touch $previous
fi

find /etc -type f > $current

newFile=$(comm -13 $previous $current)
modifiedFile=$(comm -23 $previous $current)
deletedFiles=$(comm -23 $current $previous)

echo "===================================" >> $log
echo "Check time: $(date)" >> $log

if [ -n "$newFile" ]; then
    echo "New files:" >> $log
    for FILE in $newFile; do
        echo $FILE >> $log
        if file "$FILE" | grep -q "text"; then
            echo "First 10 lines of $FILE:" >> $log
            head -n 10 "$FILE" >> $log
        fi
    done
fi

if [ -n "$modifiedFiles" ]; then
    echo "Modified files:" >> $log
    for FILE in $modifiedFiles; do
        echo $FILE >> $log
    done
fi

if [ -n "$deletedFiles" ]; then
    echo "Deleted files:" >> $log
    for FILE in $deletedFilesS; do
        echo $FILE >> $log
    done
fi

cat $log | mail -s "ETC Directory Check Report" $email

mv $current $previous
