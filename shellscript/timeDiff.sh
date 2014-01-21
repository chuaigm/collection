#!/bin/sh
# power by chuaiGM , 2014-1-20

# var define
F1="1.txt"
F2="2.txt"
TMF="tmpres"

let total=0
let n1=0
let avg=0

drawFunc_F() {
# %5s can be change, circumstances alter cases
	printf "%5s" "$1" | tee -a $TMF
	printf ":" | tee -a $TMF
# change the scale by this
	divi=100
	let tmp=$1/$divi
	while [ $tmp -ge 0 ]
	do
		printf "|" | tee -a $TMF
		let tmp=$tmp-1
	done
	printf "\n" | tee -a $TMF
}

n1=`cat $F1 | wc -l`
if [ $n1 -ne `cat $F2 | wc -l` ]; then
	echo "[WARNING]: the file lines are inconsistent!"
	exit 0
fi

# calculate
let i=1
while [ $i -le $n1 ]
do
	# caution " and ' are quite different
	d1=`sed -n "$i p" $F1 | awk -F '[-:.]' '{$9=$3*3600000+$4*60000+$5*1000+$6;print $9}'`
	d2=`sed -n "$i p" $F2 | awk -F '[-:.]' '{$9=$3*3600000+$4*60000+$5*1000+$6;print $9}'`
	let det=$d2-$d1
if [ 0 -eq 1 ]; then
	echo $det | tee -a $TMF
else
	drawFunc_F $det
fi
	let total=$total+$det

	let i=$i+1
done

echo "----------------------------------------" | tee -a $TMF

let avg=$total/$n1
drawFunc_F $avg
echo "Avg:"$avg | tee -a $TMF
echo "Total:"$total"   Num:"$n1 | tee -a $TMF

# delete tmp file
mv $TMF result.txt

