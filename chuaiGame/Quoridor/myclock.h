//
// 制作  programking 2009年3月
// 博客  http://blog.csdn.net/programking
//
// mod by chuai 2013-12-08 
//=======================================

#ifndef __MYCLOCK
#define __MYCLOCK

#include "stdio.h"
#include "time.h"

class MYCLOCK
{
public:
	MYCLOCK()
	{
		iNumClip=0;
		iNumDest=20;
		expire_time=iNumDest;

		timenow=clock();
		// 这个值是时间触发间隔, 1000/40=25 每隔25毫秒触发一次，每秒触发40次
		timeclip=CLOCKS_PER_SEC/40;
	}
	~MYCLOCK()
	{
	}
	// 时钟间隔触发
	int clockcount()
	{
		int t;
		t=clock()-timenow;
		// 到达指定的时间间隔差
		if(t > timeclip)
		{
			timenow=clock();
			return 1;
		}
		return 0;
	}
	// 计数间隔触发
	int clipcount()
	{
		iNumClip++;
		if(iNumClip>=timeclip)
		{
			// 到达1秒的计数值
			iNumClip=0;
			iNumDest--;
			if(iNumDest<=0)
			{
				// 到达指定计数的时间点
				return 1;
			}
		}	
		return 0;
	}
	// 重新开始计数，并设置计数总量
	void ReStart(long timetotal)
	{
		iNumDest=timetotal;
		expire_time=iNumDest;
		iNumClip=0;	
	}

public:
	int iNumClip;  // 当前的计数
	int iNumDest;  // 当前计数终点
	int expire_time;  // 本次计数终点记录
	
	clock_t timenow;  // 当前时钟值，从程序开始的tick
	long timeclip;  // 时间片，时钟精度，触发间隔
};

#endif