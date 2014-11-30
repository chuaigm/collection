//=======================================
//
// ��Ϸ������ΪӪ
//       Quoridor
// ���ߣ�chuaiGM
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
// OpenGL��������ָ�����������ļ�������
extern COpenGLbase* m_OpenGL;
// �ⲿ����Ĵ��ڿ��
extern int WinWidth;
extern int WinHeight;

/////////////////////////

CQuoridor::CQuoridor()
{
	// ��ǰ��Ϸ״̬
	iGameState=GAME_MENU;
	// ��ǰ�˵�ѡ����
	iMenu=-1;
	// �˵�����λ��
	x_menu=10;
	y_menu=10;
	// �˵���ť�Ŀ�Ⱥ͸߶�
	menu_w=10;
	menu_h=10;
	// �������淵�ز˵���ť���
	helpRetButtonW=10;
	// �������淵�ز˵���ť������
	rButtonx=10;
	// �˵����¼��
	menu_dis=10;
}

CQuoridor::~CQuoridor()
{
	
}
//��Ϸ���������ļ����,���ȱ��,������ֹ
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
			ihave=0;//ȱ���ļ�
			break;
		}
		else
			fclose(fp);
	}

	//�����ǰ�ļ���û��,�����ϼ��ļ���,�ټ��
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

//���ظ���ͼƬ,ģ��,��ʼ��ȫ������
void CQuoridor::init()
{
	//mdlģ��Ҫ���ȳ�ʼ��
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
	//LoadT8("data/images/white.bmp", g_cactus[1]);
	//button
	LoadT8("data/images/button.bmp", g_cactus[9]);

	///////////////////////////////////////////
	//������ͼ
	glEnable(GL_TEXTURE_2D);

	//��Ϸ״̬
//	iGameState=GAME_PRE_ANI;
	iGameState=GAME_MENU;
}

//����ͶӰ��ʽ
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

	// ������ά��OpenGL��ͼģ��
	m_OpenGL->init_2D();

	// �����ﴴ��һЩ��OpenGL�л�ȡ�Ĵ��ڲ���
	// ����Ҳ���Ե�����һ������ȥʵ��
	// �˵����¼��
	menu_dis=m_OpenGL->RCheight/15;
	// �˵���ť�Ŀ�Ⱥ͸߶�
	menu_w=m_OpenGL->RCwidth/9;
	menu_h=m_OpenGL->RCheight/20;
	// �������淵�ز˵���ť���
	helpRetButtonW=m_OpenGL->RCwidth/5;
	rButtonx=(m_OpenGL->RCwidth-helpRetButtonW)/2;
	// �˵�����λ��
	x_menu=m_OpenGL->RCwidth-menu_dis/2-menu_w;
	y_menu=/*menu_dis/2 +*/ menu_h;

	// ��Ϸ��������
	player_info_h=m_OpenGL->RCheight/4;
	player_info_w=(m_OpenGL->RCwidth-m_OpenGL->RCheight)*3/8;
	// ǽ��·�Ŀ�����㹫ʽ��
	//  x=roadw; y=wall_w
	//  9*x + 8*y = Height - 5*2
	//    x = 4 y
	roadw = (m_OpenGL->RCheight)/11;
	wall_w= (m_OpenGL->RCheight - 5*2)/44;
	wall_l= 2*roadw+wall_w;
}

