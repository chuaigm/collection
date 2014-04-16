
#include "GameMenu.h"

CGameMenu::CGameMenu()
{
	menuCurX = ORI_X_MENU;
	menuCurY = ORI_Y_MENU;
	column = 1;
}

CGameMenu::~CGameMenu()
{

}

// ��������
void CGameMenu::DrawTitle( DWORD inc )
{

}
// �����˵�
void CGameMenu::DrawMenuPPW( DWORD inc )
{
	/*glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);*/

	// ��һ�в˵�
	if (1 == column)
	{
		// ��
		if (KEY_UP(40))
		{
			// ���ƹ������
			menuCurY -= DET_Y_MENU;

			if (menuCurY < ORI_Y_MENU - DET_Y_MENU*(float)(nMMitem-1)-0.001)
			{
				menuCurY = ORI_Y_MENU;
			}
			// ��ѡ��ѡ��
			g_MenuStateMain ++;
			if (g_MenuStateMain > nMMitem)
			{
				g_MenuStateMain = 1;
			}
		}
		// ��
		if (KEY_UP(38))
		{
			// ���ƹ������
			menuCurY += DET_Y_MENU;
			if (menuCurY > ORI_Y_MENU+0.001)
			{
				menuCurY = ORI_Y_MENU - DET_Y_MENU*(float)(nMMitem-1);
			}
			// ��ѡ��ѡ��
			g_MenuStateMain--;
			if (g_MenuStateMain < 1)
			{
				g_MenuStateMain = nMMitem;
			}
		}
		// �س�
		if (KEY_UP(13))
		{
			// ���������������з���
			if (0!=nSMitem[g_MenuStateMain-1])
			{
				// �˵���������μ��˵�
				menuCurX = SUB_X_MENU;

				// ��¼��ǰ���˵�����Yֵ
				nowY = menuCurY;

				// ����μ��˵�
				column = 2;
				//g_GameState --;
			}
			else
			{
				// �����ĵ���
				if ( 4 == g_MenuStateMain)
				{
					g_GameState = 3;
				}
				if ( 5 == g_MenuStateMain)
				{
					g_GameState = 4;
				}
			}
		}
	}
	// �ڶ��в˵�
	else
	{
		// ESC
		if (KEY_UP(27))
		{
			menuCurY = nowY;
			menuCurX = ORI_X_MENU;
			column = 1;
			g_MenuStateSub = 1;
		}
		// ��
		if (KEY_UP(40))
		{
			// ���ƹ������
			menuCurY -= DET_Y_MENU;
			if (menuCurY < nowY - DET_Y_MENU*(float)(nSMitem[g_MenuStateMain-1]-1) -0.001)
			{
				menuCurY = nowY;
			}
			// ��ѡ��ѡ��
			g_MenuStateSub ++;
			if (g_MenuStateSub > nSMitem[g_MenuStateMain-1])
			{
				g_MenuStateSub = 1;
			}
		}
		// ��
		if (KEY_UP(38))
		{
			// ���ƹ������
			menuCurY += DET_Y_MENU;
			if (menuCurY > nowY + 0.001)
			{
				menuCurY = nowY - DET_Y_MENU*(float)(nSMitem[g_MenuStateMain-1]-1);
			}
			// ��ѡ��ѡ��
			g_MenuStateSub--;
			if (g_MenuStateSub < 1)
			{
				g_MenuStateSub = nSMitem[g_MenuStateMain-1];
			}
		}
		// �س�
		if (KEY_UP(13))
		{
			// ������Ϸ����
			g_GameState = 2;
		}

		// ���Ʋ���
		
		if (1== g_MenuStateMain)
		{
			g_myModel.TextureSquareTGA(ORI_X_MENU+10.5, ORI_Y_MENU,                0, 3.5, 1.3, 6);
			g_myModel.TextureSquareTGA(ORI_X_MENU+10.5, ORI_Y_MENU - DET_Y_MENU,   0, 3.5, 1.3, 7);
			g_myModel.TextureSquareTGA(ORI_X_MENU+10.5, ORI_Y_MENU - 2*DET_Y_MENU, 0, 3.5, 1.3, 8);
			g_myModel.TextureSquareTGA(ORI_X_MENU+10.5, ORI_Y_MENU - 3*DET_Y_MENU, 0, 3.5, 1.3, 9);
			g_myModel.TextureSquareTGA(ORI_X_MENU+10.5, ORI_Y_MENU - 4*DET_Y_MENU, 0, 3.5, 1.3, 10);
		}
		if (2 == g_MenuStateMain)
		{
			g_myModel.TextureSquareTGA(ORI_X_MENU+10.5, nowY,                0, 3.5, 1.3, 11);
			g_myModel.TextureSquareTGA(ORI_X_MENU+10.5, nowY - DET_Y_MENU,   0, 3.5, 1.3, 12);
			g_myModel.TextureSquareTGA(ORI_X_MENU+10.5, nowY - 2*DET_Y_MENU, 0, 3.5, 1.3, 13);
		}
		if (3 == g_MenuStateMain)
		{
			g_myModel.TextureSquareTGA(ORI_X_MENU+10.5, nowY,			     0, 3.5, 1.3, 14);
			g_myModel.TextureSquareTGA(ORI_X_MENU+10.5, nowY - DET_Y_MENU, 0, 3.5, 1.3, 15);
		}
	}

	// �������˵�
	g_myModel.DrawMenuCur(menuCurX,menuCurY,0,inc);

	g_myModel.TextureSquareTGA(ORI_X_MENU+3.5, ORI_Y_MENU,                0, 5.1, 1.7, 1);
	g_myModel.TextureSquareTGA(ORI_X_MENU+3.5, ORI_Y_MENU - DET_Y_MENU,   0, 5, 1.7, 2);
	g_myModel.TextureSquareTGA(ORI_X_MENU+3.5, ORI_Y_MENU - 2*DET_Y_MENU, 0, 5, 1.7, 3);
 	g_myModel.TextureSquareTGA(ORI_X_MENU+3.5, ORI_Y_MENU - 3*DET_Y_MENU, 0, 4, 1.5, 4);
 	g_myModel.TextureSquareTGA(ORI_X_MENU+3.5, ORI_Y_MENU - 4*DET_Y_MENU, 0, 4, 1.5, 5);

	/*glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);*/
	
	g_myModel.DrawGrid(1,0,inc);
}
// �����ɾ�
void CGameMenu::DrawAchievement( DWORD inc )
{
	NotFinished(inc);
	if (KEY_UP(27))
	{
		g_GameState = 1;
	}
}
// ��������
void CGameMenu::DrawAbout( DWORD inc )
{
	NotFinished(inc);
	if (KEY_UP(27))
	{
		g_GameState = 1;
	}
}

void CGameMenu::NotFinished( DWORD inc )
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