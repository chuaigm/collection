//
// 制作  programking 2009年3月
// 博客  http://blog.csdn.net/programking
//=======================================

#include "stdafx.h"
#include "gamemap.h"
#include "myclock.h"
#include "glfont.h"
#include "opengl.h"
#include "mykey.h"

//font
extern CGLFont myfont;

//clock
extern MYCLOCK c1;

//mouse
extern int Lbutdown;

extern OpenGL* m_OpenGL;

extern int WinWidth;
extern int WinHeight;

/////////////////////////

GAMEMAP::GAMEMAP()
{
	iGameState=GAME_NULL;	

	xmouse=0;
	ymouse=0;
	xmouseOld=0;
	ymouseOld=0;

	iMenu=-1;

	//camera
	g_eye[0]=50;//
	g_eye[2]=5;//
	g_Angle=0;
	g_elev=0;

	//fire
	IsFire=0;

	rocketX=INVALID_ROCKET;
	rocketY=0.0f;
	rocketZ=0.0f;
	
	iMatch=0;
	iMapRectNum=0;
	iShowHelp=0;
}

GAMEMAP::~GAMEMAP()
{
	delete m_anmobj;
}
//游戏所需数据文件检查,如果缺少,程序中止
int GAMEMAP::haveDataFile()
{
	int ihave=1;
	FILE *fp;
	char *fdata[]={
		//map
		"data/map/map.txt",
		//mdl
		"data/mdl/qian1.mdl",
		//bmp
		"data/images/start1.bmp",
		"data/images/start2.bmp",
		"data/images/box0.bmp",
		"data/images/box01.bmp",
		"data/images/box1.bmp",
		"data/images/box11.bmp",
		"data/images/box2.bmp",
		"data/images/box21.bmp",
		"data/images/map2.bmp",
		"data/images/map5.bmp",
		"data/images/TRIX1.bmp",
		"data/images/button.bmp",
		"data/images/h1.bmp",
		//md2
		"data/md2/role/tris.md2",
		"data/md2/role/weapon.md2",
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
void GAMEMAP::init()
{
	//mdl模型要最先初始化
	m_mdlobj.InitGL(0,"data/mdl/qian1.mdl");	
	
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
	//button
	LoadT8("data/images/button.bmp", g_cactus[9]);

	LoadT8("data/images/h1.bmp", g_cactus[10]);

	//md2
	m_anmobj=new anmobj;
	m_anmobj->model[0]=NULL;
	m_anmobj->model[1]=NULL;
	m_anmobj->getobj("data/md2/role/");	
	
	///////////////////////////////////////////
	//启动贴图
	glEnable(GL_TEXTURE_2D);

	//启动深度检测
	glEnable(GL_DEPTH_TEST);

	//游戏状态
//	iGameState=GAME_PRE_ANI;
	iGameState=1;
	iMatch=0;
	//参数
	param1=0;
}
//游戏界面
void GAMEMAP::showInfo()
{
	char info[50]={0};

	sprintf(info, "enemy number: %d  ",iEnemyNum);
	myfont.settext(50,50,info,FONT1,1,1,1);

	sprintf(info, "HP: %d  ",iHp);
	if(iHp>30)
	{
		//白色显示
		myfont.settext(50,550,info,FONT1,1,1,1);
	}
	else
	{
		myfont.settext(50,550,info,FONT1,1,0,0);	//红色
	}

	//攻击提示 敌人生命
	if(iAttackLife)
	{
		glLoadIdentity();
		//画头像图标
		//glDisable(GL_LIGHTING);				
		texture0(g_cactus[10]);	
		glTranslatef(-0.14,0.15,-1.0);
		tPic(0.04f);
		//glEnable(GL_LIGHTING);
		//画生命值		
		showEnemyHp();
	}


	//获胜
	if(GAME_WIN==iGameState)
	{
		sprintf(info, "胜 利");
		myfont.settext(350,250,info,FONT3,1,1,0);		
	}
	if(GAME_FAIL==iGameState)
	{
		sprintf(info, "失 败");
		myfont.settext(350,250,info,FONT3,1,1,1);		
	}
	if(GAME_PASS==iGameState)
	{
		sprintf(info, "通 关");
		myfont.settext(350,250,info,FONT4,1,1,0);		
	}
	
	if(iShowWorldFrame)
	{
		sprintf(info, "WORLD: %d",iMatch+1);
		myfont.settext(350,250,info,FONT3,1,1,0);		

		iShowWorldFrame++;
		//地图信息显示时间 TIME_WORLD_INFO
		if(iShowWorldFrame > TIME_WORLD_INFO)
		{
			iShowWorldFrame=0;
		}
	}
}
//敌人生命值
void GAMEMAP::showEnemyHp()
{
	int i;

	unsigned char rasterallf[12] = {
		0xff, 0xff,0xff, 0xff,0xff, 0xff,
		0xff, 0xff,0xff, 0xff,0xff, 0xff
		};

	glLoadIdentity();
	//属性进栈
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);    
	//glDisable(GL_LIGHTING);      	
	//颜色条
	//坐标移动
	glTranslatef(-0.1f, 0.15, -1.0);

	glColor3f (0.0, 0.0, 0.0);
	//光栅位置向后移动-0.01，比原位置低了2个像素
	glRasterPos3f (0.0, 0.0,-0.01);
	for(i=0;i<iAttackMaxLife/10;i++)
	{
		glBitmap (8, 12, 0.0, 0.0, 8.0, 0.0, rasterallf);
	}
	//绿色显示生命
	glColor3f (0.0, 1.0, 0.0);
	glRasterPos3f (0.0, 0.0,0.0);
	for(i=0;i<iAttackLife/10;i++)
	{
		glBitmap (8, 8, 0.0, 0.0, 8.0, 0.0, rasterallf);
	}

	/////////////////////////
	//glEnable(GL_LIGHTING);         
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();

}

//准心
void GAMEMAP::showTarget()
{
	unsigned char rasterallf[12] = {
		0xff, 0xff,0xff, 0xff,0xff, 0xff,
		0xff, 0xff,0xff, 0xff,0xff, 0xff
		};

	glLoadIdentity();
	//属性进栈
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);    
	//glDisable(GL_LIGHTING);      	
	//指定颜色
	glColor3f(0.0,1.0,0.0f);             
	//坐标转换，移动
	glTranslatef(0.0,0.0,-1.0f);

	//在RC中输出
	glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
	//设置光栅位置
	glRasterPos2i(0,0);                  
	glBitmap (2, 12, 0.0, 0.0, 0.0, 23.0, rasterallf);
	glBitmap (2, 12, 0.0, 0.0, -16.0, -6.0, rasterallf);
	glBitmap (12, 2, 0.0, 0.0, 22.0, 0.0, rasterallf);
	glBitmap (12, 2, 0.0, 0.0, 0.0, 0.0, rasterallf);

	/////////////////////////
	//glEnable(GL_LIGHTING);         
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();

}
//绘制鼠标
void GAMEMAP::drawMouse()
{
	glPushMatrix();
	//glLoadIdentity();
	//属性进栈
	//glPushAttrib(GL_CURRENT_BIT);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_TEXTURE_2D);    
	//glDisable(GL_LIGHTING);      	
	
	glTranslatef(xmouse,WinHeight-ymouse,0.1);

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0,0.5,0.0f); 
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(30.0, -15.0,0.0);

	glColor3f(0.0,0.8,0.0f); 
	glVertex3f(12.0, -12.0,0.0);
	glVertex3f(6.0,-20.0,0.0);

	glColor3f(0.0,0.0,0.0f); 
	glVertex3f(3.0,-20.0,0.0);
	glEnd();

	/////////////////////////
	//glEnable(GL_LIGHTING);         
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();
	glPopMatrix();
}