// ��Ϸ����ͼ����
void CQuoridor::showMain()
{
	// ��ʾ��������
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
		break;

	case GAME_HELP:
		showHelp();
		break;

	default:
		break;
	}
}
//ʵʱ���
void CQuoridor::check()
{
	int i;
	int x,y;
	//���˵�ѡ��
	x=m_OpenGL->Xmouse;
	y=m_OpenGL->Ymouse;
	iMenu=-1;//��ʼ�� û��ѡ��
	iButton=-1;

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
		//��ʱ
		if(c1.clipcount())
		{
			iGameState=GAME_MENU;
			//����ͶӰ 
			initView();
		}
		break;

	case GAME_MENU:
		if(x<x_menu	|| x>x_menu+menu_w || y<y_menu)
			break;

		for(i=0;i<MENU_NUM;i++)
		{
			if( y>y_menu+i*menu_dis && y<y_menu+i*menu_dis+menu_h )
			{
				iMenu=i;
				break;
			}
		}
		break;
	case GAME_HELP:
		if (x>rButtonx&&x<rButtonx+helpRetButtonW&&y>y_menu&&y<y_menu+menu_h)
		{
			iButton=9;
		}
		break;

	default:
		break;
	}
}
//����ɿ�
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

//���̴���
void CQuoridor::keyupproc(int keyparam)
{
	switch (keyparam)
	{
		//case KEY_F12:
		//	//ֱ�ӹ���
		//	if(iGameState=GAME_IN)
		//	{
		//		iEnemyNum=0;
		//	}		
		//	break;

	case VK_ESCAPE:
		//�ص��˵�
		iGameState=GAME_MENU;
		initView();
		break;

	default:
		break;
	}
}

//����λͼ��ͼ
bool CQuoridor::LoadT8(char *filename, GLuint &texture)
{	
	AUX_RGBImageRec *pImage = NULL;
	
	//����ͼƬ����
	pImage = auxDIBImageLoad(filename);	 
	if(pImage == NULL)	
		return false;
	
	//��������
	glGenTextures(1, &texture);		
	glBindTexture    (GL_TEXTURE_2D,texture);
	//����2ά����
	gluBuild2DMipmaps(GL_TEXTURE_2D,4,  
					  pImage->sizeX,    
					  pImage->sizeY,  
					  GL_RGB, GL_UNSIGNED_BYTE,
					  pImage->data    
					 );
	//�ͷ��ڴ�
	free(pImage->data);      
	free(pImage);	
	return true;      
}

