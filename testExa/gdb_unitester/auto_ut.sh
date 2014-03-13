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
result_report_file=./report_result.txt

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

# 1. load exe file name
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
	# 1. get total segment number
	sgn=`echo tl | awk -F '|' '{print NF}'`
	# 2. get 6 control words.
	# filename, function, breakpoint1, breakpoint2
	# user change var, check result
	cppfile=`echo $tl | cut -d '|' -f 1`
	if [ -z $cppfile ]; then
		break
	fi
	func=`echo $tl | cut -d '|' -f 2`
	eval $(echo $tl | cut -d '|' -f 3)
	eval $(echo $tl | cut -d '|' -f 4)
	user_var=`echo $tl | cut -d '|' -f 5`
	ckre=`echo $tl | cut -d '?' -f 2 | cut -d '=' -f 1`
	# 3. if the function can be called in code
	if [ -z $func ]; then
gdb << GDB_NO_FUNC
	file $ut_exe
	set height 0
	set logging file $tmp_result
	b $cppfile:$b1
	b $cppfile:$b2
	r
	set var $user_var
	c
	set logging on
	p $ckre
	set logging off
GDB_NO_FUNC
	else
gdb << GDB_FUNC_CALLED
	file $ut_exe
	set height 0
	set logging file $tmp_result
	b main
	b $cppfile:$b1
	b $cppfile:$b2
	r
	call $func
	set var $user_var
	c
	set logging on
	p $ckre
	set logging off
GDB_FUNC_CALLED
	fi
	# 4. check the answer
	expect=`echo $tl | cut -d '?' -f 2 | cut -d '=' -f 2`
	result=`head -n 1 $tmp_result | cut -d '=' -f 2`
	# 5. output a report file
	if [ $expect = $result ]; then
		printf "O|" >> $result_report_file
	else
		printf "X|" >> $result_report_file
	fi
	echo $tl >> $result_report_file
	#printf "\n"  >> $result_report_file
	# 6. clean tmp file
	rm $tmp_result

	let "ln++"
done

