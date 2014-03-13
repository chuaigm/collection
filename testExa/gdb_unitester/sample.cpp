#include <cstdio>

struct ts
{
	int ssa;
	char ssb;
	double ssc;
};
int st_test(ts var);

int main()
{
	int a=7;
	ts cc;
	cc.ssa=1;
	cc.ssb=2;
	cc.ssc=3;

	if(4==a)
	{
		printf("is 4\n");
	}
	else if(7==a)
	{
		printf("is 7\n");
	}
	else
	{
		printf("is last\n");
	}

	int ti=0;
	ti=st_test(cc);
	printf("ti=%d\n",ti);

}

int add(int a, int b)
{
	int re=0;
	re=a+b;
	return re;
}

int sp_mod(int var)
{
	int re=0;
	// re=var%61;
	re=var-(((unsigned long long)var*0x4325c53fu)>>36)*61; 
	return re;
}

int st_test(ts var)
{
	int ta=0;
	ta=var.ssa;
	return ta;
}
