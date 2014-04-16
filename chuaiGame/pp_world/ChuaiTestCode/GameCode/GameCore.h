#pragma once

#include "Bullet.h"
#include "Player.h"
#include "GlobalData.h"

class CGameCore
{
public:
	CGameCore(void);
	~CGameCore(void);
	//////////////////////////////////////////////////////////////////////////
	// 变量

	bool	gameOver;					// 游戏是否结束
	bool	initGame;					// 已经初始化游戏

	DWORD	timeCount;					// 计算累积的毫秒数
	float	timeFlash;					// 流逝的时间

	short	excellentNum;				// 记录绝妙数
	DWORD	TTscore;					// 总分数

	short	nowBullet;					// 当前子弹数量
	float	nowSpeed;					// 当前游戏速度

	CPlayer m_Player1, m_Player2;		// 两个玩家
	CBullet m_Bullet[2*BULLET_NUM];		// 子弹，(为了不同的模式，需要开足够数量的数组)

	float	excl_x,excl_y;				// 用于记录绝妙的位置
	DWORD	excl_time;					// 发生绝妙的时间
	bool	maybeExcl;					// 可能是绝妙

	// 双人游戏所需变量

	char	win;						// 用于记录双人游戏结局的三态变量 0：平局，1：玩家一胜 2：玩家二胜

	short	excellentP2;				// 玩家二的绝妙数
	float	excl_x2,excl_y2;			// 用于记录绝妙的位置
	DWORD	excl_time2;					// 发生绝妙的时间
	bool	maybeExcl2;					// 可能是绝妙

	//////////////////////////////////////////////////////////////////////////
	// 函数

	// 进行游戏
	void Gaming(DWORD inc);
	// 三种游戏模式
	void GameSinglePlayer(DWORD inc);
	void GameDoublePlayer(DWORD inc);
	void GameEntertainment(DWORD inc);
	// 初始化游戏，初始化玩家和子弹
	void InitGame();
	// 计时函数
	void CountTimeFlash(DWORD inc);
	// 碰撞检测(暂时无用)
	void CollisionDetection(int i);
	// 游戏结束画面
	void GameOver(DWORD inc);

	// 玩家控制
	void Player1MoveUp(DWORD inc);
	void Player1MoveDown(DWORD inc);
	void Player1MoveLeft(DWORD inc);
	void Player1MoveRight(DWORD inc);

	void Player2MoveUp(DWORD inc);
	void Player2MoveDown(DWORD inc);
	void Player2MoveLeft(DWORD inc);
	void Player2MoveRight(DWORD inc);

	// 用于测试
	void NotFinished(DWORD inc);

};
