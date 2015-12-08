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
#include <deque>
#include <algorithm>
#include "gData.h"
//#include "Quoridor_Network.h"

// 菜单数量
#define MENU_NUM 5
#define INVALID_VAL -1
// 游戏菜单与按钮的状态
// 这里的顺序请注意与绘制菜单时的对应关系
// 所以新加入的枚举值，加在后面，不要影响前面给主菜单的赋值部分
enum {
    MENU_QUIT=0,
    MENU_HELP,
    MENU_SENDBOX,
    MENU_NETWORK,
    MENU_SINGE,
    BUTTON_RETURN,
    BUTTON_INIT_OR_CONFIRM,
    BUTTON_MUSIC,
    BUTTON_SERVER,
    BUTTON_CLIENT,
    BUTTON_SERVER_START,
    BUTTON_SERVER_TEST,
    BUTTON_CLIENT_TEST
};

struct point2d{
    float x;
    float y;
};

class Quoridor_Network;
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

    // 开场动画绘制
    //void showpreani();
    // 绘制主菜单
    void showmenu();
    // 显示测试数据
    void show_Font_test();
    // 鼠标处理函数
    //void mouseproc(int lparam);

    //绑定贴图
    void texture_select(UINT texture);
    //显示图片
    //void tPic(float e);
    void tPicRectangle(float x,float y, float w, float h,float deep=-0.1f);
    void tPicButton(float x,float y,float w,float h,float ytex);
    void tRectangle(float x, float y, float z, float w, float h, float r, float g, float b, float a);

    // 显示帮助信息
    void showHelp();
    // 绘制独特样式的鼠标
    void drawMouse();
    // 绘制棋盘
    void drawChessBorad();
    // 绘制附属画面
    void drawAccessory();
    // 绘制玩家和墙
    void drawPlayerWall();
    // 绘制选择游戏配置
    void drawInConfig();
    // 绘制联机游戏时的选择联机方式的画面
    void drawNetworkOp();
    // 绘制选定遮罩层
    void drawPickMask();
    // 确认窗口
    void drawConfirm();
    // 绘制胜利窗口
    void drawVictory();
    // 重设游戏数据
    void resetGameData();
    // 鼠标左键单击时，需要的游戏规则,正常游戏的规则(放在左键单击时的响应中)
    void playerActionRule(bool network=false);
    // 鼠标左键点击时，沙盒模式使用的，自由移动的规则
    void freeSendBoxRule();
    // 正常游戏时，点击玩家位置后，生成可移动的位置
    void playerMovablePos(pos2d selected);
    // 判断新加入的墙位置是否合法
    bool judgeWallLegal();
#ifdef __DEBUG__
    void drawTestOptimalPath();
#endif

    // 接收网络消息的回调函数 (静态)
    //static void OnReceiveNetData(char* data, int length, DWORD userdata);
    // 定义一个自身的静态指针
    //static CQuoridor* pThis;

    //data
    int iMenu;          // 当前选择的菜单项
    int iButton;        // 按钮选择的结果
    int win_flag;       // 哪位玩家获得了胜利，0没人胜利，1，黄色....(复用GD_系列数据)
    bool g_debug_flag;  // 显示调试信息
    bool b_show_warning;// 单机游戏和网络游戏时，防止一次性操作导致退出

    //images
    unsigned int g_cactus[GAME_TEX_NUM];

    //------------------------------------
    // 主菜单相关的计算参数
    // 这些参数，在initview时，根据opengl视口情况再具体赋值
    pos2d menu;
    // 菜单按钮的宽度和高度
    int menu_w;
    int menu_h;
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
    float player_info_w;
    float player_info_h;
    // 棋盘的起点定位坐标,棋盘左下角
    // |-----------------|
    // |   |         |   |
    // |   |         |   |
    // |   |         |   |
    // |---*-------------|
    int board_x;

    // 游戏棋盘花边宽度
    int lace;

    //------------------------------------
    // 实时计算的获取鼠标在棋盘上的整数位置
    // 注意，这个值并不是只按照road计算，是包含墙在内的坐标
    pos2d arr;

    // 鼠标选定的位置,这个位置对应于算法数组的下标的一半
    pos2d pickup;

    // 正式游戏时，玩家可行动预选位置
    std::vector<pos2d> preselect_pos;

    //------------------------------------
    // 游戏随时间计数的变量
    unsigned int tcounter;

    char n_loaclIP[16];         // 本机IP
    int n_netWorkStatus;        // 网络联机时的状态，0:未选择，1:服务器，2:客户端
    // TODO 由于多线程问题，这个变量也应该放到gData里，赋予volatile属性
    //char n_NameAll[4][16];      // 目前所有连接的玩家名

private:
    //------------------------------------
    // 网络相关
    Quoridor_Network *n_net;
};

#endif
