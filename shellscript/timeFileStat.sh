#!/bin/sh
# power by chuaiGM , 2014-1-20

# var define
F1="1.txt"
F2="2.txt"
TF1="tmpfile1"
TF2="tmpfile2"
TMF="tmpres"

let total=0
let n1=0
let n2=0
let avg=0

let hour=0 # 1 hour = 3600000 ms
let mini=0 # 1 mini =   60000 ms
let senc=0 # 1 senc =    1000 ms
let msen=0
<<"ZZ"
ZZ
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

# data select
while read myline
do
	# method 1
	#let hour=`echo $myline | awk '{sub(/^.*--/,"");sub(/:.*$/,"");print}'`
	#let mini=`echo $myline | awk -F ":|:" '{print $2}'`
	#let senc=`echo $myline | awk '{sub(/^.*:/,"");sub(/\..*$/,"");print}'`
	#let msen=`echo $myline | awk '{sub(/^.*\./,"");sub(/$/,"");print}'`
	# method 2
	#echo $myline | cut -d '-' -f 3 | cut -d ':' -f 1
	#echo $myline | cut -d ':' -f 2
	#echo $myline | cut -d ':' -f 3 | cut -d '.' -f 1
	#echo $myline | cut -d '.' -f 2
	# method 3
	let hour=`echo $myline | sed "s/^.*--\(.*\):.*:.*$/\1/"`
	let mini=`echo $myline | sed "s/^.*:\(.*\):.*$/\1/"`
	let senc=`echo $myline | sed "s/^.*:\(.*\)\..*$/\1/"`
	let msen=`echo $myline | sed "s/^.*\.\(.*\)$/\1/"`
	let hour=$hour*3600000
	let mini=$mini*60000
	let senc=$senc*1000
	let msen=$hour+$mini+$senc+$msen
	echo $msen >> $TF1
	let n1=$n1+1
done < $F1

while read myline
do
	let hour=`echo $myline | sed "s/^.*--\(.*\):.*:.*$/\1/"`
	let mini=`echo $myline | sed "s/^.*:\(.*\):.*$/\1/"`
	let senc=`echo $myline | sed "s/^.*:\(.*\)\..*$/\1/"`
	let msen=`echo $myline | sed "s/^.*\.\(.*\)$/\1/"`
	let hour=$hour*3600000
	let mini=$mini*60000
	let senc=$senc*1000
	let msen=$hour+$mini+$senc+$msen
	echo $msen >> $TF2
	let n2=$n2+1
done < $F2

if [ $n1 -ne $n2 ]; then
	echo "[WARNING]: the file lines are inconsistent!"
	rm $TF1 $TF2
	exit 0
fi

# calculate
let i=1
while [ $i -le $n1 ]
do
	# caution " and ' are quite different
	# or: awk "{if(NR==$i)print}" $TF1
	d1=`sed -n "$i p" $TF1`
	d2=`sed -n "$i p" $TF2`
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
rm -f $TF1 $TF2
mv $TMF result.txt

