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
	//显示
	void show();
	void showpreani();
	void showmenu();
	// cgm test
	void show_2D_test();
	void show_Font_test();
	
	void mouseproc(int lparam);
	void lbuttonproc(int lparam);
	void keyupproc(int keyparam);
	//实时检测
	void check();
	void checkRole();
	void checkFire();
	int RoleInMap(float x,float y,float z);
	//
	void DisplayScene();
	void showmap();
	void showmapBox(float *ppos,float *psize,float *ptex,int itex,int iHastop);
	void light0();
	//加载位图
	bool LoadT8(char *filename, GLuint &texture);
	//绑定贴图
	void texture(UINT textur);			
	void texture0(UINT textur);			
	//显示图片
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
	int iGameState;//游戏状态
	int iMatch;//第几关

	//images
	unsigned int g_cactus[GAME_TEX_NUM];
	unsigned int TexBox[GAME_TEX_NUM];
	GLUquadricObj *g_text; 

	//pre ani
	int param1;
	
	//鼠标位置
	//int xmouse;
	//int ymouse;
	//int xmouseOld;
	//int ymouseOld;
	//当前选择的菜单项
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

	//map人物活动区域
	RECT maprect[MAX_MAP_OBJECT];
	unsigned int  maprectTex[MAX_MAP_OBJECT];
	int iMapRectNum;
	
	//mdl
	//mdlobj	m_mdlobj;

	//FIRE
	int IsFire;//是否正在发射子弹
	float rocketX;
	float rocketY;
	float rocketZ;  
	//子弹撞击效果
	int iShowBombFrame;

	//
	int iEnemyNum;	//敌人数量
	int iHp;	//玩家生命值
	//攻击提示
	int iAttackLife;
	int iAttackMaxLife;
	//地图提示
	int iShowWorldFrame;
	//帮助菜单
	int iShowHelp;
};

#endif