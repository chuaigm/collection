
/*!
 *		Copyright(C),2010-2010
 * \file:		GameMenu.cpp	,	GameMenu.h
 * \brief:		啜国明 PPW游戏 封装菜单选项
 * \author:		啜国明
 * \version:	1.0
 * \Create:		2011-03-05
 * \modify：	2011-03-05
 * \History:	
 * \Run:		OpenGLUT.dll
 * \Environment MicroSoft windows XP; Windows 7; Visual Studio 2005
 *				
 *	<啜国明><2011-01-04><0.9><初步搭建菜单框架>
 *	
 */

#pragma once

#include "GlobalData.h"
#include "..\ChuaiOpenGL\SCOpenGLmodel.h"

const short nMMitem = 5;									// 主菜单选项数
const short nSMitem[nMMitem] = { 5, 3, 2 ,0, 0};			// 对应的次级菜单选项数
//
//
//const float		DET_Y_MENU	=	1.9	;							// 菜单每项之间的距离增量
//
//const float		ORI_X_MENU	=	-9	;							// 菜单光标起始X坐标值
//const float		ORI_Y_MENU	=	6		;						// 菜单光标起始Y坐标值
//const float		SUB_X_MENU	=	-1		;						// 次级菜单X坐标


#define		DET_Y_MENU		1.6								// 菜单每项之间的距离增量

#define		ORI_X_MENU		-9								// 菜单光标起始X坐标值
#define		ORI_Y_MENU		6								// 菜单光标起始Y坐标值
#define		SUB_X_MENU		-1								// 次级菜单X坐标


class CGameMenu
{
public:
	CGameMenu(void);
public:
	~CGameMenu(void);
	//////////////////////////////////////////////////////////////////////////
	// 变量

	// 模型类对象
	//CSCOpenGLmodel g_myModel;

	// 菜单指针位置
	float		menuCurX, menuCurY;		// 菜单光标位置
	short		column;					// 主层菜单与次级菜单

	float		nowY;

	//////////////////////////////////////////////////////////////////////////
	// 函数

	// 画出封面
	void DrawTitle(DWORD inc);
	// 画出菜单
	void DrawMenuPPW(DWORD inc);
	// 画出成就
	void DrawAchievement(DWORD inc);
	// 画出关于
	void DrawAbout(DWORD inc);

	void NotFinished(DWORD inc);
};
