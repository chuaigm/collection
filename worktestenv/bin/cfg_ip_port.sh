#!/bin/sh
# powered by chuaiGM!
# 2014-09-24

# this file will collect all the configure info
# rules here :
#     1. the first line must be a absolute path and cfg file name
#     2. following line are cfg info need to change(grep by keywords)
#     3. different cfg files need to be separated by a blank line
TAG_CFG="./collection_cfg.ini"

# change all these config files, or just change part of them,
# that's depends on you, just change this condition
if [ 1 -eq 1 ]; then
	cd ..
	THIS_PATH=`pwd`
	CFG_FILES=`find $THIS_PATH -name "*.ini" | grep -v $TAG_CFG`
	cd -
else
	CFG_FILES="
	../busgroup/ma_bus/conf/ma_bus.ini
	../busgroup/fr_bus/conf/fr_bus.ini
	"
fi
KEY_WORDS="
ArbListenSvc
FtListenSvc
DevListenSvc
RestoreListenSvc
FtConnectPoint
ArbConnectPoint
SwitchAConnect
SwitchBConnect
FrontSwitchAPoint
FrontSwitchBPoint
MatchSwitchAPoint
MatchSwitchBPoint
QuotSwitchAPoint
QuotSwitchBPoint
ExternSvc
InterSvc
"

# if no configure collect file here, then, find the 
# configure file in each module, and generate the file.
if [ ! -f $TAG_CFG ]; then
echo "-----------------------------------------------"
echo "[Tips: there is no configure collect file here]"
echo "[      so, now will generate ...]"
echo "-----------------------------------------------"
for X in $CFG_FILES
do
	echo $X | tee -a $TAG_CFG
	for Y in $KEY_WORDS
	do
		grep -w $Y $X | tee -a $TAG_CFG
	done

	echo "" | tee -a $TAG_CFG
done
# if there is configure collect file in this path,
# means that now will change target according to this file
else 
echo "-----------------------------------------------"
echo "[Tips: now, will change target file ...]"
echo "-----------------------------------------------"
# read file by line
flag_file=1
file_path=""
# if you want to modify the target file, 
# read the reference at beginning of this file
while read myline
do
	#echo "$myline"
	if [[ -z $myline ]]; then
		# next line will be a file path 
		flag_file=1
	else
		if [ $flag_file -eq 1 ]; then 
			file_path=$myline
			echo ""
			echo $myline
			flag_file=0
		else
			for KW in $KEY_WORDS
			do
				re_grep=`echo $myline | grep -w $KW`
				if [[ $re_grep != "" ]]; then
					re2=`grep -w $KW $file_path`
					if [[ $re_grep != $re2 ]]; then
						# <> method may have some problem
						#awk '/'$KW'/ {print "'$myline'"; next} {print}' $file_path 1<>$file_path
						# core cmd
						awk '/\<'$KW'\>/ {print "'$myline'"; next} {print}' $file_path >> tmp_file_cgm
						mv tmp_file_cgm $file_path
						echo "[changing:]" $myline
					fi
				fi
			done
		fi
	fi
done < $TAG_CFG
fi
