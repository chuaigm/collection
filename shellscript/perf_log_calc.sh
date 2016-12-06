#!/usr/bin/bash
# Powered by cgm
# This script is build for calculate the performance log
# base on the valid number is continuous, and between zero s
# here, kick the first valid number, and the last
# 只适用于开头一堆0，中间的有效数字不出现0，最后并以0结束的数值统计
# 去掉了开头第一个数字与最后一个数字
#set -x

# var define
TF="tmp.tmp"
FILE="./perf.log"
let total=0
let nn=0
let avg=0
let last_num=0

# data select
sed -n 's/.*|80001|\(.*\)|80002|.*/\1/p' $FILE > $TF

# set some flag
let f_is_first=0
let f_is_last=0

# calculate
while read myline
do
	#echo "line:"$myline
	#if [ $myline -ne 0 -a $myline -ne 1 ]; then
	if [ $f_is_first -eq 0 ]; then
		if [ $myline -ne 0 ]; then
			let f_is_first=1
			#skip the first not 0 number
		fi
	# begin the first not 0 number
	elif [ $f_is_last -eq 0 ]; then
		if [ $myline -ne 0 ]; then
			let total=$total+$myline
			let nn=$nn+1
			let last_num=$myline
		else # 0 appear again
			let f_is_last=1
			let total=$total-$last_num
			let nn=$nn-1
			#remove the last number
		fi
	fi
	#echo $total
done < $TF

echo "--------------------"
echo "Total:"$total"   Num:"$nn
#echo "Zero value num: "$n0
let avg=$total/$nn
echo "Avg:"$avg
echo "--------------------"

# delete tmp file
rm $TF

