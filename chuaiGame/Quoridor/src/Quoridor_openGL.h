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
#include <vector>
#include <algorithm>
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
#define INVALID_VAL -1
// Game Menu state
// 这里的顺序请注意与绘制菜单时的对应关系
// 所以新加入的枚举值，加在后面，不要影响前面给主菜单的赋值部分
enum {
	MENU_QUIT,
	MENU_HELP,
	MENU_SENDBOX,
	MENU_MULTIP,
	MENU_SINGE,
	BUTTON_RETURN,
	BUTTON_INIT
};
// 棋盘数组里代表的含义
enum {
	GD_BLANK,
	GD_YELLOW,
	GD_RED,
	GD_GREEN,
	GD_BLUE,
	GD_WALL
};

struct pos2d{
	int x;
	int y;
	bool operator == (const pos2d &a)
	{
		return (a.x==this->x && a.y==this->y);
	}
};
struct point2d{
	float x;
	float y;
};
class player {
public:
	player():id(0),x(0),y(0),wall_num_left(0),next(NULL){};
	~player(){};
	// 玩家的数字编号，复用GD_系列数据，能反应颜色
	int id;
	// 玩家的位置，是按照玩家可站的位置计算，0~8
	int x;
	int y;
	unsigned int wall_num_left;
	player* next;
};
//struct walls{
//	pos2d w[2];
//};
// 游戏棋盘数据的数组尺寸
const int sz=17;

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
	// 处理鼠标右键按下
	void rbuttonproc(int lparam);
	// 键盘按键检测响应
	void keyupproc(int keyparam);

	void showpreani();
	// 绘制主菜单
	void showmenu();
	// 显示测试数据
	void show_Font_test();
	// 鼠标处理函数
	//void mouseproc(int lparam);
	
	//void light0();

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
	void drawChessBorad();
	// 绘制附属画面
	void drawAccessory();
	// 绘制玩家和墙
	void drawPlayerWall();
	// 绘制选定遮罩层
	void drawPickMask();
	// 确认窗口
	void drawConfirm();
	// 重设游戏数据
	void resetGameData();

	////////////////////////////////////////////////////////////
	//data
	int iGameState;		// 当前游戏状态
	int iMenu;			// 当前选择的菜单项
	int iButton;		// 按钮选择的结果
	bool g_debug_flag;	// 显示调试信息
	
	//images
	unsigned int g_cactus[GAME_TEX_NUM];

	//pre ani
	int param1;
	
	//鼠标位置
	//int xmouse;
	//int ymouse;
	//int xmouseOld;
	//int ymouseOld;

	//------------------------------------
	// 主菜单相关的计算参数
	// 这些参数，在initview时，根据opengl视口情况再具体赋值
	pos2d menu;
	// 菜单按钮的宽度和高度
	int menu_w;
	int menu_h;
	// 帮助界面返回菜单按钮宽度
	int helpRetButtonW;
	// 帮助界面返回菜单按钮横坐标
	int rButtonx;
	// 主菜单上下间距
	int menu_dis;

	//------------------------------------
	// 游戏棋盘数据
	// 正方形路的宽度
	float roadw;
	// 墙的长度和宽度
	float wall_l;
	float wall_w;
	// 玩家指示格的宽度和高度
	int player_info_w;
	int player_info_h;
	// 棋盘的起点定位坐标,棋盘左下角
	// |-----------------|
	// |   |         |   |
	// |   |         |   |
	// |   |         |   |
	// |---*-------------|
	int board_x;
	int board_y;
	// 游戏棋盘花边宽度
	int lace;

	//------------------------------------
	// 游戏算法数据
	// 其值如上述枚举
	char gameData[sz][sz];
	// 实时计算的获取鼠标在棋盘上的整数位置
	// 注意，这个值并不是只按照road计算，是包含墙在内的坐标
	pos2d arr;
	// 玩家位置，玩家的坐标是按照road格的位置坐标计算的，最大为9-1
	player yellow_ply;
	player red_ply;
	player green_ply;
	player blue_ply;
	// 鼠标选定的玩家位置,这个位置对应于算法数组的下标的一半
	// 也就是说，只计算玩家位置的坐标(同ply)，最大为9-1
	pos2d pickup;
	// 标记的墙的第一处选取位置，此坐标与算法数组同下标
	pos2d wall_pick;
	// 需要绘制的墙的队列,先压入的是左边的块与下边的块(方便绘制)
	// 墙的数据结构里，包含三个位置的坐标，分别是可以挡住两块路的数组坐标，
	// 和这两个位置之间的那一小块中间连接的位置，用于判断交叉情况
	std::vector<pos2d> wall_vec;

	//------------------------------------
};

#endif