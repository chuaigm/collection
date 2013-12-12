
#include "cgmtoolbox.h"
#include <fcntl.h>

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
void genRandHex(char *str, unsigned int len)
{
	char buf[len];
	char tmp[2*len+1];
	memset(buf,0,len);
	memset(tmp,0,2*len+1);
	//int fd=open("/dev/random", O_RDONLY);
	//read(fd, buf, len);
	//close(fd);
	FILE *fp=fopen("/dev/random", "rb");
	fread(buf,len,1,fp);
	fclose(fp);

	for(int i=0; i<len; ++i)
	{
		sprintf(&(tmp[i*2]), "%02X", (unsigned char)buf[i]);
		//sprintf(&(tmp[i*2]), "%02X", buf[i]);
	}
	memcpy(str, tmp, len);
}

int main()
{
	char ar[100]={0};
	genRandHex(ar, 8);

	printf("%s", ar);

/*
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
*/	
}
