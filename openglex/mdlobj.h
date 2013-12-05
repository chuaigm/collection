// mdlobj.h: interface for the mdlobj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDLOBJ_H__D8D0E709_8777_4103_B7C3_7C9513E6E4A2__INCLUDED_)
#define AFX_MDLOBJ_H__D8D0E709_8777_4103_B7C3_7C9513E6E4A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class mdlobj  
{
public:
	mdlobj();
	virtual ~mdlobj();
	unsigned int Animation;

	int		InitGL(int a,char* na);
	void	DrawModels(float x,float y,float z,int a,float size,float rz,float ry);

	void	fire(int a);

};

#endif // !defined(AFX_MDLOBJ_H__D8D0E709_8777_4103_B7C3_7C9513E6E4A2__INCLUDED_)