//画两个黑色方块
void GAMEMAP::showFire()
{
	unsigned char rasterallf[12] = {
		0xff, 0xff,0xff, 0xff,0xff, 0xff,
		0xff, 0xff,0xff, 0xff,0xff, 0xff
		};
	unsigned char rasterblank[12] = {0};
	int ibombframe;
	int ibombx;
	int ibomby;

	if(!iShowBombFrame)
		return;

	glLoadIdentity();
	//属性进栈
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);    
	//glDisable(GL_LIGHTING);      	
	//指定颜色
	glColor3f(0.0,0.0,0.0f);             
	//坐标转换，移动
	glTranslatef(0.0,0.0,-1.0f);

	glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
	//用各种公式输出掉落的方块
	ibombframe=iShowBombFrame-1;

	ibombx=ibombframe*2;
	ibomby=2*(ibombframe-3)*(ibombframe-3)-30;
	glRasterPos2i(0,0);                  
	glBitmap (1,1, 0.0, 0.0, ibombx, -ibomby, rasterblank);	
	glBitmap (3, 4, 0.0, 0.0, 0.0, 0.0, rasterallf);	
	
	ibomby=2*(ibombframe-3)*(ibombframe-3)-18;
	glRasterPos2i(0,0);                  	
	glBitmap (1,1, 0.0, 0.0, -ibombx, -ibomby, rasterblank);	
	glBitmap (4, 3, 0.0, 0.0, 0.0, 0.0, rasterallf);	
	
	ibombx=ibombframe;
	ibomby=(ibombframe-3)*(ibombframe-3);
	glRasterPos2i(0,0);                  	
	glBitmap (1,1, 0.0, 0.0, -ibombx, -ibomby, rasterblank);	
	glBitmap (4, 3, 0.0, 0.0, 0.0, 0.0, rasterallf);	

	/////////////////////////
	//glEnable(GL_LIGHTING);         
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();

	//刷新帧
	iShowBombFrame++;
	if(iShowBombFrame>TIME_BOMB)
	{
		iShowBombFrame=0;
	}	
}
//屏幕刷新
void GAMEMAP::show()
{
	int i;

	float xx0,zz0;	//gun
	
	switch(iGameState)
	{
	case GAME_PRE_ANI:
		showpreani();
		break;

	case GAME_MENU:
		glLoadIdentity();
		showmenu();		
		glLoadIdentity();
		drawMouse();
		break;

	case GAME_IN:
	case GAME_WIN:
	case GAME_FAIL:
	case GAME_PASS:
		//初始化单位矩阵
		glLoadIdentity();
		showTarget();
		showInfo();
		showFire();

		glLoadIdentity();
		DisplayScene();

		showmap();
		//ENEMY
		texture0(g_cactus[8]);
		for(i=0;i<MAX_MAP_OBJECT;i++)
		{
			if(MapEnemyArray[i].show)
			{				
			m_anmobj->drawModel(MapEnemyArray[i].id,
					MapEnemyArray[i].x,
					MapEnemyArray[i].y+ENEMY_MODEL_Y,
					MapEnemyArray[i].z,
					MapEnemyArray[i].iframe,
					MapEnemyArray[i].iAngle);
			}
		}
		//gun
		//当前人物位置
		xx0=g_eye [0];
		zz0=g_eye [2];
		//如果生命值不为0，根据人物的位置，角度显示手枪
		if(iHp)
		{
			m_mdlobj.DrawModels(xx0,1.8,zz0,
				0,
				0.1f,
				-g_Angle,g_elev);//水平角度取反
		}

		break;

	case GAME_ERR:		
		break;

	default:
		break;
	}
}
//构造位图贴图
bool GAMEMAP::LoadT8(char *filename, GLuint &texture)
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
void GAMEMAP::texture(UINT textur)
{
	glBindTexture  (GL_TEXTURE_2D, textur);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

//指定贴图
void GAMEMAP::texture0(UINT textur)
{
	glBindTexture  (GL_TEXTURE_2D, textur);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

//画正方形 ，边长2e
void GAMEMAP::tPic(float e)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-e, -e,  0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( e, -e,  0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( e,  e,  0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-e,  e,  0.0f);
	glEnd();
}

//画矩形，左下角坐标xy，宽度w，高h
void GAMEMAP::tPic(float x,float y,float w,float h)
{ 
	glPushMatrix();

	//画背景矩形
	glTranslatef(x,y,-0.1f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0f);
		
		glTexCoord2f(1.0f, 0.0f);		
		glVertex3f(w, 0.0,  0.0f);		

		//up
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(w, h,  0.0f);
				
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, h,  0.0f);
	glEnd();	

	glPopMatrix();
}
//画按钮
void GAMEMAP::tPicButton(float x,float y,float w,float h,float ytex)
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
void GAMEMAP::light0()
{
	GLfloat light_position[] = {10.0,10.0,3.0,1.0};

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	
	//启动深度检测
	glEnable(GL_DEPTH_TEST);
	//启动颜色材质
	glEnable(GL_COLOR_MATERIAL);
}
//开屏动画
void GAMEMAP::showpreani()
{
	glPushMatrix();
	//移动到当前位置
	glTranslatef(0,0,-8.0);
	texture0(g_cactus[0]);	
	tPic(2.0f);
	//移动动画
	tSquare(param1*0.2, 0.0, 0.1, 3.0);
	glPopMatrix();
}

//指定位置画一个正方形
void GAMEMAP::tSquare(float x, float y, float z, float a)
{
	//属性进栈
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);

	glTranslatef(x,y,z);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
		glVertex3f(-a, -a,  0.0f);
		glVertex3f( a, -a,  0.0f);
		glVertex3f( a,  a,  0.0f);
		glVertex3f(-a,  a,  0.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();
}

//实时检测
void GAMEMAP::check()
{
	int i;
	int x,y;

	switch(iGameState)
	{
	case GAME_PRE_ANI:
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
		//检测菜单选择
		x=xmouse;
		y=WinHeight-ymouse;
		iMenu=-1;//初始化 没有选择
		
		if(x<XMENU	|| x>XMENU+WIDTH_MENU || y<YMENU)
			break;

		for(i=0;i<MENU_NUM;i++)
		{
			if( y>YMENU+i*Y_2MENU && y<YMENU+i*Y_2MENU+HEIGHT_MENU )
			{
				iMenu=i;
				break;
			}
		}
		break;
	
	case GAME_IN_INIT:
		//进入地图前初始化
		initMap();
		iGameState=GAME_IN;
		c1.ReStart(300);
		//设置游戏投影
		initView();
		break;

	case GAME_IN:
		//计时
		c1.clipcount();
		//角色检测
		checkRole();
		checkFire();		
		//胜负判断
		if(iEnemyNum<=0)
		{
			iMatch++;
			if(iMatch>=MAX_MATCH)
			{
				iGameState=GAME_PASS;
				c1.ReStart(3);
			}
			else
			{
				iGameState=GAME_WIN;
				c1.ReStart(4);
			}
		}
		if(iHp<=0)
		{
			iHp=0;
			iGameState=GAME_FAIL;
			c1.ReStart(3);
		}
		
		break;	

	case GAME_WIN:
		checkRole();
		//计时
		if(c1.clipcount())
		{
			iGameState=GAME_IN_INIT;
		}
		break;

	case GAME_FAIL:
		checkRole();
		//计时
		if(c1.clipcount())
		{
			iGameState=GAME_MENU;
			//设置投影 
			initView();
		}
		break;

	case GAME_PASS:
		checkRole();
		//计时
		if(c1.clipcount())
		{
			iMatch=0;//第0关
			iGameState=GAME_MENU;
			//设置投影 
			initView();
		}
		break;

	default:
		break;
	}
}
//显示菜单
void GAMEMAP::showmenu()
{
	int i;
	char *menustr[]={"退    出","操作说明","开始游戏"};
	char *helpstr[]={
		"http://blog.csdn.net/programking",
		"制作: programking 2009年3月",
		"  F1 :窗口最小化",
		"  Esc:回主菜单",
		"  开枪:鼠标左键",
		"  视角:鼠标移动",
		"操作: 移动: W A S D "		
	};

	glLoadIdentity();

	//在glOrtho模式下绘制菜单
	for(i=0;i<MENU_NUM;i++)
	{
		//文字
		myfont.settextTest(XMENU+30,YMENU+15+i*Y_2MENU,menustr[i],2,1,1,1);
		//取消光照
		//glDisable(GL_LIGHTING);
		//图片
		texture0(g_cactus[9]);
		if(iMenu==i)
		{
			tPicButton(XMENU,YMENU+i*Y_2MENU,WIDTH_MENU,HEIGHT_MENU,0.0f);
		}
		else
		{
			tPicButton(XMENU,YMENU+i*Y_2MENU,WIDTH_MENU,HEIGHT_MENU,0.5f);
		}
	}
	//画背景图片
	//glLoadIdentity();
	//glTranslatef(0.0,0.0,-0.1f);
	texture0(g_cactus[1]);
	tPic(0,0,WinWidth,WinHeight);	

	//画操作说明菜单
	if(iShowHelp)
	{
		for(i=0;i<sizeof(helpstr)/sizeof(helpstr[0]);i++)
		{
			//文字
			myfont.settextTest(370,150+i*40,helpstr[i],1,1,1,1);
		}
	}
	
	//glEnable(GL_LIGHTING);
}

//键盘处理
void GAMEMAP::keyupproc(int keyparam)
{
    switch (keyparam)
    {
	case KEY_F12:
		//直接过关
		if(iGameState=GAME_IN)
		{
			iEnemyNum=0;
		}		
		break;

	case VK_ESCAPE:
		//回到菜单
		iGameState=GAME_MENU;
		initView();
		break;

	default:
		break;
	}
}

//鼠标检测函数
void GAMEMAP::mouseproc(int lparam)
{
	xmouseOld=xmouse;
	ymouseOld=ymouse;
	xmouse=LOWORD(lparam);
	ymouse=HIWORD(lparam);
}
//左键松开
void GAMEMAP::lbuttonproc(int lparam)
{
	switch(iGameState)
	{
	case GAME_MENU:
		if(iMenu<0)
			break;

		switch(iMenu)
		{
		case MENU_START:
			iGameState=GAME_IN_INIT;
			break;

		case MENU_HELP:
			iShowHelp=1;
			break;

		case MENU_QUIT:
			m_OpenGL->CleanUp();
			PostQuitMessage(0);				
			break;
		}		
		break;	

	default:
		break;
	}
}

//画立方体
//入参 	位置 长宽高 贴图坐标 贴图编号 iHastop 是否有顶面和底面
void GAMEMAP::showmapBox(float *ppos, float *psize, float *ptex,int itex,int iHastop)
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
	texture0(TexBox[itex]);	
	glBegin(GL_QUADS);		
		glTexCoord2f(fstex,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(fstex,fhtex); glVertex3f(x+width,y+height,z); 
		glTexCoord2f(0.0f,fhtex); glVertex3f(x,		y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z);
	glEnd();

	texture0(TexBox[itex]);	
	glBegin(GL_QUADS);		
		glTexCoord2f(fstex,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(fstex,fhtex); glVertex3f(x,		y+height,z+length);
		glTexCoord2f(0.0f,fhtex); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z+length);
	glEnd();

	if(iHastop)
	{
		//sky
		texture0(g_cactus[4]);	
		glBegin(GL_QUADS);	
			glTexCoord2f(0.0f,fttex); glVertex3f(x+width,y+height,z);
			glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y+height,z+length); 
			glTexCoord2f(fstex,0.0f); glVertex3f(x,		y+height,z+length);
			glTexCoord2f(fstex,fttex); glVertex3f(x,		y+height,z);
		glEnd();
	}
	//wall 垂直于x轴
	texture0(TexBox[itex+1]);	
	glBegin(GL_QUADS);		
		glTexCoord2f(fttex,fhtex); glVertex3f(x,		y+height,z);	
		glTexCoord2f(0.0f,fhtex); glVertex3f(x,		y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(fttex,0.0f); glVertex3f(x,		y,		 z);		
	glEnd();
	texture0(TexBox[itex+1]);	
	glBegin(GL_QUADS);			
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(fttex,0.0f); glVertex3f(x+width,y,		 z+length);
		glTexCoord2f(fttex,fhtex); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,fhtex); glVertex3f(x+width,y+height,z);
	glEnd();

	//ground
	if(iHastop)
	{
		texture0(g_cactus[7]);	
		glBegin(GL_QUADS);	
			glTexCoord2f(0.0f,fttex); glVertex3f(x+width,y,z);
			glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,z+length); 
			glTexCoord2f(fstex,0.0f); glVertex3f(x,		y,z+length);
			glTexCoord2f(fstex,fttex); glVertex3f(x,		y,z);
		glEnd();
	}
}
//画地图场景
void GAMEMAP::showmap()
{
	//地图数据
	float pmappos[]={0,0,0};
	float pmapsize[]={100,50,5};
	float pmaptex[]={50,25,5};
	int i;
	int iTex;

	//MAP
	//设置起点
	pmappos[0]=maprect[0].bottom;
	pmappos[2]=maprect[0].left;
	//设置高 宽
	pmapsize[0]=maprect[0].top;
	pmapsize[1]=maprect[0].right;
	//设置贴图坐标
	pmaptex[0]=pmapsize[0]/2;
	pmaptex[1]=pmapsize[1]/2;
	//设置贴图编号
	iTex=maprectTex[0]*2;
	//画立方体 包含顶 底面
	showmapBox(pmappos,pmapsize,pmaptex,iTex,1);

	for(i=1;i<iMapRectNum;i++)
	{			
		pmappos[0]=maprect[i].bottom;
		pmappos[2]=maprect[i].left;
		pmapsize[0]=maprect[i].top-maprect[i].bottom;
		pmapsize[1]=maprect[i].right-maprect[i].left;
		pmaptex[0]=pmapsize[0]/2;
		pmaptex[1]=pmapsize[1]/2;
		iTex=maprectTex[i]*2;//每个立方体对应两张贴图
		showmapBox(pmappos,pmapsize,pmaptex,iTex,0);
	}	
}
//设置摄像机
void GAMEMAP::DisplayScene()
{
	float speed=0.5f;					
	float x=g_eye[0],y=g_eye[1],z=g_eye[2];
	
	if (KEY_DOWN(VK_SHIFT))  speed   =speed*2;

	//mouse
/*  */
	g_Angle +=  (xmouse-xmouseOld)*.2f;            
	g_elev  += -(ymouse-ymouseOld)*.2f;          

	if(xmouse<50 || xmouse>750 || ymouse<50 || ymouse>550)
	{
		SetCursorPos(400,300);
	}
	xmouseOld=xmouse;
	ymouseOld=ymouse;

	//
	if (KEY_DOWN(VK_LEFT))   g_Angle-=speed*2;
	if (KEY_DOWN(VK_RIGHT))  g_Angle+=speed*2;
	
	//转换成弧度
	rad_xz = float (3.13149* g_Angle/180.0f);	
	//仰角
	if (KEY_DOWN(33))		   g_elev +=speed;
	if (KEY_DOWN(34))		   g_elev -=speed;
	if (g_elev<-360)		   g_elev  =-360;
	if (g_elev> 360)		   g_elev  = 360;
	
	//w s a d 移动 支持上下键移动
	if (KEY_DOWN(KEY_W) || KEY_DOWN(VK_UP) )					
	{
		z+=(float)sin(rad_xz)*speed;	
		x+=(float)cos(rad_xz)*speed;	
	}
	if (KEY_DOWN(KEY_S) || KEY_DOWN(VK_DOWN) )					
	{
		z-=(float)sin(rad_xz)*speed;	
		x-=(float)cos(rad_xz)*speed;
	}
	if (KEY_DOWN(KEY_A))					
	{
		z-=(float)cos(rad_xz)*speed;	
		x+=(float)sin(rad_xz)*speed;	
	}
	if (KEY_DOWN(KEY_D))		
	{
		z+=(float)cos(rad_xz)*speed;	
		x-=(float)sin(rad_xz)*speed;	
	}


	//移动范围判断
	if(RoleInMap(x,y,z))
	{
		g_eye[0]=x;
		g_eye[2]=z;
	}

	g_eye[1] =VIEW_HEIGHT;

	g_look[0] = (float)(g_eye[0] +100*cos(rad_xz));
	g_look[2] = (float)(g_eye[2] +100*sin(rad_xz));
	g_look[1] = g_eye[1] +g_elev;	
	
	gluLookAt(g_eye[0],g_eye[1],g_eye[2],g_look[0],g_look[1],g_look[2],0.0,1.0,0.0);
	//攻击
	//按下左键
	if(Lbutdown && iHp)
	{
		//初始化坐标
		rocketX=g_eye[0];
		rocketZ=g_eye[2];
		rocketY=ENEMY_MODEL_Y;
		//设定枪手模型动作序列
		m_mdlobj.fire(1);//
		IsFire=1;//非零，表示有子弹
		Lbutdown=0;
		//sound
		sndPlaySound("data/sound/explode1.wav",SND_ASYNC);
		//bomb 如果没有撞击效果，启动撞击动画
		if(!iShowBombFrame)
		{
			iShowBombFrame=1;//开枪时，启动撞击效果
		}
	}

	return;
}

