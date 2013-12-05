#ifndef __GAMEMAP
#define __GAMEMAP

#include "mdlobj.h"

typedef struct tagRECT_F
{
    float   left;
    float   top;
    float   right;
    float   bottom;
} RECT_F, *PRECT_F;

struct ROLE
{
	int id;	//ID
	//坐标
	int x;
	int y;
	int z;

	int w;
	int h;
	
	int iframe;
	int iCurrentAni;	

	//移动部分
	int iAngle;
	int iMoveDir;
	int iMoveSpeed;

	//人物属性
	int hp;//生命值
	int hpMax;

	int show;	//是否显示

	int r;//旋转角度
};

class GAMEMAP
{
public:
	GAMEMAP();
	~GAMEMAP();
	
	void init();
	//
	void initView();
	void initMap();
	int haveDataFile();
	//显示
	void show();
	void showpreani();
	void showmenu();
	
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
	void tPic(int x,int y,int w,int h);
	void tPicButton(int x,int y,int w,int h,float ytex);
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
	int xmouse;
	int ymouse;
	int xmouseOld;
	int ymouseOld;
	//当前选择的菜单项
	int iMenu;

	// look
	float	g_eye[3];		//
	float	g_look[3];		//
	float	rad_xz;	
	float	g_Angle;	
	float	g_elev;	
	
	//enemy
	struct ROLE MapEnemyArray[MAX_MAP_OBJECT];
	int iMapEnemyCursor;

	//md2
	anmobj* m_anmobj;

	//map人物活动区域
	RECT maprect[MAX_MAP_OBJECT];
	unsigned int  maprectTex[MAX_MAP_OBJECT];
	int iMapRectNum;
	
	//mdl
	mdlobj	m_mdlobj;

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