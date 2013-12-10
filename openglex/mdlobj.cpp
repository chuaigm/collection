//
// 制作  programking 2009年3月
// 博客  http://blog.csdn.net/programking
//=======================================
// mdlobj.cpp: implementation of the mdlobj class.
// 学程序编游戏系列丛书
// 唐明理 E_mail: cqtmL@163.com
//====================================================================
#include "stdafx.h"
#include "mdlobj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
TMDLModel		MdlModel[2];
//////////////////////////////////////////////////////////////////////////
mdlobj::mdlobj()
{
	Animation=0;
}
mdlobj::~mdlobj()
{
}
/////////////////////////////////////////////////////////////////////
//初始化
int mdlobj::InitGL(int a,char* na)	
{	
	MdlModel[a].Init(na);
	MdlModel[a].SetSequence(0);

	return TRUE;							// Initialization Went OK
}

//绘制模型
void mdlobj::DrawModels(float x,float y,float z,int a,float size,float rz,float ry)
{
	glPushMatrix();

	//glEnable(GL_LIGHTING);            
	glEnable(GL_TEXTURE_2D);     
	//移动到指定坐标
	glTranslatef(x, y, z);
	glScalef( size, size,size);	//缩放
	//旋转
	glRotated(-90,1.0f,0.0f,0.0f);
	//水平角
	glRotated(rz+5,0.0f,0.0f,1.0f);
	//仰角
	glRotated(-ry*2/3-5,0.0f,1.0f,0.0f);

	//刷新当前动作序列的帧
	MdlModel[a].AdvanceFrame2();

	//绘制
	MdlModel[a].DrawModel();///////////////

	if(Animation==1)
	{
		Animation=0;
		MdlModel[a].SetSequence(0);
	}
	if(Animation>0) 
		Animation--;

	glPopMatrix();
}

void mdlobj::fire(int a)
{
	MdlModel[0].SetSequence(a);
	tagMDLSeqDescription *aa=(tagMDLSeqDescription *)((BYTE *)MdlModel[0].Header
							  + MdlModel[0].Header->SequenceOffset)+a;

	//开火动作序列 Animation : 5 , NumFrames:29
	Animation=aa->NumFrames/5;
}
