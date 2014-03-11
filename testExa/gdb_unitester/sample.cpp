#include <cstdio>

struct ts
{
	int ssa;
	char ssb;
	double ssc;
};

int main()
{
	int a=7;
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
