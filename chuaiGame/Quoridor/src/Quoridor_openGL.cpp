//=======================================
//
// 游戏：步步为营
//       Quoridor
// 作者：chuaiGM
// date: 2014-11-29
//
//=======================================

#include "stdafx.h"
#include "myclock.h"
#include "glfont.h"
#include "OpenGLbase.h"
#include "Quoridor_openGL.h"

//font
extern CGLFont myfont;
//clock
extern MYCLOCK c1;
//mouse
extern int Lbutdown;
// OpenGL环境对象指针在主函数文件中声明
extern COpenGLbase* m_OpenGL;
// 外部输入的窗口宽高
extern int WinWidth;
extern int WinHeight;

/////////////////////////

CQuoridor::CQuoridor()
{
	// 当前游戏状态
	iGameState=GAME_MENU;
	// 当前菜单选择项
	iMenu=-1;
	// 菜单坐标位置
	menu.x=10;
	menu.y=10;
	// 菜单按钮的宽度和高度
	menu_w=10;
	menu_h=10;
	// 帮助界面返回菜单按钮宽度
	helpRetButtonW=10;
	// 帮助界面返回菜单按钮横坐标
	rButtonx=10;
	// 菜单上下间距
	menu_dis=10;
	// 游戏棋盘与窗口边界间距
	lace=6;

	iButton=-1;

	// 游戏数据初始化
	roadw = 10;
	wall_l=10;
	wall_w=10;
	player_info_h=10;
	player_info_w=10;
	board_x=10;
	board_y=10;
	arr.x=-1;
	arr.y=-1;
	yellow_ply.x=0;
	yellow_ply.y=4;
	red_ply.x=4;
	red_ply.y=8;
	green_ply.x=8;
	green_ply.y=4;
	blue_ply.x=4;
	blue_ply.y=0;

	// 棋盘数据初始化
	memset(gameData,0,sizeof(gameData));
	// 这里的顺序需要注意，这里暂时按照先x后y的顺序去做，有问题再说
	gameData[2*yellow_ply.x][2*yellow_ply.y]=GD_YELLOW;
	gameData[2*red_ply.x][2*red_ply.y]=GD_RED;
	gameData[2*green_ply.x][2*green_ply.y]=GD_GREEN;
	gameData[2*blue_ply.x][2*blue_ply.y]=GD_BLUE;
	// 鼠标选取的位置
	pickup.x=-1;
	pickup.y=-1;
	// 墙的第一个选取位置
	wall_pick.x=-1;
	wall_pick.y=-1;
}

CQuoridor::~CQuoridor()
{
	
}
//游戏所需数据文件检查,如果缺少,程序中止
int CQuoridor::haveDataFile()
{
	int ihave=1;
	FILE *fp;
	char *fdata[]={
		//bmp texture
		"data/images/quoridor_cover.bmp",
		"data/images/button.bmp",
		//sound
		"data/sound/explode1.wav"
		};
	int i;
	
	for(i=0;i<sizeof(fdata)/sizeof(fdata[0]);i++)
	{
		fp=fopen(fdata[i],"r");
		if(!fp)
		{
			ihave=0;//缺少文件
			break;
		}
		else
			fclose(fp);
	}

	//如果当前文件夹没有,返回上级文件夹,再检查
	if(!ihave)
		SetCurrentDirectory("..");

	for(i=0;i<sizeof(fdata)/sizeof(fdata[0]);i++)
	{
		fp=fopen(fdata[i],"r");
		if(!fp)
		{
			return 0;
		}
		else
			fclose(fp);
	}

	return 1;
}

//加载各个图片,模型,初始化全局数据
void CQuoridor::init()
{
	//mdl模型要最先初始化
//	m_mdlobj.InitGL(0,"data/mdl/qian1.mdl");	
/*
	//load images
	LoadT8("data/images/start1.bmp", g_cactus[0]);
	LoadT8("data/images/start2.bmp", g_cactus[1]);

	LoadT8("data/images/box0.bmp", TexBox[0]);
	LoadT8("data/images/box01.bmp", TexBox[1]);
	LoadT8("data/images/box1.bmp", TexBox[2]);
	LoadT8("data/images/box11.bmp", TexBox[3]);
	LoadT8("data/images/box2.bmp", TexBox[4]);
	LoadT8("data/images/box21.bmp", TexBox[5]);
	//up
	LoadT8("data/images/map2.bmp", g_cactus[4]);
	//ground
	LoadT8("data/images/map5.bmp", g_cactus[7]);

//	g_text = gluNewQuadric();
//	gluQuadricTexture(g_text, GL_TRUE);

	LoadT8("data/images/TRIX1.bmp", g_cactus[8]);
*/
	//load images
	LoadT8("data/images/quoridor_cover.bmp", g_cactus[0]);
	LoadT8("data/images/chess_board_shading.bmp", g_cactus[1]);
	LoadT8("data/images/road.bmp", g_cactus[2]);
	LoadT8("data/images/bidiu1.bmp", g_cactus[3]);
	LoadT8("data/images/huangshou1.bmp", g_cactus[4]);
	LoadT8("data/images/diting1.bmp", g_cactus[5]);
	LoadT8("data/images/xiaohei1.bmp", g_cactus[6]);
	LoadT8("data/images/wall1.bmp", g_cactus[7]);
	//button
	LoadT8("data/images/button.bmp", g_cactus[9]);

	///////////////////////////////////////////
	//启动贴图
	glEnable(GL_TEXTURE_2D);

	//游戏状态
//	iGameState=GAME_PRE_ANI;
	iGameState=GAME_MENU;
}

