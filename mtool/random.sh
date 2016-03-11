#!/bin/bash

# ��ȡmin �� max֮��������
if [ $# -ne 2 ]; then
	echo Usage: $@ min max
	exit
fi

# ����$2��$1֮����������������$1��������$2����
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
#1.ʱ�䷨
function random()
{
    min=$1;
    max=$2-$1;
    num=$(date +%s+%N);
    ((retnum=num%max+min));
    #�������������㼴��
    echo $retnum;
}

out=$(random $1 $2);
echo $out
XXX

#2. ϵͳ����
# echo $RANDOM

#3. /dev/random urandom
# head -2000 /dev/urandom | cksum | cut -f1 -d " "

#4. cat /proc/sys/kernel/random/uuid| cksum | cut -f1 -d" "
