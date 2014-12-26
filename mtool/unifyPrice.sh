# 单价计算器
# 本文件编码: GB2312,GBK

# config file name
ini_file="./unit_price.ini"
# seperator
sp="----------"
# tmp file
tmp_file="unitp_tmp_file"

if [ ! -f $ini_file ]; then
	echo "Error: need [$ini_file] file at local directory!"
	exit 0
fi
<<"XXX"
bool_utf=`sed -n 1p $ini_file | cut -d '=' -f 2`
if [ $bool_utf = 1 ]; then
	disp_code=utf8
else
	disp_code=gbk
fi
XXX

let i=2
while true
do
	#echo $i
	line=`sed -n "$i p" $ini_file`
	if [[ $line = "" ]]; then
		break
	fi
	# 换算为标准市斤单价
	echo $line | awk -F '|' '{printf "%s_%s:%s: %.2f\n",$1,$2,$3,500.0*$6/($4*$5)}' >> $tmp_file
	# 测试用
	#echo $line | awk -F '|' '{printf "%s|%s|%s|%s\n",$1,$2,$3,$4}'

	let i=$i+1
done

echo "--------------------------------------------------"
echo " 单位： 元/市斤 (元/500g) "
echo "--------------------------------------------------"
echo "        名称              |   单价  "
# 对第二列排序，升序 -n 数值排序， -k 第几列， -t 指定分隔符
sort -n -k 2 $tmp_file
echo "--------------------------------------------------"
# 对第二列排序，升序 -n 数值排序， -k 第几列， -t 指定分隔符
sort -k 2 -t : $tmp_file
echo "--------------------------------------------------"

rm $tmp_file

