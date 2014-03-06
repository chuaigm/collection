//
// OpenGL�����࣬opengl����ӿ����װ
//====================================================================
#include "stdafx.h"
#include "OpenGLbase.h"
#include "myclock.h"
#include "gamemap.h"

extern HWND	hWnd;

extern MYCLOCK c1;

extern GAMEMAP gm;

extern CGLFont myfont;

extern int WinWidth;
extern int WinHeight;

//////////////////////////////////////////////////////////////////////
COpenGLbase::COpenGLbase()
{
	
}

COpenGLbase::~COpenGLbase()
{
	CleanUp();
}

//�������ظ�ʽ������RC��ѡ��RC
BOOL COpenGLbase::SetupPixelFormat(HDC hDC0)
{
	int nPixelFormat;
	hDC=hDC0;
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR), 
	    1,     
	    PFD_DRAW_TO_WINDOW | 
	    PFD_SUPPORT_OPENGL | 
	    PFD_DOUBLEBUFFER, 
	    PFD_TYPE_RGBA,  
	    24,
	    0, 0, 0, 0, 0, 0,
	    0,
	    0,
	    0,
	    0, 0, 0, 0, 
	    16,   
	    0, 
	    0,  
	    PFD_MAIN_PLANE, 
	    0,
	    0, 0, 0 
	}; 
	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{ MessageBox(NULL,"û�ҵ����ʵ���ʾģʽ","Error",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	SetPixelFormat(hDC,nPixelFormat,&pfd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//�������䷽ʽ
	glShadeModel(GL_SMOOTH); //or GL_FLAT
	//���
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_NORMALIZE);

	//��������εķ���
	//	glFrontFace(GL_CCW);
	//	glEnable(GL_CULL_FACE);
	//	glCullFace(GL_BACK);

	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	/////////////////////////////////////////////////////////������
	//	  glEnable(GL_SCISSOR_TEST);
	//	  glDisable(GL_DEPTH_TEST);
	//glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	//glPolygonOffset(0.01f,0.002f);
	//glPolygonMode(GL_FRONT,GL_FILL);
	
	return TRUE;
}

//�����ӿ�
//����ӳ�䷽ʽ
void COpenGLbase::init_3D()
{
	glViewport(0,0,RCwidth,RCheight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective	
		( 54.0f,
		  (GLfloat)RCwidth/(GLfloat)RCheight,
		  0.1f,	
		  3000.0f
		);
	
	//ָ��ģ����ͼ��ջ
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();

	//������ɫ����
	glEnable(GL_COLOR_MATERIAL);
	//��ʼ���ƹ�
	//gm.light0();

//====================================================
}
// 
void COpenGLbase::init_2D()
{
	glViewport(0, 0, RCwidth, RCheight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0, RCwidth, 0, RCheight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//������ɫ����
	//glEnable(GL_COLOR_MATERIAL);
}

void COpenGLbase::SetViewSize(int w,int h)
{
	RCwidth=w;
	RCheight=h;
}

//��Ⱦ���
void COpenGLbase::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();		
	text();	//���ʱ�Ӽ�ʱ
	gm.show();	//

	glFlush();		
	SwapBuffers(hDC);	
}

void COpenGLbase::CleanUp()
{ 
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
}

void COpenGLbase::text()
{
	//char str[128];
	//sprintf(str, "C %d/%d ",c1.iNumDest, c1.expire_time);
	//myfont.Print2D(0.1,0.1,str,FONT1,0.0f,0.0f,0.0f);
	//myfont.settext(WinWidth/2,WinHeight*7/8,"111111",FONT0,1.0,1.0,1.0);

	// Y /-------------\
	//   |600          |(800,600)
	//   |             |
	//   |0         800|
	//   \-------------/ X

	myfont.Print2D(0,2,"0,0",FONT0,1.0f,1.0f,1.0f);
	myfont.Print2D(100,2,"1,0",FONT1,1.0f,0.0f,0.0f);
	myfont.Print2D(200,2,"2,0",FONT2,1.0f,1.0f,0.0f);
	myfont.Print2D(300,2,"3,0",FONT3,1.0f,0.0f,1.0f);
	myfont.Print2D(400,2,"4,0",FONT4,0.0f,0.0f,1.0f);
	myfont.Print2D(500,2,"5,0",FONT5,0.0f,1.0f,0.0f);
	//myfont.Print2D(600,2,"6,0",FONT6,1.0f,1.0f,1.0f);
	
	
}


