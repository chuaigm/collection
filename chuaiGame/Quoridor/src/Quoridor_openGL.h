#ifndef __CQuoridor
#define __CQuoridor
//=======================================
//
// 游戏：步步为营
//       Quoridor
// 作者：chuaiGM
// date: 2014-11-29
//
//=======================================
// Game state
enum {
	GAME_PRE,
	GAME_MENU,
	GAME_SINGE,
	GAME_IN_CONFIG,
	GAME_MULTIP,
	GAME_SENDBOX,
	GAME_HELP
};
// 菜单数量
#define MENU_NUM 5
// Game Menu state
// 这里的顺序请注意与绘制菜单时的对应关系
enum {
	MENU_QUIT,
	MENU_HELP,
	MENU_SENDBOX,
	MENU_MULTIP,
	MENU_SINGE
};

class CQuoridor
{
public:
	CQuoridor();
	~CQuoridor();
	// 检查并打开素材文件
	int haveDataFile();
	// 初始化加载贴图素材
	void init();
	// 初始化视角模型，这里只初始化为2d
	void initView();
	
	// 游戏主绘图函数
	void showMain();
	//实时检测
	void check();
	// 处理鼠标左键按下
	void lbuttonproc(int lparam);
	// 键盘按键检测响应
	void keyupproc(int keyparam);


	void showpreani();
	// 绘制主菜单
	void showmenu();
	// cgm test
	void show_2D_test();
	void show_Font_test();
	
	void mouseproc(int lparam);
	
	void showmapBox(float *ppos,float *psize,float *ptex,int itex,int iHastop);
	void light0();

	//加载位图
	bool LoadT8(char *filename, GLuint &texture);
	//绑定贴图
	void texture_select(UINT textur);			
	//显示图片
	void tPic(float e);
	void tPicRectangle(float x,float y, float w, float h,float deep=-0.1f);
	void tPicButton(float x,float y,float w,float h,float ytex);
	void tRectangle(float x, float y, float z, float w, float h, float r, float g, float b, float a);

	// 显示帮助信息
	void showHelp();

	void drawMouse();
	// 绘制棋盘
	void showChessBorad();
	// 绘制游戏过程中的按钮
	void showInGameBotton();

	////////////////////////////////////////////////////////////
	//data
	int iGameState;		// 当前游戏状态
	int iMenu;			// 当前选择的菜单项
	int iButton;
	int iShowHelp;

	//images
	unsigned int g_cactus[GAME_TEX_NUM];
	unsigned int TexBox[GAME_TEX_NUM];

	//pre ani
	int param1;
	
	//鼠标位置
	//int xmouse;
	//int ymouse;
	//int xmouseOld;
	//int ymouseOld;

	// 主菜单相关的计算参数
	// 这些参数，在initview时，根据opengl视口情况再具体赋值
	int x_menu;
	int y_menu;
	// 菜单按钮的宽度和高度
	int menu_w;
	int menu_h;
	// 帮助界面返回菜单按钮宽度
	int helpRetButtonW;
	// 帮助界面返回菜单按钮横坐标
	int rButtonx;
	// 菜单上下间距
	int menu_dis;

	// 游戏棋盘数据
	// 正方形路的宽度
	float roadw;
	// 墙的长度和宽度
	float wall_l;
	float wall_w;
	// 玩家指示格的宽度和高度
	int player_info_w;
	int player_info_h;
	// 棋盘的起点定位坐标,期盼左下角
	int board_x;
	int board_y;
	// 游戏棋盘花边宽度
	int lace;

	// 游戏算法数据
	char gameData[17][17];
	int arr_x;
	int arr_y;
};

#endif