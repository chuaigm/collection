#!/bin/sh
# power by chuaiGM

if [ $# -ne 3 ] ; then
	echo "Usage like:"
	echo "$0 [old_version] [new_version] [svn_local_path]"
	exit 1
fi

if [ $1 -gt $2 ] ; then
	echo "Usage like:"
	echo "$0 [old_version] [new_version] [svn_local_path]"
	exit 1
fi

OLD_VERSION=$1
NEW_VERSION=$2

# path for patch files directory
WORK_PATH=`pwd`
# local svn directory ready for export(modify here)
SVN_PATH=$3

cd ${SVN_PATH}

echo "start update ......"
svn up
echo "end update"

SVN_URL=`svn info | grep "URL" | awk 'NR==1' | awk -F " " '{print $2}'`

echo "start version diff"
DIFF_NUM=`svn diff -r ${OLD_VERSION}:${NEW_VERSION} . | grep "^Index:*" | awk -F " " '{print $2}' | wc -l`

if [ ${DIFF_NUM} -ne 0 ]; then
	DIFF_LIST=`svn diff -r ${OLD_VERSION}:${NEW_VERSION} . | grep "^Index:*" | awk -F " " '{print $2}'`
	for file in ${DIFF_LIST} ; do
		echo ${file}
		FILE_NAME=`basename ${file}`
		FOLDER_NAME=`dirname ${file}`
		if test ! -e "${WORK_PATH}/version_patch/${OLD_VERSION}_${NEW_VERSION}/${FOLDER_NAME}"; then
			mkdir -p ${WORK_PATH}/version_patch/${OLD_VERSION}_${NEW_VERSION}/${FOLDER_NAME}
		fi
		cd ${WORK_PATH}/version_patch/${OLD_VERSION}_${NEW_VERSION}/${FOLDER_NAME}/
		svn export -r ${NEW_VERSION} ${SVN_URL}/${file}
		cd ${WORK_PATH}
	done
	echo "OK"
else
	echo "nodiff"
fi
