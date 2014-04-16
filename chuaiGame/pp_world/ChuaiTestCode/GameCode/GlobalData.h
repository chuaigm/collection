
#pragma once


#include "../ChuaiOpenGL/SCOpenGLmodel.h"		// 全局模型类
#include <ctime>								// 系统时间，为了生成随机数种子


//////////////////////////////////////////////////////////////////////////
// 宏定义部分

#define BOUNDARY_TOP	8				// 当前窗口坐标上界
#define BOUNDARY_BOTTOM -8				// 当前窗口坐标下界
#define BOUNDARY_LEFT	-11				// 当前窗口坐标左界
#define BOUNDARY_RIGHT	11				// 当前窗口坐标右界

#define	BULLET_NUM		120				// 屏幕中子弹的数量
#define	GAME_SPEED		100				// 游戏速度

#define PLAYER_R		0.3				// 玩家碰撞半径
#define BULLET_R		0.1				// 子弹碰撞半径

#define PLAYER_SPEED	0.003
#define BULLET_SPEED	0.0025

//////////////////////////////////////////////////////////////////////////
// 键盘相应宏
#define PRESSED					0x01
#define PRESSED_UP				0x08

// 记录键盘状态数组
extern unsigned char KeyState[ 256 ];

//用于接受键盘消息的宏定义
// 键盘按下
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code)&0x8000)?1:0)
// 按下之后的抬起
#define KEY_UP(vk_code)((KeyState[vk_code]&PRESSED_UP)?1:0)

//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// 
extern short g_GameState;				// 游戏状态：0：游戏标题. 1:菜单选择  2:游戏核心  3:成就  4:关于

extern short g_MenuStateMain;			// 主菜单选项
extern short g_MenuStateSub;			// 次级菜单选项

extern CSCOpenGLmodel g_myModel;		// 全局模型类

extern int g_Height, g_Width;			// 窗口高与宽

//////////////////////////////////////////////////////////////////////////
// 记录游戏成绩
// 0空，1,2,3记录单人游戏。
extern float HIGHtime[10];				// 最高时间
extern short HIGHexce[10];				// 最高绝妙数
extern DWORD HIGHscore[10];				// 最高分数