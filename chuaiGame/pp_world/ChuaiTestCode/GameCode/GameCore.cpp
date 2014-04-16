#include "GameCore.h"

CGameCore::CGameCore(void)
{
	gameOver = true;
	initGame = false;
	
	maybeExcl = false;

	excellentNum = 0;
	TTscore = 0;

	win = 0;

	// ��ʱ����
	float TMPtime[10];				// ���ʱ��
	short TMPexce[10];				// ��߾�����
	DWORD TMPscore[10];				// ��߷���

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
// ��Ϸ����
void CGameCore::Gaming( DWORD inc )
{
	// ��ʱ��
	CountTimeFlash(inc);
	// ���ϽǼ�ʱ��
	glColor3f(0.0,1.0,0.0);
	g_myModel.DrawLetter(timeFlash, 9.5,7.5,0);
	// ���½����һ�������
	g_myModel.DrawLetter(excellentNum,-10,-7.5,0);
	// ���˫����Ϸ�Ļ�
	if (2 == g_MenuStateMain)
	{
		// ���½����һ�������
		g_myModel.DrawLetter(excellentP2,10,-7.5,0);
	}
	glColor3f(1,1,1);

	if (initGame == false)
	{
		InitGame();
		initGame = true;
	}

	// ������Ϸ
	if (1 == g_MenuStateMain)
	{
		GameSinglePlayer(inc);
	}
	// ˫����Ϸ
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
// ������Ϸ
void CGameCore::GameSinglePlayer( DWORD inc )
{
	if (true == gameOver)
	{
		// ��Ϸ����
		GameOver(inc);
	}
	else	// ��ʼ��Ϸ
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

		// ���������ʱ����
		int tmpI = -1;

		// �ж��ӵ�״̬��ѭ���ӵ�������
		for (int i = 0; i<nowBullet; i++)
		{
			// ����
			m_Bullet[i].x += float(nowSpeed*inc) * m_Bullet[i].v_x;
			m_Bullet[i].y += float(nowSpeed*inc) * m_Bullet[i].v_y;

			// ���Խ��
			if (m_Bullet[i].y < BOUNDARY_BOTTOM-0.1 ||
				m_Bullet[i].y > BOUNDARY_TOP+0.1	||
				m_Bullet[i].x > BOUNDARY_RIGHT+0.1  ||
				m_Bullet[i].x < BOUNDARY_LEFT-0.1)
			{
				m_Bullet[i].InitBullet(m_Player1.x , m_Player1.y);
			}

			// ��ײ���
			float tmpX = m_Bullet[i].x - m_Player1.x;
			float tmpY = m_Bullet[i].y - m_Player1.y;
			if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R -0.06)
			{
				gameOver = true;
			}

			// ������
			if(false == m_Player1.b_excl)
			{
				if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R + 0.3)
				{
					// ����ѭ�����е㣬��һ���ҵ���������
					if (tmpI == -1)
					{
						tmpI = i;
					}
					// ������ͬ�ĵ㣬���ҽ�������������ӵ�������ܺ�
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

	// �������
	m_Player1.DrawPlayer(m_Player1.x, m_Player1.y,PLAYER_R,40);

	// �����ӵ�
	for (int i = 0; i<nowBullet; i++)
	{
		m_Bullet[i].DrawBullet(m_Bullet[i].x, m_Bullet[i].y, BULLET_R, -1);
	}
	// ��������
	if ((true == m_Player1.b_excl)&&(timeCount - excl_time > 220))
	{
		if (maybeExcl)
		{
			// ����
			excellentNum++;
			maybeExcl = false;
		}
		m_Player1.DrawExcellent(excl_x, excl_y,inc);
	}
	////////////////////////////////////////////////////////////////////////////
	//// ����
	//if (KEY_UP(27))
	//{
	//	gameOver = true;
	//}
}

//////////////////////////////////////////////////////////////////////////
// ˫����Ϸ
void CGameCore::GameDoublePlayer( DWORD inc )
{
	if (true == gameOver)
	{
		GameOver(inc);
	}
	else
	{
		// ���1 ���� P1
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
		// ���2 ���� P2
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

		// ���������ʱ����
		int tmpI = -1,tmpI2 = -1;
		// �����ӵ�
		for (int i = 0; i<nowBullet; i++)
		{
			m_Bullet->DrawBullet(m_Bullet[i].x, m_Bullet[i].y, BULLET_R, -1);

			m_Bullet[i].x += float(nowSpeed*inc) * m_Bullet[i].v_x;
			m_Bullet[i].y += float(nowSpeed*inc) * m_Bullet[i].v_y;

			// ���Խ��
			if (m_Bullet[i].y < BOUNDARY_BOTTOM-0.1 ||
				m_Bullet[i].y > BOUNDARY_TOP+0.1	||
				m_Bullet[i].x > BOUNDARY_RIGHT+0.1  ||
				m_Bullet[i].x < BOUNDARY_LEFT-0.1)
			{
				m_Bullet[i].InitBullet((m_Player1.x + m_Player2.x)/2 , (m_Player1.y+m_Player2.y)/2);
			}
			//////////////////////////////////////////////////////////////////////////
			// ��ײ��⡪�����1 �� �ӵ�
			float tmpX = m_Bullet[i].x - m_Player1.x;
			float tmpY = m_Bullet[i].y - m_Player1.y;
			if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R -0.1)
			{
				gameOver = true;
				win = 2;
			}
			// ��� ���1 ����
			if(false == m_Player1.b_excl)
			{
				if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R + 0.3)
				{
					// ����ѭ�����е㣬��һ���ҵ���������
					if (tmpI == -1)
					{
						tmpI = i;
					}
					// ������ͬ�ĵ㣬���ҽ�������������ӵ�������ܺ�
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
			// ��ײ��⡪�����2 �� �ӵ�
			tmpX = m_Bullet[i].x - m_Player2.x;
			tmpY = m_Bullet[i].y - m_Player2.y;
			if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R -0.1)
			{
				gameOver = true;
				win = 1;
			}
			// ��� ���2 ����
			if(false == m_Player2.b_excl)
			{
				if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R + BULLET_R + 0.3)
				{
					// ����ѭ�����е㣬��һ���ҵ���������
					if (tmpI2 == -1)
					{
						tmpI2 = i;
					}
					// ������ͬ�ĵ㣬���ҽ�������������ӵ�������ܺ�
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
			// �������һ�ײ
			tmpX = m_Player1.x - m_Player2.x;
			tmpY = m_Player1.y - m_Player2.y;
			if (sqrt(tmpX*tmpX+tmpY*tmpY) < PLAYER_R * 2 -0.05)
			{
				gameOver = true;
				win = 0;
			}
		}
	}

	// ����ģʽ����
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


	//�������
	m_Player1.DrawPlayer(m_Player1.x, m_Player1.y,PLAYER_R,40);
	m_Player2.DrawPlayer(m_Player2.x, m_Player2.y,PLAYER_R,41);

	// �����ӵ�
	for (int i = 0; i<nowBullet; i++)
	{
		m_Bullet[i].DrawBullet(m_Bullet[i].x, m_Bullet[i].y, BULLET_R, -1);
	}
	// ���� ���1 ����
	if ((true == m_Player1.b_excl)&&(timeCount - excl_time > 220))
	{
		if (maybeExcl)
		{
			// ����
			excellentNum++;
			maybeExcl = false;
		}
		m_Player1.DrawExcellent(excl_x, excl_y,inc);
	}
	// ���� ���2 ����
	if ((true == m_Player2.b_excl)&&(timeCount - excl_time2 > 220))
	{
		if (maybeExcl2)
		{
			// ����
			excellentP2++;
			maybeExcl2 = false;
		}
		m_Player2.DrawExcellent(excl_x2, excl_y2,inc);
	}

	//////////////////////////////////////////////////////////////////////////
	// ����
	if (KEY_UP(27))
	{
		gameOver = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// ����ģʽ
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
// ��Ϸ��ʼ��
void CGameCore::InitGame()
{
	// ��ʱ������
	timeCount = 0;
	timeFlash = 0.0;
	// ��������
	excellentNum = 0;
	excellentP2 = 0;
	// ��������
	TTscore = 0;

	//////////////////////////////////////////////////////////////////////////
	// ��ʼ������ģʽ

	// ����ģʽ
	if (1 == g_MenuStateMain)
	{
		m_Player1.InitPlayer(0,-2,PLAYER_R,1);
		m_Player1.b_excl = false;
		// ��ʼ���ӵ�����Ϸ�ٶ�()
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
	// ˫��ģʽ
	if (2 == g_MenuStateMain)
	{
		m_Player1.InitPlayer(-2.5, -1,PLAYER_R,1);	// ��ʼ�����1
		m_Player1.b_excl = false;					// ���һ�����ʶ
		m_Player2.InitPlayer(2.5, -1,PLAYER_R,1);	// ��ʼ�����2
		m_Player2.b_excl = false;					// ��Ҷ������ʶ

		nowBullet = BULLET_NUM;
		nowSpeed = BULLET_SPEED;

		// ��ʼ���ӵ�
		for (int i = 0; i<BULLET_NUM; i++)
		{
			m_Bullet[i].InitBullet((m_Player1.x + m_Player2.x)/2 , (m_Player1.y+m_Player2.y)/2);
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
// ��ʱ�����������ŵ�ʱ��
void CGameCore::CountTimeFlash(DWORD inc)
{
	if (gameOver == false)
	{
		timeCount += inc;
		timeFlash = float(timeCount)/1000;
	}
}


//////////////////////////////////////////////////////////////////////////
// ��ײ���
void CGameCore::CollisionDetection( int i)
{
	
}


//////////////////////////////////////////////////////////////////////////
// ��Ϸ����
void CGameCore::GameOver( DWORD inc )
{

	//////////////////////////////////////////////////////////////////////////
	// ����ģʽ
	if (1 == g_MenuStateMain)
	{
		// ����÷�
		TTscore = timeFlash*100+300.0*excellentNum;

		// ��¼�߷�
		switch(g_MenuStateSub)
		{
		case 1:		// ������Ϸ
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
		case 2:		// ����ģʽ
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
		case 3:		// �൯ģʽ
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
		// ������Ϸģʽ
		g_myModel.TextureSquareTGA(0,5,0,6,2,g_MenuStateSub+5);
		// ���س���ʼ��Ϸ
		g_myModel.TextureSquareTGA(0,1.5,0,9,1,20);
		// ʱ��
		g_myModel.TextureSquareTGA(-2.2,-1.5,0,1.3,0.7,21);
		// ����
		g_myModel.TextureSquareTGA(0.5,-1.5,0,1.3,0.7,22);
		// �÷�
		g_myModel.TextureSquareTGA(3.0,-1.5,0,1.3,0.7,23);
		// ����
		g_myModel.TextureSquareTGA(-4.5,-2.65,0,1.3,0.7,24);

		if (g_MenuStateSub != 4 && g_MenuStateSub != 5)
		{
			// ���
			g_myModel.TextureSquareTGA(-4.5,-3.9,0,1.3,0.7,26);
		}

		glMatrixMode(GL_PROJECTION);	

		glPushMatrix();								
		glLoadIdentity();						
		glOrtho(0,g_Width,0,g_Height,-1,1);				// ����Ϊ��ͶӰ
		glMatrixMode(GL_MODELVIEW);						

		glColor3f(0.0,1.0,0.0);
		// ��ǰʱ��
		g_myModel.glPrint(g_Width*300/800, g_Height*190/600,"%.2f",timeFlash);
		// ��ǰ����
		g_myModel.glPrint(g_Width*410/800, g_Height*190/600,"%d",excellentNum);
		// ��ǰ�ܷ�
		g_myModel.glPrint(g_Width*490/800, g_Height*190/600,"%d",TTscore);
		// ���Ѷ�ģʽû�м�¼
		if (g_MenuStateSub != 4 && g_MenuStateSub != 5)
		{
			// ���ʱ��
			g_myModel.glPrint(g_Width*300/800, g_Height*140/600,"%.2f",HIGHtime[g_MenuStateSub]);
			// ��߾���
			g_myModel.glPrint(g_Width*410/800, g_Height*140/600,"%d",HIGHexce[g_MenuStateSub]);
			// ����ܷ�
			g_myModel.glPrint(g_Width*490/800, g_Height*140/600,"%d",HIGHscore[g_MenuStateSub]);
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	//////////////////////////////////////////////////////////////////////////
	// ˫����Ϸ
	if (2 == g_MenuStateMain)
	{
		// ������Ϸģʽ
		g_myModel.TextureSquareTGA(0,6,0,6,2,g_MenuStateSub+10);
		// ���س���ʼ��Ϸ
		g_myModel.TextureSquareTGA(0,-2.5,0,8,0.7,20);
		// ʱ��
		g_myModel.TextureSquareTGA(-0.5,-4.5,0,1.3,0.7,21);
		// ���� ���1
		g_myModel.TextureSquareTGA(-6,-3.5,0,1.3,0.7,22);
		// ���� ���2
		g_myModel.TextureSquareTGA(6,-3.5,0,1.3,0.7,22);

		switch(win)
		{
		case 0:
			// ƽ��
			g_myModel.TextureSquareTGA( 6, 2, 0, 5.5, 5.5, 29);
			g_myModel.TextureSquareTGA(-6, 2, 0, 5.5, 5.5, 29);
			break;
		case 1:
			// ���1 ʤ
			g_myModel.TextureSquareTGA(-6, 2, 0, 5.5, 5.5, 28);
			g_myModel.TextureSquareTGA( 6, 2, 0, 5.5, 5.5, 27);
			break;
		case 2:
			// ���2 ʤ
			g_myModel.TextureSquareTGA(-6, 2, 0, 5.5, 5.5, 27);
			g_myModel.TextureSquareTGA( 6, 2, 0, 5.5, 5.5, 28);
			break;
		}
	}
	

	// ��ʼ��Ϸ
	if ( KEY_UP(13) || KEY_UP(32) )
	{
		gameOver = false;
		InitGame();
	}
	// �����ϼ��˵�
	if (KEY_UP(27))
	{
		g_GameState = 1;
		initGame = false;
	}
}

//////////////////////////////////////////////////////////////////////////
// �������

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
// ������
void CGameCore::NotFinished( DWORD inc )
{
	// ������ͼ������ͼ�Ĳ������������
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);	

	glPushMatrix();								
	glLoadIdentity();						
	glOrtho(0,g_Width,0,g_Height,-1,1);				// ����Ϊ��ͶӰ
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