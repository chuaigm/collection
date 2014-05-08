#include <stdio.h>
#include <string.h>
#include "math.h"
//#include "cgmtoolbox.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

typedef struct {
	int a;
	char b;
	short c;
} AAA;
void func(const int i)
{
	int ta = i;
};

template <int length>
class BA
{
public:
	BA(){
		memset(buf,0,length+1);
	}
	char buf[length+1];
};
typedef BA<8> Tba;

int main()
{
#if 1

#endif	
#if 1
	// 2014-5-8
	AAA a[7];
	AAA *p=a;
	char *pc=new char[7];
	AAA *pn=new AAA[7];
	int *pi=new int[7];
	printf("sizeof(a)=%d\n", sizeof(a));
	printf("sizeof(*p)=%d\n", sizeof(p));

	printf("pc=%p\n", pc);
	for(int i=8; i>0;i--) {
	printf("%02X_", *((unsigned char*)(pc)-i));
	}

	printf("\n");
	printf("pn(AAA)=%p\n", pn);
	for(int i=8; i>0;i--) {
	printf("%02X_", *((unsigned char*)(pn)-i));
	}
	printf("\n");

	printf("pi=%p\n", pi);
	for(int i=8; i>0;i--) {
	printf("%02X_", *((unsigned char*)(pi)-i));
	}
	printf("\n");
#endif	

#if 0
	int i=0;
	while(i<5)
	{
		printf("%d\n",i++);
		system("stty raw -echo");
		printf("Press key...");
		getchar();
		system("stty -raw echo");
		printf("A\n");
	}
#endif	

#if 0
	CGM_VER_INFO;

	printf("------------------------------\n");
	char st[128]={1};
	memset(st,0, sizeof(st));
	printf("%p\n", st);
	printf("%p\n", &st);
	const char* cs = new char[128];
	printf("%p\n", cs);
	printf("%p\n", &cs);
	printf("------------------------------\n");
#endif	

#if 0
	fstream m_file;
	m_file.open("./aaaaa.txt", ios::out|ios::app);
	char buf[128];
	char tmp[]="大豆";
#endif	

#if 0
	char a=0x9F;
	printf("%02X \n", a+1);
	printf("%02X \n", (unsigned char)a+1);
	printf("------------------------------\n");

	double f=2558.625;
	printf("%.2f\n", f);
#endif

}

