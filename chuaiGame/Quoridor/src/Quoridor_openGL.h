#ifndef __CQuoridor
#define __CQuoridor
//=======================================
//
// ��Ϸ������ΪӪ
//       Quoridor
// ���ߣ�chuaiGM
// date: 2014-11-29
//
//=======================================
#include <vector>
#include <deque>
#include <algorithm>
#include "gData.h"
//#include "Quoridor_Network.h"

// �˵�����
#define MENU_NUM 5
#define INVALID_VAL -1
// ��Ϸ�˵��밴ť��״̬
// �����˳����ע������Ʋ˵�ʱ�Ķ�Ӧ��ϵ
// �����¼����ö��ֵ�����ں��棬��ҪӰ��ǰ������˵��ĸ�ֵ����
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
    // ��鲢���ز��ļ�
    int haveDataFile();
    // ��ʼ��������ͼ�ز�
    void init();
    // ��ʼ���ӽ�ģ�ͣ�����ֻ��ʼ��Ϊ2d
    void initView();
    
    // ��Ϸ����ͼ����
    void showMain();
    //ʵʱ���
    void check();
    // ��������������
    void lbuttonproc(int lparam);
    // ��������Ҽ�����
    void rbuttonproc(int lparam);
    // ���̰��������Ӧ
    void keyupproc(int keyparam);

    // ������������
    //void showpreani();
    // �������˵�
    void showmenu();
    // ��ʾ��������
    void show_Font_test();
    // ��괦����
    //void mouseproc(int lparam);

    //����ͼ
    void texture_select(UINT texture);
    //��ʾͼƬ
    //void tPic(float e);
    void tPicRectangle(float x,float y, float w, float h,float deep=-0.1f);
    void tPicButton(float x,float y,float w,float h,float ytex);
    void tRectangle(float x, float y, float z, float w, float h, float r, float g, float b, float a);

    // ��ʾ������Ϣ
    void showHelp();
    // ���ƶ�����ʽ�����
    void drawMouse();
    // ��������
    void drawChessBorad();
    // ���Ƹ�������
    void drawAccessory();
    // ������Һ�ǽ
    void drawPlayerWall();
    // ����ѡ����Ϸ����
    void drawInConfig();
    // ����������Ϸʱ��ѡ��������ʽ�Ļ���
    void drawNetworkOp();
    // ����ѡ�����ֲ�
    void drawPickMask();
    // ȷ�ϴ���
    void drawConfirm();
    // ����ʤ������
    void drawVictory();
    // ������Ϸ����
    void resetGameData();
    // ����������ʱ����Ҫ����Ϸ����,������Ϸ�Ĺ���(�����������ʱ����Ӧ��)
    void playerActionRule(bool network=false);
    // ���������ʱ��ɳ��ģʽʹ�õģ������ƶ��Ĺ���
    void freeSendBoxRule();
    // ������Ϸʱ��������λ�ú����ɿ��ƶ���λ��
    void playerMovablePos(pos2d selected);
    // �ж��¼����ǽλ���Ƿ�Ϸ�
    bool judgeWallLegal();
#ifdef __DEBUG__
    void drawTestOptimalPath();
#endif

    // ����������Ϣ�Ļص����� (��̬)
    //static void OnReceiveNetData(char* data, int length, DWORD userdata);
    // ����һ������ľ�ָ̬��
    //static CQuoridor* pThis;

    //data
    int iMenu;          // ��ǰѡ��Ĳ˵���
    int iButton;        // ��ťѡ��Ľ��
    int win_flag;       // ��λ��һ����ʤ����0û��ʤ����1����ɫ....(����GD_ϵ������)
    bool g_debug_flag;  // ��ʾ������Ϣ
    bool b_show_warning;// ������Ϸ��������Ϸʱ����ֹһ���Բ��������˳�

    //images
    unsigned int g_cactus[GAME_TEX_NUM];

    //------------------------------------
    // ���˵���صļ������
    // ��Щ��������initviewʱ������opengl�ӿ�����پ��帳ֵ
    pos2d menu;
    // �˵���ť�Ŀ�Ⱥ͸߶�
    int menu_w;
    int menu_h;
    // ���˵����¼��
    int menu_dis;

    //------------------------------------
    // ��Ϸ��������
    // ������·�Ŀ��
    float roadw;
    // ǽ�ĳ��ȺͿ��
    float wall_l;
    float wall_w;
    // ���ָʾ��Ŀ�Ⱥ͸߶�
    float player_info_w;
    float player_info_h;
    // ���̵���㶨λ����,�������½�
    // |-----------------|
    // |   |         |   |
    // |   |         |   |
    // |   |         |   |
    // |---*-------------|
    int board_x;

    // ��Ϸ���̻��߿��
    int lace;

    //------------------------------------
    // ʵʱ����Ļ�ȡ����������ϵ�����λ��
    // ע�⣬���ֵ������ֻ����road���㣬�ǰ���ǽ���ڵ�����
    pos2d arr;

    // ���ѡ����λ��,���λ�ö�Ӧ���㷨������±��һ��
    pos2d pickup;

    // ��ʽ��Ϸʱ����ҿ��ж�Ԥѡλ��
    std::vector<pos2d> preselect_pos;

    //------------------------------------
    // ��Ϸ��ʱ������ı���
    unsigned int tcounter;

    char n_loaclIP[16];         // ����IP
    int n_netWorkStatus;        // ��������ʱ��״̬��0:δѡ��1:��������2:�ͻ���
    // TODO ���ڶ��߳����⣬�������ҲӦ�÷ŵ�gData�����volatile����
    //char n_NameAll[4][16];      // Ŀǰ�������ӵ������

private:
    //------------------------------------
    // �������
    Quoridor_Network *n_net;
};

#endif
