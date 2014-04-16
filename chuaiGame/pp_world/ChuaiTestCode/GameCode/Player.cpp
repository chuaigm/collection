#include "Player.h"

CPlayer::CPlayer(void)
{
	b_excl = false;
}

CPlayer::~CPlayer(void)
{
}


void CPlayer::InitPlayer( float px, float py, float pr, float pspeed )
{
	x = px;
	y = py;
	R = pr;
	speed = pspeed;
}

void CPlayer::DrawPlayer()
{

}

void CPlayer::DrawPlayer( float px, float py, float pr, int pt )
{
	if (pt < 0)
	{
		glColor3f(0,1,0);
		g_myModel.DrawSolidCircle(px,py,0,pr);
	}
	else
	{
		glColor3f(1,1,1);
		g_myModel.TextureSquareTGA(px,py,0,pr*3,pr*3,pt);
	}
	glColor3f(1,1,1);
}

void CPlayer::DrawExcellent( float px, float py, DWORD inc )
{
	static DWORD ori = 0;
	// 是否绝妙
	if (b_excl)
	{
		ori += inc;
		if ( ori < 700 )
		{
			glColor3f(1,1,1);
			g_myModel.TextureSquareTGA(px,py,0.2,0.5+float(ori)/1500.0, 0.5+float(ori)/1500.0, 39);
		}
		else
		{
			ori = 0;
			b_excl = false;  // 标识取反
		}
	}
}
