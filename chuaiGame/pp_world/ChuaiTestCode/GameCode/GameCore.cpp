#include "GameCore.h"

CGameCore::CGameCore(void)
{
	gameOver = true;
	initGame = false;
	
	maybeExcl = false;

	excellentNum = 0;
	TTscore = 0;

	win = 0;

	// 临时变量
	float TMPtime[10];				// 最高时间
	short TMPexce[10];				// 最高绝妙数
	DWORD TMPscore[10];				// 最高分数

	FILE * pFile = fopen("./GameData/RECdata.chuai","r");
	if (pFile ==NULL)
	{
		FILE * pf = fopen("./GameData/RECdata.chuai","w");
		for (short i = 0; i < 10; i++)
		{
			fprintf(pf,"%f %d %d\n",HIGHtime[i]*10+145, HIGHexce[i]*100+217, HIGHscore[i]*10+13654);
		}
		fclose(pf);

		return ;
	}
	else
	{
		for (short i = 0; i < 10; i++)
		{
			fscanf(pFile,"%f %d %d\n",&TMPtime[i], &TMPexce[i], &TMPscore[i]);
			HIGHtime[i] = (TMPtime[i] - 145)/10;
			HIGHexce[i] = (short)(TMPexce[i] - 217)/100;
			HIGHscore[i] = (DWORD)(TMPscore[i] - 13654)/10;
		}
		fclose(pFile);
	}
}

CGameCore::~CGameCore(void)
{
	FILE * pf = fopen("./GameData/RECdata.chuai","w");
	for (short i = 0; i < 10; i++)
	{
		fprintf(pf,"%f %d %d\n",HIGHtime[i]*10+145, HIGHexce[i]*100+217, HIGHscore[i]*10+13654);
	}
	fclose(pf);
}
//////////////////////////////////////////////////////////////////////////
// 游戏核心
void CGameCore::Gaming( DWORD inc )
{
	// 计时器
	CountTimeFlash(inc);
	// 右上角计时器
	glColor3f(0.0,1.0,0.0);
	g_myModel.DrawLetter(timeFlash, 9.5,7.5,0);
	// 左下角玩家一绝妙计数
	g_myModel.DrawLetter(excellentNum,-10,-7.5,0);
	// 如果双人游戏的话
	if (2 == g_MenuStateMain)
	{
		// 右下角玩家一绝妙计数
		g_myModel.DrawLetter(excellentP2,10,-7.5,0);
	}
	glColor3f(1,1,1);

	if (initGame == false)
	{
		InitGame();
		initGame = true;
	}

	// 单人游戏
	if (1 == g_MenuStateMain)
	{
		GameSinglePlayer(inc);
	}
	// 双人游戏
	if (2 == g_MenuStateMain)
	{
		GameDoublePlayer(inc);
	}
	if (3 == g_MenuStateMain)
	{
		GameEntertainment(inc);
	}
}

