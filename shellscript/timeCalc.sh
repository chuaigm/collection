#!/bin/sh
# power by chuaiGM , 2014-1-20

function getTiming(){
	start=$1
	end=$2
	start_s=`echo $start | cut -d '.' -f 1`
	start_ns=`echo $start | cut -d '.' -f 2`
	end_s=`echo $end | cut -d '.' -f 1`
	end_ns=`echo $end | cut -d '.' -f 2`
	time_micro=$(( (10#$end_s-10#$start_s)*1000000+(10#$end_ns/1000-10#$start_ns/1000) ))
	#time_ms=`expr $time_micro/1000 | bc ` # not good way
	#time_ms=`echo "scale=2; $time_micro/1000" | bc `
	let time_ms=$time_micro/1000
	#echo "$time_micro microseconds"
	echo "$time_ms ms"
}

el=3
echo "begin sleep $el s"
begin_time=`date +%s.%N`
sleep $el
end_time=`date +%s.%N`
echo "finish"

b_time=`date +%s.%N`
getTiming $begin_time $end_time
e_time=`date +%s.%N`
getTiming $b_time $e_time

