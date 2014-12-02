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
	menu.x=10;
	menu.y=10;
	// �˵���ť�Ŀ�Ⱥ͸߶�
	menu_w=10;
	menu_h=10;
	// �������淵�ز˵���ť���
	helpRetButtonW=10;
	// �������淵�ز˵���ť������
	rButtonx=10;
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
	yellow_ply.x=0;
	yellow_ply.y=4;
	red_ply.x=4;
	red_ply.y=8;
	green_ply.x=8;
	green_ply.y=4;
	blue_ply.x=4;
	blue_ply.y=0;

	// �������ݳ�ʼ��
	memset(gameData,0,sizeof(gameData));
	// �����˳����Ҫע�⣬������ʱ������x��y��˳��ȥ������������˵
	gameData[2*yellow_ply.x][2*yellow_ply.y]=GD_YELLOW;
	gameData[2*red_ply.x][2*red_ply.y]=GD_RED;
	gameData[2*green_ply.x][2*green_ply.y]=GD_GREEN;
	gameData[2*blue_ply.x][2*blue_ply.y]=GD_BLUE;
	// ���ѡȡ��λ��
	pickup.x=-1;
	pickup.y=-1;
	// ǽ�ĵ�һ��ѡȡλ��
	wall_pick.x=-1;
	wall_pick.y=-1;
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
	menu.x=m_OpenGL->RCwidth-menu_dis/2-menu_w;
	menu.y=/*menu_dis/2 +*/ menu_h;

	// ��Ϸ��������
	player_info_h=m_OpenGL->RCheight/4;
	player_info_w=(m_OpenGL->RCwidth-m_OpenGL->RCheight)*3/8;
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
				iMenu=i;
				break;
			}
		}
		break;
	case GAME_SINGE:
	case GAME_MULTIP:
	case GAME_SENDBOX:
		// ʵʱ������λ��
		// ���ʱ��������λ�����̱߽���
		if (x<board_x+lace || x>board_x+m_OpenGL->RCheight-lace)
			break;
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
// ����ɿ�
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
		// ������ѡȡ��λ�ã���������ٴε��ʱ��λ��Ϊ˫ż��λ
		if (-1 != pickup.x && -1 != pickup.y && 0==arr.x%2 && 0==arr.y%2)
		{	// ������������뵽�������Ӵ���׶�
			char tmp=0;
			// ����
			tmp=gameData[arr.x][arr.y];
			gameData[arr.x][arr.y]=gameData[2*pickup.x][2*pickup.y];
			gameData[2*pickup.x][2*pickup.y]=tmp;
			// ��Ŀ��λ���ϣ�������ұ���
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
			// ���ھɵ�λ���ϣ�������ұ���
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
			// ��ѡȡλ�ø�ֵΪ��Чֵ
			pickup.x=-1;
			pickup.y=-1;
		}	// �����￪ʼ����ǽ��ص�
		else if (arr.x>-1 && arr.y>-1)
		{
			// �����һ��˫ż���꣬��ѡȡλ���Ƿǿհ�λ��(˵���˴�Ӧ����ĳһ�������λ��)
			if (arr.x%2==0 && arr.y%2==0 && GD_BLANK != gameData[arr.x][arr.y])
			{
				pickup.x = arr.x/2;
				pickup.y = arr.y/2;
			}
			// �����������һ��������һ����ż����˵���⴦��һ��ǽ���Դ��ڵ�λ��
			else if ((arr.x+arr.y)%2==1 && GD_BLANK == gameData[arr.x][arr.y])
			{	// ����ϴ�����Ԥѡǽ��λ��
				if (wall_pick.x>-1&&wall_pick.y>-1)
				{	// ����Ǻ�ǽ,�������ѡ�ĺ��ϴ�ѡ����ͬһ����
					if (wall_pick.x%2==0&&arr.y==wall_pick.y)
					{	// ������ѡ������һ��ѡ�����һ��λ��
						// �����������ӵ��м�λ���ǿ��õ�
						if(arr.x==wall_pick.x-2&&gameData[arr.x+1][arr.y]==GD_BLANK)
						{
							// ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
							wall_vec.push_back(arr);
							wall_vec.push_back(wall_pick);
							// ������Ϸ�㷨����,����ע�⣬��������ǽλ�õ��м����Ӵ�Ҳ��ֵ
							gameData[arr.x][arr.y]=GD_WALL;
							gameData[arr.x+1][arr.y]=GD_WALL;
							gameData[wall_pick.x][wall_pick.y]=GD_WALL;
							// Ԥѡǽ���
							wall_pick.x=-1;
							wall_pick.y=-1;
							break;
						}
						// ��������ѡ������һ���ұ�һ��λ��
						// �����������ӵ��м�λ���ǿ��õ�
						else if(arr.x==wall_pick.x+2&&gameData[arr.x-1][arr.y]==GD_BLANK)
						{
							// ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
							wall_vec.push_back(wall_pick);
							wall_vec.push_back(arr);
							// ������Ϸ�㷨����
							gameData[arr.x][arr.y]=GD_WALL;
							gameData[arr.x-1][arr.y]=GD_WALL;
							gameData[wall_pick.x][wall_pick.y]=GD_WALL;
							// Ԥѡǽ���
							wall_pick.x=-1;
							wall_pick.y=-1;
							break;
						}
					}
					// �������ǽ���������ѡ�ĺ��ϴ�ѡ����ͬһ����
					else if(wall_pick.y%2==0&&arr.x==wall_pick.x)
					{	// ������ѡ�������һ��ѡ�������һ��
						// �����������ӵ��м�λ���ǿ��õ�
						if (arr.y==wall_pick.y-2&&gameData[arr.x][arr.y+1]==GD_BLANK)
						{
							// ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
							wall_vec.push_back(arr);
							wall_vec.push_back(wall_pick);
							// ������Ϸ�㷨����
							gameData[arr.x][arr.y]=GD_WALL;
							gameData[arr.x][arr.y+1]=GD_WALL;
							gameData[wall_pick.x][wall_pick.y]=GD_WALL;
							// Ԥѡǽ���
							wall_pick.x=-1;
							wall_pick.y=-1;
							break;
						}
						// ������ѡ�������һ��ѡ�������һ��
						// �����������ӵ��м�λ���ǿ��õ�
						else if (arr.y==wall_pick.y+2&&gameData[arr.x][arr.y-1]==GD_BLANK)
						{
							// ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
							wall_vec.push_back(wall_pick);
							wall_vec.push_back(arr);
							// ������Ϸ�㷨����
							gameData[arr.x][arr.y]=GD_WALL;
							gameData[arr.x][arr.y-1]=GD_WALL;
							gameData[wall_pick.x][wall_pick.y]=GD_WALL;
							// Ԥѡǽ���
							wall_pick.x=-1;
							wall_pick.y=-1;
							break;
						}
					}
				}
				// �����������������ģ�����Ϊ��
				// ��ѡȡ��һ��Ԥѡǽ��λ��
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
				// ��������ż�������±�Ҳ��ż��
				if (dist%2==0)
				{
					// ע��������ܴ��ڵ�����ʧЧ������
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

	case VK_ESCAPE:
		//�ص��˵�
		iGameState=GAME_MENU;
		initView();
		break;

	default:
		break;
	}
}
// ������Ϸ�����еİ�ť
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
	char *menustr[]={"��    ��","��Ϸ����","ɳ��ģʽ","������Ϸ","������Ϸ"};
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
	sprintf(tmpstr,"��ESC�������˵�");
	myfont.Print2D(rButtonx+10,menu.y+5,tmpstr,FONT1,1,1,1);

	//ͼƬ
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
	// ���������Ϣָʾ��־����
	tRectangle(0,m_OpenGL->RCheight*3/4.0f,layer,(float)player_info_w,(float)player_info_h,1,1,0,0.7f);
	tRectangle(0,m_OpenGL->RCheight*2/4.0f,layer,(float)player_info_w,(float)player_info_h,1,0,0,0.7f);
	tRectangle(0,m_OpenGL->RCheight*1/4.0f,layer,(float)player_info_w,(float)player_info_h,0,1,0,0.7f);
	tRectangle(0,0,layer,(float)player_info_w,(float)player_info_h,0,0,1,1);

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
	// ��߻�ɫ���
	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f( (float)board_x, 0,  0.0f);
	glVertex3f( (float)board_x, (float)m_OpenGL->RCheight,  0.0f);
	glVertex3f( (float)board_x+lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glVertex3f( (float)board_x+lace, (float)lace,  0.0f);
	glEnd();
	// �ϱߺ�ɫ���
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f( (float)board_x, (float)m_OpenGL->RCheight, 0.0f);
	glVertex3f( (float)board_x+m_OpenGL->RCheight, (float)m_OpenGL->RCheight,  0.0f);
	glVertex3f( (float)board_x+m_OpenGL->RCheight-lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glVertex3f( (float)board_x+lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glEnd();
	// �ұ���ɫ���
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f((float) board_x+m_OpenGL->RCheight, (float)m_OpenGL->RCheight,  0.0f);
	glVertex3f((float) board_x+m_OpenGL->RCheight, 0,  0.0f);
	glVertex3f((float) board_x+m_OpenGL->RCheight-lace, (float)lace,  0.0f);
	glVertex3f((float) board_x+m_OpenGL->RCheight-lace, (float)m_OpenGL->RCheight-lace,  0.0f);
	glEnd();
	// �±���ɫ���
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

	//����
	char tmpstr[64]={0};
	sprintf(tmpstr,"ȷ    ��");
	myfont.Print2D(menu.x+10,menu.y+5,tmpstr,FONT1,1,1,1);
	// ��ťͼƬ
	texture_select(g_cactus[9]);
	if(1==iButton)
	{
		tPicButton((float)menu.x,(float)(menu.y),(float)menu_w,(float)menu_h,0.0f);
	}
	else
	{
		tPicButton((float)menu.x,(float)(menu.y),(float)menu_w,(float)menu_h,0.5f);
	}

	sprintf(tmpstr,"���ز˵�");
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
	// ��Ӧ��ɫ����ң��ƣ��죬�̣���
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
	if (-1 != pickup.x && -1 !=pickup.y)
	{
		// �����Ժ󻻸���ͼ
		tRectangle(board_x+lace+pickup.x*(roadw+wall_w),lace+pickup.y*(roadw+wall_w),0,(float)roadw,(float)roadw,0.1f,0.5f,1,0.6f);
	}
	// ���������ʱѡȡ��Ԥѡǽ����ǽλ��
	if (-1 != wall_pick.x && -1 != wall_pick.y)
	{
		// ѡ�е�ǽԤѡλ�ã�*�ߵ�����*��һ����һ����һż��
		if (wall_pick.x%2==0)
		{	// ��ǽ(TODO,ע�������ʱǽ�ĳ���4/9�����ֵ�Ǹ���ǽ����·��ı�Ϊ1:4��ǰ��)
			tRectangle(board_x+lace+wall_pick.x/2*(roadw+wall_w),lace+(int)(wall_pick.y/2)*(roadw+wall_w)+roadw,0,(float)wall_l*4/9,(float)wall_w,1.0f,1,0,0.6f);
		}
		else
		{	// ��ǽ
			tRectangle(board_x+lace+(int)(wall_pick.x/2)*(roadw+wall_w)+roadw,lace+wall_pick.y/2*(roadw+wall_w),0,(float)wall_w,(float)wall_l*4/9,1.0f,1,0,0.6f);
		}
	}
}
