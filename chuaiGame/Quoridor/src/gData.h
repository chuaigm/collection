#pragma once

// 游戏棋盘数据的数组尺寸
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

// 墙总数，待除数
const int wall_total_num=21;

// 当前游戏状态
extern volatile int iGameState;
// 游戏算法数据，其值如上述枚举
//extern volatile char gameData[GDSIZE][GDSIZE];
extern char gameData[GDSIZE][GDSIZE];

