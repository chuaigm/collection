
#include "cgmtoolbox.h"

void fun1()
{
	FUNC_TRACER;
	MMM;
	INFO_LOG("%d-test1\n", 56);
	ERROR_LOG("%d-test1\n", 25);
}
void fun2()
{
	FUNC_TRACER;
	MMM;
	INFO_LOG("%d-test2\n", 56);
	ERROR_LOG("%d-test2\n", 25);
}

int main()
{
	FUNC_TRACER;
	BEGIN_CALC_TIME;
	printf("C1---------------\n");
	MMM;
	fun1();
	fun2();
	printf("C2--------------\n");
	CGM_VER_INFO;
	printf("C3--------------\n");
	END_CALC_TIME;
	SHOW_TIME_US;
	SHOW_TIME_MS;
	SHOW_TIME_S;
	printf("C4--------------\n");
	BEGIN_CALC_TIME;
	delaySf(1.56);
	printf("C3--------------\n");
	END_CALC_TIME;
	SHOW_TIME_US;
	SHOW_TIME_MS;
	SHOW_TIME_S;
	printf("C5--------------\n");
	plantRandomSeed(0,0,1);
	plantRandomSeed(1,3526,1);
	printf("C6--------------\n");
	red<<11111;
	std::cout<<std::endl;
	green<<11111;
	yellow<<11111;
	blue<<11111;
	purple<<11111;
	cyan<<11111;
	white<<11111;
	red_bk<<11111;
	green_bk<<11111;
	yellow_bk<<11111;
	blue_bk<<11111;
	purple_bk<<11111;
	cyan_bk<<11111;
	white_bk<<11111;
	std::cout<<std::endl;
	printf("C7--------------\n");
	endianJdg(1);
#ifdef _LITTLE_ENDIAN_
	printf("little macro\n");
#endif
#ifdef _BIG_ENDIAN_
	printf("big macro\n");
#endif
	printf("C8--------------\n");
	
}
