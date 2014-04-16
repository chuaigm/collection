
#pragma once


#include "../ChuaiOpenGL/SCOpenGLmodel.h"		// ȫ��ģ����
#include <ctime>								// ϵͳʱ�䣬Ϊ���������������


//////////////////////////////////////////////////////////////////////////
// �궨�岿��

#define BOUNDARY_TOP	8				// ��ǰ���������Ͻ�
#define BOUNDARY_BOTTOM -8				// ��ǰ���������½�
#define BOUNDARY_LEFT	-11				// ��ǰ�����������
#define BOUNDARY_RIGHT	11				// ��ǰ���������ҽ�

#define	BULLET_NUM		120				// ��Ļ���ӵ�������
#define	GAME_SPEED		100				// ��Ϸ�ٶ�

#define PLAYER_R		0.3				// �����ײ�뾶
#define BULLET_R		0.1				// �ӵ���ײ�뾶

#define PLAYER_SPEED	0.003
#define BULLET_SPEED	0.0025

//////////////////////////////////////////////////////////////////////////
// ������Ӧ��
#define PRESSED					0x01
#define PRESSED_UP				0x08

// ��¼����״̬����
extern unsigned char KeyState[ 256 ];

//���ڽ��ܼ�����Ϣ�ĺ궨��
// ���̰���
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code)&0x8000)?1:0)
// ����֮���̧��
#define KEY_UP(vk_code)((KeyState[vk_code]&PRESSED_UP)?1:0)

//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// 
extern short g_GameState;				// ��Ϸ״̬��0����Ϸ����. 1:�˵�ѡ��  2:��Ϸ����  3:�ɾ�  4:����

extern short g_MenuStateMain;			// ���˵�ѡ��
extern short g_MenuStateSub;			// �μ��˵�ѡ��

extern CSCOpenGLmodel g_myModel;		// ȫ��ģ����

extern int g_Height, g_Width;			// ���ڸ����

//////////////////////////////////////////////////////////////////////////
// ��¼��Ϸ�ɼ�
// 0�գ�1,2,3��¼������Ϸ��
extern float HIGHtime[10];				// ���ʱ��
extern short HIGHexce[10];				// ��߾�����
extern DWORD HIGHscore[10];				// ��߷���