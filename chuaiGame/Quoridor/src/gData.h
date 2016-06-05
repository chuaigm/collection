#pragma once
#include <vector>

// ��Ϸ�������ݵ�����ߴ�
// Ŀǰ����������ǽ��Ϊ17 X 17�Ķ�ά����
#define GDSIZE 17

// ��Ϸ��״̬
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
// �������������ĺ���
enum {
    GD_BLANK,
    GD_YELLOW,
    GD_RED,
    GD_GREEN,
    GD_BLUE,
    GD_WALL
};

// �������״̬,ע�⣬ǰ����˳���ܱ䶯��
// ��Ϊ��Ϊ�˵�����Ϸʱ����������˳�򣬰�ť˳��
enum {
    ID_HUMAN=0,
    ID_COMPUTER,
    ID_CLOSED,
    ID_NET_PLAYER
};
// ������ӵĶ���
class player {
public:
    player():id(0),color(0),x(0),y(0),wall_num_left(0),next(NULL){};
    ~player(){};
    // �����ô˱�����Ϊ���ƽ�ɫ�Ƿ��ڳ��ϣ�0:��ң�1:���ԣ�2:�رգ�3:�������
    // (�����ڵ���ģʽʱ����ֵ����forѭ���ж����λ��ʵ�ֵģ����ԣ���Ӧ��ϵ�����ϸ�)
    int id;
    // ����ҵ���ɫ������GD_ö����ֵ��������ֵ��Ӧλ�û������䣩
    int color;
    // ��ҵ�λ�ã��ǰ�����ҿ�վ��λ�ü��㣬0~8
    int x;
    int y;
    unsigned int wall_num_left;
    player* next;
};

// �����ϣ����������λ�ã����磬���ӵ�λ�ã�����ǽ��ռλ
class pos2d{
public:
    int x;
    int y;
    bool operator == (const pos2d &a)
    {
        return (a.x==this->x && a.y==this->y);
    }
};
// ǽ������������
const int wall_total_num=21;

// �������±�����volatile��ʹ�ã�������ʹ������ͨ�����
// ���ڶ��̷߳��ʱ��������⣬Ϊ�˷�ֹ�������Ż�������Ӱ�죬
// ���¼����ڶ���߳���ʹ�õı���������volatile���ԣ�
// Ӧ�û������б�Ľ�����������ǣ�Ŀǰ�һ���֪����ô������
// ��ǰ��Ϸ״̬
extern volatile int iGameState;
// ��Ϸ�㷨���ݣ���ֵ������ö��
extern volatile char gameData[GDSIZE][GDSIZE];
// Ŀǰ�������ӵ������(��ʱ������?)
// ������ǣ������ʹ��cosnt_castȥת���Ļ�������û������̵߳�����
// ���ԣ�ʹ�õĵط�����ʱ������ָ��ת��
extern volatile char n_NameAll[4][16];

extern player g_player[4];

extern std::vector<pos2d> wall_vec;

extern player* ply_head;

//extern CQuoridor gm;
//
//extern CQuoridor* pgm;

#ifdef __DEBUG__
extern std::vector<pos2d> best_path;
#endif