//////////////////////////////////////////////////////////////////////////
// 单人游戏
void CGameCore::GameSinglePlayer( DWORD inc )
{
	if (true == gameOver)
	{
		// 游戏结束
		GameOver(inc);
	}
	else	// 开始游戏
	{
		if (KEY_DOWN(38)||KEY_DOWN(87))
		{
			Player1MoveUp(inc);
		}
		if (KEY_DOWN(40)||KEY_DOWN(83))
		{
			Player1MoveDown(inc);
		}
		if (KEY_DOWN(37)||KEY_DOWN(65))
		{
			Player1MoveLeft(inc);
		}
		if (KEY_DOWN(39)||KEY_DOWN(68))
		{
			Player1MoveRight(inc);
		}

		// 检测绝妙的临时变量
		int tmpI = -1;

		// 判断子弹状态，循环子弹个数次
		for (int i = 0; i<nowBullet; i++)
		{
			// 增量
			m_Bullet[i].x += float(nowSpeed*inc) * m_Bullet[i].v_x;
			m_Bullet[i].y += float(nowSpeed*inc) * m_Bullet[i].v_y;

			// 检测越界
			if (m_Bullet[i].y < BOUNDARY_BOTTOM-0.1 ||
				m_Bullet[i].y > BOUNDARY_TOP+0.1	||
				m_Bullet[i].x > BOUNDARY_RIGHT+0.1  ||
				m_Bullet[i].x < BOUNDARY_LEFT-0.1)
			{
				m_Bullet[i].InitBullet(m_Player1.x , m_Player1.y);
			}

			// 碰撞检测
			float tmpX = m_Bullet[i].x - m_Player1.x;
			float tmpY = m_Bullet[i].y - m_Player1.y;
			if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R -0.06)
			{
				gameOver = true;
			}

			// 检测绝妙
			if(false == m_Player1.b_excl)
			{
				if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R + 0.3)
				{
					// 本次循环所有点，第一次找到满足条件
					if (tmpI == -1)
					{
						tmpI = i;
					}
					// 两个不同的点，并且角力大于玩家与子弹距离的总和
					else if ((i != tmpI)&&(sqrt((m_Bullet[i].x-m_Bullet[tmpI].x)*(m_Bullet[i].x-m_Bullet[tmpI].x)+(m_Bullet[i].y-m_Bullet[tmpI].y)*(m_Bullet[i].y-m_Bullet[tmpI].y))>PLAYER_R + BULLET_R))
					{
						excl_x = m_Player1.x;
						excl_y = m_Player1.y;
						m_Player1.b_excl = true;
						maybeExcl = true;
						excl_time = timeCount;
					}
				}
			}

		}
	}

	// 画出玩家
	m_Player1.DrawPlayer(m_Player1.x, m_Player1.y,PLAYER_R,40);

	// 画出子弹
	for (int i = 0; i<nowBullet; i++)
	{
		m_Bullet[i].DrawBullet(m_Bullet[i].x, m_Bullet[i].y, BULLET_R, -1);
	}
	// 画出绝妙
	if ((true == m_Player1.b_excl)&&(timeCount - excl_time > 220))
	{
		if (maybeExcl)
		{
			// 绝妙
			excellentNum++;
			maybeExcl = false;
		}
		m_Player1.DrawExcellent(excl_x, excl_y,inc);
	}
	////////////////////////////////////////////////////////////////////////////
	//// 测试
	//if (KEY_UP(27))
	//{
	//	gameOver = true;
	//}
}

