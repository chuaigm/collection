
<<"XXX"
n=3
cmd=echo
cmd="$cmd \$$n"
eval $cmd
XXX

<<"XXX"
echo "sdfasdf"
echo "Input a string store in YY"
read YY
echo $YY

if [[ $1 = "all" ]]; then
	echo yessss
fi
XXX

<<"XXX"
AA="home/chuai/test.cpp"

TA=${AA%/*}
echo $TA
TB=${AA##*/}
echo $TB
XXX
