#!/bin/sh
# powered by chuaiGM!
# 2014-09-24

TAG_CFG="./collection_cfg.ini"

BUS_CFG_DIR="
../busgroup/ma_bus/conf/ma_bus.ini
../busgroup/fr_bus/conf/fr_bus.ini
"
KW_bus="
FtListenSvc
DevListenSvc
RestoreListenSvc
FtConnectPoint
ArbConnectPoint
"
# if no configure collect file here, then, find the 
# configure file in each module, and generate the file.
if [ ! -f $TAG_CFG ]; then
echo "[Tips: there is no configure collect file here]"
echo "[      so, now will generate ...]"
for X in $BUS_CFG_DIR
do
	echo $X | tee -a $TAG_CFG
	for Y in $KW_bus
	do
		grep $Y $X | tee -a $TAG_CFG
	done

	echo "" | tee -a $TAG_CFG
done
# if there is configure collect file in this path,
# means that now will change target according to this file
else 
echo "[Tips: now, will change target file ...]"
# read file by line
flag_file=1
file_path=""
while read myline
do
	#echo "$myline"
	if [ -z $myline ]; then
		echo 1
		# next line will be a file path 
		flag_file=1
	else
		echo 2
		if [ $flag_file -eq 1 ]; then 
			file_path=$myline
			flag_file=0
			break
		fi
	fi
done < $TAG_CFG
fi

