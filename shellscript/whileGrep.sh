#!/bin/sh

let ta=0;

echo "while start!"
while true
do
	FLAG=`grep "xxx" test.log`
	if [ x"$FLAG" != x ]; then  # 这里只是想做一个不为空的判断
		break
	fi
	printf ">>$ta"
	sleep 1
	let ta=$ta+1
	if [ "$ta" = 10 ]; then
		let ta=0
		printf "\n"
	fi
done
echo "while End"
