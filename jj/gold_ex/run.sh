
# 配置python3的路径 
pyexe="/usr/local/bin/python3"

# 被定时任务调用的脚本尽量使用绝对路径
pth="/home/cgm/git_coll/jj/gold_ex"
# 怎么可能用pwd呢，调用此脚本的时候，路径是不确定的
#pth=`pwd`
cd $pth

#output_log="$pth/../message_log/unsent.log"

# 按年存储文件
year=`date +'%Y'`
gold_price_file="$pth/$year""_gold_price.txt"

# 执行抓取今天的金价
$pyexe ./get_bosera_gold_price.py | tee -a $gold_price_file

# 使用sort，按日期对金价记录去重
sort -u $gold_price_file -o $gold_price_file

# 调用数据分析脚本


