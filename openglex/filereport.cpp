//
// 制作  programking 2009年3月
// 博客  http://blog.csdn.net/programking
//=======================================
#include "stdafx.h"
#include "filereport.h"

#include "stdio.h"
#include "stdlib.h"


FILEREPORT::FILEREPORT()
{
	fp=fopen("trace.txt","w");
	
	memset(num,0,sizeof(num));

	iPut=0;
}

FILEREPORT::FILEREPORT(char *p)
{
	fp=fopen(p,"w");

	memset(num,0,sizeof(num));

	iPut=0;
}

FILEREPORT::~FILEREPORT()
{
	fclose(fp);
}

void FILEREPORT::put(char *p)
{
	fwrite(p,sizeof(char),strlen(p),fp);
	fwrite("\n",1,1,fp);
}

//带计数器的跟踪
void FILEREPORT::putnum(int i,char *p)
{
	char temp[50];
	sprintf(temp,"(%d) ",num[i]);
	fwrite(temp,sizeof(char),strlen(temp),fp);
	num[i]++;

	fwrite(p,sizeof(char),strlen(p),fp);
	fwrite("\n",1,1,fp);
}
void FILEREPORT::put(char *p,int a)
{
	char temp[50];
	sprintf(temp,"%s : %d ",p,a);
	fwrite(temp,sizeof(char),strlen(temp),fp);
	fwrite("\n",1,1,fp);
}

void FILEREPORT::put(char *p,int a,int b)
{
	char temp[50];
	sprintf(temp,"%s : %d , %d",p,a,b);
	fwrite(temp,sizeof(char),strlen(temp),fp);
	fwrite("\n",1,1,fp);
}
void FILEREPORT::put(char *p,double a,double b,double c,double d)
{
	char temp[100];
	sprintf(temp,"%s: %f,%f,%f,%f",p,a,b,c,d);
	fwrite(temp,sizeof(char),strlen(temp),fp);
	fwrite("\n",1,1,fp);

}
void FILEREPORT::put(char *p,double a,double b, double c)
{
	char temp[50];
	sprintf(temp,"%s: %f,%f,%f",p,a,b,c);
	fwrite(temp,sizeof(char),strlen(temp),fp);
	fwrite("\n",1,1,fp);

}
// put once
void FILEREPORT::putOnce(char *p,double a,double b, double c)
{
	if(iPut)
		return;
	put(p,a,b,c);	
}
void FILEREPORT::putOnceStop()
{
	iPut=1;
}

///////////////////////////////////////////////////////////////////////
#ifdef WIN_REPORT

void FILEREPORT::CheckDC(HDC h,char *p)
{
	if(h==NULL)
	{
		fwrite(p,sizeof(char),strlen(p),fp);
		fwrite("\n",1,1,fp);
	}
}

void FILEREPORT::SetDevice(HDC h,int x,int y)
{
	hshow=h;
	xpos=x;
	ypos=y;
}

void FILEREPORT::Output(char *p,int a,int b)
{
	sprintf(info,"%s:%d,%d",p,a,b);

	
}

void FILEREPORT::Show()
{
	TextOut(hshow,xpos,ypos,info,strlen(info));

}


#endif 