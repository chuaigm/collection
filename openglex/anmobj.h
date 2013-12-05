// anmobj.h: interface for the anmobj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANMOBJ_H__46249C44_7C55_4407_AAE7_790A324FB6DE__INCLUDED_)
#define AFX_ANMOBJ_H__46249C44_7C55_4407_AAE7_790A324FB6DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

//////////////////////////////////////////

struct animation
{	char name[16];
	int	 start;		
	int	 end;		
};

////////////////////////////////////////////
class anmobj
{
public:
	anmobj();
	virtual ~anmobj();

public:
	md2_model_t*	model[2];	//两个MD2模型
	animation*	anim[2];	//各个模型的动画数据
	int		size[2];	//动作数量
	
	//初始化模型
	void			getobj(CString na);
	//读取模型的动画信息
	animation*		animations (md2_model_t* model,int p);
	//绘制模型
	//入参 ID	坐标 帧
	void drawModel(int id,float x,float y,float z,int iframe,int iAngle);

};

#endif // !defined(AFX_ANMOBJ_H__46249C44_7C55_4407_AAE7_790A324FB6DE__INCLUDED_)
