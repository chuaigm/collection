#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

void func()
{
	printf("don't want 2\n");
};

int main()
{
	char str[64]={0};
	printf("\twant 1\n");
	
	FILE *fp=freopen("/dev/null","w",stdout);
	func();
	FILE *fpp=popen("tty","r");
	fgets(str,sizeof(str),fpp);
	str[strlen(str)-1]=0;
	freopen(str,"w",fp);

	printf("\twant 3\n");
}
