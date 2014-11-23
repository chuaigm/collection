//
// OpenGL�����࣬opengl����ӿ����װ
//====================================================================
#include "stdafx.h"
#include "OpenGLbase.h"
#include "myclock.h"
#include "gameMain.h"

//extern HWND	hWnd;

extern MYCLOCK c1;
// Ϊ�������Զ���ĳ�����Ի��ƣ�������Ҫ�����ⲿ�Ļ�������
extern gameMain gm;
// �������ⲿ��
extern CGLFont myfont;
// ���ⲿ����Ĵ���ĸ߶�����
//extern int WinWidth;
//extern int WinHeight;

//////////////////////////////////////////////////////////////////////
COpenGLbase::COpenGLbase()
{
	hDC = NULL;
	hRC = NULL;
	RCwidth = 0;
	RCheight = 0;

	Xmouse = 0;
	Ymouse = 0;
	XmouseOld = 0;
	YmouseOld = 0;
}

COpenGLbase::~COpenGLbase()
{
	CleanUp();
}

//�������ظ�ʽ������RC��ѡ��RC
BOOL COpenGLbase::SetupPixelFormat(HDC hDC0)
{
	int nPixelFormat;
	// ����豸�����ģ��Ӵ��ھ����ȡ��
	hDC=hDC0;
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),	// ���ظ�ʽ�����ӽṹ�Ĵ�С
		1,							// ��PIXELFORMATDESCRIPTOR�ṹ�İ汾��һ����Ϊ1
	    PFD_DRAW_TO_WINDOW | 
	    PFD_SUPPORT_OPENGL | 
	    PFD_DOUBLEBUFFER,			// ��һ��������ػ������Եı�־λ,�绺���Ƿ�֧��GDI��OpenGL��
	    PFD_TYPE_RGBA,				// ˵����������������RGBA������ɫ����
	    24,							// ÿ����ɫ����������ɫλƽ�����Ŀ������ɫ������ʽ�ǻ�������С
	    0,							// ÿ��RGBA��ɫ�������к�ɫλƽ�����Ŀ
		0,							// ÿ��RGBA��ɫ�������к�ɫλƽ���ƫ����
		0,							// ÿ��RGBA��ɫ����������ɫλƽ�����Ŀ
		0,							// ÿ��RGBA��ɫ����������ɫλƽ���ƫ����
		0,							// ÿ��RGBA��ɫ����������ɫλƽ�����Ŀ
		0,							// ÿ��RGBA��ɫ����������ɫλƽ���ƫ����
	    0,							// ÿ��RGBA��ɫ��������alphaλƽ�����Ŀ(�����ģ��ֲ�֧��)
	    0,							// ÿ��RGBA��ɫ��������alphaλƽ���ƫ����(�����ģ��ֲ�֧��)
	    0,							// �ۼӻ�������ȫ��λƽ�����Ŀ
		0,							// �ۼӻ������к�ɫλƽ�����Ŀ
		0,							// �ۼӻ���������ɫλƽ�����Ŀ
		0,							// �ۼӻ���������ɫλƽ�����Ŀ
		0,							// �ۼӻ�������alphaλƽ�����Ŀ
	    16,							// Z(���)�����������
	    0, 							// ģ�建���������
	    0,							// ���򻺳���������(һ��1.0�汾��֧��)
	    PFD_MAIN_PLANE,				// �����ԣ�Ϊ��һ���Զ�������
	    0, 							// ���͵ײ�ƽ�������::λ0-3�����15����ƽ�棬λ4-7��ײ�
	    0,							// �����ԣ�Ϊ��һ���Զ�������
		0,							// ��͸��ɫ�ʵ�ֵ(RGBA��ʽ)����һ���ײ�ƽ�������(Index)
		0 							// �����ԣ�Ϊ��һ���Զ�������
	}; 
	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		// MB_ICONEXCLAMATION : �Ի������и���̾��
		MessageBox(NULL,"û�ҵ����ʵ���ʾģʽ","Error",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (!SetPixelFormat(hDC,nPixelFormat,&pfd))
	{
		MessageBox(NULL,"�����������ظ�ʽ","Error",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	// ����һ���µ�OpenGL��Ⱦ������,����������������ڻ��Ƶ���hdc���ص��豸.
	// �����Ⱦ�������к��豸������(dc)һ�������ظ�ʽ
	hRC = wglCreateContext(hDC);
	// ʹһ��ָ����OpenGL��Ⱦ�����ĵ����̵߳ĵ�ǰ����������
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
	// glViewport(x, y, width, height)
	// x, y ������Ϊ��λ��ָ�����ӿڵ����½�
	// ��ʾ����ӿھ��εĿ�Ⱥ͸߶ȣ����ݴ��ڵ�ʵʱ�仯�ػ洰��
	glViewport(0, 0, RCwidth, RCheight);

	// ���õ�ǰ����ģʽ��ͶӰ�������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// ƽ���޼��ռ�
	//            _____.(right,top,far)
	//          /|_____|
	//         /_/___/ /
	//        .|/____|/
	// (left,bottom,near)  
	glOrtho (0.0, (GLdouble)RCwidth, 0.0, (GLdouble)RCheight, -1.0, 1.0);

	// ����ģ�;���
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
	// Ĭ�ϱ���ɫ(ֵΪ0~1֮��)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// ��Ȼ����� [0, 1]��Ч����glDepthFunc()�����й�
	glClearDepth(1.0f);
	// �����ɫ��������Ȼ���
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	// ���GL�����һЩ����
	text_OpenGL_log();
	// ��Ϸ����Ļ���
	gm.show();

	glFlush();		
	SwapBuffers(hDC);
}

void COpenGLbase::CleanUp()
{ 
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
}

void COpenGLbase::text_OpenGL_log()
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

	myfont.Print2D(50,2,"0,0",FONT0,1.0f,1.0f,1.0f);
	myfont.Print2D(100,2,"1,0",FONT1,1.0f,0.0f,0.0f);
	myfont.Print2D(200,2,"2,0",FONT2,1.0f,1.0f,0.0f);
	myfont.Print2D(300,2,"3,0",FONT3,1.0f,0.0f,1.0f);
	myfont.Print2D(400,2,"4,0",FONT4,0.0f,0.0f,1.0f);
	myfont.Print2D(500,2,"5,0",FONT5,0.0f,1.0f,0.0f);
	//myfont.Print2D(600,2,"6,0",FONT6,1.0f,1.0f,1.0f);
	
	// ��ʾ�������ֵ
	char str[64];
	sprintf(str, "mx=%4d",Xmouse);
	myfont.Print2D(1,8,str,FONT0,1.0f,1.0f,1.0f);
	sprintf(str, "mx=%4d",Ymouse);
	myfont.Print2D(1,16,str,FONT0,1.0f,1.0f,1.0f);
}

void COpenGLbase::mouseProc(long long lparam)
{
	// ��win��ת���������������ϵ
	//
	//   /-------------\ X
	//   |(0,0)        | 800
	//   |             |
	//   | 600         |
	// Y \-------------/(800,600)  

	XmouseOld=Xmouse;
	YmouseOld=Ymouse;
	Xmouse=LOWORD(lparam);
	Ymouse=HIWORD(lparam);
}


