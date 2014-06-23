#include <stdio.h>
#include <string.h>
#include "math.h"
//#include "cgmtoolbox.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>

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

enum { _MAX_SIZE=8,
	   _ttt
	 };

int main()
{
#if 0
	// 枚举
	printf("--%d\n", _MAX_SIZE);	
	printf("--%d\n", _ttt);	
#endif

#if 0
	//生病时，国庆微信发的讨论
	vector<int> c;
	c.push_back(1);
	c.push_back(2);
	vector<int>::iterator it=c.end()--;
	cout<<*it<<endl;
#endif

#if 0
	// 2014-5-9 map op
	multimap<int, string> mmpp;
	typedef multimap<int, string>::iterator mmitr;
	typedef multimap<int, string>::reverse_iterator rmmitr;
	// insert
	// 默认升序
	mmpp.insert(multimap<int,string>::value_type(1,"aaa"));
	mmpp.insert(multimap<int,string>::value_type(1,"bbb"));
	mmpp.insert(multimap<int,string>::value_type(1,"ccc"));
	mmpp.insert(multimap<int,string>::value_type(2,"ddd"));
	mmpp.insert(multimap<int,string>::value_type(2,"eee"));
	mmpp.insert(multimap<int,string>::value_type(2,"fff"));
	mmpp.insert(multimap<int,string>::value_type(3,"ggg"));
	mmpp.insert(multimap<int,string>::value_type(3,"hhh"));
	mmpp.insert(multimap<int,string>::value_type(3,"iii"));
	// output
	cout<<"------------------------------"<<endl;
	cout<<" size="<<mmpp.size()<<endl;
	cout<<"[Output a multimap]:"<<endl;
	for(mmitr iter=mmpp.begin();iter!=mmpp.end();) {
		cout<<setw(3)<<iter->first<<" : "<<iter->second<<endl;
		++iter;
	}
	cout<<"------------------------------"<<endl;
	// erase
	for(mmitr iter=mmpp.begin();iter!=mmpp.end();) {
		if( iter->first % 2==0){
			mmpp.erase(iter++);
		} else {
			++iter;
		}
	}
	// reverse_iterator 
	cout<<"------------------------------"<<endl;
	cout<<"[reverse_iter Output]:"<<endl;
	for(rmmitr iter=mmpp.rbegin();iter!=mmpp.rend();) {
		cout<<setw(3)<<iter->first<<" : "<<iter->second<<endl;
		++iter;
	}
	cout<<"------------------------------"<<endl;
	// find a key
	int sfkey=3;
	mmitr finditer=mmpp.find(sfkey);
	cout<<finditer->first<<" : "<<finditer->second<<endl;
	cout<<finditer->first<<" : "<<(finditer)->second<<endl;
#endif	

#if 0
	// 2014-5-9 map op
	map<int, string> mpp;
	typedef map<int, string>::iterator mitr;
	typedef map<int, string>::reverse_iterator rmitr;
	// insert
	#if 1
	// 默认升序
	mpp.insert(map<int,string>::value_type(1,"aaa"));
	mpp.insert(map<int,string>::value_type(9,"iii"));
	mpp.insert(map<int,string>::value_type(2,"bbb"));
	mpp.insert(map<int,string>::value_type(4,"ddd"));
	mpp.insert(map<int,string>::value_type(3,"ccc"));
	mpp.insert(map<int,string>::value_type(7,"ggg"));
	mpp.insert(map<int,string>::value_type(5,"eee"));
	mpp.insert(map<int,string>::value_type(6,"fff"));
	mpp.insert(map<int,string>::value_type(8,"hhh"));
	#else
	for(int i=1; i<=9; ++i){
		char tmpc='a'+i-1;
		string tmps;
		tmps+=tmpc;
		mpp.insert(map<int,string>::value_type(i,tmps));
	}
	#endif
	// output
	cout<<"------------------------------"<<endl;
	cout<<" size="<<mpp.size()<<endl;
	cout<<"[Output a map]:"<<endl;
	for(mitr iter=mpp.begin();iter!=mpp.end();) {
		cout<<setw(3)<<iter->first<<" : "<<iter->second<<endl;
		++iter;
	}
	cout<<"------------------------------"<<endl;
	// erase
	#if 1
	for(mitr iter=mpp.begin();iter!=mpp.end();) {
		if( iter->first % 2==0){
			mpp.erase(iter++);
		} else {
			++iter;
		}
	}
	#else
	// may crash ex
	for(mitr iter=mpp.begin();iter!=mpp.end();++iter) {
		if( iter->first % 2==0){
			mpp.erase(iter);
		}
		// if access mpp here
	}
	#endif
	// reverse_iterator 
	cout<<"------------------------------"<<endl;
	cout<<"[reverse_iter Output]:"<<endl;
	for(rmitr iter=mpp.rbegin();iter!=mpp.rend();) {
		cout<<setw(3)<<iter->first<<" : "<<iter->second<<endl;
		++iter;
	}
	cout<<"------------------------------"<<endl;
	// find a key
	int sfkey=5;
	mitr finditer=mpp.find(sfkey);
	cout<<finditer->first<<" : "<<finditer->second<<endl;
	cout<<finditer->first<<" : "<<(finditer)->second<<endl;

#endif	

#if 0
	// 2014-5-8
	AAA a[7];
	AAA *p=a;
	char *pc=new char[13];
	AAA *pn=new AAA[7];
	int *pi=new int[7];
	printf("sizeof(a)=%d\n", sizeof(a));
	printf("sizeof(*p)=%d\n", sizeof(p));
	printf("sizeof(AAA)=%d\n", sizeof(AAA));

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

