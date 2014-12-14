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
#include <stdlib.h>

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
	menu.x=10;
	menu.y=10;
	// �˵���ť�Ŀ�Ⱥ͸߶�
	menu_w=10;
	menu_h=10;
	// �������淵�ز˵���ť���
	//helpRetButtonW=10;
	// �������淵�ز˵���ť������
	//rButtonx=10;
	// �˵����¼��
	menu_dis=10;
	// ��Ϸ�����봰�ڱ߽���
	lace=6;

	iButton=-1;

	// ��Ϸ���ݳ�ʼ��
	roadw = 10;
	wall_l=10;
	wall_w=10;
	player_info_h=10;
	player_info_w=10;
	board_x=10;
	board_y=10;
	arr.x=-1;
	arr.y=-1;
	// �����Ϣ����
	// ��
	plyer[0].id=2;
	plyer[0].color=GD_YELLOW;
	plyer[0].x=0;
	plyer[0].y=4;
	// ��
	plyer[1].id=1;
	plyer[1].color=GD_RED;
	plyer[1].x=4;
	plyer[1].y=8;
	// ��
	plyer[2].id=2;
	plyer[2].color=GD_GREEN;
	plyer[2].x=8;
	plyer[2].y=4;
	// ��
	plyer[3].id=0;
	plyer[3].color=GD_BLUE;
	plyer[3].x=4;
	plyer[3].y=0;

	// �������ݳ�ʼ��
	memset(gameData,0,sizeof(gameData));
	// �����˳����Ҫע�⣬������ʱ������x��y��˳��ȥ������������˵
	gameData[2*plyer[0].x][2*plyer[0].y]=GD_YELLOW;
	gameData[2*plyer[1].x][2*plyer[1].y]=GD_RED;
	gameData[2*plyer[2].x][2*plyer[2].y]=GD_GREEN;
	gameData[2*plyer[3].x][2*plyer[3].y]=GD_BLUE;
	// ���ѡȡ��λ��
	pickup.x=-1;
	pickup.y=-1;
	// ��ʽ��Ϸʱ�����������˳��
	ply_head=NULL;
	// ���ʤ����־
	win_flag=GD_BLANK;
	// ��ʾ������Ϣ
	g_debug_flag=false;
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
		"data/images/road.bmp",
		"data/images/bidiu1.bmp",
		"data/images/huangshou1.bmp",
		"data/images/diting1.bmp",
		"data/images/xiaohei1.bmp",
		"data/images/wall1.bmp",
		"data/images/computer_logo.bmp",
		//sound
		//"data/sound/explode1.wav"
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
	//load images
	LoadT8("data/images/quoridor_cover.bmp", g_cactus[0]);		// ����
	LoadT8("data/images/chess_board_shading.bmp", g_cactus[1]);	// ���̵���
	LoadT8("data/images/road.bmp", g_cactus[2]);				// ��ҿ�վ��λ��
	LoadT8("data/images/bidiu1.bmp", g_cactus[3]);				// ���1����
	LoadT8("data/images/huangshou1.bmp", g_cactus[4]);			// ���1����
	LoadT8("data/images/diting1.bmp", g_cactus[5]);				// ���2����
	LoadT8("data/images/xiaohei1.bmp", g_cactus[6]);			// ���3����
	LoadT8("data/images/wall1.bmp", g_cactus[7]);				// ���4����
	LoadT8("data/images/computer_logo.bmp", g_cactus[8]);		// ����ͼ��
	//button
	LoadT8("data/images/button.bmp", g_cactus[9]);				// ��ť

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
	//helpRetButtonW=m_OpenGL->RCwidth/5;
	//rButtonx=(m_OpenGL->RCwidth-helpRetButtonW)/2;
	// �˵�����λ��
	menu.x=m_OpenGL->RCwidth-menu_dis/2-menu_w;
	menu.y=/*menu_dis/2 +*/ menu_h;

	// ��Ϸ��������
	player_info_h=m_OpenGL->RCheight/4.0f;
	player_info_w=(m_OpenGL->RCwidth-m_OpenGL->RCheight)*3/8.0f;
	// �������½ǣ��������
	board_x = (m_OpenGL->RCwidth-m_OpenGL->RCheight)/2;
	board_y = 0;
	// ǽ��·�Ŀ�����㹫ʽ��
	//  x=roadw; y=wall_w
	//  9*x + 8*y = Height - lace*2
	//    x = 4 y
	roadw = (m_OpenGL->RCheight - lace*2)/11.0f;
	wall_w= (m_OpenGL->RCheight - lace*2)/44.0f;
	wall_l= 2*roadw+wall_w;
}

