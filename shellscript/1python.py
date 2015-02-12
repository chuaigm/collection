#!/bin/python
import os

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


if __name__ == "__main__":
	func()

