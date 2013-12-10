// OpenGL.h: interface for the OpenGL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGL_H__17B7289C_7956_41C5_89B9_621E3C435389__INCLUDED_)
#define AFX_OPENGL_H__17B7289C_7956_41C5_89B9_621E3C435389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//font
#include "GLFont.h"		//

// OpenGLÄ¬ÈÏ×ø±êÏµ
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

	HDC		hDC;	
	HGLRC	hRC;	

	BOOL	SetupPixelFormat(HDC hDC);
	void	SetViewSize(int w,int h);
	void	init();
	void	initMenu();
	void	Render();
	void	CleanUp();

	void	text();	
	////////////////
	int RCwidth;
	int RCheight;
};

#endif // !defined(AFX_OPENGL_H__17B7289C_7956_41C5_89B9_621E3C435389__INCLUDED_)
