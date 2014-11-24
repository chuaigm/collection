#ifndef __gameMain
#define __gameMain

class gameMain
{
public:
	gameMain();
	~gameMain();
	
	void init();
	//
	void initView();
	void initMap();
	int haveDataFile();
	//��ʾ
	void show();
	void showpreani();
	void showmenu();
	// cgm test
	void show_2D_test();
	void show_Font_test();
	
	void mouseproc(int lparam);
	void lbuttonproc(int lparam);
	void keyupproc(int keyparam);
	//ʵʱ���
	void check();
	void checkRole();
	void checkFire();
	int RoleInMap(float x,float y,float z);
	//
	void DisplayScene();
	void showmap();
	void showmapBox(float *ppos,float *psize,float *ptex,int itex,int iHastop);
	void light0();
	//����λͼ
	bool LoadT8(char *filename, GLuint &texture);
	//����ͼ
	void texture(UINT textur);			
	void texture0(UINT textur);			
	//��ʾͼƬ
	void tPic(float e);
	void tPic(float x,float y,float w,float h);
	void tPicButton(float x,float y,float w,float h,float ytex);
	void tSquare(float x, float y, float z, float a);
	//fire
	void showFire();
	//
	void showTarget();
	//hittest
	int hittest();
	//
	void showInfo();
	void showEnemyHp();
	void drawMouse();
	//
	int LoadMap();

	////////////////////////////////////////////////////////////
	//data
	int iGameState;//��Ϸ״̬
	int iMatch;//�ڼ���

	//images
	unsigned int g_cactus[GAME_TEX_NUM];
	unsigned int TexBox[GAME_TEX_NUM];
	GLUquadricObj *g_text; 

	//pre ani
	int param1;
	
	//���λ��
	//int xmouse;
	//int ymouse;
	//int xmouseOld;
	//int ymouseOld;
	//��ǰѡ��Ĳ˵���
	int iMenu;

	int iButton;

	bool b_font_test;
	bool b_func_test;

	// look
	//float	g_eye[3];		//
	//float	g_look[3];		//
	//float	rad_xz;	
	//float	g_Angle;	
	//float	g_elev;	
	
	//enemy
	/*struct ROLE MapEnemyArray[MAX_MAP_OBJECT];
	int iMapEnemyCursor;*/

	//map��������
	RECT maprect[MAX_MAP_OBJECT];
	unsigned int  maprectTex[MAX_MAP_OBJECT];
	int iMapRectNum;
	
	//mdl
	//mdlobj	m_mdlobj;

	//FIRE
	int IsFire;//�Ƿ����ڷ����ӵ�
	float rocketX;
	float rocketY;
	float rocketZ;  
	//�ӵ�ײ��Ч��
	int iShowBombFrame;

	//
	int iEnemyNum;	//��������
	int iHp;	//�������ֵ
	//������ʾ
	int iAttackLife;
	int iAttackMaxLife;
	//��ͼ��ʾ
	int iShowWorldFrame;
	//�����˵�
	int iShowHelp;
};

#endif