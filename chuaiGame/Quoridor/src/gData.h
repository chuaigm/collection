#pragma once
#include <vector>

// 游戏棋盘数据的数组尺寸
// 目前包含棋子与墙，为17 X 17的二维数组
#define GDSIZE 17

// 游戏的状态
enum {
    GAME_PRE,
    GAME_MENU,
    GAME_SINGE,
    GAME_IN_CONFIG,
    GAME_NETWORK,
    GAME_NET_CONFIG,
    GAME_SENDBOX,
    GAME_HELP,
    GAME_WIN
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

// 定义玩家状态,注意，前三个顺序不能变动，
// 因为，为了单人游戏时，复用数字顺序，按钮顺序
enum {
    ID_HUMAN=0,
    ID_COMPUTER,
    ID_CLOSED,
    ID_NET_PLAYER
};
// 玩家棋子的定义
class player {
public:
    player():id(0),color(0),x(0),y(0),wall_num_left(0),next(NULL){};
    ~player(){};
    // 并复用此变量作为控制角色是否在场上，0:玩家，1:电脑，2:关闭，3:网络玩家
    // (由于在单人模式时，此值是由for循环判断鼠标位置实现的，所以，对应关系必须严格)
    int id;
    // 此玩家的颜色，复用GD_枚举数值，（此数值对应位置基本不变）
    int color;
    // 玩家的位置，是按照玩家可站的位置计算，0~8
    int x;
    int y;
    unsigned int wall_num_left;
    player* next;
};

// 棋盘上，抽象的整数位置，比如，棋子的位置，或者墙的占位
class pos2d{
public:
    int x;
    int y;
    bool operator == (const pos2d &a)
    {
        return (a.x==this->x && a.y==this->y);
    }
};
// 墙总数，待除数
const int wall_total_num=21;

// 当前游戏状态
extern volatile int iGameState;
// 游戏算法数据，其值如上述枚举
extern volatile char gameData[GDSIZE][GDSIZE];
//extern char gameData[GDSIZE][GDSIZE];

extern player g_player[4];

extern std::vector<pos2d> wall_vec;

extern player* ply_head;

//extern CQuoridor gm;
//
//extern CQuoridor* pgm;

#ifdef __DEBUG__
extern std::vector<pos2d> best_path;
#endif
