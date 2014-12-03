//
// ����  programking 2009��3��
// ����  http://blog.csdn.net/programking
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
		// ���ֵ��ʱ�䴥�����, 1000/40=25 ÿ��25���봥��һ�Σ�ÿ�봥��40��
		timeclip=CLOCKS_PER_SEC/40;
	}
	~MYCLOCK()
	{
	}
	// ʱ�Ӽ������
	int clockcount()
	{
		int t;
		t=clock()-timenow;
		// ����ָ����ʱ������
		if(t > timeclip)
		{
			timenow=clock();
			return 1;
		}
		return 0;
	}
	// �����������
	int clipcount()
	{
		iNumClip++;
		if(iNumClip>=timeclip)
		{
			// ����1��ļ���ֵ
			iNumClip=0;
			iNumDest--;
			if(iNumDest<=0)
			{
				// ����ָ��������ʱ���
				return 1;
			}
		}	
		return 0;
	}
	// ���¿�ʼ�����������ü�������
	void ReStart(long timetotal)
	{
		iNumDest=timetotal;
		expire_time=iNumDest;
		iNumClip=0;	
	}

public:
	int iNumClip;  // ��ǰ�ļ���
	int iNumDest;  // ��ǰ�����յ�
	int expire_time;  // ���μ����յ��¼
	
	clock_t timenow;  // ��ǰʱ��ֵ���ӳ���ʼ��tick
	long timeclip;  // ʱ��Ƭ��ʱ�Ӿ��ȣ��������
};

#endif