//碰撞检测
int GAMEMAP::hittest()
{
	int i;
	int iHit=0;
	float x,y,z;

	//原理：子弹每次刷新坐标,增加距离7.0
	//		检测时，从当前坐标循环6次,每次增加一个单位做检测
	if(IsFire)
	{
		x=rocketX;
		y=rocketY;
		z=rocketZ;
		iHit=0;
		for(int k=0;k<6;k++)					
		{	
			for(i=0;i<MAX_MAP_OBJECT;i++)
			{
				if(!MapEnemyArray[i].show)
					continue;

				if(MapEnemyArray[i].hp<=0)
					continue;		 
 		
				if(		abs(MapEnemyArray[i].x-x)<0.1f	
					&&	abs(MapEnemyArray[i].y+ENEMY_MODEL_Y-y)<0.3f
					&&	abs(MapEnemyArray[i].z-z)<0.1f)	
				{				
					MapEnemyArray[i].hp-=20;
					if(MapEnemyArray[i].hp<=0)
					{
						//设定动作
						MapEnemyArray[i].iframe=178;	//death frame
						MapEnemyArray[i].iCurrentAni=ENEMY_DEATH_ANI;
						iEnemyNum--;
					}
					//攻击对象提示
					iAttackLife=MapEnemyArray[i].hp;
					iAttackMaxLife=MapEnemyArray[i].hpMax;
					param1=0;
					iHit=1;		//已经检测到碰撞
					break;
				}					
			}
			if(iHit)
			{	
				//清除
				Lbutdown=0;				
				IsFire=0;	//	
				rocketX=INVALID_ROCKET;		//清除子弹数据
				break;//停止检测
			}
		  
			x+=1.0f*cos(rad_xz);				
	  		z+=1.0f*sin(rad_xz);				
	  		y+=1.0f*tan(g_elev*PI/360);		
		}
	}//end 子弹和enemy的检测

	//子弹和障碍物的检测
	if(IsFire)
	{
		x=rocketX;
		y=rocketY;
		z=rocketZ;
		iHit=0;
		for(int k=0;k<6;k++)					
		{	
			if(!POINT_IN_RECT(x,z,maprect[0]))
			{			
				iHit=1;
			}	
			for(i=1;i<iMapRectNum;i++)
			{
				//在任何障碍矩形内,非法
				if(POINT_IN_RECT(x,z,maprect[i]))
				{
					iHit=1;
				}
			}
			if(iHit)
			{
				//清除
				Lbutdown=0;				
				IsFire=0;	//	
				rocketX=INVALID_ROCKET;		//清除子弹数据
				break;//碰到障碍
			}
			x+=1.0f*cos(rad_xz);				
	  		z+=1.0f*sin(rad_xz);				
	  		y+=1.0f*tan(g_elev*PI/360);		
		}
	}//end 子弹和障碍物的检测

	//enemy和玩家的碰撞
	for(i=0;i<MAX_MAP_OBJECT;i++)
	{
		if(!MapEnemyArray[i].show)
				continue;

		//不是跑动状态, 不检测
		if(ENEMY_RUN_ANI!=MapEnemyArray[i].iCurrentAni)
				continue;

		if(	abs(MapEnemyArray[i].x-g_eye[0])<1.0f	
			&&	abs(MapEnemyArray[i].z-g_eye[2])<1.0f)	
		{				
			iHp-=10;	//和enemy碰撞,生命值减少
			break;
		}	
	}
	return 0;
}

