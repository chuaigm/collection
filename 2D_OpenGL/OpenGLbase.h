// OpenGL.h: interface for the OpenGL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGL_H__17B7289C_7956_41C5_89B9_621E3C435389__INCLUDED_)
#define AFX_OPENGL_H__17B7289C_7956_41C5_89B9_621E3C435389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//font
#include "GLFont.h"

// OpenGLĬ������ϵ
//          Y ^
//            |
//            |
//            |--------> X
//           /
//          /
//       Z V

class COpenGLbase
{
public:	
	COpenGLbase();
	virtual ~COpenGLbase();
	
public:
	//-----------------------------------------
	BOOL	SetupPixelFormat(HDC hDC);
	void	SetViewSize(int w,int h);
	void	init_3D();
	void	init_2D();
	void	Render();
	void	CleanUp();

	void	text_OpenGL_log();
	// �������
	void	mouseProc(long long lparam);
	//-----------------------------------------
	HDC		hDC;		// �ⲿ���������λ
	HGLRC	hRC;		// ���ɵ��������
	int		RCwidth;	// ��OpenGLʵ���ڲ����ӿڿ��
	int		RCheight;	// ��OpenGLʵ���ڲ����ӿڸ߶�
	//���λ��
	int		Xmouse;
	int		Ymouse;
	int		XmouseOld;
	int		YmouseOld;
};

#endif // !defined(AFX_OPENGL_H__17B7289C_7956_41C5_89B9_621E3C435389__INCLUDED_)
