#include "stdafx.h"
#include "gData.h"
//#include "Quoridor_openGL.h"

// ���ļ�Ϊȫ����Ϸ���ݽṹ��ʵ�ʱ�ﶨ��֮��

// ��ǰ��Ϸ״̬
volatile int iGameState;
// ��Ϸ�㷨���ݣ���ֵ������ö��
volatile char gameData[GDSIZE][GDSIZE];
// Ŀǰ�������ӵ������
volatile char n_NameAll[4][16];


// �����Ϣ����ҵ������ǰ���road���λ���������ģ����Ϊ9-1
// ���Ĭ�ϰ�����ɫ��Ϣ��0:��ɫ��1:��ɫ��2:��ɫ��3:��ɫ
player g_player[4];

// ��Ҫ���Ƶ�ǽ�Ķ���,��ѹ�������ߵĿ����±ߵĿ�(�������)
// ǽ�����ݽṹ���������λ�õ����꣬�ֱ��ǿ��Ե�ס����·���������꣬
// ��������λ��֮�����һС���м����ӵ�λ�ã������жϽ������
std::vector<pos2d> wall_vec;

// ������Ϸ��ʼʱ�����˳������,��ǰӦ��ִ�е����ָ��
player* ply_head;

//// Ŀǰ��Ϸ����У���Ҫ����(ȫ��)
//CQuoridor gm;
//// ȫ��ָ��
//CQuoridor* pgm=&gm;
////CQuoridor* CQuoridor::pThis=&gm;

#ifdef __DEBUG__
std::vector<pos2d> best_path;
#endif
