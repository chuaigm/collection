#!/usr/bin/python

import os
import re
import sys

i_file=open('time-03f3','r')
o_file=open('out.txt','w')

tmplast='0'
counter=0

for line in i_file.readlines():
	if tmplast != line[17]:
		str='%6d' %counter
		tmpl='  ' + str + '  ' + line
		o_file.write(tmpl)
		counter=0
		tmplast = line[17]
	else:
		counter=counter+1
		tmplast = line[17]


i_file.close()
o_file.close()
