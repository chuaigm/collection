//
// 制作  programking 2009年3月
// 博客  http://blog.csdn.net/programking
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

		timenow=clock();
		timeclip=CLOCKS_PER_SEC/40;
	}
	~MYCLOCK()
	{
	}

	int clockcount()
	{
		int t;

		t=clock()-timenow;
		if(t > timeclip)
		{
			timenow=clock();
			return 1;
		}
		return 0;
	}

	int clipcount()
	{
		iNumClip++;
		if(iNumClip>=25)
		{
			// 到达1秒
			iNumClip=0;
			iNumDest--;
			if(iNumDest<=0)
			{
				// 到达指定时间
				return 1;
			}
		}	
		return 0;
	}

	void ReStart(int timetotal)
	{
		iNumDest=timetotal;	
		iNumClip=0;	
	}

public:
	int iNumClip;
	int iNumDest;
	
	clock_t timenow;
	int timeclip;
};


#endif