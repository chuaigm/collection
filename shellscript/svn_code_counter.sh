#! /bin/bash

#
# 计算有效变更代码量的脚本
# 包括注释，但不包括新增的空行
#

version() {
	OS=`uname -o`
	echo "Source_counter ($OS) 0.0.1"
	echo " tony bai (http://tonybai.com)"
}

usage() {
	echo "usage: source-counter [-t SVN_REPOSITORY_URL] [-s START_REVISION]"
	echo " [-e END_REVISION] [-u USER_NAME]"
	echo " [-p PASSWD]"
	echo " source-counter [-v|-h]"
	echo
	echo " -t, 目标SVN库地址"
	echo " -s, 起始修订号"
	echo " -e, 结束修订号"
	echo " -u, svn帐号"
	echo " -p, svn密码"
	echo " -h, 帮助"
	echo " -v, 版本信息"
}

if [ $# -lt 2 ]; then
	usage
	exit 1
fi

while getopts "t:s:e:u:p:vh" opt; do
	case $opt in
		t) target=$OPTARG;;
		s) start_revision=$OPTARG;;
		e) end_revision=$OPTARG;;
		u) user=$OPTARG;;
		p) passwd=$OPTARG;;
		v) version; exit 1;;
		h) usage; exit 1;;
	esac
done

if [ -z $target ]; then
	echo "请输入目标SVN库地址!"
	exit 1
fi

if [ -z $start_revision ]; then
	echo "请输入起始修订号!"
	exit 1
fi

if [ -z $end_revision ]; then
	echo "请输入终止修订号!"
	exit 1
fi

TEMPFILE=temp.log
USERNAME=${user:-}
PASSWD=${passwd:-}

svn diff -r$start_revision:$end_revision $target $USERNAME $PASSWD > $TEMPFILE
add_lines_count=`grep "^+" $TEMPFILE|grep -v "^+++"|sed 's/^.//'|sed '/^$/d'|wc -l`
rm -fr $TEMPFILE
echo "the actually incremental source code lines = $add_lines_count"


