// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

//mfc
#include <afxwin.h>	

#include <windows.h>

#include "resource.h"

#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
//////////////////////

// 屏蔽编译警告
#pragma warning (disable:4996)

#pragma comment( lib, "winmm.lib")
#pragma comment( lib, "opengl32.lib")
#pragma comment( lib, "glu32.lib")
#pragma comment( lib, "glaux.lib")

// 检测键盘按键，使用这个会有个问题，就是窗口脱离焦点，也会检测
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// window size
#define WIN_WIDTH 1024
#define WIN_HEIGHT 700

// 贴图数组的大小
#define GAME_TEX_NUM 32

#define PI  (3.14159265359)

// 定义此宏，会开启调试代码
#define __DEBUG__

//字体
enum {
    FONT0,
    FONT1,
    FONT2,
    FONT3,
    FONT4,
    FONT5,
    FONT6,
    FONT7,
    FONT8,
    FONT9,
};

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
