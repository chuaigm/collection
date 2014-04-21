#! /bin/bash

#
# ������Ч����������Ľű�
# ����ע�ͣ��������������Ŀ���
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
	echo " -t, Ŀ��SVN���ַ"
	echo " -s, ��ʼ�޶���"
	echo " -e, �����޶���"
	echo " -u, svn�ʺ�"
	echo " -p, svn����"
	echo " -h, ����"
	echo " -v, �汾��Ϣ"
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
	echo "������Ŀ��SVN���ַ!"
	exit 1
fi

if [ -z $start_revision ]; then
	echo "��������ʼ�޶���!"
	exit 1
fi

if [ -z $end_revision ]; then
	echo "��������ֹ�޶���!"
	exit 1
fi

TEMPFILE=temp.log
USERNAME=${user:-}
PASSWD=${passwd:-}

svn diff -r$start_revision:$end_revision $target $USERNAME $PASSWD > $TEMPFILE
add_lines_count=`grep "^+" $TEMPFILE|grep -v "^+++"|sed 's/^.//'|sed '/^$/d'|wc -l`
rm -fr $TEMPFILE
echo "the actually incremental source code lines = $add_lines_count"


