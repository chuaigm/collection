﻿#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include <stdint.h>
#include <sys/time.h>
//#include "cgmtoolbox.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

#define __TPL printf("line:[%06d]\n",__LINE__);

typedef struct {
	int a;
	char b;
	short c;
} AAA;

map<string, AAA> ssmap;
typedef map<string, AAA>::iterator ssmitor;

void func(const int i)
{
	int ta = i;
};

void func1()
{
	AAA s1;
	s1.a=2;
	s1.b='b';
	s1.c=2;
	ssmap.insert(make_pair(string("2"),s1));
};

void func2()
{
	AAA s1;
	s1.a=3;
	s1.b='c';
	s1.c=3;
	ssmap.insert(make_pair(string("3"),s1));
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

union obj{
	union obj * link;
	char data[1];
};

int random(double start, double end)
{
	return (int)(start+(end-start)*rand()/(RAND_MAX + 1.0));
};

int main()
{
#if 1
    int tot = 100;
    int cnt = 0;
    while(tot--)
    {
        printf("cnt = %d\n", cnt);
        if(++cnt > 8)
        {
            printf("do\n");
            cnt = 0;
        }
    }
#endif

#if 0
    uint64_t i=0xFFFFFFFFFFFFFFFF;
    printf("i=%lu\n", i);
#endif

#if 0
    // 20191209
    struct  timeval    tv;
    struct  timezone   tz;
    gettimeofday(&tv,&tz);
    printf("tv_sec:%d\n", tv.tv_sec);
    printf("tv_usec:%d\n", tv.tv_usec);
    printf("tz_minuteswest:%d\n",tz.tz_minuteswest);
    printf("tz_dsttime:%d\n",tz.tz_dsttime);

    printf("==============\n");
    struct timespec tsp;
    clock_gettime(CLOCK_REALTIME, &tsp);
    printf("tv_sec:%d\n", tsp.tv_sec);
    printf("tv_nsec:%d\n", tsp.tv_nsec);
    printf("==============\n");
    printf("UINT64_MAX = %lu\n", UINT64_MAX);   // compile need --std=c++11
#endif

#if 0
    // 20190620
    // gdb 并列条件测试
    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j<10; j++)
        {
            if(j==0)
            {
                printf("%d, ",i);
            } else {
                printf("%d, ",j);
            }
        }
        printf("\n");
    }

#endif

#if 0
	//2018-3-27
	// multimap order test
	multimap<int, int> mmp;
	for(int i = 1; i<10; ++i) {
	//	for(int j = 1; j<10; ++j) {
	//		mmp.insert(make_pair(i,j));
			mmp.insert(make_pair(i,2));
			mmp.insert(make_pair(i,4));
			mmp.insert(make_pair(i,6));
			mmp.insert(make_pair(i,8));
			mmp.insert(make_pair(i,1));
			mmp.insert(make_pair(i,3));
			mmp.insert(make_pair(i,9));
			mmp.insert(make_pair(i,7));
			mmp.insert(make_pair(i,5));
		//}
	}
	for(auto itr=mmp.begin(); itr!=mmp.end(); ++itr) {
		cout<<itr->first<<":"<<itr->second<<endl;
	}
#endif

#if 0
    // floating number test
    double a=0;
    a=1887;
    printf("%.64f\n",a);
    a=51*37;
    printf("%.64f\n",a);
    a=5.1*370; // attention here
    printf("%.64f\n",a);
    a=510*3.7;
    printf("%.64f\n",a);
    a=0.9+188.61*10; // attention here
    printf("%.64f\n",a);
#endif

#if 0
    // test ramdom
	srand(unsigned(time(0)));
	for(int i=0; i<10;i++)
	{
		cout<<random(0,10)<<endl;
	}
#endif
#if 0
	char* a= (char*)malloc(24);
	char* b= (char*)malloc(15);
	char* c= (char*)malloc(43);
	printf("[a addr=%d]\n", a);
	printf("[b addr=%d]\n", b);
	printf("[c addr=%d]\n", c);
	printf("------------------\n");
	printf("[a header:]\n");
	for (int i=0; i<8; i++)
	{
		char t = *(unsigned char *) (a-8+i);
		printf("_%d", t);
	}
	printf("\n");
	free(a);
	printf("[a header after free(a):]\n");
	for (int i=0; i<8; i++)
	{
		char t = *(unsigned char *) (a-8+i);
		printf("_%d", t);
	}
	printf("\n");
	printf("\n");

	printf("[b header:]\n");
	for (int i=0; i<8; i++)
	{
		char t = *(unsigned char *) (b-8+i);
		printf("_%d", t);
	}
	printf("\n");
	free(b);
	printf("[b header after free(b):]\n");
	for (int i=0; i<8; i++)
	{
		char t = *(unsigned char *) (b-8+i);
		printf("_%d", t);
	}
	printf("\n");
	printf("\n");

	printf("[c header:]\n");
	for (int i=0; i<8; i++)
	{
		char t = *(unsigned char *) (c-8+i);
		printf("_%d", t);
	}
	printf("\n");
	free(c);
	printf("[c header after free(c):]\n");
	for (int i=0; i<8; i++)
	{
		char t = *(unsigned char *) (c-8+i);
		printf("_%d", t);
	}
	printf("\n");
	printf("\n");
