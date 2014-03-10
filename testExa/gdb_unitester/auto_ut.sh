#!/bin/sh
# powerd by chuaiGM 2014-3-10
#tips:
# 1. make your executable file first, don't forget use -g
#    like: g++ -g sample.cpp -o $ut_exe
# 2. in gdb enviroment, set height 0, so, it's important

# file name which you wanna test
ut_exe=./a
ut_scheme=./target.ut
right_answer=right_answer1
tmp_file=./tmp.tmp
tmp_result=re.log

# load target file
# $1 : patch of the scheme file
# $2 : line number will be processed
load_cfg_file()
{
	echo test_load_file
	#str=`sed -n "1 p" $1`
	#eval $(grep exe_file $1)
	eval $(sed -n "1 p" $1)
	ut_exe=$exe_file
}

# 1. load cfg
eval $(sed -n "1 p" $ut_scheme)
ut_exe=$exe_file
# 2. from to
ln=2 # do it form line 2
lines=`cat $ut_scheme | wc -l`
# 3.
while (($ln<=$lines))
do
	# tl means "this line"
	tl=`sed -n "$ln p" $ut_scheme`
	# 1. get file and function name, b this
	ff=`echo $tl | cut -d ',' -f 1`
	func=`echo $ff | cut -d ':' -f 2`
	if [ -z $ff ]; then
		break
	fi
	echo $ff -----$func
<<ZZZ
gdb <<GDB_CMD 
file $ut_exe
GDB_CMD 
ZZZ
	let "ln++"
done

exit 0

load_cfg_file $1
echo $ut_exe

exit 0

# breakpoint in which line?
bl1=6
b_func1=sp_mod
case1a=4
case1b=5
case1c=9
case2a=1234
case2b=5432
case2c=9
case3a=204
case3b=512
case3c=9

gdb <<GDBCMD
file $ut_exe
set height 0
set logging file $tmp_result
b main
b $b_func1
r
call $b_func1(100)
s
s
set logging on
p re
set logging off
call $b_func1(100)
s
s
set logging on
p re
set logging off
call $b_func1(100)
s
s
set logging on
p re
set logging off
call $b_func1(100)
s
s
set logging on
p re
set logging off
call $b_func1(100)
s
s
set logging on
p re
set logging off
call $b_func1(100)
s
s
set logging on
p re
set logging off
GDBCMD

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

