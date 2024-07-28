#!/bin/bash
echo "System information"
echo -n "Operating system name:"
os=$(lsb_release -d | cut -d":"f2 | xargs)
echo $os
echo -n "Machine name:
name=$(hostname)
echo $name
echo -n "Architecture:
archi uname -m
echo $archi
echo -n "Total memory (MB): "
memo=$(freem | awk '/^Mem:/{print $2}')
echo $memo
emptyMemo=$(free -m | awk '/^Mem:/{print $4]')
echo "Free memory ${emptyMemo) (MB)"
listIP=$(ip addr show | grep -op (?<=inet\s)\d+(\.\d+){3}')
echo "List of tp:
echo $listIP
listUser=$(cut -d: f1 /etc/passwd | sort)
echo "List of user:
echo $listUser
rootTask=$(ps -eo user, pid, comm-sort=comm | grep 'root' | sort -k3 | awk '{print $3}' | tr '\n')
echo "List of root task:
echo $rootTask
openPort=$(netstat -tuln | grep "LISTEN" | awk '{print $1, $4}')
echo "Open port:
echo $openPort
writePer=$(find / type d-perm-002 2>/dev/null)
echo "List of folder allow write permision"
echo $writePer
allPackage=$(dpkg -l | awk '/^ii/ {print $2 $3}')
echo "List of all install package"
echo $allPackage