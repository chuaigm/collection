
/*!
 *		Copyright(C),2010-2010
 * \file:		GameMenu.cpp	,	GameMenu.h
 * \brief:		ਹ��� PPW��Ϸ ��װ�˵�ѡ��
 * \author:		ਹ���
 * \version:	1.0
 * \Create:		2011-03-05
 * \modify��	2011-03-05
 * \History:	
 * \Run:		OpenGLUT.dll
 * \Environment MicroSoft windows XP; Windows 7; Visual Studio 2005
 *				
 *	<ਹ���><2011-01-04><0.9><������˵����>
 *	
 */

#pragma once

#include "GlobalData.h"
#include "..\ChuaiOpenGL\SCOpenGLmodel.h"

const short nMMitem = 5;									// ���˵�ѡ����
const short nSMitem[nMMitem] = { 5, 3, 2 ,0, 0};			// ��Ӧ�Ĵμ��˵�ѡ����
//
//
//const float		DET_Y_MENU	=	1.9	;							// �˵�ÿ��֮��ľ�������
//
//const float		ORI_X_MENU	=	-9	;							// �˵������ʼX����ֵ
//const float		ORI_Y_MENU	=	6		;						// �˵������ʼY����ֵ
//const float		SUB_X_MENU	=	-1		;						// �μ��˵�X����


#define		DET_Y_MENU		1.6								// �˵�ÿ��֮��ľ�������

#define		ORI_X_MENU		-9								// �˵������ʼX����ֵ
#define		ORI_Y_MENU		6								// �˵������ʼY����ֵ
#define		SUB_X_MENU		-1								// �μ��˵�X����


class CGameMenu
{
public:
	CGameMenu(void);
public:
	~CGameMenu(void);
	//////////////////////////////////////////////////////////////////////////
	// ����

	// ģ�������
	//CSCOpenGLmodel g_myModel;

	// �˵�ָ��λ��
	float		menuCurX, menuCurY;		// �˵����λ��
	short		column;					// ����˵���μ��˵�

	float		nowY;

	//////////////////////////////////////////////////////////////////////////
	// ����

	// ��������
	void DrawTitle(DWORD inc);
	// �����˵�
	void DrawMenuPPW(DWORD inc);
	// �����ɾ�
	void DrawAchievement(DWORD inc);
	// ��������
	void DrawAbout(DWORD inc);

	void NotFinished(DWORD inc);
};