// ��Ϸ����ͼ����
void CQuoridor::showMain()
{
	// ��ʾ��������
	if (g_debug_flag)
	{
		show_Font_test();
	}

	switch(iGameState)
	{
	case GAME_PRE:
//		showpreani();
		break;
	case GAME_MENU:
		showmenu();
		break;
	case GAME_IN_CONFIG:
		drawAccessory();
		drawChessBorad();
		drawInConfig();
		break;
	case GAME_SINGE:
		drawAccessory();
		drawChessBorad();
		drawPlayerWall();
		// ע��˴���͸��ͼ�������˳���й�
		drawPickMask();
		break;
	case GAME_WIN:
		drawAccessory();
		drawChessBorad();
		drawPlayerWall();
		drawVictory();
	case GAME_MULTIP:
	case GAME_SENDBOX:
		drawAccessory();
		drawChessBorad();
		drawPlayerWall();
		// ע��˴���͸��ͼ�������˳���й�
		drawPickMask();
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
	int it;
	float remain;
	//���˵�ѡ��
	x=m_OpenGL->Xmouse;
	y=m_OpenGL->Ymouse;
	iMenu=-1;//��ʼ�� û��ѡ��
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
		//��ʱ
		if(c1.clipcount())
		{
			iGameState=GAME_MENU;
			//����ͶӰ 
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
				// ע�⣬������˵�ѡȡֵ����ֵ��i�����붨���ö�ٱ�������Ӧ��
				iMenu=i;
				break;
			}
		}
		break;
	case GAME_SINGE:
		if (ply_head->id==1)
		{
			computer_AI();
		}
	case GAME_IN_CONFIG:
	case GAME_MULTIP:
	case GAME_SENDBOX:
		// ʵʱ������λ��
		// ���ʱ��������λ�����̱߽���
		if (x>board_x+lace && x<board_x+m_OpenGL->RCheight-lace)
		{
			// ��ǰλ�ó�һ��·�Ŀ�Ⱥ�һ��ǽ�Ŀ�ȵ���������
			it=(int)((x-board_x-lace)/(roadw+wall_w));
			// ��������
			remain = (x-board_x-lace)-it*(roadw+wall_w);
			if (remain>roadw)
			{	// ������·��ȴ�����ǽ��λ�ã�����
				arr.x = it*2+1;
			} else {
				// ��ҿ��Է��õ�λ�ã�·��ż��
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
		}
		// �����ⷵ�ز˵���ť���������̰�ť
		else if (x>menu.x&&x<menu.x+menu_w)
		{
			if (y>menu.y&&y<menu.y+menu_h)
			{
				iButton=BUTTON_INIT_OR_CONFIRM;
			}
			else if (y>m_OpenGL->RCheight-menu.y-menu_h&&y<m_OpenGL->RCheight-menu.y)
			{
				iButton=BUTTON_RETURN;
			}
		}
		break;
	case GAME_WIN:
		if (x>menu.x&&x<menu.x+menu_w)
		{
			if (y>menu.y&&y<menu.y+menu_h)
			{
				iButton=BUTTON_INIT_OR_CONFIRM;
			}
			else if (y>m_OpenGL->RCheight-menu.y-menu_h&&y<m_OpenGL->RCheight-menu.y)
			{
				iButton=BUTTON_RETURN;
			}
		}
		break;
	case GAME_HELP:
		if (x>menu.x&&x<menu.x+menu_w&&y>menu.y&&y<menu.y+menu_h)
		{
			iButton=BUTTON_RETURN;
		}
		break;

	default:
		break;
	}
}
// ����ɿ�
void CQuoridor::lbuttonproc(int lparam)
{
	if (iButton==BUTTON_RETURN)
	{
		iGameState=GAME_MENU;
		return;
	}
	switch(iGameState)
	{
	case GAME_MENU:
		if(iMenu<0)
			break;
		switch(iMenu)
		{
		case MENU_SINGE:
			iGameState=GAME_IN_CONFIG;
			resetGameData();
			break;
		case MENU_MULTIP:
			//iGameState=GAME_MULTIP;
			break;
		case MENU_SENDBOX:
			iGameState=GAME_SENDBOX;
			resetGameData();
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
	case GAME_IN_CONFIG:
		// �ڵ�������ģʽ�£��˰�ťΪȷ��
		if (iButton==BUTTON_INIT_OR_CONFIRM)
		{
			// ����൱�ڵ�����Ϸ�ĳ�ʼ������
			int nn=0;
			ply_head=NULL;
			player* tail=NULL;
			// ����Ҳ���Կ�����һ��ÿ����ҵ�nextָ��
			for (int i=0; i<4; i++)
			{	// �����ǰ�����״̬�ǲ�����Ϸ��(�������˿��ƣ�Ҳ�����ǵ���)
				if (plyer[i].id!=2)
				{
					if (ply_head==NULL)
					{
						ply_head=&plyer[i];
						tail=&plyer[i];
					}
					tail->next=&plyer[i];
					tail=&plyer[i];
					nn++;
				}
				// �����ǰ��ҵ�״̬�ǹرյģ���������Ϸ��
				else
				{	// ��Ϊ��ʼ����ʱ���Ѿ��������Ӧ��վ�ĳ�ʼλ������Ϸ�����н�����
					// �����������Ϸ����Ҫ����������Ϸ����
					gameData[plyer[i].x*2][plyer[i].y*2]=GD_BLANK;
				}
			}
			// �������������������2ʱ���޷�������Ϸ
			if (nn<2)
			{	// TODO �Ժ���ӵ�����ʾ, ���������������2��
				ply_head=NULL;
				break;
			}
			// ������������
			tail->next=ply_head;
			player* tmp_head=ply_head;
			// ѭ����ʣ��ǽ����ֵ
			do
			{	// ����������,21��2��ȡ��=10
				//            21��3��ȡ��=7
				//            21��4��ȡ��=5
				tmp_head->wall_num_left=wall_total_num/nn;
				tmp_head=tmp_head->next;
			}while (ply_head!=tmp_head);
			iGameState=GAME_SINGE;
		}
		// ���ѡȡ��ͬ��ҵ�����ѡ��ʱ�Ĵ���
		// �������
		for (size_t j=0; j<4; j++)
		{
			// ������ѡ�ĵ����ǣ���ң����ԣ����ǹر�
			for (size_t i=0; i< 3; i++)
			{
				if (m_OpenGL->Xmouse>(float)board_x+i*3*(roadw+wall_w)
					&& m_OpenGL->Xmouse<(float)board_x+i*3*(roadw+wall_w)+menu_w
					&& m_OpenGL->Ymouse>(player_info_h-menu_h)/2+player_info_h*j
					&& m_OpenGL->Ymouse<(player_info_h-menu_h)/2+menu_h+player_info_h*j
					)
				{
					plyer[3-j].id=i;
				}
			}
		}
		break;
	case GAME_SINGE:
		// �������ҿɿصĽ�ɫ
		if (ply_head->id==0)
		{
			playerActionRule();
		}
		break;
	case GAME_MULTIP:
		break;
	case GAME_SENDBOX:
		freeRuleSendBox();
		break;
	case GAME_HELP:
		break;

	default:
		break;
	}
}
// ����Ҽ�����
void CQuoridor::rbuttonproc( int lparam )
{
	switch(iGameState)
	{
	case GAME_SINGE:
		break;
	case GAME_MULTIP:
		break;
	case GAME_SENDBOX:
		switch (gameData[arr.x][arr.y])
		{
		case GD_BLANK:
			break;
		case GD_YELLOW:
			plyer[0].x=-1;
			plyer[0].y=-1;
			gameData[arr.x][arr.y]=0;
			break;
		case GD_RED:
			plyer[1].x=-1;
			plyer[1].y=-1;
			gameData[arr.x][arr.y]=0;
			break;
		case GD_GREEN:
			plyer[2].x=-1;
			plyer[2].y=-1;
			gameData[arr.x][arr.y]=0;
			break;
		case GD_BLUE:
			plyer[3].x=-1;
			plyer[3].y=-1;
			gameData[arr.x][arr.y]=0;
			break;
		case GD_WALL:
			{	// ��vector�У�������ǰ�Ҽ�ѡȡ��ǽ��λ��
				std::vector<pos2d>::iterator itor=find(wall_vec.begin(),wall_vec.end(),arr);
				if (itor==wall_vec.end())
				{
					//continue;
					break;
				}
				int dist=distance(wall_vec.begin(),itor);
				// ��������ż�������±�Ҳ��ż��
				if (dist%2==0)
				{
					// ע����������ʧЧ������
					gameData[itor->x][itor->y]=GD_BLANK;
					gameData[(itor->x+(itor+1)->x)/2][(itor->y+(itor+1)->y)/2]=GD_BLANK;
					gameData[(itor+1)->x][(itor+1)->y]=GD_BLANK;
					wall_vec.erase(itor,itor+2);
					break;
				}
				// �����������������±�Ҳ������
				else
				{
					// ע�������ʧЧ����
					gameData[itor->x][itor->y]=GD_BLANK;
					gameData[(itor->x+(itor-1)->x)/2][(itor->y+(itor-1)->y)/2]=GD_BLANK;
					gameData[(itor-1)->x][(itor-1)->y]=GD_BLANK;
					wall_vec.erase(itor-1,itor+1);
					break;
				}
				// ����ɼӿɲ��ӣ����������ɾ������ѡȡ�Ĺ���Ҳ������
				pickup.x=-1;
				pickup.y=-1;
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
	case VK_F1:
		g_debug_flag=!g_debug_flag;
		break;
	case VK_ESCAPE:
		//�ص��˵�
		iGameState=GAME_MENU;
		//initView();
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
void CQuoridor::tPicRectangle(float x,float y,float w,float h,float deep)
{ 
	glPushMatrix();

	//����������
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
	char *menustr[]={"��    ��","��Ϸ˵��","ɳ��ģʽ","������Ϸ","������Ϸ"};
	//char *menustr[]={"������Ϸ","������Ϸ","ɳ��ģʽ","��Ϸ����","��    ��"};

	// ��glOrthoģʽ�»��Ʋ˵�
	// �˵����Ǵ������ϻ��Ƶģ����Բ˵����������ʾ
	for(int i=0;i<MENU_NUM;i++)
	{
		//����
		myfont.Print2D(menu.x+10,menu.y+5+i*menu_dis,menustr[i],FONT1,1,1,1);

		//ͼƬ
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

//����⺯��
//void CQuoridor::mouseproc(int lparam)
//{
//	xmouseOld=xmouse;
//	ymouseOld=ymouse;
//	xmouse=LOWORD(lparam);
//	ymouse=HIWORD(lparam);
//}

// ��ʾ��������
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

	//for (int i=0; i<9;i++)
	//{
	//	for (int j=0; j<9;j++)
	//	{
	//		sprintf(tmpstr, "%d,",tmpflag[j][i]);
	//		myfont.Print2D(10*(j+1),260+10*i,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);
	//	}
	//}

	for (int i=0; i<17;i++)
	{
		for (int j=0; j<17;j++)
		{
			sprintf(tmpstr, "%d,",gameData[j][i]);
			myfont.Print2D(10*(j+1),70+10*i,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);
		}
	}

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
	//������ͼƬ
	glPushMatrix();
	glTranslatef(0.0,0.0,-0.5f);
	texture_select(g_cactus[0]);
	float det=m_OpenGL->RCheight / 6.0f;
	tPicRectangle((m_OpenGL->RCwidth-m_OpenGL->RCheight)/2.0f + det, 0 + det, (float)m_OpenGL->RCheight*0.6f, (float)m_OpenGL->RCheight*0.6f);
	glPopMatrix();

	tRectangle((float)board_x,0,-0.3f,(float)m_OpenGL->RCheight,(float)m_OpenGL->RCheight,1,1,1,0.5);

	char tmpstr[64]={"��Ϸ˵��"};
	myfont.Print2D(board_x+lace,500,tmpstr,FONT1,0.0,0.0,0.0);
	sprintf(tmpstr,"���������Ժ���д�ɾ��������Ժ���д��");
	myfont.Print2D(board_x+lace,400,tmpstr,FONT1,0.0,0.0,1.0);
	sprintf(tmpstr,"���������Ժ���д��");
	myfont.Print2D(board_x+lace,300,tmpstr,FONT1,1.0,1.0,0.0);

	//����
	sprintf(tmpstr,"��ESC����");
	myfont.Print2D(menu.x+5,menu.y+5,tmpstr,FONT1,1,1,1);

	//ͼƬ
	texture_select(g_cactus[9]);
	if(iButton==BUTTON_RETURN)
	{
		tPicButton((float)menu.x,(float)menu.y,(float)menu_w,(float)menu_h,0.0f);
	}
	else
	{
		tPicButton((float)menu.x,(float)menu.y,(float)menu_w,(float)menu_h,0.5f);
	}
}

void CQuoridor::drawChessBorad()
{
	// ��ռͼ�����
	float layer=-0.5;

	// �������̵�
	texture_select(g_cactus[1]);
	tPicRectangle((float)board_x,0,(float)m_OpenGL->RCheight,(float)m_OpenGL->RCheight,layer-0.1f);
	// ��������
	texture_select(g_cactus[2]);
	for (int i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
			tPicRectangle(board_x+lace+i*(roadw+wall_w),lace+j*(roadw+wall_w),roadw,roadw,layer);
		}
	}

	// ���Ʋ�ɫ��
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glTranslatef(0,0,layer);
	// ��߻�ɫ����
	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f( (float)board_x, 0,  0.0f);
	glVertex3f( (float)board_x, (float)m_OpenGL->RCheight,  0.0f);
	glVertex3f( (float)board_x+lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glVertex3f( (float)board_x+lace, (float)lace,  0.0f);
	glEnd();
	// �ϱߺ�ɫ����
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f( (float)board_x, (float)m_OpenGL->RCheight, 0.0f);
	glVertex3f( (float)board_x+m_OpenGL->RCheight, (float)m_OpenGL->RCheight,  0.0f);
	glVertex3f( (float)board_x+m_OpenGL->RCheight-lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glVertex3f( (float)board_x+lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glEnd();
	// �ұ���ɫ����
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f((float) board_x+m_OpenGL->RCheight, (float)m_OpenGL->RCheight,  0.0f);
	glVertex3f((float) board_x+m_OpenGL->RCheight, 0,  0.0f);
	glVertex3f((float) board_x+m_OpenGL->RCheight-lace, (float)lace,  0.0f);
	glVertex3f((float) board_x+m_OpenGL->RCheight-lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glEnd();
	// �±���ɫ����
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
}

void CQuoridor::drawAccessory()
{
	char tmpstr[64]={0};
	// ��ռͼ�����
	float layer=-0.5;
	float alp=0.8f;
	// ���������Ϣָʾ��־����
	if (ply_head == &plyer[0])
	{
		tRectangle(0,3*player_info_h,layer,player_info_w*7/6,player_info_h,1,1,0,alp);
	}
	else
	{
		tRectangle(0,3*player_info_h,layer,player_info_w,player_info_h,1,1,0,alp);
	}
	if (ply_head == &plyer[1])
	{
		tRectangle(0,2*player_info_h,layer,player_info_w*7/6,player_info_h,1,0,0,alp);
	}
	else
	{
		tRectangle(0,2*player_info_h,layer,player_info_w,player_info_h,1,0,0,alp);
	}
	if (ply_head == &plyer[2])
	{
		tRectangle(0,1*player_info_h,layer,player_info_w*7/6,player_info_h,0,1,0,alp);
	}
	else
	{
		tRectangle(0,1*player_info_h,layer,player_info_w,player_info_h,0,1,0,alp);
	}
	if (ply_head == &plyer[3])
	{
		tRectangle(0,0,layer,(float)player_info_w*7/6,(float)player_info_h,0,0,1,alp);
	}
	else
	{
		tRectangle(0,0,layer,(float)player_info_w,(float)player_info_h,0,0,1,alp);
	}
	

	for (int i=0; i<4; i++)
	{
		if (plyer[i].id!=2)
		{
			// ������Ҫע����ͼ�ı��˳��
			texture_select(g_cactus[3+i]);
			tPicRectangle(0,(3-i+1/2.0f)*player_info_h,roadw,roadw,layer+0.1f);
			if (plyer[i].id==1)
			{	// ���Ƶ��Ե�ͼ��
				texture_select(g_cactus[8]);
				tPicRectangle((roadw+wall_w),(3-i+1/2.0f)*player_info_h,roadw*0.6f,roadw*0.6f,layer+0.1f);
			}
			sprintf(tmpstr,"ǽʣ��: %d",plyer[i].wall_num_left);
			myfont.Print2D(10,(int)((3-i+1/4.0f)*player_info_h),tmpstr,FONT1,1,1,1);
		}
	}
	
/*
// ע�͵��ɷ���
	if (plyer[1].id!=2)
	{
		texture_select(g_cactus[4]);
		tPicRectangle(0,(2+1/2.0f)*player_info_h,roadw,roadw,layer+0.1f);
		sprintf(tmpstr,"ǽʣ��: %d",plyer[0].wall_num_left);
		myfont.Print2D(10,(int)((2+1/4.0f)*player_info_h),tmpstr,FONT1,1,1,1);
	}
	if (plyer[2].id!=2)
	{
		texture_select(g_cactus[5]);
		tPicRectangle(0,(1+1/2.0f)*player_info_h,roadw,roadw,layer+0.1f);
		sprintf(tmpstr,"ǽʣ��: %d",plyer[0].wall_num_left);
		myfont.Print2D(10,(int)((1+1/4.0f)*player_info_h),tmpstr,FONT1,1,1,1);
	}
	if (plyer[3].id!=2)
	{
		texture_select(g_cactus[6]);
		tPicRectangle(0,(0+1/2.0f)*player_info_h,roadw,roadw,layer+0.1f);
		sprintf(tmpstr,"ǽʣ��: %d",plyer[0].wall_num_left);
		myfont.Print2D(10,(int)((0+1/4.0f)*player_info_h),tmpstr,FONT1,1,1,1);
	}
*/

	// ��ť��ͼ
	texture_select(g_cactus[9]);
	sprintf(tmpstr,"���ز˵�");
	myfont.Print2D(menu.x+10,m_OpenGL->RCheight-menu.y-menu_h+5,tmpstr,FONT1,1,1,1);
	if(BUTTON_RETURN==iButton)
	{
		tPicButton((float)menu.x,(float)(m_OpenGL->RCheight-menu.y-menu_h),(float)menu_w,(float)menu_h,0.0f);
	}
	else
	{
		tPicButton((float)menu.x,(float)(m_OpenGL->RCheight-menu.y-menu_h),(float)menu_w,(float)menu_h,0.5f);
	}

	switch (iGameState)
	{
	case GAME_IN_CONFIG:
	case GAME_SINGE:
	case GAME_WIN:
		sprintf(tmpstr,"ȷ    ��");
		break;
	case GAME_SENDBOX:
		sprintf(tmpstr,"��������");
		break;
	default:
		break;
	}
	myfont.Print2D(menu.x+10,menu.y+5,tmpstr,FONT1,1,1,1);
	// ��ťͼƬ
	if(BUTTON_INIT_OR_CONFIRM==iButton)
	{
		tPicButton((float)menu.x,(float)(menu.y),(float)menu_w,(float)menu_h,0.0f);
	}
	else
	{
		tPicButton((float)menu.x,(float)(menu.y),(float)menu_w,(float)menu_h,0.5f);
	}
}

void CQuoridor::drawPlayerWall()
{
	// ��Ӧ��ɫ����ң��ƣ��죬�̣���
	for (int i=0; i<4; i++)
	{
		if (plyer[i].id!=2)
		{
			texture_select(g_cactus[3+i]);
			if (plyer[i].x>-1 && plyer[i].y>-1)
			{
				tPicRectangle(board_x+lace+(roadw+wall_w)*plyer[i].x,lace+(roadw+wall_w)*plyer[i].y,roadw,roadw);
				/*switch (i)
				{
				case 0:
					glColor4f(1.0f,1.0f,0.0f,0.8f);
				case 1:
					glColor4f(1.0f,0.0f,0.0f,0.8f);
				case 2:
					glColor4f(0.0f,1.0f,0.0f,0.8f);
				case 3:
					glColor4f(0.0f,0.0f,1.0f,0.8f);
				}*/
			}
		}
	}
	
/*
	texture_select(g_cactus[4]);
	if (plyer[1].x>-1 && plyer[1].y>-1)
	{
		tPicRectangle(board_x+lace+(roadw+wall_w)*plyer[1].x,lace+(roadw+wall_w)*plyer[1].y,roadw,roadw);
	}
	texture_select(g_cactus[5]);
	if (plyer[2].x>-1 && plyer[2].y>-1)
	{
		tPicRectangle(board_x+lace+(roadw+wall_w)*plyer[2].x,lace+(roadw+wall_w)*plyer[2].y,roadw,roadw);
	}
	texture_select(g_cactus[6]);
	if (plyer[3].x>-1 && plyer[3].y>-1)
	{
		tPicRectangle(board_x+lace+(roadw+wall_w)*plyer[3].x,lace+(roadw+wall_w)*plyer[3].y,roadw,roadw);
	}
*/
	// ����ǽ
	for (size_t i=0; i<wall_vec.size();i+=2)
	{
		texture_select(g_cactus[7]);
		// ����Ǻ�ǽ
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
	//static int det=1;
	int det = 2;
	//static int ctick=0;
	if (ply_head!=NULL)
	{
		// �ֵ�˭�ߣ������ͼ���ϣ�������̬��ʾ
		float cx=board_x+lace+ply_head->x*(roadw+wall_w);
		float cy=        lace+ply_head->y*(roadw+wall_w);
		//���Խ�ջ
		glPushAttrib(GL_CURRENT_BIT);
		glPushMatrix();
		glDisable(GL_TEXTURE_2D);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		// ����
		glVertex3f( cx-det,  cy-det,  0.5f);
		glVertex3f( cx-det,  cy-det+roadw/4.0f,  0.5f);	// ����
		glVertex3f( cx-det,  cy-det,  0.5f);
		glVertex3f( cx-det+roadw/4.0f,  cy-det,  0.5f);	//	����
		// ����
		glVertex3f( cx-det,  cy+roadw+det,  0.5f);
		glVertex3f( cx-det+roadw/4.0f,  cy+roadw+det,  0.5f);	// ����
		glVertex3f( cx-det,  cy+roadw+det,  0.5f);
		glVertex3f( cx-det,  cy+roadw+det-roadw/4,  0.5f);	// ����
		// ����
		glVertex3f( cx+roadw+det,  cy+roadw+det,  0.5f);
		glVertex3f( cx+roadw+det-roadw/4,  cy+roadw+det,  0.5f);	// ����
		glVertex3f( cx+roadw+det,  cy+roadw+det,  0.5f);
		glVertex3f( cx+roadw+det,  cy+roadw+det-roadw/4,  0.5f);	// ����
		// ����
		glVertex3f( cx+roadw+det,  cy-det,  0.5f);
		glVertex3f( cx+roadw+det-roadw/4,  cy-det,  0.5f);	// ����
		glVertex3f( cx+roadw+det,  cy-det,  0.5f);
		glVertex3f( cx+roadw+det,  cy-det+roadw/4,  0.5f);	// ����
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glPopMatrix();
		glPopAttrib();
		// ��ʱע�͵���̬��ʾ
		//if (ctick>g_refresh_rate/10)
		//{
		//	det++;
		//	if (det>3)
		//	{
		//		det=1;
		//	}
		//	ctick=0;
		//}
		//ctick++;
	}
	if (pickup.x < 0 && pickup.y < 0)
	{
		return ;
	}
	else if (pickup.x%2==0 && pickup.y%2==0)
	{
		if (iGameState==GAME_SINGE)
		{
			// �����Ժ󻻸���ͼ
			tRectangle(board_x+lace+pickup.x/2*(roadw+wall_w),lace+pickup.y/2*(roadw+wall_w),0,roadw,roadw,0.1f,0.5f,1,0.6f);
			// ������ҿ���Ԥѡ��
			for (size_t i=0; i<preselect_pos.size();i++)
			{
				tRectangle(board_x+lace+preselect_pos[i].x/2*(roadw+wall_w),lace+preselect_pos[i].y/2*(roadw+wall_w),0,roadw,roadw,0.1f,0.5f,1,0.6f);
			}
			// ����ģʽʱ����ʾ��ҵ�Ŀ�ĵ�
			for (int i=0; i<9; i++)
			{
				switch(ply_head->color)
				{
				case GD_YELLOW:
					tRectangle(board_x+lace+8*(roadw+wall_w)+roadw-wall_w, lace+i*(roadw+wall_w),0,(float)wall_w,roadw,0.9f,0.9f,0.0f,0.6f);
					break;
				case GD_RED:
					tRectangle(board_x+lace+i*(roadw+wall_w), (float)lace,0,roadw,(float)wall_w,1.0f,0.0f,0.0f,0.8f);
					break;
				case GD_GREEN:
					tRectangle((float)board_x+lace, lace+i*(roadw+wall_w),0,(float)wall_w,roadw,0.0f,0.9f,0,0.6f);
					break;
				case GD_BLUE:
					tRectangle(board_x+lace+i*(roadw+wall_w),lace+8*(roadw+wall_w)+roadw-wall_w,0,roadw,(float)wall_w,0.0f,0.0f,0.9f,0.6f);
					break;
				default:
					break;
				}
			}
		}
		else if (iGameState==GAME_SENDBOX)
		{
			// �����Ժ󻻸���ͼ
			tRectangle(board_x+lace+pickup.x/2*(roadw+wall_w),lace+pickup.y/2*(roadw+wall_w),0,roadw,roadw,0.1f,0.5f,1,0.6f);
		}
	}
	else if (pickup.x%2==0 && pickup.y%2!=0)
	{
		// ������Ϊż����������Ϊ������Ϊ��ǽ
		tRectangle(board_x+lace+pickup.x/2*(roadw+wall_w),lace+(int)(pickup.y/2)*(roadw+wall_w)+roadw,0,(float)wall_l*4/9,(float)wall_w,1.0f,1,0,0.6f);
	}
	else if (pickup.x%2!=0 && pickup.y%2==0)
	{
		// ������Ϊ������������Ϊż����Ϊ��ǽ
		tRectangle(board_x+lace+(int)(pickup.x/2)*(roadw+wall_w)+roadw,lace+pickup.y/2*(roadw+wall_w),0,(float)wall_w,(float)wall_l*4/9,1.0f,1,0,0.6f);
	}

	return ;
}
// ����ȷ�ϴ���
void CQuoridor::drawConfirm()
{
	float layer=0.3f;
	float tri_w=m_OpenGL->RCwidth/3.0f;
	float tri_h=m_OpenGL->RCheight/3.0f;
	//���Ʊ�����͸�����ƴ���
	tRectangle(tri_w-menu_w,tri_h,layer,tri_w+2*menu_w,tri_h,0.0f,0.0f,0.0f,0.7f);
}

void CQuoridor::resetGameData()
{
	// ���vector
	wall_vec.swap(std::vector<pos2d>());
	// �������ݳ�ʼ��
	memset(gameData,0,sizeof(gameData));
	// �������λ������
	plyer[0].x=0;
	plyer[0].y=4;
	plyer[0].wall_num_left=0;
	plyer[0].next=NULL;
	plyer[1].x=4;
	plyer[1].y=8;
	plyer[1].wall_num_left=0;
	plyer[1].next=NULL;
	plyer[2].x=8;
	plyer[2].y=4;
	plyer[2].wall_num_left=0;
	plyer[2].next=NULL;
	plyer[3].x=4;
	plyer[3].y=0;
	plyer[3].wall_num_left=0;
	plyer[3].next=NULL;
	if (iGameState==GAME_SENDBOX)
	{
		plyer[0].id=0;
		plyer[1].id=0;
		plyer[2].id=0;
		plyer[3].id=0;
	}
	else if (iGameState==GAME_IN_CONFIG)
	{
		plyer[0].id=2;
		plyer[1].id=1;
		plyer[2].id=2;
		plyer[3].id=0;
	}
	// �����˳����Ҫע�⣬������ʱ������x��y��˳��ȥ������������˵
	gameData[2*plyer[0].x][2*plyer[0].y]=GD_YELLOW;
	gameData[2*plyer[1].x][2*plyer[1].y]=GD_RED;
	gameData[2*plyer[2].x][2*plyer[2].y]=GD_GREEN;
	gameData[2*plyer[3].x][2*plyer[3].y]=GD_BLUE;
	// ���ѡȡ��λ��
	pickup.x=-1;
	pickup.y=-1;
	// ��һ�װ����ͷָ��
	ply_head=NULL;
	// ���ʤ����־
	win_flag=GD_BLANK;
}

void CQuoridor::drawInConfig()
{
	// ���������Ϣָʾ��־����
	tRectangle(0,3.1f*player_info_h,-0.2f,player_info_w*7,player_info_h*0.8f,1,1,0,0.4f);
	tRectangle(0,2.1f*player_info_h,-0.2f,player_info_w*7,player_info_h*0.8f,1,0,0,0.4f);
	tRectangle(0,1.1f*player_info_h,-0.2f,player_info_w*7,player_info_h*0.8f,0,1,0,0.4f);
	tRectangle(0,0.1f*player_info_h,-0.2f,player_info_w*7,player_info_h*0.8f,0,0,1,0.4f);

	char *cfgstr[]={" ��  �� "," ��  �� "," ��  �� "};
	// ��ť��ͼ
	texture_select(g_cactus[9]);
	for (int j=0; j<4; j++)
	{
		for (int i=0; i<3; i++)
		{
			if(i==plyer[3-j].id)
			{
				myfont.Print2D((int)(board_x+i*3*(roadw+wall_w))+10,(int)((player_info_h-menu_h)/2)+5+(int)player_info_h*j,cfgstr[i],FONT1,1,1,1);
				tPicButton((float)board_x+i*3*(roadw+wall_w),(player_info_h-menu_h)/2+player_info_h*j,(float)menu_w,(float)menu_h,0.0f);
			}
			else
			{
				myfont.Print2D((int)(board_x+i*3*(roadw+wall_w))+10,(int)((player_info_h-menu_h)/2)+5+(int)player_info_h*j,cfgstr[i],FONT1,0,0,0);
				tPicButton((float)board_x+i*3*(roadw+wall_w),(player_info_h-menu_h)/2+player_info_h*j,(float)menu_w,(float)menu_h,0.5f);
			}
		}
	}
	
/*
	for (int i=0; i< 3; i++)
	{
		if(i==plyer[2].id)
		{
			myfont.Print2D((int)(board_x+i*3*(roadw+wall_w))+10,(int)((player_info_h-menu_h)/2)+5+(int)player_info_h,cfgstr[i],FONT1,1,1,1);
			tPicButton((float)board_x+i*3*(roadw+wall_w),(player_info_h-menu_h)/2+player_info_h,(float)menu_w,(float)menu_h,0.0f);
		}
		else
		{
			myfont.Print2D((int)(board_x+i*3*(roadw+wall_w))+10,(int)((player_info_h-menu_h)/2)+5+(int)player_info_h,cfgstr[i],FONT1,0,0,0);
			tPicButton((float)board_x+i*3*(roadw+wall_w),(player_info_h-menu_h)/2+player_info_h,(float)menu_w,(float)menu_h,0.5f);
		}
	}
	for (int i=0; i< 3; i++)
	{
		if(i==plyer[1].id)
		{
			myfont.Print2D((int)(board_x+i*3*(roadw+wall_w))+10,(int)((player_info_h-menu_h)/2)+5+(int)player_info_h*2,cfgstr[i],FONT1,1,1,1);
			tPicButton((float)board_x+i*3*(roadw+wall_w),(player_info_h-menu_h)/2+player_info_h*2,(float)menu_w,(float)menu_h,0.0f);
		}
		else
		{
			myfont.Print2D((int)(board_x+i*3*(roadw+wall_w))+10,(int)((player_info_h-menu_h)/2)+5+(int)player_info_h*2,cfgstr[i],FONT1,0,0,0);
			tPicButton((float)board_x+i*3*(roadw+wall_w),(player_info_h-menu_h)/2+player_info_h*2,(float)menu_w,(float)menu_h,0.5f);
		}
	}
	for (int i=0; i< 3; i++)
	{
		if(i==plyer[0].id)
		{
			myfont.Print2D((int)(board_x+i*3*(roadw+wall_w))+10,(int)((player_info_h-menu_h)/2)+5+(int)player_info_h*3,cfgstr[i],FONT1,1,1,1);
			tPicButton((float)board_x+i*3*(roadw+wall_w),(player_info_h-menu_h)/2+player_info_h*3,(float)menu_w,(float)menu_h,0.0f);
		}
		else
		{
			myfont.Print2D((int)(board_x+i*3*(roadw+wall_w))+10,(int)((player_info_h-menu_h)/2)+5+(int)player_info_h*3,cfgstr[i],FONT1,0,0,0);
			tPicButton((float)board_x+i*3*(roadw+wall_w),(player_info_h-menu_h)/2+player_info_h*3,(float)menu_w,(float)menu_h,0.5f);
		}
	}
*/
}

void CQuoridor::playerActionRule()
{
	// �Ѿ����ڵ�ǽ��λ�ã��ǲ��ܱ�ѡȡ��
	if (gameData[arr.x][arr.y]==GD_WALL)
	{
		return ;
	}
	// ���֮ǰû��ѡȡ�κ�λ��
	if ( pickup.x <0 && pickup.y <0 )
	{
		// �հ׵����λ�����߲��ܿ��Ƶ����λ
		if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]!=ply_head->color)
		{
			return;
		}
		pickup.x=arr.x;
		pickup.y=arr.y;
		// ���ѡ���ǵ�ǰ���λ��
		if (gameData[pickup.x][pickup.y]==ply_head->color)
		{
			playerMovablePos(pickup);
		}
		// ������if else ������return�������൱��return
	}
	// ֮ǰ��ѡȡ��λ��
	else
	{	// ������ѡȡ��λ��,������������ͬλ�ã����ʼ����
		if (arr == pickup)
		{
			pickup.x = -1;
			pickup.y = -1;
			// �����ҿ��ߴ�ѡλ��(��Ȼ����Ҳ�ǿ��Ե�)
			preselect_pos.clear();
			return ;	// �������������ͬ������Ϊ��ѡ
		}
		// ��������ٴε��ʱ��ѡȡ��λ������������ӿ����ƶ���λ��
		// Ҫ�ߵ�λ���ڿ���λ�õ�������
		//else if (0==arr.x%2 && 0==arr.y%2 && 0==pickup.x%2 && 0==pickup.y%2)
		else if (find(preselect_pos.begin(),preselect_pos.end(),arr)!=preselect_pos.end())
		{
			// ������������뵽�������Ӵ���׶�
			char tmp=0;
			// ����
			tmp=gameData[arr.x][arr.y];
			gameData[arr.x][arr.y]=gameData[pickup.x][pickup.y];
			gameData[pickup.x][pickup.y]=tmp;
			// ��Ŀ��λ���ϣ�������ұ���
			switch (gameData[arr.x][arr.y])
			{
			case GD_BLANK:
				break;
			case GD_YELLOW:
				plyer[0].x=arr.x/2;
				plyer[0].y=arr.y/2;
				if (plyer[0].x==8)
				{
					win_flag=GD_YELLOW;
					iGameState=GAME_WIN;
				}
				break;
			case GD_RED:
				plyer[1].x=arr.x/2;
				plyer[1].y=arr.y/2;
				if (plyer[1].y==0)
				{
					win_flag=GD_RED;
					iGameState=GAME_WIN;
				}
				break;
			case GD_GREEN:
				plyer[2].x=arr.x/2;
				plyer[2].y=arr.y/2;
				if (plyer[2].x==0)
				{
					win_flag=GD_GREEN;
					iGameState=GAME_WIN;
				}
				break;
			case GD_BLUE:
				plyer[3].x=arr.x/2;
				plyer[3].y=arr.y/2;
				if (plyer[3].y==8)
				{
					win_flag=GD_BLUE;
					iGameState=GAME_WIN;
				}
				break;
			default:
				break;
			}
			// ʵ����Ϸʱ����ɳ��ģʽ��Ŀ��λ���ϣ�һ���ǿ�
			//// ���ھɵ�λ���ϣ�������ұ���
			//switch (gameData[pickup.x][pickup.y])
			//{
			//case GD_BLANK:
			//	break;
			//case GD_YELLOW:
			//	plyer[0].x=pickup.x/2;
			//	plyer[0].y=pickup.y/2;
			//	break;
			//case GD_RED:
			//	plyer[1].x=pickup.x/2;
			//	plyer[1].y=pickup.y/2;
			//	break;
			//case GD_GREEN:
			//	plyer[2].x=pickup.x/2;
			//	plyer[2].y=pickup.y/2;
			//	break;
			//case GD_BLUE:
			//	plyer[3].x=pickup.x/2;
			//	plyer[3].y=pickup.y/2;
			//	break;
			//default:
			//	break;
			//}
			// �������Ҫ���㷨��ת����������ƶ���
			goto ACTION_RULE_EXIT;
		}
		// ���ߵ����Ӧ����ֻ���Ѿ�ѡ��ǽ�������
		// ���ﵥ�����ˣ�����ֵ���˵�ǰ��ң���ѡ����Ҵ���
		else if (gameData[arr.x][arr.y]==ply_head->color)
		{
			pickup.x=arr.x;
			pickup.y=arr.y;
			playerMovablePos(pickup);
		}
		// ��ѡǽ�Ĺ����У��������з�ǽ��λ��
		else if ((arr.x+arr.y)%2!=1)
		{
			return ;
		}
		// ����ϴ�ѡȡ��λ��Ҳ��ǽ�����ѡȡ��Ҳ��ǽ���������ѡȡ��λ���ǿ�
		// ����ѡȡ��ͬλ���Ѿ����ʼ����
		// �����ǲ���ʵ��ǽλ�õ�Ψһ��ڣ������������ж����ǽʣ�����Ƿ����
		else if ((pickup.x+pickup.y)%2==1 
			&& (arr.x+arr.y)%2==1 
			&& GD_BLANK == gameData[arr.x][arr.y]
			&& ply_head->wall_num_left>0)
		{
			// ����Ǻ�ǽ,�������ѡ�ĺ��ϴ�ѡ����ͬһ����
			if (pickup.x%2==0&&arr.y==pickup.y)
			{	// ������ѡ������һ��ѡ�����һ��λ��
				// �����������ӵ��м�λ���ǿ��õ�
				if(arr.x==pickup.x-2&&gameData[arr.x+1][arr.y]==GD_BLANK)
				{
					// ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
					wall_vec.push_back(arr);
					wall_vec.push_back(pickup);
					// ������Ϸ�㷨����,����ע�⣬��������ǽλ�õ��м����Ӵ�Ҳ��ֵ
					gameData[arr.x][arr.y]=GD_WALL;
					gameData[arr.x+1][arr.y]=GD_WALL;
					gameData[pickup.x][pickup.y]=GD_WALL;
					goto RULE_WALL_EXIT;
				}
				// ��������ѡ������һ���ұ�һ��λ��
				// �����������ӵ��м�λ���ǿ��õ�
				else if(arr.x==pickup.x+2&&gameData[arr.x-1][arr.y]==GD_BLANK)
				{
					// ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
					wall_vec.push_back(pickup);
					wall_vec.push_back(arr);
					// ������Ϸ�㷨����
					gameData[arr.x][arr.y]=GD_WALL;
					gameData[arr.x-1][arr.y]=GD_WALL;
					gameData[pickup.x][pickup.y]=GD_WALL;
					goto RULE_WALL_EXIT;
				}
			}
			// �������ǽ���������ѡ�ĺ��ϴ�ѡ����ͬһ����
			else if(pickup.y%2==0&&arr.x==pickup.x)
			{	// ������ѡ�������һ��ѡ�������һ��
				// �����������ӵ��м�λ���ǿ��õ�
				if (arr.y==pickup.y-2&&gameData[arr.x][arr.y+1]==GD_BLANK)
				{
					// ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
					wall_vec.push_back(arr);
					wall_vec.push_back(pickup);
					// ������Ϸ�㷨����
					gameData[arr.x][arr.y]=GD_WALL;
					gameData[arr.x][arr.y+1]=GD_WALL;
					gameData[pickup.x][pickup.y]=GD_WALL;
					goto RULE_WALL_EXIT;
				}
				// ������ѡ�������һ��ѡ�������һ��
				// �����������ӵ��м�λ���ǿ��õ�
				else if (arr.y==pickup.y+2&&gameData[arr.x][arr.y-1]==GD_BLANK)
				{
					// ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
					wall_vec.push_back(pickup);
					wall_vec.push_back(arr);
					// ������Ϸ�㷨����
					gameData[arr.x][arr.y]=GD_WALL;
					gameData[arr.x][arr.y-1]=GD_WALL;
					gameData[pickup.x][pickup.y]=GD_WALL;
					goto RULE_WALL_EXIT;
				}
			}
			// ����ʹ��goto��䣬����ܹ��ɹ�����ǽ����ô�Ѿ�����ȥ�ˣ�
			// ������������һ���ɷ�ǽ����������Ϊ����ѡȡ
			pickup.x=arr.x;
			pickup.y=arr.y;
		}
		// ���������һ����Ϊ����ѡȡǽ
		else if ( gameData[arr.x][arr.y]!=GD_WALL )
		{
			pickup.x=arr.x;
			pickup.y=arr.y;
			// �����ҿ��ߴ�ѡλ��
			preselect_pos.clear();
		}
	}
	return ;
RULE_WALL_EXIT:
	// �����жϣ������ǽ�Ƿ�Ϸ����ж�������·ŵ�һ��ǽ��ʹ���κ�һ������޽⣬��ô��ǽ�Ƿ�
	// ������Ϸ�����ô��Ҫ��ԭԭ������Ϸ����
	if (!judgeWallLegal())
	{
		// ��ԭ��Ϸ�㷨����
		gameData[arr.x][arr.y]=GD_BLANK;
		gameData[(arr.x+pickup.x)/2][(arr.y+pickup.y)/2]=GD_BLANK;
		gameData[pickup.x][pickup.y]=GD_BLANK;
		// ������ѹ���ǽ����(������)
		wall_vec.pop_back();
		wall_vec.pop_back();
		// ���Ԥѡλ��
		pickup.x=-1;
		pickup.y=-1;
		// �����ҿ��ߴ�ѡλ��
		preselect_pos.clear();
		return ;
	}
	// ��ǰ��ҵĿ���ǽ����1
	ply_head->wall_num_left--;
ACTION_RULE_EXIT:
	////sound
	//sndPlaySound("data/sound/explode1.wav",SND_ASYNC);
	// �����ѡ�����ƶ������ô����ҵ��ж��ͽ����ˣ�����ȨӦ�ý�����һλ���
	ply_head=ply_head->next;
	// ���Ԥѡλ��
	pickup.x=-1;
	pickup.y=-1;
	// �����ҿ��ߴ�ѡλ��
	preselect_pos.clear();
	return ;
}
// �˺�����ǰ���ǣ�selected����������ѡȡ�Ŀ���λ��
// �˺����ĳ����ǣ�preselect_pos��ҿ���λ�õ�����
void CQuoridor::playerMovablePos( pos2d selected )
{
	// ��ҿ��ߵ�λ�ã�����ֵ����ȶ���ٷ����򣬶��ڱ�Ե���û��˵��
	// ������Ϊ�ڱ�Եʱ����Ϊ��ǽ��Ҳ���ǲ�����
	pos2d tmppos;
	// ���vector
	preselect_pos.swap(std::vector<pos2d>());
	// �ж���ҿ��ߵĿ���λ��

	// ������������һ�У������û��ǽ�������һ�����
	if ( selected.x > 0 
		&& gameData[selected.x-1][selected.y]!=GD_WALL 
		&& gameData[selected.x-2][selected.y]==GD_BLANK )
	{
		tmppos.x=selected.x-2;
		tmppos.y=selected.y;
		preselect_pos.push_back(tmppos);
	}
	// �������λ����ߵڶ��л��һ�У����������ң������������û��ǽ�������λ�ÿ���
	else if (selected.x > 2
		&& gameData[selected.x-1][selected.y]!=GD_WALL 
		&& gameData[selected.x-2][selected.y]!=GD_BLANK 
		&& gameData[selected.x-3][selected.y]!=GD_WALL 
		&& gameData[selected.x-4][selected.y]==GD_BLANK )
	{
		tmppos.x=selected.x-4;
		tmppos.y=selected.y;
		preselect_pos.push_back(tmppos);
	}
	//// �����߲����������ڵڶ����ϣ�
	//else if ()
	//{
	//}
	// ����������ұ�һ�У����ұ�û��ǽ�����ұ�һ�����
	if ( selected.x < 16 
		&& gameData[selected.x+1][selected.y]!=GD_WALL 
		&& gameData[selected.x+2][selected.y]==GD_BLANK )
	{
		tmppos.x=selected.x+2;
		tmppos.y=selected.y;
		preselect_pos.push_back(tmppos);
	}
	// �������λ���ұߵڶ��л��һ�У����ұ�����ң����ұ�������û��ǽ�����ұ�λ�ÿ���
	else if (selected.x < 14
		&& gameData[selected.x+1][selected.y]!=GD_WALL 
		&& gameData[selected.x+2][selected.y]!=GD_BLANK 
		&& gameData[selected.x+3][selected.y]!=GD_WALL 
		&& gameData[selected.x+4][selected.y]==GD_BLANK )
	{
		tmppos.x=selected.x+4;
		tmppos.y=selected.y;
		preselect_pos.push_back(tmppos);
	}
	// ����������±�һ�У������û��ǽ�������һ�����
	if ( selected.y > 0 
		&& gameData[selected.x][selected.y-1]!=GD_WALL 
		&& gameData[selected.x][selected.y-2]==GD_BLANK )
	{
		tmppos.x=selected.x;
		tmppos.y=selected.y-2;
		preselect_pos.push_back(tmppos);
	}
	// �������λ����ߵڶ��л��һ�У����������ң������������û��ǽ�������λ�ÿ���
	else if (selected.y > 2
		&& gameData[selected.x][selected.y-1]!=GD_WALL 
		&& gameData[selected.x][selected.y-2]!=GD_BLANK 
		&& gameData[selected.x][selected.y-3]!=GD_WALL 
		&& gameData[selected.x][selected.y-4]==GD_BLANK )
	{
		tmppos.x=selected.x;
		tmppos.y=selected.y-4;
		preselect_pos.push_back(tmppos);
	}
	// ����������ұ�һ�У����ұ�û��ǽ�����ұ�һ�����
	if ( selected.y < 16 
		&& gameData[selected.x][selected.y+1]!=GD_WALL 
		&& gameData[selected.x][selected.y+2]==GD_BLANK )
	{
		tmppos.x=selected.x;
		tmppos.y=selected.y+2;
		preselect_pos.push_back(tmppos);
	}
	// �������λ����ߵڶ��л��һ�У����������ң������������û��ǽ�������λ�ÿ���
	else if (selected.y < 14
		&& gameData[selected.x][selected.y+1]!=GD_WALL 
		&& gameData[selected.x][selected.y+2]!=GD_BLANK 
		&& gameData[selected.x][selected.y+3]!=GD_WALL 
		&& gameData[selected.x][selected.y+4]==GD_BLANK )
	{
		tmppos.x=selected.x;
		tmppos.y=selected.y+4;
		preselect_pos.push_back(tmppos);
	}
//-----------------
	//// �����Ͻǿ��ߵ���������ж�
	//// ���ж�λ�ÿ���
	//if ( selected.x>=2 && selected.y<=14 && gameData[selected.x-2][selected.y+2]==GD_BLANK )
	//{	// �����Ͻǵ�����������е�������
	//	if (selected.x==2 && selected.y==14)
	//	{	// ������û��ǽ����������еж���ң����ߣ�����û��ǽ�����������еж����
	//		if ((gameData[1][14]!=GD_WALL && gameData[0][14]!=GD_BLANK ) 
	//		 || (gameData[2][15]!=GD_WALL && gameData[2][16]!=GD_BLANK ))
	//		{
	//			tmppos.x= 0;
	//			tmppos.y=16;
	//			preselect_pos.push_back(tmppos);
	//		}
	//	}
	//	// �����֧�������������Ͻǿ�����(��������������������)
	//	else if ((gameData[selected.x-1][selected.y]!=GD_WALL && gameData[selected.x-2][selected.y]!=GD_BLANK ) 
	//		|| (gameData[selected.x][selected.y+1]!=GD_WALL && gameData[selected.x][selected.y+2]!=GD_BLANK ))
	//	{
	//		tmppos.x=selected.x-2;
	//		tmppos.y=selected.y+2;
	//		preselect_pos.push_back(tmppos);
	//	}
	//}
	//
	//// �����Ͻǿ��ߵ���������ж�
	//// ���ж�λ�ÿ���
	//if ( selected.x<=14 && selected.y<=14 && gameData[selected.x+2][selected.y+2]==GD_BLANK )
	//{	// �����Ͻǵ�����������е�������
	//	if (selected.x==14 && selected.y==14)
	//	{	// ����ұ�û��ǽ�������ұ��еж���ң����ߣ�����û��ǽ�����������еж����
	//		if ((gameData[15][14]!=GD_WALL && gameData[16][14]!=GD_BLANK ) 
	//			|| (gameData[14][15]!=GD_WALL && gameData[14][16]!=GD_BLANK ))
	//		{
	//			tmppos.x=16;
	//			tmppos.y=16;
	//			preselect_pos.push_back(tmppos);
	//		}
	//	}
	//	// �����֧�������������Ͻǿ�����(��������������������)
	//	else if ((gameData[selected.x+1][selected.y]!=GD_WALL && gameData[selected.x+2][selected.y]!=GD_BLANK ) 
	//		|| (gameData[selected.x][selected.y+1]!=GD_WALL && gameData[selected.x][selected.y+2]!=GD_BLANK ))
	//	{
	//		tmppos.x=selected.x+2;
	//		tmppos.y=selected.y+2;
	//		preselect_pos.push_back(tmppos);
	//	}
	//}
//-----------------	
	// �����Ͻǿ��ߵ���������ж�(�������߽�ֵ)
	if ( selected.x > 2 && selected.y < 14 && gameData[selected.x-2][selected.y+2]==GD_BLANK )
	{	// �������еж��������ߵ��˵������ǽ���������еж������������˵��ϱ���ǽ
		if ( (gameData[selected.x-1][selected.y]!=GD_WALL 
			&& gameData[selected.x-2][selected.y]!=GD_BLANK 
			&& gameData[selected.x-3][selected.y]==GD_WALL 
			&& gameData[selected.x-2][selected.y+1]!=GD_WALL) 
		|| (gameData[selected.x][selected.y+1]!=GD_WALL 
			&& gameData[selected.x][selected.y+2]!=GD_BLANK 
			&& gameData[selected.x][selected.y+3]==GD_WALL 
			&& gameData[selected.x-1][selected.y+2]!=GD_WALL) )
		{
			tmppos.x=selected.x-2;
			tmppos.y=selected.y+2;
			preselect_pos.push_back(tmppos);
		}
		// ������ߵ������߻�����ң����ϱߵ��ϱ߻��е���
		else if ( (gameData[selected.x-1][selected.y]!=GD_WALL 
			&& gameData[selected.x-2][selected.y]!=GD_BLANK 
			&& gameData[selected.x-4][selected.y]!=GD_BLANK 
			&& gameData[selected.x-2][selected.y+1]!=GD_WALL) 
		|| (gameData[selected.x][selected.y+1]!=GD_WALL 
			&& gameData[selected.x][selected.y+2]!=GD_BLANK 
			&& gameData[selected.x][selected.y+4]!=GD_BLANK 
			&& gameData[selected.x-1][selected.y+2]!=GD_WALL) )
		{
			tmppos.x=selected.x-2;
			tmppos.y=selected.y+2;
			preselect_pos.push_back(tmppos);
		}
	}
	//// ������λ�ڵڶ��е����
	//else if ( selected.x == 2 && selected.y < 14 && gameData[0][selected.y+2]==GD_BLANK )
	//{
	//	if ( (gameData[0][selected.y]!=GD_BLANK ) || (gameData[2][selected.y+2]!=GD_BLANK && gameData[2][selected.y+3]==GD_WALL ) )
	//	{
	//		tmppos.x=selected.x-2;
	//		tmppos.y=selected.y+2;
	//		preselect_pos.push_back(tmppos);
	//	}
	//}
	// �����Ͻǿ��ߵ���������ж�(�������߽�ֵ)
	if ( selected.x < 14 && selected.y < 14 && gameData[selected.x+2][selected.y+2]==GD_BLANK )
	{
		if ( (gameData[selected.x+1][selected.y]!=GD_WALL 
			&& gameData[selected.x+2][selected.y]!=GD_BLANK 
			&& gameData[selected.x+3][selected.y]==GD_WALL 
			&& gameData[selected.x+2][selected.y+1]!=GD_WALL) 
		|| (gameData[selected.x][selected.y+1]!=GD_WALL 
			&& gameData[selected.x][selected.y+2]!=GD_BLANK 
			&& gameData[selected.x][selected.y+3]==GD_WALL 
			&& gameData[selected.x+1][selected.y+2]!=GD_WALL) )
		{
			tmppos.x=selected.x+2;
			tmppos.y=selected.y+2;
			preselect_pos.push_back(tmppos);
		}
		else if ( (gameData[selected.x+1][selected.y]!=GD_WALL 
			&& gameData[selected.x+2][selected.y]!=GD_BLANK 
			&& gameData[selected.x+4][selected.y]!=GD_BLANK 
			&& gameData[selected.x+2][selected.y+1]!=GD_WALL) 
		|| (gameData[selected.x][selected.y+1]!=GD_WALL 
			&& gameData[selected.x][selected.y+2]!=GD_BLANK 
			&& gameData[selected.x][selected.y+4]!=GD_BLANK 
			&& gameData[selected.x+1][selected.y+2]!=GD_WALL) )
		{
			tmppos.x=selected.x+2;
			tmppos.y=selected.y+2;
			preselect_pos.push_back(tmppos);
		}
	}
	//// ������λ�������ڶ���????
	//else if ( selected.x == 14 && selected.y < 14 && gameData[16][selected.y+2]==GD_BLANK )
	//{
	//	if ( (gameData[16][selected.y]!=GD_BLANK ) || (gameData[16][selected.y+2]!=GD_BLANK && gameData[16][selected.y+3]==GD_WALL ) )
	//	{
	//		tmppos.x=selected.x+2;
	//		tmppos.y=selected.y+2;
	//		preselect_pos.push_back(tmppos);
	//	}
	//}
	// �����½ǿ��ߵ���������ж�(�������߽�ֵ)
	if ( selected.x > 2 && selected.y >2 && gameData[selected.x-2][selected.y-2]==GD_BLANK )
	{
		if ( (gameData[selected.x-1][selected.y]!=GD_WALL 
			&& gameData[selected.x-2][selected.y]!=GD_BLANK 
			&& gameData[selected.x-3][selected.y]==GD_WALL 
			&& gameData[selected.x-2][selected.y-1]!=GD_WALL ) 
		|| (gameData[selected.x][selected.y-1]!=GD_WALL 
			&& gameData[selected.x][selected.y-2]!=GD_BLANK 
			&& gameData[selected.x][selected.y-3]==GD_WALL 
			&& gameData[selected.x-1][selected.y-2]!=GD_WALL ) )
		{
			tmppos.x=selected.x-2;
			tmppos.y=selected.y-2;
			preselect_pos.push_back(tmppos);
		}
		else if ( (gameData[selected.x-1][selected.y]!=GD_WALL 
			&& gameData[selected.x-2][selected.y]!=GD_BLANK 
			&& gameData[selected.x-4][selected.y]!=GD_BLANK 
			&& gameData[selected.x-2][selected.y-1]!=GD_WALL ) 
		|| (gameData[selected.x][selected.y-1]!=GD_WALL 
			&& gameData[selected.x][selected.y-2]!=GD_BLANK 
			&& gameData[selected.x][selected.y-4]!=GD_BLANK 
			&& gameData[selected.x-1][selected.y-2]!=GD_WALL ) )
		{
			tmppos.x=selected.x-2;
			tmppos.y=selected.y-2;
			preselect_pos.push_back(tmppos);
		}
	}
	// �����½ǿ��ߵ���������ж�(�������߽�ֵ)
	if ( selected.x < 14 && selected.y > 2 && gameData[selected.x+2][selected.y-2]==GD_BLANK )
	{
		if ( (gameData[selected.x+1][selected.y]!=GD_WALL 
			&& gameData[selected.x+2][selected.y]!=GD_BLANK 
			&& gameData[selected.x+3][selected.y]==GD_WALL 
			&& gameData[selected.x+2][selected.y-1]!=GD_WALL ) 
		|| (gameData[selected.x][selected.y-1]!=GD_WALL 
			&& gameData[selected.x][selected.y-2]!=GD_BLANK 
			&& gameData[selected.x][selected.y-3]==GD_WALL 
			&& gameData[selected.x+1][selected.y-2]!=GD_WALL ) )
		{
			tmppos.x=selected.x+2;
			tmppos.y=selected.y-2;
			preselect_pos.push_back(tmppos);
		}
		else if ( (gameData[selected.x+1][selected.y]!=GD_WALL 
			&& gameData[selected.x+2][selected.y]!=GD_BLANK 
			&& gameData[selected.x+4][selected.y]!=GD_BLANK 
			&& gameData[selected.x+2][selected.y-1]!=GD_WALL ) 
		|| (gameData[selected.x][selected.y-1]!=GD_WALL 
			&& gameData[selected.x][selected.y-2]!=GD_BLANK 
			&& gameData[selected.x][selected.y-4]!=GD_BLANK 
			&& gameData[selected.x+1][selected.y-2]!=GD_WALL ) )
		{
			tmppos.x=selected.x+2;
			tmppos.y=selected.y-2;
			preselect_pos.push_back(tmppos);
		}
	}
}

void CQuoridor::freeRuleSendBox()
{
	// ɳ��ģʽ�£���ʼ�����̵İ�ť
	if (iButton==BUTTON_INIT_OR_CONFIRM)
	{
		resetGameData();
		return ;
	}
	// �Ѿ����ڵ�ǽ��λ�ã��ǲ��ܱ�ѡȡ��
	if (gameData[arr.x][arr.y]==GD_WALL)
	{
		return ;
	}
	// ���֮ǰû��ѡȡ�κ�λ��
	if ( pickup.x <0 && pickup.y <0 )
	{
		// �հ׵����λ
		if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]==GD_BLANK)
		{
			return ;
		}
		pickup.x=arr.x;
		pickup.y=arr.y;
	}
	else
	{	// ������ѡȡ��λ��,������������ͬλ�ã����ʼ����
		if (arr == pickup)
		{
			pickup.x = -1;
			pickup.y = -1;
			return ;
		}
		// ��������ٴε��ʱ��ǰ�����ζ������λ�ã�����Ϊ˫ż��λ
		else if (0==arr.x%2 && 0==arr.y%2 && 0==pickup.x%2 && 0==pickup.y%2)
		{
			// ������������뵽�������Ӵ���׶�
			char tmp=0;
			// ����
			tmp=gameData[arr.x][arr.y];
			gameData[arr.x][arr.y]=gameData[pickup.x][pickup.y];
			gameData[pickup.x][pickup.y]=tmp;
			// ��Ŀ��λ���ϣ�������ұ���
			switch (gameData[arr.x][arr.y])
			{
			case GD_BLANK:
				break;
			case GD_YELLOW:
				plyer[0].x=arr.x/2;
				plyer[0].y=arr.y/2;
				break;
			case GD_RED:
				plyer[1].x=arr.x/2;
				plyer[1].y=arr.y/2;
				break;
			case GD_GREEN:
				plyer[2].x=arr.x/2;
				plyer[2].y=arr.y/2;
				break;
			case GD_BLUE:
				plyer[3].x=arr.x/2;
				plyer[3].y=arr.y/2;
				break;
			default:
				break;
			}
			// ���ھɵ�λ���ϣ�������ұ���
			switch (gameData[pickup.x][pickup.y])
			{
			case GD_BLANK:
				break;
			case GD_YELLOW:
				plyer[0].x=pickup.x/2;
				plyer[0].y=pickup.y/2;
				break;
			case GD_RED:
				plyer[1].x=pickup.x/2;
				plyer[1].y=pickup.y/2;
				break;
			case GD_GREEN:
				plyer[2].x=pickup.x/2;
				plyer[2].y=pickup.y/2;
				break;
			case GD_BLUE:
				plyer[3].x=pickup.x/2;
				plyer[3].y=pickup.y/2;
				break;
			default:
				break;
			}
			// ��ѡȡλ�ø�ֵΪ��Чֵ
			goto SEND_BOX_EXIT;
		}
		// ��ѡǽ�Ĺ����У��������λ��
		else if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]==GD_BLANK)
		{
			return ;
		}
		// ����ϴ�ѡȡ��λ��Ҳ��ǽ�����ѡȡ��Ҳ��ǽ���������ѡȡ��λ���ǿ�
		// ����ѡȡ��ͬλ���Ѿ����ʼ����
		else if ((pickup.x+pickup.y)%2==1 
			&& (arr.x+arr.y)%2==1 
			&& GD_BLANK == gameData[arr.x][arr.y])
		{
			// ����Ǻ�ǽ,�������ѡ�ĺ��ϴ�ѡ����ͬһ����
			if (pickup.x%2==0&&arr.y==pickup.y)
			{	// ������ѡ������һ��ѡ�����һ��λ��
				// �����������ӵ��м�λ���ǿ��õ�
				if(arr.x==pickup.x-2&&gameData[arr.x+1][arr.y]==GD_BLANK)
				{
					// ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
					wall_vec.push_back(arr);
					wall_vec.push_back(pickup);
					// ������Ϸ�㷨����,����ע�⣬��������ǽλ�õ��м����Ӵ�Ҳ��ֵ
					gameData[arr.x][arr.y]=GD_WALL;
					gameData[arr.x+1][arr.y]=GD_WALL;
					gameData[pickup.x][pickup.y]=GD_WALL;
					goto SEND_BOX_EXIT;
				}
				// ��������ѡ������һ���ұ�һ��λ��
				// �����������ӵ��м�λ���ǿ��õ�
				else if(arr.x==pickup.x+2&&gameData[arr.x-1][arr.y]==GD_BLANK)
				{
					// ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
					wall_vec.push_back(pickup);
					wall_vec.push_back(arr);
					// ������Ϸ�㷨����
					gameData[arr.x][arr.y]=GD_WALL;
					gameData[arr.x-1][arr.y]=GD_WALL;
					gameData[pickup.x][pickup.y]=GD_WALL;
					goto SEND_BOX_EXIT;
				}
			}
			// �������ǽ���������ѡ�ĺ��ϴ�ѡ����ͬһ����
			else if(pickup.y%2==0&&arr.x==pickup.x)
			{	// ������ѡ�������һ��ѡ�������һ��
				// �����������ӵ��м�λ���ǿ��õ�
				if (arr.y==pickup.y-2&&gameData[arr.x][arr.y+1]==GD_BLANK)
				{
					// ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
					wall_vec.push_back(arr);
					wall_vec.push_back(pickup);
					// ������Ϸ�㷨����
					gameData[arr.x][arr.y]=GD_WALL;
					gameData[arr.x][arr.y+1]=GD_WALL;
					gameData[pickup.x][pickup.y]=GD_WALL;
					goto SEND_BOX_EXIT;
				}
				// ������ѡ�������һ��ѡ�������һ��
				// �����������ӵ��м�λ���ǿ��õ�
				else if (arr.y==pickup.y+2&&gameData[arr.x][arr.y-1]==GD_BLANK)
				{
					// ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
					wall_vec.push_back(pickup);
					wall_vec.push_back(arr);
					// ������Ϸ�㷨����
					gameData[arr.x][arr.y]=GD_WALL;
					gameData[arr.x][arr.y-1]=GD_WALL;
					gameData[pickup.x][pickup.y]=GD_WALL;
					goto SEND_BOX_EXIT;
				}	
			}
		}
		if ( gameData[arr.x][arr.y]!=GD_WALL )
		{
			// ���������һ����Ϊ����ѡȡ
			pickup.x=arr.x;
			pickup.y=arr.y;
		}
	}
	return ;
SEND_BOX_EXIT:
	// ���Ԥѡλ��
	pickup.x=-1;
	pickup.y=-1;
	return ;
}

bool CQuoridor::judgeWallLegal()
{
	// ����һ����ʱ�Ĵ����ҿ���λ�õĶ���
	std::deque<pos2d> que;
	// 9x9����ʱ��ǣ���¼��ҿ��ߵ�λ��,0Ϊ�գ�1Ϊ�����������ߣ�2Ϊ�Ѿ������
	char tmpflag[9][9];
	bool jump_flag=false;
	for (int i=0; i<4; i++)
	{	// ��յ�ǰ��ҿ���λ�õı��
		memset(tmpflag,0,sizeof(tmpflag));
		// ��մ�ѡ����
		que.clear();
		// ֤�������ҵ����ڵ���ת��־��Ϊfalse
		jump_flag=false;
		// ֻҪ���ǹرյ����
		if (plyer[i].id!=2)
		{	// �Ƚ���ǰ��ҵ�λ�ô��룬ע�⣬���������0~8�ķ�Χ��ת��Ϊ0~16�ķ�Χ
			// ��Ϊ��ѡ���㷨���ص���0~16�ķ�Χ
			pos2d tmpp;
			tmpp.x=plyer[i].x*2;
			tmpp.y=plyer[i].y*2;
			// ���ߵ��Ƕ�ά�����ǰ���0~8��Ƶ�
			tmpflag[plyer[i].x][plyer[i].y]=1;
			que.push_back(tmpp);
			// ������߶����ﻹ�����ݣ���û���ҵ��յ�Ŀ���λ��
			while (que.size()>0 && jump_flag == false)
			{	// ʹ�ö��е�ͷ������������λ��
				playerMovablePos(*que.begin());
				// ��ʵ������˵��ѡ���ߵ��㷨�ǲ����ܷ��ؿ�������
				if (preselect_pos.size()>0)
				{
					for (size_t j=0;j<preselect_pos.size();j++)
					{	// ��ͬ��Ҳ�ͬ�ĵ����յ������
						switch(plyer[i].color)
						{
						case GD_YELLOW:
							if (preselect_pos[j].x==16)
								jump_flag=true;
							break;
						case GD_RED:
							if (preselect_pos[j].y==0)
								jump_flag=true;
							break;
						case GD_GREEN:
							if (preselect_pos[j].x==0)
								jump_flag=true;
							break;
						case GD_BLUE:
							if (preselect_pos[j].y==16)
								jump_flag=true;
							break;
						default:
							break;
						}
						if (jump_flag)
							break;
						// �����һλ�û�û�б����ǹ�,û�б����
						if (tmpflag[preselect_pos[j].x/2][preselect_pos[j].y/2]==0)
						{
							// ���ߵ��Ƕ�ά�����ǰ���0~8��Ƶ�, ��Ԥѡλ��תΪ���λ��
							tmpflag[preselect_pos[j].x/2][preselect_pos[j].y/2]=1;
							que.push_back(preselect_pos[j]);
						}
					}
				}
				// ����һ��ȥ����Ŀ���λ���Ѿ����������������ǰ��
				que.pop_front();
			}
			if (jump_flag==false /*&& que.size()==0*/)
			{	// �����ǰ���û�м�⵽���ߵ��յ�
				return false;
			}
		}
	}
	// ������Ϸ�����ȫ���н�
	return true;
}

void CQuoridor::drawVictory()
{
	if (win_flag==GD_BLANK)
	{
		return ;
	}
	char tmpstr[64]={0};
	float layer=0.3f;
	float tri_w=m_OpenGL->RCwidth/3.0f;
	float tri_h=m_OpenGL->RCheight/3.0f;
	//���Ʊ�����͸�����ƴ���
	tRectangle(tri_w-menu_w,tri_h,layer,tri_w+2*menu_w,tri_h,0.0f,0.0f,0.0f,0.8f);

	glPushMatrix();
	glTranslatef(0,0,0.5f);
	switch (win_flag)
	{
	case GD_YELLOW:
		sprintf(tmpstr,"�� ɫ �� ��");
		myfont.Print2D(m_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT2,1,1,0);
		break;
	case GD_RED:
		sprintf(tmpstr,"�� ɫ �� ��");
		myfont.Print2D(m_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT2,1,0,0);
		break;
	case GD_GREEN:
		sprintf(tmpstr,"�� ɫ �� ��");
		myfont.Print2D(m_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT2,0,1,0);
		break;
	case GD_BLUE:
		sprintf(tmpstr,"�� ɫ �� ��");
		myfont.Print2D(m_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT2,0,0,1);
		break;
	default:
		break;
	}
	sprintf(tmpstr,"  ʤ  �� ");
	myfont.Print2D(m_OpenGL->RCwidth/2,(int)(tri_h*(1+3/7.0)),tmpstr,FONT2,1,1,1);
	sprintf(tmpstr," �� �� ����");
	myfont.Print2D(m_OpenGL->RCwidth/2,(int)(tri_h*(1+1/8.0)),tmpstr,FONT2,1,1,1);
	glPopMatrix();

	// �������дһ���̻����� 
}

void CQuoridor::computer_AI()
{	// ֻ�е�ǰ����ǵ��Բ�������
	if (ply_head->id==1)
	{
		//Sleep(800);
		//if (random(0,2.0)==0)
		if (random(0,1.0)==0)
		{
			/*
			switch (ply_head->color)
			{
			case GD_YELLOW:
				break;
			case GD_RED:
				break;
			case GD_GREEN:
				break;
			case GD_BLUE:
				break;
			}
			*/
			pos2d tmppoint;
			tmppoint.x=ply_head->x*2;
			tmppoint.y=ply_head->y*2;
			playerMovablePos(tmppoint);
			pos2d targ=preselect_pos[random(0,preselect_pos.size())];
			// ������������뵽�������Ӵ���׶�
			char tmp=0;
			// ����
			tmp=gameData[targ.x][targ.y];
			gameData[targ.x][targ.y]=gameData[ply_head->x*2][ply_head->y*2];
			gameData[ply_head->x*2][ply_head->y*2]=tmp;

			ply_head->x=targ.x/2;
			ply_head->y=targ.y/2;

			preselect_pos.clear();
			// ��Ŀ��λ���ϣ�������ұ���
			switch (ply_head->color)
			{
			case GD_YELLOW:
				if (ply_head->x==8)
				{
					win_flag=GD_YELLOW;
					iGameState=GAME_WIN;
				}
				break;
			case GD_RED:
				if (ply_head->y==0)
				{
					win_flag=GD_RED;
					iGameState=GAME_WIN;
				}
				break;
			case GD_GREEN:
				if (ply_head->x==0)
				{
					win_flag=GD_GREEN;
					iGameState=GAME_WIN;
				}
				break;
			case GD_BLUE:
				if (ply_head->y==8)
				{
					win_flag=GD_BLUE;
					iGameState=GAME_WIN;
				}
				break;
			default:
				break;
			}
		}
		else
		{	// �ڵ������ǰ���ǽ
			switch (ply_head->next->color)
			{
			case GD_YELLOW:
				break;
			case GD_RED:
				break;
			case GD_GREEN:
				break;
			case GD_BLUE:
				if (ply_head->next->x>0&&ply_head->next->x<8)
				{
					//int x=ply_head->next->x;
					//int y=ply_head->next->y;
					//pos2d tpwall1,tmpwall2;
					//tpwall1.x=x;
					//tpwall1.y=y+1;
					//tpwall2.x=x+1;
					//tpwall2.y=y;
					//wall_vec.push_back()
					//gameData[][]=GD_WALL;
				}
				break;
			}
		}
		// ��һλ���
		ply_head=ply_head->next;
	}
}
