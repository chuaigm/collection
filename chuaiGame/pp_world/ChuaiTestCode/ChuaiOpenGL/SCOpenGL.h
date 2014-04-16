
/*!
 *		Copyright(C),2010-2010
 * \file:		CSCOpenGL.cpp	,	CSCOpenGL.h
 * \brief:		啜国明封装的OpenGL类，利用GLUT库，依赖的库openglut.h; openglut_exp.h; openglut_ext.h; openglut_std.h;
 * \author:		啜国明
 * \version:	1.0
 * \Create:		2011-01-04
 * \modify：	2011-03-05
 * \History:	
 * \Run:		OpenGLUT.dll
 * \Environment MicroSoft windows XP; Windows 7; Visual Studio 2005
 *				
 *	<啜国明><2011-01-04><0.9><初步搭建OpenGL类框架>
 *  <啜国明><2011-03-01><1.0><基于glut库的OpenGL类初步成立，可以实现视角自由移动，用户可以修改DrawYourData函数来绘制想要绘制出的图形>
 *  <啜国明><2011-03-05><1.0><添加了多用户数据独立处理功能，添加了三个独立用户绘制，Main,Your,Thee，并且增加了帧率同步，UpdateScdata()>
 *	
 */

#pragma once

//#define GLUT_DISABLE_ATEXIT_HACK				// 一定要加这句话，要不然链接错误
//#include "glut.h"								// GLut的头文件

// 屏蔽VC2005的字符操作安全警告
#pragma warning (disable : 4996)

// 另一个非官方的OpenGL库，解决了官方库的一些致命问题
#include "openglut.h"
// 
#include <process.h>			// 线程
#include "math.h"				// 数学库
#include "stdlib.h"				// 标准库
#include "stdio.h"				// 标准输入输出

#include "..\GameCode/GlobalData.h"			// 全局状态数据

#include "..\GameCode/GameMenu.h"			// 游戏菜单
#include "..\GameCode/GameCore.h"			// 游戏核心




//////////////////////////////////////////////////////////////////////////
// 宏定义

// 平台定义
//#ifndef DWORD
//#define DWORD unsigned long
//#endif

//
//// 线程宏
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


//// 键盘消息宏
//#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code)&0x8000)?1:0)

// 需要使用的窗口方案数,两个人做不同工作，可以吧user2，3打开

//#define USER_NUM2
//#define USER_NUM3
//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
// 全局变量

static int mainWindow;							// 主窗口序号

static int m_Height, m_Width;					// 窗口高与宽

//static const float m_Step = 0.2;				// 平移部分的步长
//static const float r_Step = 1;					// 旋转部分的步长

//static float rad_hor , rad_ver;					// 水平与垂直转过的角度

//// 视点9个参数
//static float eye[3];							
//static float center[3];
//static float up[3];
//// 前向与右向向量
//static float forwardVector[3];
//static float rightVector[3];

// 用于帧率同步的时间戳
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
	// 非线程模式主函数
	void	MainScOpenGL();

#ifdef USER_NUM2
	void	YourScOpenGL();
#endif
#ifdef USER_NUM3
	void	TheeScOpenGL();
#endif
	

	//// 线程函数(绘图)
	//static BOOL WINAPI ThreadMainScOpenGL(void*);
	//// 线程模式主函数
	//void	threadDraw();

	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// OpenGL初始化函数,与相关控制
	static void	InitSCOpenGL();
	// 窗体尺寸改变函数
	static void ReShape(int width, int hight);
	//// 鼠标消息
	//static void SCMouseMove(int button, int state, int x, int y);
	//// 键盘消息
	//static void SCKeyBoard(unsigned char key, int x, int y);
	//// 键盘消息宏方式控制
	//static void SCKeyBoardMarco(DWORD inc);
	// 帧率同步,加入增量控制
	static void UpdateSCdata(DWORD millionSecond);
	// 检测键盘状态
	static void DetectKeyBoardState();

	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// 主绘图函数
	static void RenderScene();

#ifdef USER_NUM2
	static void RenderYourScene();
#endif
#ifdef USER_NUM3
	static void RenderTheeScene();
#endif

	// 画出文字
	static void DrawLetter(float x, float y , float z, char* string);
	static void DrawLetter(float x, float y , float z, int string);
	static void DrawLetter(float x, float y , float z, float string);
	static void DrawLetter(float x, float y , float z, double string);
	//// 画出测试盒子
	//static void	DrawTestCube(float x, float y, float z);
	// 画出地面网格
	static void	DrawGrid(GLfloat unit, int degree);

	//////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////
	//// 得到当前视点坐标
	//static void GetEyePos(float & x, float & y, float & z);
	//// 得到当前center点坐标
	//static void GetCenterPos(float & x, float & y, float & z);
	//// 得到当前上方向向量坐标
	//static void GetUpVector(float & x, float & y, float & z);
	//// 得到当正前方向向量
	//static void GetForwardVector(float & x, float & y, float & z);
	////得到右方向向量
	//static void GetRightVector(float &x, float &y, float &z);
	//// 得到center点与eye点的距离
	//static float GetCenterToEye(void);
	//// 得到给定点与视点的距离
	//static float GetPointToEye(float x, float y, float z);

	////////////////////////////////////////////////////////////////////////////



	// 画出激光数据
	static void DrawMainData(DWORD inc);
	static void DrawAssitData(DWORD inc);
#ifdef USER_NUM2
	// 画出第二方案数据数据
	static void DrawYourData();
#endif
#ifdef USER_NUM3
	// 画出第三方案数据
	static void DrawTheeData();
#endif

};
