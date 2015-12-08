#include "stdafx.h"
#include "gData.h"
//#include "Quoridor_openGL.h"

// 此文件为全局游戏数据结构的实际表达定义之处

// 当前游戏状态
volatile int iGameState;
// 游戏算法数据，其值如上述枚举
volatile char gameData[GDSIZE][GDSIZE];
// 目前所有连接的玩家名
volatile char n_NameAll[4][16];


// 玩家信息，玩家的坐标是按照road格的位置坐标计算的，最大为9-1
// 序号默认包含颜色信息，0:黄色，1:红色，2:绿色，3:蓝色
player g_player[4];

// 需要绘制的墙的队列,先压入的是左边的块与下边的块(方便绘制)
// 墙的数据结构里，包含三个位置的坐标，分别是可以挡住两块路的数组坐标，
// 和这两个位置之间的那一小块中间连接的位置，用于判断交叉情况
std::vector<pos2d> wall_vec;

// 正常游戏开始时，玩家顺序单链表,当前应该执行的玩家指针
player* ply_head;

//// 目前游戏框架中，主要代码(全局)
//CQuoridor gm;
//// 全局指针
//CQuoridor* pgm=&gm;
////CQuoridor* CQuoridor::pThis=&gm;

#ifdef __DEBUG__
std::vector<pos2d> best_path;
#endif
