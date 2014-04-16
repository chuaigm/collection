
#include "stdio.h"
#include "stdlib.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "ChuaiOpenGL/SCOpenGL.h"
//启动不显示控制台
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

using namespace std;

CSCOpenGL myOpenGL;

int main()
{
	myOpenGL.MainScOpenGL();
	
	return 0;
}