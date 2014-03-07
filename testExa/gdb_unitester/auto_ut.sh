#!/bin/sh

#g++ -g sample.cpp -o $ut_exe

# file name should be under test
ut_exe=./a
right_answer=right_answer1
tmp_result=ut.log

# breakpoint in which line?
bl1=6
b_func1=add
case1a=4
case1b=5
case1c=9
case2a=1234
case2b=5432
case2c=9
case3a=204
case3b=512
case3c=9

gdb <<GDBEOF
file $ut_exe
set logging file $tmp_result
b main
b $b_func1
r
call $b_func1($case1a, $case1b)
s
set logging on
p c
set logging off
call $b_func1($case2a, $case2b)
s
set logging on
p c
set logging off
call $b_func1($case1a, $case1b)
s
set logging on
p c
set logging off
call $b_func1($case3a, $case3b)
s
set logging on
p c
set logging off
GDBEOF

re=`diff $tmp_result right_answer1`
if [ -z $re ]; then
	echo "--------------------"
	echo "      right!"
	echo "--------------------"
	# clean?
	#rm -f ./$ut_name
	rm -f ./$tmp_result
else
	echo "--------------------"
	echo "      ERROR!"
	echo "--------------------"
fi

