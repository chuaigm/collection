
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

// 画出标题
void CGameMenu::DrawTitle( DWORD inc )
{

}
// 画出菜单
void CGameMenu::DrawMenuPPW( DWORD inc )
{
	/*glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);*/

	// 第一列菜单
	if (1 == column)
	{
		// 下
		if (KEY_UP(40))
		{
			// 绘制光标下移
			menuCurY -= DET_Y_MENU;

			if (menuCurY < ORI_Y_MENU - DET_Y_MENU*(float)(nMMitem-1)-0.001)
			{
				menuCurY = ORI_Y_MENU;
			}
			// 主选单选项
			g_MenuStateMain ++;
			if (g_MenuStateMain > nMMitem)
			{
				g_MenuStateMain = 1;
			}
		}
		// 上
		if (KEY_UP(38))
		{
			// 绘制逛遍上移
			menuCurY += DET_Y_MENU;
			if (menuCurY > ORI_Y_MENU+0.001)
			{
				menuCurY = ORI_Y_MENU - DET_Y_MENU*(float)(nMMitem-1);
			}
			// 主选单选项
			g_MenuStateMain--;
			if (g_MenuStateMain < 1)
			{
				g_MenuStateMain = nMMitem;
			}
		}
		// 回车
		if (KEY_UP(13))
		{
			// 如果这个主项下面有分项
			if (0!=nSMitem[g_MenuStateMain-1])
			{
				// 菜单光标移至次级菜单
				menuCurX = SUB_X_MENU;

				// 记录当前主菜单光标的Y值
				nowY = menuCurY;

				// 进入次级菜单
				column = 2;
				//g_GameState --;
			}
			else
			{
				// 独立的单项
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
	// 第二列菜单
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
		// 下
		if (KEY_UP(40))
		{
			// 绘制光标下移
			menuCurY -= DET_Y_MENU;
			if (menuCurY < nowY - DET_Y_MENU*(float)(nSMitem[g_MenuStateMain-1]-1) -0.001)
			{
				menuCurY = nowY;
			}
			// 主选单选项
			g_MenuStateSub ++;
			if (g_MenuStateSub > nSMitem[g_MenuStateMain-1])
			{
				g_MenuStateSub = 1;
			}
		}
		// 上
		if (KEY_UP(38))
		{
			// 绘制逛遍上移
			menuCurY += DET_Y_MENU;
			if (menuCurY > nowY + 0.001)
			{
				menuCurY = nowY - DET_Y_MENU*(float)(nSMitem[g_MenuStateMain-1]-1);
			}
			// 主选单选项
			g_MenuStateSub--;
			if (g_MenuStateSub < 1)
			{
				g_MenuStateSub = nSMitem[g_MenuStateMain-1];
			}
		}
		// 回车
		if (KEY_UP(13))
		{
			// 进入游戏核心
			g_GameState = 2;
		}

		// 绘制部分
		
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

	// 绘制主菜单
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
// 画出成就
void CGameMenu::DrawAchievement( DWORD inc )
{
	NotFinished(inc);
	if (KEY_UP(27))
	{
		g_GameState = 1;
	}
}
// 画出关于
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