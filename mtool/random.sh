#!/bin/bash

# 获取min 和 max之间的随机数
if [ $# -ne 2 ]; then
	echo Usage: $@ min max
	exit
fi

# 生成$2到$1之间的随机整数，包括$1但不包括$2本身
function random()
{
	let delta=$2-$1;
	let ran=$RANDOM*$delta;
	let retnum=$ran/32768;
	let retnum=$retnum+$1;
	echo $retnum;
}

out=$(random $1 $2+1);
echo $out;


<<"XXX"
echo "--------------------------"
echo "probability test"

start=1;
end=10;
n=$end-$start+1;
array=(0 0 0 0 0 0 0 0 0 0);
for i in {1..100};
do
	out=$(random $start $end+1);
	let array[$out]=${array[$out]}+1;
done;

for i in {1..10};
do
	echo ${array[$i]};
done


echo "--------------------------"
XXX


<<"XXX"
#1.时间法
function random()
{
    min=$1;
    max=$2-$1;
    num=$(date +%s+%N);
    ((retnum=num%max+min));
    #进行求余数运算即可
    echo $retnum;
}

out=$(random $1 $2);
echo $out
XXX

#2. 系统变量
# echo $RANDOM

#3. /dev/random urandom
# head -2000 /dev/urandom | cksum | cut -f1 -d " "

#4. cat /proc/sys/kernel/random/uuid| cksum | cut -f1 -d" "
