//
// 制作  programking 2009年3月
// 博客  http://blog.csdn.net/programking
//=======================================
// anmobj.cpp: implementation of the anmobj class.
// 学程序编游戏系列丛书
// 唐明理 E_mail: cqtmL@163.com
//====================================================================
#include "stdafx.h"
#include "anmobj.h"
#include "opengl.h"

//////////////////////////////////////////////////////////////////////
int dongzuo[7]={0,2,2,7,12,14,14};

#include "filereport.h"

//////////////////////////////////////////////////////////////////////
anmobj::anmobj()
{
}
anmobj::~anmobj()
{}
void anmobj::getobj(CString na)//
{
	anim[0]=anim[1]=NULL;	
	
	if(model[0]!=NULL)
	{
		delete[] model[0];
		model[0]=NULL;
	}
	
	if(model[1]!=NULL)
	{
		delete[] model[1];
		model[1]=NULL;
	}
	
	model[0]   = md2_readModel(na+"tris.md2");
	
	model[1]   = md2_readModel(na+"weapon.md2");
	

	if(model[0]!=NULL) 
	{
		size[0] = md2_getAnimationCount(model[0]);
		// 获取动作序列
		animations (model[0],0);//
	}

	if(model[1]!=NULL)
	{
		size[1] = md2_getAnimationCount(model[1]);
		animations (model[1],1);
	}


}

animation* anmobj::animations (md2_model_t* model,int p)
{	
	if (size[p] == 0) 
		return 0;//

	if(anim[p]!=NULL) 
		delete[] anim[p];//
	
	anim[p] = new animation[size[p]];//
	//设置该模型的所有动画信息
	for (int i=0; i<size[p]; i++)//
	{
		//得到动画名称
		strcpy (anim[p][i].name,md2_getAnimationName(model,i));//
		//得到动画起始结束帧
		md2_getAnimationFrames(model,i,&anim[p][i].start,&anim[p][i].end);//
	}
	return 0;//
}

void anmobj::drawModel(int id,float x,float y,float z,int iframe,int iAngle)
{
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	glTranslatef(x,y,z);
	//旋转
	glRotatef(iAngle*90,0,1,0);
	//缩小
	glScaled(.06f,.06f,.06f);

	//role 模型不为空，且 iframe小于最大帧数
	if(model[0]!=NULL && model[0]->header.numFrames>iframe)
	{
		md2_drawModel (model[0],iframe,0,0);
	}
	//weapon
	if(model[1]!=NULL && model[1]->header.numFrames>iframe)
	{	
		md2_drawModel (model[1],iframe,0,0);
	}

	glPopMatrix();
	glPopAttrib();
}