##################################################
# 根据原来的xxxxxDesc.cpp文件，按照一定规则，
# 重新整理命名
# date: 2017-5-16
# powered by cgm
# 原则：TID后的名字，将决定: 1.TID名字 2.包名字 3.业务处理函数名字
#       TID名保持全部大写，并用下划线分词，分词不超过四个
#       域名全部小写，采用下划线分词
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

# 把需要的行先拿出来
grep "\s*{\(TID_.*\),{" $src_file > $target_file

sed "s///g;
" $target_file > $tmp_file
mv $tmp_file $target_file

#XXX

