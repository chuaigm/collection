#pragma once

// ��Ϸ�������ݵ�����ߴ�
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

// ǽ������������
const int wall_total_num=21;

// ��ǰ��Ϸ״̬
extern volatile int iGameState;
// ��Ϸ�㷨���ݣ���ֵ������ö��
//extern volatile char gameData[GDSIZE][GDSIZE];
extern char gameData[GDSIZE][GDSIZE];

