#!/bin/python

import os
import re
import sys
"""
print "scriptName:", sys.argv[0]
for i in range(1, len(sys.argv)):
	print "para", i, sys.argv[i]

exit()
"""
flag=0
if len(sys.argv) == 2 and sys.argv[1] in ['l','m']:
	flag=sys.argv[1]
else:
	print "Usage:"
	print sys.argv[0]," l or m"
	exit()

if flag == 'm':
	print "mmmm"
	mf_name="./1.txt"
	befmodmf="1.txt_befmod"
	f_target='mmmm'
elif flag == 'l':
	print "llll"
	mf_name="./2.txt"
	befmodmf="2.txt_befmod"
	f_target='llll'

os.rename(mf_name, befmodmf)
i_file=open(befmodmf,'r')
o_file=open(mf_name,'w')

pattern='^.*'+f_target+'.*'

for line in i_file.readlines():
	#if re.match('^.*mmmm.*', line):
	if re.match(pattern, line):
		print mf_name ,"has been mod"
		tmps=re.split(f_target,line)
		tmpl=tmps[0]+"oooo"+tmps[1]
		o_file.write(tmpl)
	else:
		o_file.write(line)

i_file.close()
o_file.close()