//////////////////////////////////////////////////////////////////////////
// 双人游戏
void CGameCore::GameDoublePlayer( DWORD inc )
{
	if (true == gameOver)
	{
		GameOver(inc);
	}
	else
	{
		// 玩家1 —— P1
		if (KEY_DOWN(87))
		{
			Player1MoveUp(inc);
		}
		if (KEY_DOWN(83))
		{
			Player1MoveDown(inc);
		}
		if (KEY_DOWN(65))
		{
			Player1MoveLeft(inc);
		}
		if (KEY_DOWN(68))
		{
			Player1MoveRight(inc);
		}
		// 玩家2 —— P2
		if (KEY_DOWN(38))
		{
			Player2MoveUp(inc);
		}
		if (KEY_DOWN(40))
		{
			Player2MoveDown(inc);
		}
		if (KEY_DOWN(37))
		{
			Player2MoveLeft(inc);
		}
		if (KEY_DOWN(39))
		{
			Player2MoveRight(inc);
		}

		// 检测绝妙的临时变量
		int tmpI = -1,tmpI2 = -1;
		// 画出子弹
		for (int i = 0; i<nowBullet; i++)
		{
			m_Bullet->DrawBullet(m_Bullet[i].x, m_Bullet[i].y, BULLET_R, -1);

			m_Bullet[i].x += float(nowSpeed*inc) * m_Bullet[i].v_x;
			m_Bullet[i].y += float(nowSpeed*inc) * m_Bullet[i].v_y;

			// 检测越界
			if (m_Bullet[i].y < BOUNDARY_BOTTOM-0.1 ||
				m_Bullet[i].y > BOUNDARY_TOP+0.1	||
				m_Bullet[i].x > BOUNDARY_RIGHT+0.1  ||
				m_Bullet[i].x < BOUNDARY_LEFT-0.1)
			{
				m_Bullet[i].InitBullet((m_Player1.x + m_Player2.x)/2 , (m_Player1.y+m_Player2.y)/2);
			}
			//////////////////////////////////////////////////////////////////////////
			// 碰撞检测——玩家1 与 子弹
			float tmpX = m_Bullet[i].x - m_Player1.x;
			float tmpY = m_Bullet[i].y - m_Player1.y;
			if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R -0.1)
			{
				gameOver = true;
				win = 2;
			}
			// 检测 玩家1 绝妙
			if(false == m_Player1.b_excl)
			{
				if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R + 0.3)
				{
					// 本次循环所有点，第一次找到满足条件
					if (tmpI == -1)
					{
						tmpI = i;
					}
					// 两个不同的点，并且角力大于玩家与子弹距离的总和
					else if ((i != tmpI)&&(sqrt((m_Bullet[i].x-m_Bullet[tmpI].x)*(m_Bullet[i].x-m_Bullet[tmpI].x)+(m_Bullet[i].y-m_Bullet[tmpI].y)*(m_Bullet[i].y-m_Bullet[tmpI].y))>PLAYER_R + BULLET_R))
					{
						excl_x = m_Player1.x;
						excl_y = m_Player1.y;
						m_Player1.b_excl = true;
						maybeExcl = true;
						excl_time = timeCount;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			// 碰撞检测——玩家2 与 子弹
			tmpX = m_Bullet[i].x - m_Player2.x;
			tmpY = m_Bullet[i].y - m_Player2.y;
			if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R -0.1)
			{
				gameOver = true;
				win = 1;
			}
			// 检测 玩家2 绝妙
			if(false == m_Player2.b_excl)
			{
				if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R + 0.3)
				{
					// 本次循环所有点，第一次找到满足条件
					if (tmpI2 == -1)
					{
						tmpI2 = i;
					}
					// 两个不同的点，并且角力大于玩家与子弹距离的总和
					else if ((i != tmpI2)&&(sqrt((m_Bullet[i].x-m_Bullet[tmpI2].x)*(m_Bullet[i].x-m_Bullet[tmpI2].x)+(m_Bullet[i].y-m_Bullet[tmpI2].y)*(m_Bullet[i].y-m_Bullet[tmpI2].y))>PLAYER_R + BULLET_R))
					{
						excl_x2 = m_Player2.x;
						excl_y2 = m_Player2.y;
						m_Player2.b_excl = true;
						maybeExcl2 = true;
						excl_time2 = timeCount;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			// 玩家与玩家互撞
			tmpX = m_Player1.x - m_Player2.x;
			tmpY = m_Player1.y - m_Player2.y;
			if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R * 2 -0.05)
			{
				gameOver = true;
				win = 0;
			}
		}
	}

	// 合作模式连线
	if (g_MenuStateSub == 2)
	{
		const float dNear = 1.5;
		const float dFar = 12;
		const float dMid = dNear + (dFar - dNear)/2;

		float dplanes = sqrt((m_Player1.x-m_Player2.x)*(m_Player1.x-m_Player2.x)+(m_Player1.y-m_Player2.y)*(m_Player1.y-m_Player2.y));
		if (dplanes < dNear || dplanes > dFar)
		{
			gameOver = true;
		}
		else
		{
			glLineWidth(5);
			glColor3f(abs(dplanes-dMid)/(dMid-dNear),1-abs(dplanes-dMid)/(dMid-dNear),0);
			glBegin(GL_LINES);
			glVertex3f(m_Player1.x,m_Player1.y,-0.1);
			glVertex3f(m_Player2.x,m_Player2.y,-0.1);
			glEnd();
			glColor3f(1,1,1);
		}
	}


	//画出玩家
	m_Player1.DrawPlayer(m_Player1.x, m_Player1.y,PLAYER_R,40);
	m_Player2.DrawPlayer(m_Player2.x, m_Player2.y,PLAYER_R,41);

	// 画出子弹
	for (int i = 0; i<nowBullet; i++)
	{
		m_Bullet[i].DrawBullet(m_Bullet[i].x, m_Bullet[i].y, BULLET_R, -1);
	}
	// 画出 玩家1 绝妙
	if ((true == m_Player1.b_excl)&&(timeCount - excl_time > 220))
	{
		if (maybeExcl)
		{
			// 绝妙
			excellentNum++;
			maybeExcl = false;
		}
		m_Player1.DrawExcellent(excl_x, excl_y,inc);
	}
	// 画出 玩家2 绝妙
	if ((true == m_Player2.b_excl)&&(timeCount - excl_time2 > 220))
	{
		if (maybeExcl2)
		{
			// 绝妙
			excellentP2++;
			maybeExcl2 = false;
		}
		m_Player2.DrawExcellent(excl_x2, excl_y2,inc);
	}

	//////////////////////////////////////////////////////////////////////////
	// 测试
	if (KEY_UP(27))
	{
		gameOver = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// 娱乐模式
void CGameCore::GameEntertainment( DWORD inc )
{
	if (1 == g_MenuStateSub)
	{
		NotFinished(inc);
		if (KEY_UP(27))
		{
			g_GameState = 1;
		}
	}
	if (2 == g_MenuStateSub)
	{
		NotFinished(inc);
		if (KEY_UP(27))
		{
			g_GameState = 1;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 游戏初始化
void CGameCore::InitGame()
{
	// 计时器清零
	timeCount = 0;
	timeFlash = 0.0;
	// 绝妙清零
	excellentNum = 0;
	excellentP2 = 0;
	// 分数清零
	TTscore = 0;

	//////////////////////////////////////////////////////////////////////////
	// 初始化两种模式

	// 单人模式
	if (1 == g_MenuStateMain)
	{
		m_Player1.InitPlayer(0,-2,PLAYER_R,1);
		m_Player1.b_excl = false;
		// 初始化子弹与游戏速度()
		switch(g_MenuStateSub)
		{
		case 1:
			nowBullet = BULLET_NUM;
			nowSpeed = BULLET_SPEED;
			break;
		case 2:
			nowBullet = BULLET_NUM;
			nowSpeed = BULLET_SPEED*1.8;
			break;
		case 3:
			nowBullet = BULLET_NUM*1.8;
			nowSpeed = BULLET_SPEED;
			break;
		case 4:
			nowBullet = BULLET_NUM;
			nowSpeed = BULLET_SPEED*0.6;
			break;
		case 5:
			nowBullet = BULLET_NUM*0.6;
			nowSpeed = BULLET_SPEED;
			break;
		}
		for (int i = 0; i<nowBullet; i++)
		{
			m_Bullet[i].InitBullet(m_Player1.x , m_Player1.y);
		}

	}
	// 双人模式
	if (2 == g_MenuStateMain)
	{
		m_Player1.InitPlayer(-2.5, -1,PLAYER_R,1);	// 初始化玩家1
		m_Player1.b_excl = false;					// 玩家一绝妙标识
		m_Player2.InitPlayer(2.5, -1,PLAYER_R,1);	// 初始化玩家2
		m_Player2.b_excl = false;					// 玩家二绝妙标识

		nowBullet = BULLET_NUM;
		nowSpeed = BULLET_SPEED;

		// 初始化子弹
		for (int i = 0; i<BULLET_NUM; i++)
		{
			m_Bullet[i].InitBullet((m_Player1.x + m_Player2.x)/2 , (m_Player1.y+m_Player2.y)/2);
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
// 计时器，计算流逝的时间
void CGameCore::CountTimeFlash(DWORD inc)
{
	if (gameOver == false)
	{
		timeCount += inc;
		timeFlash = float(timeCount)/1000;
	}
}


//////////////////////////////////////////////////////////////////////////
// 碰撞检测
void CGameCore::CollisionDetection( int i)
{
	
}


//////////////////////////////////////////////////////////////////////////
// 游戏结束
void CGameCore::GameOver( DWORD inc )
{

	//////////////////////////////////////////////////////////////////////////
	// 单人模式
	if (1 == g_MenuStateMain)
	{
		// 计算得分
		TTscore = timeFlash*100+300.0*excellentNum;

		// 记录高分
		switch(g_MenuStateSub)
		{
		case 1:		// 常规游戏
			if (timeFlash > HIGHtime[g_MenuStateSub])
			{
				HIGHtime[g_MenuStateSub] = timeFlash;
			}
			if (excellentNum > HIGHexce[g_MenuStateSub])
			{
				HIGHexce[g_MenuStateSub] = excellentNum;
			}
			if (TTscore > HIGHscore[g_MenuStateSub])
			{
				HIGHscore[g_MenuStateSub] = TTscore;
			}
			break;
		case 2:		// 高速模式
			if (timeFlash > HIGHtime[g_MenuStateSub])
			{
				HIGHtime[g_MenuStateSub] = timeFlash;
			}
			if (excellentNum > HIGHexce[g_MenuStateSub])
			{
				HIGHexce[g_MenuStateSub] = excellentNum;
			}
			if (TTscore > HIGHscore[g_MenuStateSub])
			{
				HIGHscore[g_MenuStateSub] = TTscore;
			}
			break;
		case 3:		// 多弹模式
			if (timeFlash > HIGHtime[g_MenuStateSub])
			{
				HIGHtime[g_MenuStateSub] = timeFlash;
			}
			if (excellentNum > HIGHexce[g_MenuStateSub])
			{
				HIGHexce[g_MenuStateSub] = excellentNum;
			}
			if (TTscore > HIGHscore[g_MenuStateSub])
			{
				HIGHscore[g_MenuStateSub] = TTscore;
			}
			break;
		}

		glColor3f(1,1,1);
		// 画出游戏模式
		g_myModel.TextureSquareTGA(0,5,0,6,2,g_MenuStateSub+5);
		// 按回车开始游戏
		g_myModel.TextureSquareTGA(0,1.5,0,9,1,20);
		// 时间
		g_myModel.TextureSquareTGA(-2.2,-1.5,0,1.3,0.7,21);
		// 绝妙
		g_myModel.TextureSquareTGA(0.5,-1.5,0,1.3,0.7,22);
		// 得分
		g_myModel.TextureSquareTGA(3.0,-1.5,0,1.3,0.7,23);
		// 本次
		g_myModel.TextureSquareTGA(-4.5,-2.65,0,1.3,0.7,24);

		if (g_MenuStateSub != 4 && g_MenuStateSub != 5)
		{
			// 最高
			g_myModel.TextureSquareTGA(-4.5,-3.9,0,1.3,0.7,26);
		}

		glMatrixMode(GL_PROJECTION);	

		glPushMatrix();								
		glLoadIdentity();						
		glOrtho(0,g_Width,0,g_Height,-1,1);				// 设置为正投影
		glMatrixMode(GL_MODELVIEW);						

		glColor3f(0.0,1.0,0.0);
		// 当前时间
		g_myModel.glPrint(g_Width*300/800, g_Height*190/600,"%.2f",timeFlash);
		// 当前绝妙
		g_myModel.glPrint(g_Width*410/800, g_Height*190/600,"%d",excellentNum);
		// 当前总分
		g_myModel.glPrint(g_Width*490/800, g_Height*190/600,"%d",TTscore);
		// 低难度模式没有记录
		if (g_MenuStateSub != 4 && g_MenuStateSub != 5)
		{
			// 最高时间
			g_myModel.glPrint(g_Width*300/800, g_Height*140/600,"%.2f",HIGHtime[g_MenuStateSub]);
			// 最高绝妙
			g_myModel.glPrint(g_Width*410/800, g_Height*140/600,"%d",HIGHexce[g_MenuStateSub]);
			// 最高总分
			g_myModel.glPrint(g_Width*490/800, g_Height*140/600,"%d",HIGHscore[g_MenuStateSub]);
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	//////////////////////////////////////////////////////////////////////////
	// 双人游戏
	if (2 == g_MenuStateMain)
	{
		// 画出游戏模式
		g_myModel.TextureSquareTGA(0,6,0,6,2,g_MenuStateSub+10);
		// 按回车开始游戏
		g_myModel.TextureSquareTGA(0,-2.5,0,8,0.7,20);
		// 时间
		g_myModel.TextureSquareTGA(-0.5,-4.5,0,1.3,0.7,21);
		// 绝妙 玩家1
		g_myModel.TextureSquareTGA(-6,-3.5,0,1.3,0.7,22);
		// 绝妙 玩家2
		g_myModel.TextureSquareTGA(6,-3.5,0,1.3,0.7,22);

		switch(win)
		{
		case 0:
			// 平局
			g_myModel.TextureSquareTGA( 6, 2, 0, 5.5, 5.5, 29);
			g_myModel.TextureSquareTGA(-6, 2, 0, 5.5, 5.5, 29);
			break;
		case 1:
			// 玩家1 胜
			g_myModel.TextureSquareTGA(-6, 2, 0, 5.5, 5.5, 28);
			g_myModel.TextureSquareTGA( 6, 2, 0, 5.5, 5.5, 27);
			break;
		case 2:
			// 玩家2 胜
			g_myModel.TextureSquareTGA(-6, 2, 0, 5.5, 5.5, 27);
			g_myModel.TextureSquareTGA( 6, 2, 0, 5.5, 5.5, 28);
			break;
		}
	}
	

	// 开始游戏
	if ( KEY_UP(13) || KEY_UP(32) )
	{
		gameOver = false;
		InitGame();
	}
	// 返回上级菜单
	if (KEY_UP(27))
	{
		g_GameState = 1;
		initGame = false;
	}
}

//////////////////////////////////////////////////////////////////////////
// 控制玩家

// 1P
void CGameCore::Player1MoveUp( DWORD inc )
{
	m_Player1.y = (m_Player1.y + (nowSpeed+0.0005)*inc) > BOUNDARY_TOP - 0.5 ? m_Player1.y :m_Player1.y += (nowSpeed+0.0005)*inc;
}
void CGameCore::Player1MoveDown( DWORD inc )
{
	m_Player1.y = (m_Player1.y - (nowSpeed+0.0005)*inc) < BOUNDARY_BOTTOM + 0.5 ? m_Player1.y :m_Player1.y -= (nowSpeed+0.0005)*inc;
}
void CGameCore::Player1MoveLeft( DWORD inc )
{
	m_Player1.x = (m_Player1.x - (nowSpeed+0.0005)*inc) < BOUNDARY_LEFT + 1.0 ? m_Player1.x :m_Player1.x -= (nowSpeed+0.0005)*inc;
}
void CGameCore::Player1MoveRight( DWORD inc )
{
	m_Player1.x = (m_Player1.x + (nowSpeed+0.0005)*inc) > BOUNDARY_RIGHT - 1.0 ? m_Player1.x :m_Player1.x += (nowSpeed+0.0005)*inc;
}
// 2P
void CGameCore::Player2MoveUp( DWORD inc )
{
	m_Player2.y = (m_Player2.y + (nowSpeed+0.0005)*inc) > BOUNDARY_TOP - 0.5 ? m_Player2.y :m_Player2.y += (nowSpeed+0.0005)*inc;
}
void CGameCore::Player2MoveDown( DWORD inc )
{
	m_Player2.y = (m_Player2.y - (nowSpeed+0.0005)*inc) < BOUNDARY_BOTTOM + 0.5 ? m_Player2.y :m_Player2.y -= (nowSpeed+0.0005)*inc;
}
void CGameCore::Player2MoveLeft( DWORD inc )
{
	m_Player2.x = (m_Player2.x - (nowSpeed+0.0005)*inc) < BOUNDARY_LEFT + 1.0 ? m_Player2.x :m_Player2.x -= (nowSpeed+0.0005)*inc;
}
void CGameCore::Player2MoveRight( DWORD inc )
{
	m_Player2.x = (m_Player2.x + (nowSpeed+0.0005)*inc) > BOUNDARY_RIGHT - 1.0 ? m_Player2.x :m_Player2.x += (nowSpeed+0.0005)*inc;
}




//////////////////////////////////////////////////////////////////////////
// 测试用
void CGameCore::NotFinished( DWORD inc )
{
	// 开启贴图，有贴图的部分在这里绘制
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);	

	glPushMatrix();								
	glLoadIdentity();						
	glOrtho(0,g_Width,0,g_Height,-1,1);				// 设置为正投影
	glMatrixMode(GL_MODELVIEW);						

	glColor3f(1,1,0);
	g_myModel.glPrint(g_Width/2-140, g_Height/2+60,"****************************");
	g_myModel.glPrint(g_Width/2-140, g_Height/2-50,"****************************");


	glColor3f(1,1,1);
	g_myModel.glPrint(g_Width/2-70, g_Height/2+12,"No Finished!!!");
	g_myModel.glPrint(g_Width/2-70, g_Height/2-20,"Press 'ESC'!!!");

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}