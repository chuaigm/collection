#include "Bullet.h"

CBullet::CBullet(void)
{
	// 生成随机数种子
	srand(time(0));
}

CBullet::~CBullet(void)
{
}

void CBullet::InitBullet(float bx, float by)
{
	//		   初始位置：
	//                 0
	//           -------------
	//           |           |
	//        1  |           |  3
	//           |           |
	//           -------------
	//                 2
	char initDire = rand()%4;
	switch(initDire)
	{
	case 0:
		{
			x = (float)(BOUNDARY_RIGHT*1000-rand()%(BOUNDARY_RIGHT*2000))/1000;
			y = BOUNDARY_TOP;
		}
		break;
	case 1:
		{
			x = BOUNDARY_LEFT;
			y = (float)(BOUNDARY_TOP*1000-rand()%(BOUNDARY_TOP*2000))/1000;
		}
		break;
	case 2:
		{
			x = (float)(BOUNDARY_RIGHT*1000-rand()%(BOUNDARY_RIGHT*2000))/1000;
			y = BOUNDARY_BOTTOM;
		}
		break;
	case 3:
		{
			x = BOUNDARY_RIGHT;
			y = (float)(BOUNDARY_TOP*1000-rand()%(BOUNDARY_TOP*2000))/1000;
		}
		break;
	}
	// 计算向量
	float tmp_x = bx - x;
	float tmp_y = by - y;
	// 单位化 + 并加上少量偏移
	v_x = tmp_x/sqrt(tmp_x*tmp_x+tmp_y*tmp_y)+(float)(20-rand()%40)/100;
	v_y = tmp_y/sqrt(tmp_x*tmp_x+tmp_y*tmp_y)+(float)(20-rand()%40)/100;
}

void CBullet::DrawBullet()
{

}

void CBullet::DrawBullet( float bx, float by, float br, int bt )
{
	if (bt < 0)
	{
		glColor3f(1,1,0);
		g_myModel.DrawSolidCircle(bx,by,0,br);
	}
	else
	{
		g_myModel.TextureSquareTGA(bx,by,0,br*3,br*3,bt);
	}
	glColor3f(1,1,1);
}