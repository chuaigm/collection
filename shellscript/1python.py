#!/bin/python
import os
import re

def func():
	ffrom=open("./trade20150212.dat","r")
	fto=open("./tttttt","w")
	while True:
		l = ffrom.readline()
		if not l:
			break
		if '1111111111' in l:
			tmp = "9999999999"
		else:
			tmp = l
		fto.write(tmp)
	os.system("mv ./tttttt trade20150212.dat")

def testf():
	tt="trade20100503.dat"
	tr=tt[5:13]
	print(tr)

if __name__ == "__main__":
	testf()

