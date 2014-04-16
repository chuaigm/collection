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
	// ����

	bool	gameOver;					// ��Ϸ�Ƿ����
	bool	initGame;					// �Ѿ���ʼ����Ϸ

	DWORD	timeCount;					// �����ۻ��ĺ�����
	float	timeFlash;					// ���ŵ�ʱ��

	short	excellentNum;				// ��¼������
	DWORD	TTscore;					// �ܷ���

	short	nowBullet;					// ��ǰ�ӵ�����
	float	nowSpeed;					// ��ǰ��Ϸ�ٶ�

	CPlayer m_Player1, m_Player2;		// �������
	CBullet m_Bullet[2*BULLET_NUM];		// �ӵ���(Ϊ�˲�ͬ��ģʽ����Ҫ���㹻����������)

	float	excl_x,excl_y;				// ���ڼ�¼�����λ��
	DWORD	excl_time;					// ���������ʱ��
	bool	maybeExcl;					// �����Ǿ���

	// ˫����Ϸ�������

	char	win;						// ���ڼ�¼˫����Ϸ��ֵ���̬���� 0��ƽ�֣�1�����һʤ 2����Ҷ�ʤ

	short	excellentP2;				// ��Ҷ��ľ�����
	float	excl_x2,excl_y2;			// ���ڼ�¼�����λ��
	DWORD	excl_time2;					// ���������ʱ��
	bool	maybeExcl2;					// �����Ǿ���

	//////////////////////////////////////////////////////////////////////////
	// ����

	// ������Ϸ
	void Gaming(DWORD inc);
	// ������Ϸģʽ
	void GameSinglePlayer(DWORD inc);
	void GameDoublePlayer(DWORD inc);
	void GameEntertainment(DWORD inc);
	// ��ʼ����Ϸ����ʼ����Һ��ӵ�
	void InitGame();
	// ��ʱ����
	void CountTimeFlash(DWORD inc);
	// ��ײ���(��ʱ����)
	void CollisionDetection(int i);
	// ��Ϸ��������
	void GameOver(DWORD inc);

	// ��ҿ���
	void Player1MoveUp(DWORD inc);
	void Player1MoveDown(DWORD inc);
	void Player1MoveLeft(DWORD inc);
	void Player1MoveRight(DWORD inc);

	void Player2MoveUp(DWORD inc);
	void Player2MoveDown(DWORD inc);
	void Player2MoveLeft(DWORD inc);
	void Player2MoveRight(DWORD inc);

	// ���ڲ���
	void NotFinished(DWORD inc);

};
