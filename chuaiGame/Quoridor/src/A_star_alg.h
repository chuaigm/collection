#pragma once
#include "gData.h"

typedef struct AStarNode
{
    int s_x;    // 坐标(最终输出路径需要)
    int s_y;
    int s_g;    // 起点到此点的距离( 由g和h可以得到f，此处f省略，f=g+h )
    int s_h;    // 启发函数预测的此点到终点的距离
    int s_style;// 结点类型：起始点，终点，障碍物
    int s_is_in_closetable;         // 是否在close表中
    int s_is_in_opentable;          // 是否在open表中
    struct AStarNode * s_parent;    // 父节点
}AStarNode, *pAStarNode;

enum {
    BLANKNODE,
    STARTNODE,
    ENDNODE,
    BARRIER,
};

class AStarAlg
{
public:
    AStarAlg(void);
    ~AStarAlg(void);

    void Init(volatile char gameData[][GDSIZE],int start_x, int start_y,int end_x,int end_y);
    // 使用A星算法计算路径，如果返回0，则找到路径，并将路径保存在结果数组中
    // 如果非0，则找路径失败
    // 使用前，需调用Init来设置迷宫数据与起点和终点
    int A_start_calculate();
    void get_path();

private:
    void swap_node( int idx1, int idx2 );
    void adjust_heap( int nIndex );
    void insert_to_opentable( int x, int y, pAStarNode curr_node, pAStarNode end_node, int w );
    void get_neighbors( pAStarNode curr_node, pAStarNode end_node );

    AStarNode  map_maze[GDSIZE][GDSIZE];      // 结点数组
    pAStarNode open_table[GDSIZE*GDSIZE];     // open表
    pAStarNode close_table[GDSIZE*GDSIZE];    // close表
    pAStarNode path_stack[GDSIZE*GDSIZE];     // 保存路径的栈
    int   open_node_count;          // open表中节点数量
    int   close_node_count;         // close表中结点数量
    int   top;                      // 栈顶

    AStarNode *start_node;          // 起始点
    AStarNode *end_node;            // 结束点
};