//设置投影方式
void CQuoridor::initView()
{
	//switch(iGameState)
	//{
	//case GAME_MENU:
	//	m_OpenGL->init_2D();
	//	break;

	//default:
	//	//m_OpenGL->init_3D();
	//	m_OpenGL->init_2D();
	//	break;
	//}

	// 创建二维的OpenGL视图模型
	m_OpenGL->init_2D();

	// 在这里创建一些从OpenGL中获取的窗口参数
	// 这里也可以单独立一个函数去实现
	// 菜单上下间距
	menu_dis=m_OpenGL->RCheight/15;
	// 菜单按钮的宽度和高度
	menu_w=m_OpenGL->RCwidth/9;
	menu_h=m_OpenGL->RCheight/20;
	// 帮助界面返回菜单按钮宽度
	helpRetButtonW=m_OpenGL->RCwidth/5;
	rButtonx=(m_OpenGL->RCwidth-helpRetButtonW)/2;
	// 菜单坐标位置
	menu.x=m_OpenGL->RCwidth-menu_dis/2-menu_w;
	menu.y=/*menu_dis/2 +*/ menu_h;

	// 游戏棋盘数据
	player_info_h=m_OpenGL->RCheight/4;
	player_info_w=(m_OpenGL->RCwidth-m_OpenGL->RCheight)*3/8;
	// 棋盘左下角，起点坐标
	board_x = (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2;
	board_y = 0;
	// 墙和路的宽度推算公式：
	//  x=roadw; y=wall_w
	//  9*x + 8*y = Height - lace*2
	//    x = 4 y
	roadw = (m_OpenGL->RCheight - lace*2)/11.0f;
	wall_w= (m_OpenGL->RCheight - lace*2)/44.0f;
	wall_l= 2*roadw+wall_w;
}

// 游戏主绘图函数
void CQuoridor::showMain()
{
	// 显示测试数据
	show_Font_test();

	switch(iGameState)
	{
	case GAME_PRE:
//		showpreani();
		break;
	case GAME_MENU:
		showmenu();
		break;
	case GAME_SINGE:
	case GAME_MULTIP:
	case GAME_SENDBOX:
		showChessBorad();
		showPlayerWall();
		// 注意此处，透明图层与绘制顺序有关
		drawPickMask();
		break;

	case GAME_HELP:
		showHelp();
		break;

	default:
		break;
	}
}
//实时检测
void CQuoridor::check()
{
	int i;
	int x,y;
	int it;
	float remain;
	//检测菜单选择
	x=m_OpenGL->Xmouse;
	y=m_OpenGL->Ymouse;
	iMenu=-1;//初始化 没有选择
	iButton=-1;
	arr.x=-1;
	arr.y=-1;

	switch(iGameState)
	{
	case GAME_PRE:
		if(c1.iNumClip%2==0)
		{
			if(param1<30)
			{			
				param1++;		
			}
		}
		//计时
		if(c1.clipcount())
		{
			iGameState=GAME_MENU;
			//设置投影 
			initView();
		}
		break;

	case GAME_MENU:
		if(x<menu.x	|| x>menu.x+menu_w || y<menu.y)
			break;

		for(i=0;i<MENU_NUM;i++)
		{
			if( y>menu.y+i*menu_dis && y<menu.y+i*menu_dis+menu_h )
			{
				iMenu=i;
				break;
			}
		}
		break;
	case GAME_SINGE:
	case GAME_MULTIP:
	case GAME_SENDBOX:
		// 实时检查鼠标位置
		// 检查时，如果鼠标位于棋盘边界外
		if (x<board_x+lace || x>board_x+m_OpenGL->RCheight-lace)
			break;
		// 当前位置除一个路的宽度和一个墙的宽度的整数部分
		it=(int)((x-board_x-lace)/(roadw+wall_w));
		// 余数部分
		remain = (x-board_x-lace)-it*(roadw+wall_w);
		if (remain>roadw)
		{	// 余数比路宽度大，这是墙的位置，奇数
			arr.x = it*2+1;
		} else {
			// 玩家可以放置的位置，路，偶数
			arr.x = it*2;
		}
		it = (int)((y-lace)/(roadw+wall_w));
		remain = (y-lace) - it*(roadw+wall_w);
		if (remain>roadw)
		{
			arr.y = it*2+1;
		} else {
			arr.y = it*2;
		}
		break;
	case GAME_HELP:
		if (x>rButtonx&&x<rButtonx+helpRetButtonW&&y>menu.y&&y<menu.y+menu_h)
		{
			iButton=9;
		}
		break;

	default:
		break;
	}
}
// 左键松开
void CQuoridor::lbuttonproc(int lparam)
{
	switch(iGameState)
	{
	case GAME_MENU:
		if(iMenu<0)
			break;
		switch(iMenu)
		{
		case MENU_SINGE:
			//iGameState=GAME_SINGE;
			break;
		case MENU_MULTIP:
			//iGameState=GAME_MULTIP;
			break;
		case MENU_SENDBOX:
			iGameState=GAME_SENDBOX;
			break;
		case MENU_HELP:
			iGameState=GAME_HELP;
			break;
		case MENU_QUIT:
			m_OpenGL->CleanUp();
			PostQuitMessage(0);
			exit(0);
			break;
		default:
			break;
		}		
		break;
	case GAME_SINGE:
	case GAME_MULTIP:
	case GAME_SENDBOX:
		// 存在已选取的位置，并且鼠标再次点击时，位置为双偶数位
		if (-1 != pickup.x && -1 != pickup.y && 0==arr.x%2 && 0==arr.y%2)
		{	// 这种情况，进入到人物棋子处理阶段
			char tmp=0;
			// 交换
			tmp=gameData[arr.x][arr.y];
			gameData[arr.x][arr.y]=gameData[2*pickup.x][2*pickup.y];
			gameData[2*pickup.x][2*pickup.y]=tmp;
			// 在目标位置上，更新玩家变量
			switch (gameData[arr.x][arr.y])
			{
			case GD_BLANK:
				break;
			case GD_YELLOW:
				yellow_ply.x=arr.x/2;
				yellow_ply.y=arr.y/2;
				break;
			case GD_RED:
				red_ply.x=arr.x/2;
				red_ply.y=arr.y/2;
				break;
			case GD_GREEN:
				green_ply.x=arr.x/2;
				green_ply.y=arr.y/2;
				break;
			case GD_BLUE:
				blue_ply.x=arr.x/2;
				blue_ply.y=arr.y/2;
				break;
			default:
				break;
			}
			// 对于旧的位置上，更新玩家变量
			switch (gameData[2*pickup.x][2*pickup.y])
			{
			case GD_BLANK:
				break;
			case GD_YELLOW:
				yellow_ply.x=pickup.x;
				yellow_ply.y=pickup.y;
				break;
			case GD_RED:
				red_ply.x=pickup.x;
				red_ply.y=pickup.y;
				break;
			case GD_GREEN:
				green_ply.x=pickup.x;
				green_ply.y=pickup.y;
				break;
			case GD_BLUE:
				blue_ply.x=pickup.x;
				blue_ply.y=pickup.y;
				break;
			default:
				break;
			}
			// 将选取位置赋值为无效值
			pickup.x=-1;
			pickup.y=-1;
		}	// 从这里开始处理墙相关的
		else if (arr.x>-1 && arr.y>-1)
		{
			// 如果是一个双偶坐标，且选取位置是非空白位置(说明此处应该是某一玩家棋子位置)
			if (arr.x%2==0 && arr.y%2==0 && GD_BLANK != gameData[arr.x][arr.y])
			{
				pickup.x = arr.x/2;
				pickup.y = arr.y/2;
			}
			// 如果两个数，一个是奇数一个是偶数，说明这处是一个墙可以存在的位置
			else if ((arr.x+arr.y)%2==1 && GD_BLANK == gameData[arr.x][arr.y])
			{	// 如果上次已有预选墙的位置
				if (wall_pick.x>-1&&wall_pick.y>-1)
				{	// 如果是横墙,并且这次选的和上次选的在同一行上
					if (wall_pick.x%2==0&&arr.y==wall_pick.y)
					{	// 如果这次选的在上一次选的左边一块位置
						// 并且两块连接的中间位置是可用的
						if(arr.x==wall_pick.x-2&&gameData[arr.x+1][arr.y]==GD_BLANK)
						{
							// 压入墙绘制队列,一定先压入左边的块
							wall_vec.push_back(arr);
							wall_vec.push_back(wall_pick);
							// 更新游戏算法数据,这里注意，把相连两墙位置的中间连接处也赋值
							gameData[arr.x][arr.y]=GD_WALL;
							gameData[arr.x+1][arr.y]=GD_WALL;
							gameData[wall_pick.x][wall_pick.y]=GD_WALL;
							// 预选墙清空
							wall_pick.x=-1;
							wall_pick.y=-1;
							break;
						}
						// 如果在这次选的在上一次右边一块位置
						// 并且两块连接的中间位置是可用的
						else if(arr.x==wall_pick.x+2&&gameData[arr.x-1][arr.y]==GD_BLANK)
						{
							// 压入墙绘制队列,一定先压入左边的块
							wall_vec.push_back(wall_pick);
							wall_vec.push_back(arr);
							// 更新游戏算法数据
							gameData[arr.x][arr.y]=GD_WALL;
							gameData[arr.x-1][arr.y]=GD_WALL;
							gameData[wall_pick.x][wall_pick.y]=GD_WALL;
							// 预选墙清空
							wall_pick.x=-1;
							wall_pick.y=-1;
							break;
						}
					}
					// 如果是竖墙，并且这次选的和上次选的在同一列上
					else if(wall_pick.y%2==0&&arr.x==wall_pick.x)
					{	// 如果这次选择的在上一次选择的下面一块
						// 并且两块连接的中间位置是可用的
						if (arr.y==wall_pick.y-2&&gameData[arr.x][arr.y+1]==GD_BLANK)
						{
							// 压入墙绘制队列，一定先压入下面的一块
							wall_vec.push_back(arr);
							wall_vec.push_back(wall_pick);
							// 更新游戏算法数据
							gameData[arr.x][arr.y]=GD_WALL;
							gameData[arr.x][arr.y+1]=GD_WALL;
							gameData[wall_pick.x][wall_pick.y]=GD_WALL;
							// 预选墙清空
							wall_pick.x=-1;
							wall_pick.y=-1;
							break;
						}
						// 如果这次选择的在上一次选择的上面一块
						// 并且两块连接的中间位置是可用的
						else if (arr.y==wall_pick.y+2&&gameData[arr.x][arr.y-1]==GD_BLANK)
						{
							// 压入墙绘制队列，一定先压入下面的一块
							wall_vec.push_back(wall_pick);
							wall_vec.push_back(arr);
							// 更新游戏算法数据
							gameData[arr.x][arr.y]=GD_WALL;
							gameData[arr.x][arr.y-1]=GD_WALL;
							gameData[wall_pick.x][wall_pick.y]=GD_WALL;
							// 预选墙清空
							wall_pick.x=-1;
							wall_pick.y=-1;
							break;
						}
					}
				}
				// 如果不满足以上情况的，都认为是
				// 新选取的一个预选墙的位置
				wall_pick.x=arr.x;
				wall_pick.y=arr.y;
			}
		}
		break;
	case GAME_HELP:
		if (9==iButton)
		{
			iGameState=GAME_MENU;
		}
		break;

	default:
		break;
	}
}

void CQuoridor::rbuttonproc( int lparam )
{
	switch(iGameState)
	{
	case GAME_SINGE:
	case GAME_MULTIP:
	case GAME_SENDBOX:
		switch (gameData[arr.x][arr.y])
		{
		case GD_BLANK:
			break;
		case GD_YELLOW:
			yellow_ply.x=-1;
			yellow_ply.y=-1;
			gameData[arr.x][arr.y]=0;
			break;
		case GD_RED:
			red_ply.x=-1;
			red_ply.y=-1;
			gameData[arr.x][arr.y]=0;
			break;
		case GD_GREEN:
			green_ply.x=-1;
			green_ply.y=-1;
			gameData[arr.x][arr.y]=0;
			break;
		case GD_BLUE:
			blue_ply.x=-1;
			blue_ply.y=-1;
			gameData[arr.x][arr.y]=0;
			break;
		case GD_WALL:
			for (size_t i=0; i<wall_vec.size();i++)
			{
				std::vector<pos2d>::iterator itor=find(wall_vec.begin(),wall_vec.end(),arr);
				if (itor==wall_vec.end())
				{
					continue;
				}
				int dist=distance(wall_vec.begin(),itor);
				// 如果间距是偶数，则下标也是偶数
				if (dist%2==0)
				{
					// 注意这里，可能存在迭代器失效的问题
					gameData[itor->x][itor->y]=GD_BLANK;
					gameData[(itor->x+(itor+1)->x)/2][(itor->y+(itor+1)->y)/2]=GD_BLANK;
					gameData[(itor+1)->x][(itor+1)->y]=GD_BLANK;
					wall_vec.erase(itor,itor+2);
					break;
				}
				// 如果间距是奇数，则下标也是奇数
				else
				{
					// 注意迭代器失效问题
					gameData[itor->x][itor->y]=GD_BLANK;
					gameData[(itor->x+(itor-1)->x)/2][(itor->y+(itor-1)->y)/2]=GD_BLANK;
					gameData[(itor-1)->x][(itor-1)->y]=GD_BLANK;
					wall_vec.erase(itor-1,itor+1);
					break;
				}
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

//键盘处理
void CQuoridor::keyupproc(int keyparam)
{
	switch (keyparam)
	{
		//case KEY_F12:
		//	//直接过关
		//	if(iGameState=GAME_IN)
		//	{
		//		iEnemyNum=0;
		//	}		
		//	break;

	case VK_ESCAPE:
		//回到菜单
		iGameState=GAME_MENU;
		initView();
		break;

	default:
		break;
	}
}
// 绘制游戏进行中的按钮
void CQuoridor::showInGameBotton()
{
	switch(iGameState)
	{
	case GAME_SINGE:
	case GAME_MULTIP:
	case GAME_IN_CONFIG:
		break;
	case GAME_HELP:
		break;
	default:
		break;
	}
}

//构造位图贴图
bool CQuoridor::LoadT8(char *filename, GLuint &texture)
{	
	AUX_RGBImageRec *pImage = NULL;
	
	//加载图片内容
	pImage = auxDIBImageLoad(filename);	 
	if(pImage == NULL)	
		return false;
	
	//创建纹理
	glGenTextures(1, &texture);		
	glBindTexture    (GL_TEXTURE_2D,texture);
	//生成2维纹理
	gluBuild2DMipmaps(GL_TEXTURE_2D,4,  
					  pImage->sizeX,    
					  pImage->sizeY,  
					  GL_RGB, GL_UNSIGNED_BYTE,
					  pImage->data    
					 );
	//释放内存
	free(pImage->data);      
	free(pImage);	
	return true;      
}

//指定贴图
void CQuoridor::texture_select(UINT textur)
{
	glBindTexture  (GL_TEXTURE_2D, textur);

	// GL_TEXTURE_2D: 操作2D纹理.
	// GL_TEXTURE_WRAP_S: S方向上的贴图模式.
	// GL_CLAMP: 将纹理坐标限制在0.0,1.0的范围之内.如果超出了会如何呢.
	//           不会错误,只是会边缘拉伸填充.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	// 这里同上,只是它是T方向
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	// 这是纹理过滤
	// GL_TEXTURE_MIN_FILTER: 缩小过滤
	// GL_LINEAR: 线性过滤, 使用距离当前渲染像素中心最近的4个纹素加权平均值.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	// GL_TEXTURE_MAG_FILTER: 放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// GL_LINEAR_MIPMAP_NEAREST: 使用GL_NEAREST对最接近当前多边形的解析度
	//                  的两个层级贴图进行采样,然后用这两个值进行线性插值.
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

//画正方形 ，边长2e
void CQuoridor::tPic(float e)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-e, -e,  0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( e, -e,  0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( e,  e,  0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-e,  e,  0.0f);
	glEnd();
}

//画矩形，左下角坐标xy，宽度w，高h
void CQuoridor::tPicRectangle(float x,float y,float w,float h,float deep)
{ 
	glPushMatrix();

	//画背景矩形
	glTranslatef(x,y,deep);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		
		glTexCoord2f(1.0f, 0.0f);		
		glVertex3f(w, 0.0f,  0.0f);		

		//up
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(w, h,  0.0f);
				
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, h,  0.0f);
	glEnd();	

	glPopMatrix();
}
//画按钮
void CQuoridor::tPicButton(float x,float y,float w,float h,float ytex)
{ 
	glPushMatrix();

	//画背景矩形
	glTranslatef(x,y,-0.1f);	//z轴向后移动
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, ytex);
		glVertex3f(0.0, 0.0, 0.0f);
		
		glTexCoord2f(1.0f, ytex);		
		glVertex3f(w, 0.0,  0.0f);		

		//up
		glTexCoord2f(1.0f, ytex+0.5f);
		glVertex3f(w, h,  0.0f);
				
		glTexCoord2f(0.0f, ytex+0.5f);
		glVertex3f(0.0, h,  0.0f);
	glEnd();	

	glPopMatrix();
}

//设置灯光
//void CQuoridor::light0()
//{
//	GLfloat light_position[] = {10.0,10.0,3.0,1.0};
//
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//	
//	//glEnable(GL_LIGHTING);
//	//glEnable(GL_LIGHT0);
//	
//	//启动深度检测
//	glEnable(GL_DEPTH_TEST);
//	//启动颜色材质
//	glEnable(GL_COLOR_MATERIAL);
//}

//开屏动画
//void CQuoridor::showpreani()
//{
//	glPushMatrix();
//	//移动到当前位置
//	glTranslatef(0,0,-8.0);
//	texture_select(g_cactus[0]);	
//	tPic(2.0f);
//	//移动动画
//	tSquare(param1*0.2, 0.0, 0.1, 3.0);
//	glPopMatrix();
//}

//指定位置画一个正方形
void CQuoridor::tRectangle(float x, float y, float z, float w, float h, float r, float g, float b, float a)
{
	//属性进栈
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);

	glTranslatef(x,y,z);
	glColor4f(r, g, b, a);
	//glColor3f(r, g, b);
	glBegin(GL_QUADS);
		glVertex3f( 0,  0,  0.0f);
		glVertex3f( w,  0,  0.0f);
		glVertex3f( w,  h,  0.0f);
		glVertex3f( 0,  h,  0.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();
}

//绘制鼠标
void CQuoridor::drawMouse()
{
	glPushMatrix();
	//glLoadIdentity();
	//属性进栈
	//glPushAttrib(GL_CURRENT_BIT);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_TEXTURE_2D);    
	//glDisable(GL_LIGHTING);      	

	glTranslatef((GLfloat)m_OpenGL->Xmouse,(GLfloat)m_OpenGL->Ymouse,0.1f);

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.5f, 0.0f); 
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(30.0f, -15.0f,0.0f);

	glColor3f(0.0f, 0.8f, 0.0f); 
	glVertex3f(12.0f, -12.0f,0.0f);
	glVertex3f(6.0f,-20.0f,0.0f);

	glColor3f(0.0f, 0.0f, 0.0f); 
	glVertex3f(3.0f,-20.0f,0.0f);
	glEnd();

	/////////////////////////
	//glEnable(GL_LIGHTING);         
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();
	glPopMatrix();
}

//显示菜单
void CQuoridor::showmenu()
{
	//画背景图片
	glPushMatrix();
	glTranslatef(0.0,0.0,-0.2f);
	texture_select(g_cactus[0]);
	tPicRectangle((m_OpenGL->RCwidth-m_OpenGL->RCheight)/2.0f, 0, (float)m_OpenGL->RCheight, (float)m_OpenGL->RCheight);
	glPopMatrix();

	//  对应枚举关系     0          1          2          3          4
	char *menustr[]={"退    出","游戏帮助","沙盒模式","联网游戏","单机游戏"};
	//char *menustr[]={"单机游戏","联网游戏","沙盒模式","游戏帮助","退    出"};

	// 在glOrtho模式下绘制菜单
	// 菜单的是从下网上绘制的，所以菜单序号如上所示
	for(int i=0;i<MENU_NUM;i++)
	{
		//文字
		myfont.Print2D(menu.x+10,menu.y+5+i*menu_dis,menustr[i],FONT1,1,1,1);

		//图片
		texture_select(g_cactus[9]);
		if(iMenu==i)
		{
			tPicButton((float)menu.x,(float)(menu.y+i*menu_dis),(float)menu_w,(float)menu_h,0.0f);
		}
		else
		{
			tPicButton((float)menu.x,(float)(menu.y+i*menu_dis),(float)menu_w,(float)menu_h,0.5f);
		}
	}
}

//鼠标检测函数
//void CQuoridor::mouseproc(int lparam)
//{
//	xmouseOld=xmouse;
//	ymouseOld=ymouse;
//	xmouse=LOWORD(lparam);
//	ymouse=HIWORD(lparam);
//}

// 显示测试数据
void CQuoridor::show_Font_test()
{
	// Y /-------------\
	//   |600          |(800,600)
	//   |             |
	//   |0         800|
	//   \-------------/ X

	char tmpstr[64]={0};
	sprintf(tmpstr, "iMenu=%d",iMenu);
	myfont.Print2D(50,2,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);

	for (int i=0; i<17;i++)
	{
		for (int j=0; j<17;j++)
		{
			sprintf(tmpstr, "%d,",gameData[j][i]);
			myfont.Print2D(10*(j+1),70+10*i,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);
		}
	}
	sprintf(tmpstr, "wall_px=%d",wall_pick.x);
	myfont.Print2D(50,60,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);
	sprintf(tmpstr, "wall_py=%d",wall_pick.y);
	myfont.Print2D(50,50,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);
	sprintf(tmpstr, "pick_x=%d",pickup.x);
	myfont.Print2D(50,40,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);
	sprintf(tmpstr, "pick_y=%d",pickup.y);
	myfont.Print2D(50,30,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);
	sprintf(tmpstr, "arr.x=%d",arr.x);
	myfont.Print2D(50,20,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);
	sprintf(tmpstr, "arr.y=%d",arr.y);
	myfont.Print2D(50,10,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);

	//myfont.Print2D(100,2,"1,0",FONT1,1.0f,0.0f,0.0f);
	//myfont.Print2D(200,2,"2,0",FONT2,1.0f,1.0f,0.0f);
	//myfont.Print2D(300,2,"3,0",FONT3,1.0f,0.0f,1.0f);
	//myfont.Print2D(400,2,"4,0",FONT4,0.0f,0.0f,1.0f);
	//myfont.Print2D(500,2,"5,0",FONT5,0.0f,1.0f,0.0f);
}

void CQuoridor::showHelp()
{
	//画背景图片
	glPushMatrix();
	glTranslatef(0.0,0.0,-0.5f);
	texture_select(g_cactus[0]);
	float det=m_OpenGL->RCheight / 6.0f;
	tPicRectangle((m_OpenGL->RCwidth-m_OpenGL->RCheight)/2.0f + det, 0 + det, (float)m_OpenGL->RCheight*0.6f, (float)m_OpenGL->RCheight*0.6f);
	glPopMatrix();

	tRectangle((float)board_x,0,-0.3f,(float)m_OpenGL->RCheight,(float)m_OpenGL->RCheight,1,1,1,0.5);

	char tmpstr[64]={"游戏说明"};
	myfont.Print2D(board_x+lace,500,tmpstr,FONT1,0.0,0.0,0.0);
	sprintf(tmpstr,"具体内容以后在写吧具体内容以后在写吧");
	myfont.Print2D(board_x+lace,400,tmpstr,FONT1,0.0,0.0,1.0);
	sprintf(tmpstr,"具体内容以后在写吧");
	myfont.Print2D(board_x+lace,300,tmpstr,FONT1,1.0,1.0,0.0);


	//文字
	sprintf(tmpstr,"按ESC返回主菜单");
	myfont.Print2D(rButtonx+10,menu.y+5,tmpstr,FONT1,1,1,1);

	//图片
	texture_select(g_cactus[9]);
	if(iButton==9)
	{
		tPicButton((float)rButtonx,(float)menu.y,(float)helpRetButtonW,(float)menu_h,0.0f);
	}
	else
	{
		tPicButton((float)rButtonx,(float)menu.y,(float)helpRetButtonW,(float)menu_h,0.5f);
	}
}

void CQuoridor::showChessBorad()
{
	float layer=-0.5;
	// 绘制玩家信息指示标志区域
	tRectangle(0,m_OpenGL->RCheight*3/4.0f,layer,(float)player_info_w,(float)player_info_h,1,1,0,0.7f);
	tRectangle(0,m_OpenGL->RCheight*2/4.0f,layer,(float)player_info_w,(float)player_info_h,1,0,0,0.7f);
	tRectangle(0,m_OpenGL->RCheight*1/4.0f,layer,(float)player_info_w,(float)player_info_h,0,1,0,0.7f);
	tRectangle(0,0,layer,(float)player_info_w,(float)player_info_h,0,0,1,1);

	// 绘制棋盘底
	texture_select(g_cactus[1]);
	tPicRectangle((float)board_x,0,(float)m_OpenGL->RCheight,(float)m_OpenGL->RCheight,layer-0.1f);
	// 绘制棋盘
	texture_select(g_cactus[2]);
	for (int i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
			tPicRectangle(board_x+lace+i*(roadw+wall_w),lace+j*(roadw+wall_w),roadw,roadw,layer);
		}
	}

	// 绘制彩色边
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glTranslatef(0,0,layer);
	// 左边黄色标记
	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f( (float)board_x, 0,  0.0f);
	glVertex3f( (float)board_x, (float)m_OpenGL->RCheight,  0.0f);
	glVertex3f( (float)board_x+lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glVertex3f( (float)board_x+lace, (float)lace,  0.0f);
	glEnd();
	// 上边红色标记
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f( (float)board_x, (float)m_OpenGL->RCheight, 0.0f);
	glVertex3f( (float)board_x+m_OpenGL->RCheight, (float)m_OpenGL->RCheight,  0.0f);
	glVertex3f( (float)board_x+m_OpenGL->RCheight-lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glVertex3f( (float)board_x+lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glEnd();
	// 右边绿色标记
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f((float) board_x+m_OpenGL->RCheight, (float)m_OpenGL->RCheight,  0.0f);
	glVertex3f((float) board_x+m_OpenGL->RCheight, 0,  0.0f);
	glVertex3f((float) board_x+m_OpenGL->RCheight-lace, (float)lace,  0.0f);
	glVertex3f((float) board_x+m_OpenGL->RCheight-lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glEnd();
	// 下边蓝色标记
	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f((float)board_x,  0,  0.0f);
	glVertex3f((float)board_x+lace, (float)lace,  0.0f);
	glVertex3f((float)board_x+m_OpenGL->RCheight-lace, (float)lace,  0.0f);
	glVertex3f((float)board_x+m_OpenGL->RCheight,  0,  0.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();

	//文字
	char tmpstr[64]={0};
	sprintf(tmpstr,"确    认");
	myfont.Print2D(menu.x+10,menu.y+5,tmpstr,FONT1,1,1,1);
	// 按钮图片
	texture_select(g_cactus[9]);
	if(1==iButton)
	{
		tPicButton((float)menu.x,(float)(menu.y),(float)menu_w,(float)menu_h,0.0f);
	}
	else
	{
		tPicButton((float)menu.x,(float)(menu.y),(float)menu_w,(float)menu_h,0.5f);
	}

	sprintf(tmpstr,"返回菜单");
	myfont.Print2D(menu.x+10,m_OpenGL->RCheight-menu.y-menu_h+5,tmpstr,FONT1,1,1,1);
	if(0==iButton)
	{
		tPicButton((float)menu.x,(float)(m_OpenGL->RCheight-menu.y-menu_h),(float)menu_w,(float)menu_h,0.0f);
	}
	else
	{
		tPicButton((float)menu.x,(float)(m_OpenGL->RCheight-menu.y-menu_h),(float)menu_w,(float)menu_h,0.5f);
	}
}

void CQuoridor::showPlayerWall()
{
	// 对应颜色的玩家，黄，红，绿，蓝
	texture_select(g_cactus[3]);
	if (yellow_ply.x>-1 && yellow_ply.y>-1)
	{
		tPicRectangle(board_x+lace+(roadw+wall_w)*yellow_ply.x,lace+(roadw+wall_w)*yellow_ply.y,roadw,roadw);
	}
	texture_select(g_cactus[4]);
	if (red_ply.x>-1 && red_ply.y>-1)
	{
		tPicRectangle(board_x+lace+(roadw+wall_w)*red_ply.x,lace+(roadw+wall_w)*red_ply.y,roadw,roadw);
	}
	texture_select(g_cactus[5]);
	if (green_ply.x>-1 && green_ply.y>-1)
	{
		tPicRectangle(board_x+lace+(roadw+wall_w)*green_ply.x,lace+(roadw+wall_w)*green_ply.y,roadw,roadw);
	}
	texture_select(g_cactus[6]);
	if (blue_ply.x>-1 && blue_ply.y>-1)
	{
		tPicRectangle(board_x+lace+(roadw+wall_w)*blue_ply.x,lace+(roadw+wall_w)*blue_ply.y,roadw,roadw);
	}

	// 绘制墙
	for (size_t i=0; i<wall_vec.size();i+=2)
	{
		texture_select(g_cactus[7]);
		// 如果是横墙
		if (wall_vec[i].x%2==0)
		{
			tPicRectangle(board_x+lace+wall_vec[i].x/2*(roadw+wall_w),lace+(int)(wall_vec[i].y/2)*(roadw+wall_w)+roadw,(float)wall_l,(float)wall_w);
		}
		else
		{
			tPicRectangle(board_x+lace+(int)(wall_vec[i].x/2)*(roadw+wall_w)+roadw,lace+wall_vec[i].y/2*(roadw+wall_w),(float)wall_w,(float)wall_l);
		}
	}
}

void CQuoridor::drawPickMask()
{
	if (-1 != pickup.x && -1 !=pickup.y)
	{
		// 这里以后换个贴图
		tRectangle(board_x+lace+pickup.x*(roadw+wall_w),lace+pickup.y*(roadw+wall_w),0,(float)roadw,(float)roadw,0.1f,0.5f,1,0.6f);
	}
	// 这里绘制临时选取的预选墙，单墙位置
	if (-1 != wall_pick.x && -1 != wall_pick.y)
	{
		// 选中的墙预选位置，*走到这里*，一定是一奇数一偶数
		if (wall_pick.x%2==0)
		{	// 横墙(TODO,注意这里，暂时墙的长度4/9这个数值是根据墙宽与路宽的比为1:4的前提)
			tRectangle(board_x+lace+wall_pick.x/2*(roadw+wall_w),lace+(int)(wall_pick.y/2)*(roadw+wall_w)+roadw,0,(float)wall_l*4/9,(float)wall_w,1.0f,1,0,0.6f);
		}
		else
		{	// 竖墙
			tRectangle(board_x+lace+(int)(wall_pick.x/2)*(roadw+wall_w)+roadw,lace+wall_pick.y/2*(roadw+wall_w),0,(float)wall_w,(float)wall_l*4/9,1.0f,1,0,0.6f);
		}
	}
}
