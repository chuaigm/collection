
if [ -z $1 ]; then
	echo Usage:$0 [log message]
	exit 0
fi
git commit -a -m "$1"
sleep 1
git push cgm master

