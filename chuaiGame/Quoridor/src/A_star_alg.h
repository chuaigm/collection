#pragma once
#include "gData.h"

typedef struct AStarNode
{
    int s_x;    // ����(�������·����Ҫ)
    int s_y;
    int s_g;    // ��㵽�˵�ľ���( ��g��h���Եõ�f���˴�fʡ�ԣ�f=g+h )
    int s_h;    // ��������Ԥ��Ĵ˵㵽�յ�ľ���
    int s_style;// ������ͣ���ʼ�㣬�յ㣬�ϰ���
    int s_is_in_closetable;         // �Ƿ���close����
    int s_is_in_opentable;          // �Ƿ���open����
    struct AStarNode * s_parent;    // ���ڵ�
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
    // ʹ��A���㷨����·�����������0�����ҵ�·��������·�������ڽ��������
    // �����0������·��ʧ��
    // ʹ��ǰ�������Init�������Թ������������յ�
    int A_start_calculate();
    void get_path();

private:
    void swap_node( int idx1, int idx2 );
    void adjust_heap( int nIndex );
    void insert_to_opentable( int x, int y, pAStarNode curr_node, pAStarNode end_node, int w );
    void get_neighbors( pAStarNode curr_node, pAStarNode end_node );

    AStarNode  map_maze[GDSIZE][GDSIZE];      // �������
    pAStarNode open_table[GDSIZE*GDSIZE];     // open��
    pAStarNode close_table[GDSIZE*GDSIZE];    // close��
    pAStarNode path_stack[GDSIZE*GDSIZE];     // ����·����ջ
    int   open_node_count;          // open���нڵ�����
    int   close_node_count;         // close���н������
    int   top;                      // ջ��

    AStarNode *start_node;          // ��ʼ��
    AStarNode *end_node;            // ������
};
