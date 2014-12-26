# ���ۼ�����
# ���ļ�����: GB2312,GBK

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
	# ����Ϊ��׼�нﵥ��
	echo $line | awk -F '|' '{printf "%s_%s:%s: %.2f\n",$1,$2,$3,500.0*$6/($4*$5)}' >> $tmp_file
	# ������
	#echo $line | awk -F '|' '{printf "%s|%s|%s|%s\n",$1,$2,$3,$4}'

	let i=$i+1
done

echo "--------------------------------------------------"
echo " ��λ�� Ԫ/�н� (Ԫ/500g) "
echo "--------------------------------------------------"
echo "        ����              |   ����  "
# �Եڶ����������� -n ��ֵ���� -k �ڼ��У� -t ָ���ָ���
sort -n -k 2 $tmp_file
echo "--------------------------------------------------"
# �Եڶ����������� -n ��ֵ���� -k �ڼ��У� -t ָ���ָ���
sort -k 2 -t : $tmp_file
echo "--------------------------------------------------"

rm $tmp_file