void GAMEMAP::checkRole()
{
	int i,j;
	int iTurn;//是否反向运动
	struct animation	*pAni;

	//刷新敌人的动画帧
	if(c1.iNumClip%3==0)
	{
		//刷新帧 敌人
		for(i=0;i<MAX_MAP_OBJECT;i++)
		{
			if(!MapEnemyArray[i].show)
				continue;

			//刷新帧
			pAni=&(m_anmobj->anim[0][MapEnemyArray[i].iCurrentAni]);
			switch(MapEnemyArray[i].iCurrentAni)
			{
			case 1:
				//run
				MapEnemyArray[i].iframe++;
				if(MapEnemyArray[i].iframe > pAni->end)
				{
					//循环播放
					MapEnemyArray[i].iframe=pAni->start;
				}
				break;

			default:
				if(MapEnemyArray[i].iframe < pAni->end)
				{
					MapEnemyArray[i].iframe++;
				}
				break;
			}
		}// end for
	}

	//刷新敌人坐标
	if(c1.iNumClip%3==0)
	{
		for(i=0;i<MAX_MAP_OBJECT;i++)
		{
			if(!MapEnemyArray[i].show)
				continue;

			//如果是跑步动作, 刷新坐标
			if(ENEMY_RUN_ANI!=MapEnemyArray[i].iCurrentAni)
				continue;
			
			//run
			switch(MapEnemyArray[i].iMoveDir)
			{
			case MOVE_X:
				MapEnemyArray[i].x+=MapEnemyArray[i].iMoveSpeed;
				break;

			case MOVE_Z:
				MapEnemyArray[i].z+=MapEnemyArray[i].iMoveSpeed;
				break;

			default:
				break;
			}

			iTurn=0;//是否反向运动
			//碰撞检测
			if(!POINT_IN_RECT(MapEnemyArray[i].x,
					MapEnemyArray[i].z,
					maprect[0]))
			{
				iTurn=1;		
			}
			for(j=1;j<iMapRectNum;j++)
			{
				if(POINT_IN_RECT(MapEnemyArray[i].x,
					MapEnemyArray[i].z,
					maprect[j]))
				{
					iTurn=1;		
				}
			}
			if(iTurn)
			{
				MapEnemyArray[i].iMoveSpeed=-MapEnemyArray[i].iMoveSpeed;//速度矢量取反
				//角度取反（0 1 2 3表示四个方向 0 x轴正向）
				MapEnemyArray[i].iAngle=(MapEnemyArray[i].iAngle+2)%4;
			}
		}// end for
	}
}

