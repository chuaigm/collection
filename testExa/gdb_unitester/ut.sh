#!/bin/sh

# file name should be under test
ut_name=unexe
right_answer=right_answer1
tmp_result=ut.log

g++ -g sample.cpp -o $ut_name

# breakpoint in which line?
bl1=6

<<"ZZZ"
l main
ZZZ

gdb <<GDBEOF
file ./$ut_name
set logging file $tmp_result
b $bl1
r
set logging on
p a
set logging off
GDBEOF

re=`diff $tmp_result right_answer1`
if [ -z $re ]; then
	echo "--------------------"
	echo "      right!"
	echo "--------------------"
	# clean?
	rm -f ./$ut_name
	rm -f ./$tmp_result
else
	echo "--------------------"
	echo "      ERROR!"
	echo "--------------------"
fi

