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

	// 游戏数据初始化
	arr_x=-1;
	arr_y=-1;
	blue_ply.x=4;
	blue_ply.y=0;
	red_ply.x=4;
	red_ply.y=8;

	// 棋盘数据初始化
	memset(gameData,0,sizeof(gameData));
	gameData[2*blue_ply.x][2*blue_ply.y]=GD_BLUE;
	gameData[2*red_ply.x][2*red_ply.y]=GD_RED;
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
	LoadT8("data/images/xiaohei1.bmp", g_cactus[3]);
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
		showPlayerWall();
		showChessBorad();
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
	arr_x=-1;
	arr_y=-1;

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
	case GAME_SENDBOX:
		if (x<board_x+lace || x>board_x+m_OpenGL->RCheight-lace)
			break;
		it=(int)((x-board_x-lace)/(roadw+wall_w));
		remain = (x-board_x-lace)-it*(roadw+wall_w);
		if (remain>roadw)
		{
			arr_x = it*2+1;
		} else {
			arr_x = it*2;
		}
		it = (int)((y-lace)/(roadw+wall_w));
		remain = (y-lace) - it*(roadw+wall_w);
		if (remain>roadw)
		{
			arr_y = it*2+1;
		} else {
			arr_y = it*2;
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
//左键松开
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
			//iGameState=GAME_IN_INIT;
			//b_mouse_test=!b_mouse_test;
			break;
		case MENU_MULTIP:
			//iGameState=GAME_IN_INIT;
			//b_func_test=!b_func_test;
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

//画立方体
//入参 	位置 长宽高 贴图坐标 贴图编号 iHastop 是否有顶面和底面
void CQuoridor::showmapBox(float *ppos, float *psize, float *ptex,int itex,int iHastop)
{
	float width =psize[0];	
	float height=psize[2];	
	float length=psize[1];	
	float x = ppos[0];	
	float y = ppos[1];
	float z = ppos[2];
	float fstex=ptex[0];//沿x轴的贴图
	float fttex=ptex[1];
	float fhtex=ptex[2];
	///////////////////////////////////////////////////////////////////////////////
	
	//wall 垂直于Z轴
	texture_select(TexBox[itex]);	
	glBegin(GL_QUADS);		
		glTexCoord2f(fstex,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(fstex,fhtex); glVertex3f(x+width,y+height,z); 
		glTexCoord2f(0.0f,fhtex); glVertex3f(x,		y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z);
	glEnd();

	texture_select(TexBox[itex]);	
	glBegin(GL_QUADS);		
		glTexCoord2f(fstex,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(fstex,fhtex); glVertex3f(x,		y+height,z+length);
		glTexCoord2f(0.0f,fhtex); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z+length);
	glEnd();

	if(iHastop)
	{
		//sky
		texture_select(g_cactus[4]);	
		glBegin(GL_QUADS);	
			glTexCoord2f(0.0f,fttex); glVertex3f(x+width,y+height,z);
			glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y+height,z+length); 
			glTexCoord2f(fstex,0.0f); glVertex3f(x,		y+height,z+length);
			glTexCoord2f(fstex,fttex); glVertex3f(x,		y+height,z);
		glEnd();
	}
	//wall 垂直于x轴
	texture_select(TexBox[itex+1]);	
	glBegin(GL_QUADS);		
		glTexCoord2f(fttex,fhtex); glVertex3f(x,		y+height,z);	
		glTexCoord2f(0.0f,fhtex); glVertex3f(x,		y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(fttex,0.0f); glVertex3f(x,		y,		 z);		
	glEnd();
	texture_select(TexBox[itex+1]);	
	glBegin(GL_QUADS);			
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(fttex,0.0f); glVertex3f(x+width,y,		 z+length);
		glTexCoord2f(fttex,fhtex); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,fhtex); glVertex3f(x+width,y+height,z);
	glEnd();

	//ground
	if(iHastop)
	{
		texture_select(g_cactus[7]);	
		glBegin(GL_QUADS);	
			glTexCoord2f(0.0f,fttex); glVertex3f(x+width,y,z);
			glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,z+length); 
			glTexCoord2f(fstex,0.0f); glVertex3f(x,		y,z+length);
			glTexCoord2f(fstex,fttex); glVertex3f(x,		y,z);
		glEnd();
	}
}

