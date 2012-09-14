#!/bin/bash

netstat -antp | 
awk 'NF="7" {print $5"\t""\t""\t""\t""\t"$7}' | 
awk 'FS=":" {print $2}' | 
awk '!x[$0]++' | 
awk 'NF' > /home/amboxer21/Documents/netstat.txt; 
egrep -v "(443|80)" /home/amboxer21/Documents/netstat.txt > /home/amboxer21/Documents/netstat2.txt;

LINES=$(awk 'END {print NR}' /home/amboxer21/Documents/netstat2.txt);

ONE=1;

if [ $LINES -lt $ONE ]; then
echo "NO SUSPICIOUS PORTS OPEN";
cat /home/amboxer21/Documents/netstat.txt;
exit 0;

   else 
   echo "[OPEN PORTS]";
   cat /home/amboxer21/Documents/netstat2.txt;
   sudo  /home/amboxer21/Documents/warning

fi
