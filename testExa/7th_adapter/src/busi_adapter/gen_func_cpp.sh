##################################################
# 根据原来的xxxxxDesc.cpp文件，生成数据包处理函数接口
# 依赖原FTCPPackageDesc.cpp中的TID以及包体内容的定义
# date: 2017-5-16
# powered by cgm
# 用法: 本脚本只根据FTCPPackageDesc.cpp文件,进行生成
#       数据包处理函数生成后，可手动复制粘贴到目标文件中
#       对于函数的具体实现，可以手动进一步修改    
##################################################
#<<"XXX"

# 定义对象符号，方便修改以及统一替换
#XX="TEST"
XX="XXXX"

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
target_file="./handle_"$XX"_cpp_gen_by_tool.cpp"
let total_tid=0
let line_num=0
let total_line=`cat $src_file|wc -l`

rm -f $target_file

# 对于一个包中包含多个相同域的结构，声明在这个集合里
# 下面生成的时候，判断这个包名，是否在这个集合中
# 是，则最后一个field需要被定义为变长结构
# 这里填写去掉TID前缀的大写表达
multiple_uint="
REQTRADERINSERTORDERS
RSPTRADERINSERTORDERS
"

echo "//////////////////////////////////////////////////" >> $target_file
echo "// `date +%Y%m%d`" >> $target_file
echo "// generated by [$0] from [$src_file]" >> $target_file
echo "//////////////////////////////////////////////////" >> $target_file
echo "" >>  $target_file
#echo "#include "handle_"$XX"_""  >>  $target_file
printf "#include \"handle_"$XX.h"\""  >> $target_file
echo "" >>  $target_file
echo 'namespace nm_busi'              >> $target_file
echo '{'                              >> $target_file
echo ""                               >> $target_file

while read aline
do
#	echo "line content: "$aline
	# 这行有多种写法
	tid=`expr "$aline" : '\s*{\(TID_.*\),{'`
	if [[ "$tid" != "" ]]; then
		# 对于函数名、包名，未来应该还有进一步的裁剪以及分词的方法，
		# TODO 以后再补充
		# 下面这行，是将下划线'-'前的内容全部去除掉
		# 1.
		busi_name=`echo ${tid#*_}`
		# 大写转小写
		busi_name_lower=`echo $busi_name | tr [A-Z] [a-z]`
#		echo $busi_name
		# 2.
		cmnt=`echo "$aline" | cut -d '"' -f 2`
#		echo $cmnt
		# 3.
		# 获得域的数量
		field_num=`expr "$aline" : '.*,\([0-9]*\),'`
#		echo $field_num

		# 4.
		# 循环创建sed所用的正则表达式序列
		one_regl='CFld\(.*\)::.*'
		regx=".*"
		for((i=0;i<$field_num;i++));
		do
			regx=${regx}${one_regl}
		done
#		echo $regx
		# 5.
		# 循环获取域名
		for((i=1;i<=$field_num;i++));
		do
			fld_name[$i]=`echo $aline | sed 's/'"$regx"'/\'"$i"'/g'`
#			echo ${fld_name[$i]}
		done

		# 开始拼包体结构
		printf "// 处理 $cmnt\n" >> $target_file
		printf "// 维护者: ----    时间: 2017-05-18\n" >> $target_file
		printf "int32_t busi_$XX::handle_$busi_name_lower($busi_name_lower* pkg,\n" >> $target_file
		printf "            int16_t idx, void* udata, uint16_t vm_id, uint32_t cid);\n" >> $target_file
		printf "{\n" >> $target_file
		# 这里开始需要判断两种情况，即，这个包的定义是否包含变长结构
		re=`echo $multiple_uint | grep "\<$busi_name\>"`
#		if [[ $re = "" ]]; then
		# 搜索未空，表示此包不需要定义变长结构，直接遍历编写
#			for((i=1;i<=$field_num;i++));
#			do
#			printf "    fld_${fld_name[$i]}\t\t${fld_name[$i]};\n" >> $target_file
#			done
#		else
		# 搜索不为空，表示此包需要将最后一个域定义为变长结构
#			for((i=1;i<=$field_num-1;i++));
#			do
#			printf "    fld_${fld_name[$i]}\t\t${fld_name[$i]};\n" >> $target_file
#			done
#			printf "    uint32_t\t\tdynamic_len;\n" >> $target_file
#			printf "    fld_${fld_name[$field_num]}\t\t${fld_name[$field_num]}[];\n" >> $target_file
#		fi
		printf "    pub_data((char*) &send_pkg, sizeof($busi_name_lower),$busi_name_lower::TID,vm_id);\n" >> $target_file
		printf "}\n" >> $target_file
		printf "\n" >> $target_file
		# TID计数
		let total_tid++
	fi

	let line_num++
	let percent=0
	percent=`awk -v x=$line_num -v y=$total_line 'BEGIN{printf "%02d",x/y*100}'`
	printf " [In process]:$line_num/$total_line ($percent%%)\r"

done < $src_file

echo ""        >> $target_file
echo "// There are $total_tid package define in total" >> $target_file
echo "// Attention! This number is reliable depens on nobody modify this file manually!" >> $target_file
echo ""        >> $target_file
echo '#endif'  >> $target_file
echo ""        >> $target_file

#XXX