void CQuoridor::show_2D_test()
{
/*
	if (b_font_test)
	{
		show_Font_test();
	}

	if (b_mouse_test)
	{
		show_Mouse_test();
	}
*/
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_TEXTURE_2D);

	glTranslatef(m_OpenGL->RCwidth/2.0f/*-5.0f*/,m_OpenGL->RCheight/2.0f/*-20.0f*/,-0.1f);

	char tmpstr[32]={0};
	// 一个刻度线代表的值
	int scale=10;  // 100 pixel = scale

	// draw aix---------------
	// 坐标原点
	//myfont.Print2D(0,0,"O",FONT1,1.0f,1.0f,0.0f);
	// 设置点的尺寸
	glPointSize(3);
	for (int x=-4; x<5; x++)
	{
		glBegin(GL_POINTS);
		glVertex3f( x*100.0f, 0.0, 0.0);
		glEnd();
		//int ta=x*scale;
		sprintf_s(tmpstr,"%d\n",x*scale);
		myfont.Print2D(x*100,0,tmpstr,FONT0,1.0f,1.0f,1.0f);
	}
	for (int y=-3; y<4; y++)
	{
		glBegin(GL_POINTS);
		glVertex3f( 0.0, y*100.0f, 0.0);
		glEnd();
		sprintf_s(tmpstr,"%d\n",y*scale);
		myfont.Print2D(0,y*100,tmpstr,FONT0,1.0f,1.0f,1.0f);
	}
	myfont.Print2D(0,0,"O",FONT0,1.0f,1.0f,0.0f);
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);
	// X aix
	glVertex3f(-WIN_WIDTH/2, 0.0, 0.0);
	glVertex3f( WIN_WIDTH/2, 0.0, 0.0);
	glVertex3f( WIN_WIDTH/2, 0.0, 0.0);
	glVertex3f( WIN_WIDTH/2-15,  5.0, 0.0);
	glVertex3f( WIN_WIDTH/2, 0.0, 0.0);
	glVertex3f( WIN_WIDTH/2-15, -5.0, 0.0);
	// Y aix
	glVertex3f( 0.0, -WIN_HEIGHT/2, 0.0);
	glVertex3f( 0.0,  WIN_HEIGHT/2, 0.0);
	glVertex3f( 0.0,  WIN_HEIGHT/2, 0.0);
	glVertex3f( 5.0,  WIN_HEIGHT/2-15, 0.0);
	glVertex3f( 0.0,  WIN_HEIGHT/2, 0.0);
	glVertex3f(-5.0,  WIN_HEIGHT/2-15, 0.0);
	glEnd();

	// --------------------

	//if (b_func_test)
	{
		// draw a func
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_LINE_STRIP);
		for (int x=-40; x<40; x++)
		{
			// y=kx+b
			float y=0.6f*x+5;
			glVertex3f( (float)x*scale,(float)y*scale, 0.0);
		}
		glEnd();

		// draw a func
		glColor3f(0.0,1.0,0.0);
		glBegin(GL_LINE_STRIP);
		for (int x=-40; x<40; x++)
		{
			// y=ax^2+bx+c
			float y=0.07f*x*x+0.6f*x-15;
			glVertex2f( (float)x*scale,(float)y*scale);
		}
		glEnd();

		// draw a func
		glColor3f(1.0,1.0,0.0);
		glBegin(GL_LINE_STRIP);
		for (int x=-40; x<40; x++)
		{
			// y=asinx
			float y=20.0f*sin(0.1f*x);
			glVertex2f( (float)x*scale,(float)y*scale);
		}
		glEnd();

		// draw a func
		glColor3f(0.0,0.0,1.0);
		glBegin(GL_LINE_STRIP);
		for (int x=1; x<400; x++)
		{
			// y=1/x
			float y=100.0f/x;
			glVertex2f( (float)x*scale,(float)y*scale);
		}
		glEnd();

		// draw a func
		glColor3f(0.0,1.0,1.0);
		glBegin(GL_LINE_STRIP);
		for (int x=-40; x<40; x++)
		{
			// y=10*2^x
			float y=10.0f*pow(2.0f,x/10.0f);
			glVertex2f( (float)x*scale,(float)y*scale);
		}
		glEnd();

		// draw a func
		glColor3f(1.0f,0.0f,1.0f);
		glBegin(GL_LINE_STRIP);
		for (int x=1; x<400; x++)
		{
			// y=log(x)
			float y=10*log(x/10.0f);
			glVertex2f( (float)x*scale,(float)y*scale);
		}
		glEnd();
	}

	glEnable(GL_TEXTURE_2D);
	glPopAttrib();
	glPopMatrix();
}

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

	sprintf(tmpstr, "arr_x=%d",arr_x);
	myfont.Print2D(50,20,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);
	sprintf(tmpstr, "arr_y=%d",arr_y);
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
	texture_select(g_cactus[3]);
	if (blue_ply.x!=-1 && blue_ply.y!=-1)
	{
		tPicRectangle(board_x+lace+(roadw+wall_w)*blue_ply.x,lace+(roadw+wall_w)*blue_ply.y,roadw,roadw);
	}
	if (red_ply.x!=-1 && red_ply.y!=-1)
	{
		tPicRectangle(board_x+lace+(roadw+wall_w)*red_ply.x,lace+(roadw+wall_w)*red_ply.y,roadw,roadw);
	}

	//for (int i=0; i<sz; i++)
	//{
	//	for (int j=0; j<sz; j++)
	//	{
	//		switch (gameData[j][i])
	//		{
	//		case GD_BLANK:
	//			break;
	//		case GD_YELLOW:
	//			break;
	//		case GD_RED:
	//			break;
	//		case GD_GREEN:
	//			break;
	//		case GD_BLUE:
	//			break;
	//		case GD_WALL:
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}
}
