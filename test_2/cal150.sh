#!/bin/bash

rm before150.log

total=0
hits=0

times=0
for i in {1..150};
do
	if [ -f file$i.log ];then
		times=`expr $times + 1`
		if [ $times == 151 ];then
			break
		fi
		cat file$i.log >> before150.log
	fi
done

while read line
do
	echo $line > temp
	tmpVar=`sed 's/\(.*\) \(.*\)/\1/g' temp`
	total=`expr $total + $tmpVar`
	tmpVar=`sed 's/\(.*\) \(.*\)/\2/g' temp`
	hits=`expr $hits + $tmpVar`	
done < before150.log

echo $total " " $hits
echo "scale=6;`expr $hits/$total`" | bc -l