#endif

#if 0
// break 到第几层
	int c=2;
	switch (c)
	{
		case 1:
		{
			__TPL;
			break;
		}
		case 2:
		{
			__TPL;
			//for(;;){
			if(1)
			{
			__TPL;
				break;
			}
			else
			{
			}
			//}
			__TPL;
			break;
		}
			__TPL;
	}
			__TPL;
#endif

#if 0
	// 浮点数修约
	int i=100;
	printf("-- %x -- %d -- \n", i, i);
	char c[50]={0};
	printf("-- %s --\n",c);
	double db=1.605;
	printf(" -- origin -- const -- var --\n");
	printf(" -- %.24f -- %.2f -- %.2f\n", 1.605,1.605,db);db+=0.01;
	printf(" -- %.24f -- %.2f -- %.2f\n", 1.615,1.615,db);db+=0.01;
	printf(" -- %.24f -- %.2f -- %.2f\n", 1.625,1.625,db);db+=0.01;
	printf(" -- %.24f -- %.2f -- %.2f\n", 1.635,1.635,db);db+=0.01;
	printf(" -- %f -- %.2f -- %.2f\n", 1.645,1.645,db);db+=0.01;
	printf(" -- %f -- %.2f -- %.2f\n", 1.655,1.655,db);db+=0.01;
	printf(" -- %f -- %.2f -- %.2f\n", 1.665,1.665,db);db+=0.01;
	printf(" -- %f -- %.2f -- %.2f\n", 1.675,1.675,db);db+=0.01;
	printf(" -- %f -- %.2f -- %.2f\n", 1.685,1.685,db);db+=0.01;
	printf(" -- %f -- %.2f -- %.2f\n", 1.695,1.695,db);db+=0.01;
	printf("----------\n");
#endif	

#if 0
	// 2014-8-20 map op
	map<string, AAA> mpp;
	typedef map<string, AAA>::iterator mitr;
	pair<map<string, AAA>::iterator, bool> insert_stat;
//	typedef map<string, AAA>::reverse_iterator rmitr;
	AAA sta[16];
	sta[0].a=100;
	sta[0].b='a';
	sta[0].c=10;
	sta[1].a=100;
	sta[1].b='b';
	sta[1].c=10;
	sta[2].a=100;
	sta[2].b='c';
	sta[2].c=10;
	sta[3].a=100;
	sta[3].b='d';
	sta[3].c=10;
	// insert
	// 默认升序
	insert_stat=mpp.insert(map<string, AAA>::value_type("aaa", sta[0]));
	if(!insert_stat.second)
	{
		printf("insert fail...1\n");
	}
	insert_stat=mpp.insert(map<string, AAA>::value_type("bbb", sta[1]));
	if(!insert_stat.second)
	{
		printf("insert fail...2\n");
	}
	insert_stat=mpp.insert(map<string, AAA>::value_type("ccc", sta[2]));
	if(!insert_stat.second)
	{
		printf("insert fail...3\n");
	}
	insert_stat=mpp.insert(map<string, AAA>::value_type("bbb", sta[3]));
	if(!insert_stat.second)
	{
		printf("insert fail...4\n");
	}

	mitr itor = mpp.begin();
	for(;itor != mpp.end();)
	{
		cout<<itor->first<<":"<<itor->second.b<<endl;
		++itor;
	}

	itor = mpp.find("zzz");

	char a[]="zzz";
	std::string sss=std::string(a);
	std::string str="adsf";
	
	if(itor==mpp.end())
	{
		printf("not found_%s\n", a);
	} else {
		printf("found\n");
	}

#endif


#if 0
	// stl 中的联合体设计，allocator
	char mem1[64]={0};
	char mem2[128]={0};
	union obj *p1=(union obj *)mem1;
	p1->link=(union obj *)mem2;
	cout<<"mem1="<<(void*)mem1<<endl;
	cout<<"mem2="<<(void*)mem2<<endl;
	cout<<"p1="<<(void*)p1<<endl;
	cout<<"p1->link="<<(void*)(p1->link)<<endl;
	cout<<"p1->data="<<(void*)(p1->data)<<endl;
#endif
	
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

