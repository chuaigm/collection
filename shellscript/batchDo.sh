
iDir=./input
oDir=./output

tfiles=`cd $iDir && ls *.tar.gz`

for X in $tfiles
do
	echo $X 
	rm -rf ./testdir
	#echo $X | cut -d . -f 1
	tar -xzf $iDir/$X
	mv ./testdir/trade*.dat ./
	./1python.py
	mv ./trade*.dat ./testdir
	name=`echo $X | cut -d . -f 1`
	tar -cf $name.tar ./testdir
	gzip ./$name.tar
	mv ./$X output
	rm -rf ./testdir
done

