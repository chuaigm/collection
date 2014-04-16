
/*!
 *		Copyright(C),2010-2010
 * \file:		CSCOpenGL.cpp	,	CSCOpenGL.h
 * \brief:		ਹ�����װ��OpenGL�࣬����GLUT�⣬�����Ŀ�openglut.h; openglut_exp.h; openglut_ext.h; openglut_std.h;
 * \author:		ਹ���
 * \version:	1.0
 * \Create:		2011-01-04
 * \modify��	2011-03-05
 * \History:	
 * \Run:		OpenGLUT.dll
 * \Environment MicroSoft windows XP; Windows 7; Visual Studio 2005
 *				
 *	<ਹ���><2011-01-04><0.9><�����OpenGL����>
 *  <ਹ���><2011-03-01><1.0><����glut���OpenGL���������������ʵ���ӽ������ƶ����û������޸�DrawYourData������������Ҫ���Ƴ���ͼ��>
 *  <ਹ���><2011-03-05><1.0><����˶��û����ݶ��������ܣ���������������û����ƣ�Main,Your,Thee������������֡��ͬ����UpdateScdata()>
 *	
 */

#pragma once

//#define GLUT_DISABLE_ATEXIT_HACK				// һ��Ҫ����仰��Ҫ��Ȼ���Ӵ���
//#include "glut.h"								// GLut��ͷ�ļ�

// ����VC2005���ַ�������ȫ����
#pragma warning (disable : 4996)

// ��һ���ǹٷ���OpenGL�⣬����˹ٷ����һЩ��������
#include "openglut.h"
// 
#include <process.h>			// �߳�
#include "math.h"				// ��ѧ��
#include "stdlib.h"				// ��׼��
#include "stdio.h"				// ��׼�������

#include "..\GameCode/GlobalData.h"			// ȫ��״̬����

#include "..\GameCode/GameMenu.h"			// ��Ϸ�˵�
#include "..\GameCode/GameCore.h"			// ��Ϸ����




//////////////////////////////////////////////////////////////////////////
// �궨��

// ƽ̨����
//#ifndef DWORD
//#define DWORD unsigned long
//#endif

//
//// �̺߳�
//typedef unsigned (__stdcall *PTHREAD_START) (void*);
//#define chBEGINTHREADEX( psa, cbStack, pfnStartAddr, pvParam, fdwCreate, pdwThreadID )  \
//	( (HANDLE) _beginthreadex(                                          \
//	(void*)        (psa),												\
//	(unsigned)     (cbStack),											\
//	(PTHREAD_START)(pfnStartAddr),									    \
//	(void *)       (pvParam),										    \
//	(unsigned)     (fdwCreate),											\
//	(unsigned*)    (pdwThreadID)))
//


//// ������Ϣ��
//#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code)&0x8000)?1:0)

// ��Ҫʹ�õĴ��ڷ�����,����������ͬ���������԰�user2��3��

//#define USER_NUM2
//#define USER_NUM3
//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
// ȫ�ֱ���

static int mainWindow;							// ���������

static int m_Height, m_Width;					// ���ڸ����

//static const float m_Step = 0.2;				// ƽ�Ʋ��ֵĲ���
//static const float r_Step = 1;					// ��ת���ֵĲ���

//static float rad_hor , rad_ver;					// ˮƽ�봹ֱת���ĽǶ�

//// �ӵ�9������
//static float eye[3];							
//static float center[3];
//static float up[3];
//// ǰ������������
//static float forwardVector[3];
//static float rightVector[3];

// ����֡��ͬ����ʱ���
static DWORD NowTimeTicket, LastTimeTicket;

static CGameMenu		myGameMenu;
static CGameCore		myGameCore;

//////////////////////////////////////////////////////////////////////////




class CSCOpenGL
{

public:
	CSCOpenGL(void);
	~CSCOpenGL(void);

public:
	//////////////////////////////////////////////////////////////////////////
	// ���߳�ģʽ������
	void	MainScOpenGL();

#ifdef USER_NUM2
	void	YourScOpenGL();
#endif
#ifdef USER_NUM3
	void	TheeScOpenGL();
#endif
	

	//// �̺߳���(��ͼ)
	//static BOOL WINAPI ThreadMainScOpenGL(void*);
	//// �߳�ģʽ������
	//void	threadDraw();

	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// OpenGL��ʼ������,����ؿ���
	static void	InitSCOpenGL();
	// ����ߴ�ı亯��
	static void ReShape(int width, int hight);
	//// �����Ϣ
	//static void SCMouseMove(int button, int state, int x, int y);
	//// ������Ϣ
	//static void SCKeyBoard(unsigned char key, int x, int y);
	//// ������Ϣ�귽ʽ����
	//static void SCKeyBoardMarco(DWORD inc);
	// ֡��ͬ��,������������
	static void UpdateSCdata(DWORD millionSecond);
	// ������״̬
	static void DetectKeyBoardState();

	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// ����ͼ����
	static void RenderScene();

#ifdef USER_NUM2
	static void RenderYourScene();
#endif
#ifdef USER_NUM3
	static void RenderTheeScene();
#endif

	// ��������
	static void DrawLetter(float x, float y , float z, char* string);
	static void DrawLetter(float x, float y , float z, int string);
	static void DrawLetter(float x, float y , float z, float string);
	static void DrawLetter(float x, float y , float z, double string);
	//// �������Ժ���
	//static void	DrawTestCube(float x, float y, float z);
	// ������������
	static void	DrawGrid(GLfloat unit, int degree);

	//////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////
	//// �õ���ǰ�ӵ�����
	//static void GetEyePos(float & x, float & y, float & z);
	//// �õ���ǰcenter������
	//static void GetCenterPos(float & x, float & y, float & z);
	//// �õ���ǰ�Ϸ�����������
	//static void GetUpVector(float & x, float & y, float & z);
	//// �õ�����ǰ��������
	//static void GetForwardVector(float & x, float & y, float & z);
	////�õ��ҷ�������
	//static void GetRightVector(float &x, float &y, float &z);
	//// �õ�center����eye��ľ���
	//static float GetCenterToEye(void);
	//// �õ����������ӵ�ľ���
	//static float GetPointToEye(float x, float y, float z);

	////////////////////////////////////////////////////////////////////////////



	// ������������
	static void DrawMainData(DWORD inc);
	static void DrawAssitData(DWORD inc);
#ifdef USER_NUM2
	// �����ڶ�������������
	static void DrawYourData();
#endif
#ifdef USER_NUM3
	// ����������������
	static void DrawTheeData();
#endif

};