//ָ����ͼ
void CQuoridor::texture_select(UINT textur)
{
	glBindTexture  (GL_TEXTURE_2D, textur);

	// GL_TEXTURE_2D: ����2D����.
	// GL_TEXTURE_WRAP_S: S�����ϵ���ͼģʽ.
	// GL_CLAMP: ����������������0.0,1.0�ķ�Χ֮��.��������˻������.
	//           �������,ֻ�ǻ��Ե�������.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	// ����ͬ��,ֻ������T����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	// �����������
	// GL_TEXTURE_MIN_FILTER: ��С����
	// GL_LINEAR: ���Թ���, ʹ�þ��뵱ǰ��Ⱦ�������������4�����ؼ�Ȩƽ��ֵ.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	// GL_TEXTURE_MAG_FILTER: �Ŵ����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// GL_LINEAR_MIPMAP_NEAREST: ʹ��GL_NEAREST����ӽ���ǰ����εĽ�����
	//                  �������㼶��ͼ���в���,Ȼ����������ֵ�������Բ�ֵ.
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

//�������� ���߳�2e
void CQuoridor::tPic(float e)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-e, -e,  0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( e, -e,  0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( e,  e,  0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-e,  e,  0.0f);
	glEnd();
}

//�����Σ����½�����xy�����w����h
void CQuoridor::tPicRectangle(float x,float y,float w,float h)
{ 
	glPushMatrix();

	//����������
	glTranslatef(x,y,-0.1f);
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
//����ť
void CQuoridor::tPicButton(float x,float y,float w,float h,float ytex)
{ 
	glPushMatrix();

	//����������
	glTranslatef(x,y,-0.1f);	//z������ƶ�
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

//���õƹ�
//void CQuoridor::light0()
//{
//	GLfloat light_position[] = {10.0,10.0,3.0,1.0};
//
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//	
//	//glEnable(GL_LIGHTING);
//	//glEnable(GL_LIGHT0);
//	
//	//������ȼ��
//	glEnable(GL_DEPTH_TEST);
//	//������ɫ����
//	glEnable(GL_COLOR_MATERIAL);
//}

//��������
//void CQuoridor::showpreani()
//{
//	glPushMatrix();
//	//�ƶ�����ǰλ��
//	glTranslatef(0,0,-8.0);
//	texture_select(g_cactus[0]);	
//	tPic(2.0f);
//	//�ƶ�����
//	tSquare(param1*0.2, 0.0, 0.1, 3.0);
//	glPopMatrix();
//}

//ָ��λ�û�һ��������
void CQuoridor::tRectangle(float x, float y, float z, float w, float h, float r, float g, float b, float a)
{
	//���Խ�ջ
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

//�������
void CQuoridor::drawMouse()
{
	glPushMatrix();
	//glLoadIdentity();
	//���Խ�ջ
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

//��ʾ�˵�
void CQuoridor::showmenu()
{
	//������ͼƬ
	glPushMatrix();
	glTranslatef(0.0,0.0,-0.2f);
	texture_select(g_cactus[0]);
	tPicRectangle((m_OpenGL->RCwidth-m_OpenGL->RCheight)/2.0f, 0, (float)m_OpenGL->RCheight, (float)m_OpenGL->RCheight);
	glPopMatrix();

	//  ��Ӧö�ٹ�ϵ     0          1          2          3          4
	char *menustr[]={"��    ��","��Ϸ����","ɳ��ģʽ","������Ϸ","������Ϸ"};
	//char *menustr[]={"������Ϸ","������Ϸ","ɳ��ģʽ","��Ϸ����","��    ��"};

	// ��glOrthoģʽ�»��Ʋ˵�
	// �˵����Ǵ������ϻ��Ƶģ����Բ˵����������ʾ
	for(int i=0;i<MENU_NUM;i++)
	{
		//����
		myfont.Print2D(x_menu+10,y_menu+5+i*menu_dis,menustr[i],FONT1,1,1,1);

		//ͼƬ
		texture_select(g_cactus[9]);
		if(iMenu==i)
		{
			tPicButton((float)x_menu,(float)(y_menu+i*menu_dis),(float)menu_w,(float)menu_h,0.0f);
		}
		else
		{
			tPicButton((float)x_menu,(float)(y_menu+i*menu_dis),(float)menu_w,(float)menu_h,0.5f);
		}
	}
}

//����⺯��
//void CQuoridor::mouseproc(int lparam)
//{
//	xmouseOld=xmouse;
//	ymouseOld=ymouse;
//	xmouse=LOWORD(lparam);
//	ymouse=HIWORD(lparam);
//}

//��������
//��� 	λ�� ����� ��ͼ���� ��ͼ��� iHastop �Ƿ��ж���͵���
void CQuoridor::showmapBox(float *ppos, float *psize, float *ptex,int itex,int iHastop)
{
	float width =psize[0];	
	float height=psize[2];	
	float length=psize[1];	
	float x = ppos[0];	
	float y = ppos[1];
	float z = ppos[2];
	float fstex=ptex[0];//��x�����ͼ
	float fttex=ptex[1];
	float fhtex=ptex[2];
	///////////////////////////////////////////////////////////////////////////////
	
	//wall ��ֱ��Z��
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
	//wall ��ֱ��x��
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
	// һ���̶��ߴ����ֵ
	int scale=10;  // 100 pixel = scale

	// draw aix---------------
	// ����ԭ��
	//myfont.Print2D(0,0,"O",FONT1,1.0f,1.0f,0.0f);
	// ���õ�ĳߴ�
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
	sprintf(tmpstr, "%d",iMenu);
	myfont.Print2D(50,2,tmpstr,FONT0,1.0f,1.0f,1.0f);

	//myfont.Print2D(100,2,"1,0",FONT1,1.0f,0.0f,0.0f);
	//myfont.Print2D(200,2,"2,0",FONT2,1.0f,1.0f,0.0f);
	//myfont.Print2D(300,2,"3,0",FONT3,1.0f,0.0f,1.0f);
	//myfont.Print2D(400,2,"4,0",FONT4,0.0f,0.0f,1.0f);
	//myfont.Print2D(500,2,"5,0",FONT5,0.0f,1.0f,0.0f);
}

void CQuoridor::showHelp()
{
	//������ͼƬ
	glPushMatrix();
	glTranslatef(0.0,0.0,-0.3f);
	texture_select(g_cactus[0]);
	float det=m_OpenGL->RCheight / 6.0f;
	tPicRectangle((m_OpenGL->RCwidth-m_OpenGL->RCheight)/2.0f + det, 0 + det, (float)m_OpenGL->RCheight*0.6f, (float)m_OpenGL->RCheight*0.6f);
	glPopMatrix();

	tRectangle(70,70,0.3f,400,400,0.5,0.5,0.5,0.5);

	char tmpstr[64]={"��Ϸ˵��"};
	myfont.Print2D(50,500,tmpstr,FONT1,1.0,1.0,1.0);
	sprintf(tmpstr,"���������Ժ���д�ɾ��������Ժ���д��");
	myfont.Print2D(50,400,tmpstr,FONT1,1.0,1.0,1.0);
	sprintf(tmpstr,"���������Ժ���д��");
	myfont.Print2D(50,300,tmpstr,FONT1,1.0,1.0,1.0);


	//����
	sprintf(tmpstr,"��ESC�������˵�");
	myfont.Print2D(rButtonx+10,y_menu+5,tmpstr,FONT1,1,1,1);

	//ͼƬ
	texture_select(g_cactus[9]);
	if(iButton==9)
	{
		tPicButton((float)rButtonx,(float)y_menu,(float)helpRetButtonW,(float)menu_h,0.0f);
	}
	else
	{
		tPicButton((float)rButtonx,(float)y_menu,(float)helpRetButtonW,(float)menu_h,0.5f);
	}
}

void CQuoridor::showChessBorad()
{
	// ���������Ϣָʾ��־����
	tRectangle(0,m_OpenGL->RCheight*3/4.0f,-0.2f,player_info_w,player_info_h,1,1,0,0);
	tRectangle(0,m_OpenGL->RCheight*2/4.0f,-0.2f,player_info_w,player_info_h,1,0,0,0);
	tRectangle(0,m_OpenGL->RCheight*1/4.0f,-0.2f,player_info_w,player_info_h,0,1,0,0);
	tRectangle(0,0,-0.2f,player_info_w,player_info_h,0,0,1,0);

	// ��������
	for (int i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
			tRectangle((m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+5+i*(roadw+wall_w),5+j*(roadw+wall_w),-0.2f,roadw,roadw,0.5,0.5,0.1,0);
		}
	}

	// ���Ʋ�ɫ��
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	// ��߻�ɫ���
	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2,  0,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2,  m_OpenGL->RCheight,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+5,  m_OpenGL->RCheight-5,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+5,  5,  0.0f);
	glEnd();
	// �ϱߺ�ɫ���
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2,  m_OpenGL->RCheight,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+m_OpenGL->RCheight,  m_OpenGL->RCheight,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+m_OpenGL->RCheight-5,  m_OpenGL->RCheight-5,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+5,  m_OpenGL->RCheight-5,  0.0f);
	glEnd();
	// �ұ���ɫ���
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+5,  m_OpenGL->RCheight,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+5,  0,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2,  m_OpenGL->RCheight,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+5,  m_OpenGL->RCheight-5,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+5,  5,  0.0f);
	glEnd();
	// �±���ɫ���
	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2,  0,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2,  m_OpenGL->RCheight,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+5,  m_OpenGL->RCheight-5,  0.0f);
	glVertex3f( (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2+5,  5,  0.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();
}
