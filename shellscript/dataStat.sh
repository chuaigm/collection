#!/usr/bin/bash

# var define
TF="tmp.tmp"
FILE="./test.log"
let total=0
let nn=0
let n0=0
let avg=0

# data select
sed -n 's/.*|80001|\(.*\)|80002|.*/\1/p' $FILE > $TF

# calculate
while read myline
do
	#echo "line:"$myline
	if [ $myline -ne 0 -a $myline -ne 1 ]; then
		let total=$total+$myline
		let nn=$nn+1
	else
		let n0=$n0+1
	fi
	#echo $total
done < $TF

echo "Total:"$total"   Num:"$nn
echo "Zero value num: "$n0

let avg=$total/$nn
echo "Avg:"$avg

# delete tmp file
rm $TF

