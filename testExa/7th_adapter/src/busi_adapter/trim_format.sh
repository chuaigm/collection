##################################################
# ����ԭ����xxxxxDesc.cpp�ļ�������һ������
# ������������
# date: 2017-5-16
# powered by cgm
# ԭ��TID������֣�������: 1.TID���� 2.������ 3.ҵ����������
#       TID������ȫ����д�������»��߷ִʣ��ִʲ������ĸ�
#       ����ȫ��Сд�������»��߷ִ�
##################################################
#<<"XXX"

if [ $# -eq 0 ]; then
	src_file="./desc.cpp"
elif [ $# -eq 1 ]; then
	src_file=$1
else
	echo "Invalid argument!"
	printf "    Usage:\n"
	printf "        $0 (use default desc.cpp file)\n"
	printf "        $0 [config_desc.cpp file]\n"
	exit 0
fi
if [ -f $src_file ]; then
	echo "Input file=$src_file"
else
	echo "Input file does not exist!"
	exit -1
fi

tmp_file="./tmp.tmp"
target_file="./desc_trimmed.cpp"
let total_tid=0
let line_num=0
let total_line=`cat $src_file|wc -l`

rm -f $targe_file

# ����Ҫ�������ó���
grep "\s*{\(TID_.*\),{" $src_file > $target_file

sed "s///g;
" $target_file > $tmp_file
mv $tmp_file $target_file

#XXX

