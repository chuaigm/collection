# power by cgm
# data: 2014-01-09
# info: check *.con file is changed or not in a period of time

# attention, FILE_PATH must be a file' path, not directory
FILE_PATH="../store/*.con"
INFO_PATH="../log/quatation_status.log"
let TIME_BOND=6

old_size=0
old_time=0

#clean old log file
if [ -f $INFO_PATH ]; then
	rm $INFO_PATH
fi

while true
do
	sleep 1
	string=`ls -lt $FILE_PATH | head -n 1`
	size=`echo $string | awk '{print $5}'`
	time=`echo $string | awk '{print $8}'`
echo $size $time #debug use
	if [ x$old_size = x$size -a x$old_time = x$time ]; then
		let TIME_BOND=$TIME_BOND-1
echo $TIME_BOND #debug use
	if [ $TIME_BOND -le 0 ]; then
		echo "quotation_backup_succeed"
		echo "quotation_backup_succeed" > $INFO_PATH
		exit 0
	fi
	else
		old_size=$size
		old_time=$time
		let TIME_BOND=6
echo $TIME_BOND #debug use
	fi
done