void GAMEMAP::checkFire()
{
	//碰撞检测
	hittest();	
	
	//子弹检测
	if(IsFire)
	{
		//坐标不断移动,增量 前方7.0
		rocketX+=7.0f*(float)cos(rad_xz);		
		rocketZ+=7.0f*(float)sin(rad_xz);			
		rocketY+=9.0f*(float)tan(g_elev*PI/360);	

		//统计子弹移动帧数
		IsFire++;
		//子弹计数 IsFire 1到22
		if(IsFire>22)			
		{
			Lbutdown=0;						
			IsFire=0;							
			return;								
		}		
	}

	//攻击提示信息计时
	if(iAttackLife)
	{
		param1++;
		if(param1>60)
		{
			iAttackLife=0;
			param1=0;
		}
	}

}

int GAMEMAP::RoleInMap(float x,float y,float z)
{
	int i;
	RECT_F role;
	
	role.left=z-0.5f;
	role.right=z+0.5f;

	role.bottom=x-0.5f;
	role.top=x+0.5f;

	//不在最大矩形内
	if(!RECT_IN_RECT(role,maprect[0]))
	{	
		return 0;
	}
	
	for(i=1;i<iMapRectNum;i++)
	{
		//在任何障碍矩形内,非法
		if(!RECT_OUT_RECT(role,maprect[i]))
		{	
			return 0;
		}
	}
	return 1;
}
//初始化某一关地图 游戏数据
void GAMEMAP::initMap()
{

	//camera
	g_eye[0]=50;//
	g_eye[2]=5;//
	g_Angle=0;
	g_elev=0;

	//fire
	IsFire=0;

	rocketX=INVALID_ROCKET;
	rocketY=0.0f;
	rocketZ=0.0f;

	iHp=100;	//生命值
	//攻击提示
	iAttackLife=0;
	iAttackMaxLife=0;
	//
	iShowWorldFrame=1;	//显示地图信息
	iShowBombFrame=0;	//不显示子弹撞击
	//加载地图
	LoadMap();
}
//加载地图
int GAMEMAP::LoadMap()
{
	FILE *fp;
	char temp[50]={0};
	int find=0;
	int i;
	
	//清零
	memset(MapEnemyArray,0,sizeof(MapEnemyArray));
	iMapEnemyCursor=0;
	iEnemyNum=0;

	//	立方体贴图编号
	memset(maprectTex,0,sizeof(maprectTex));
	
	fp=fopen(PATH_MAP,"r");
	if(!fp)
	{
		return 0;
	}

	//找 *地图编号
	while(!find && !feof(fp))
	{
		FGetLine(temp,fp);
		if(temp[0]=='*' && temp[1]=='0'+iMatch)
		{
			find=1;
		}
	}
	if(!find)
	{
		return 0;
	}

	//找到了某一关的地图数据
	//enemy数据
	i=0;
	FGetLineJumpCom(temp,fp);	
	while(temp[0]!='#' && !feof(fp))
	{
		//map data
		sscanf(temp,"%d %d %d %d %d %d %d",
			&MapEnemyArray[i].id ,
			&MapEnemyArray[i].x ,
			&MapEnemyArray[i].z ,
			&MapEnemyArray[i].hp ,
			&MapEnemyArray[i].iAngle ,
			&MapEnemyArray[i].iMoveDir ,
			&MapEnemyArray[i].iMoveSpeed );			

		//是否显示
		MapEnemyArray[i].show = 1;
		//初始化 帧
		MapEnemyArray[i].iframe = 40;
		//初始化 动作
		MapEnemyArray[i].iCurrentAni = 1;//run
		//最大生命值
		MapEnemyArray[i].hpMax=MapEnemyArray[i].hp;
		i++;		
		//读取下一行
		FGetLineJumpCom(temp,fp);	
	}
	iMapEnemyCursor=i;
	iEnemyNum=iMapEnemyCursor;

	//地图数据
	i=0;
	FGetLineJumpCom(temp,fp);	
	while(temp[0]!='#' && !feof(fp))
	{
		//map data
		sscanf(temp,"%d %d %d %d %d",
			&maprect[i].top ,
			&maprect[i].bottom,
			&maprect[i].left,
			&maprect[i].right,
			&maprectTex[i]);			
		i++;		
		//读取下一行
		FGetLineJumpCom(temp,fp);	
	}
	iMapRectNum=i;

	fclose(fp);

	return 1;
}

//设置投影方式
void GAMEMAP::initView()
{
	ShowCursor(false);
	switch(iGameState)
	{
	case GAME_MENU:
		m_OpenGL->initMenu();
		break;

	default:
		m_OpenGL->init();		
		break;
	}
}

