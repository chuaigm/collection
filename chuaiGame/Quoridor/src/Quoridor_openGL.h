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
// Game state
enum {
    GAME_PRE,
    GAME_MENU,
    GAME_SINGE,
    GAME_IN_CONFIG,
    GAME_MULTIP,
    GAME_SENDBOX,
    GAME_HELP,
    GAME_WIN
};
// �˵�����
#define MENU_NUM 5
#define INVALID_VAL -1
// Game Menu state
// �����˳����ע������Ʋ˵�ʱ�Ķ�Ӧ��ϵ
// �����¼����ö��ֵ�����ں��棬��ҪӰ��ǰ������˵��ĸ�ֵ����
enum {
    MENU_QUIT,
    MENU_HELP,
    MENU_SENDBOX,
    MENU_MULTIP,
    MENU_SINGE,
    BUTTON_RETURN,
    BUTTON_INIT_OR_CONFIRM,
    BUTTON_MUSIC
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
//// �������״̬
//enum {
//	HUMAN,
//	COMPUTER,
//	CLOSE
//};

struct pos2d{
    int x;
    int y;
    bool operator == (const pos2d &a)
    {
        return (a.x==this->x && a.y==this->y);
    }
};
struct point2d{
    float x;
    float y;
};
class player {
public:
    player():id(0),color(0),x(0),y(0),wall_num_left(0),next(NULL){};
    ~player(){};
    // �����ô˱�����Ϊ���ƽ�ɫ�Ƿ��ڳ��ϣ�0:��ң�1:���ԣ�2:�ر�
    // (����ö������Ϊ��ֵʱ���ϸ��0,1,2������ö����������ֵ)
    int id;
    // ����ҵ���ɫ������GD_ö����ֵ
    int color;
    // ��ҵ�λ�ã��ǰ�����ҿ�վ��λ�ü��㣬0~8
    int x;
    int y;
    unsigned int wall_num_left;
    player* next;
};
//struct walls{
//	pos2d w[2];
//};
// ��Ϸ�������ݵ�����ߴ�
const int sz=17;
const int wall_total_num=21;

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

    void showpreani();
    // �������˵�
    void showmenu();
    // ��ʾ��������
    void show_Font_test();
    // ��괦����
    //void mouseproc(int lparam);
    
    //void light0();

    //����λͼ
    bool LoadT8(char *filename, GLuint &texture);
    //����ͼ
    void texture_select(UINT textur);			
    //��ʾͼƬ
    void tPic(float e);
    void tPicRectangle(float x,float y, float w, float h,float deep=-0.1f);
    void tPicButton(float x,float y,float w,float h,float ytex);
    void tRectangle(float x, float y, float z, float w, float h, float r, float g, float b, float a);

    // ��ʾ������Ϣ
    void showHelp();

    void drawMouse();
    // ��������
    void drawChessBorad();
    // ���Ƹ�������
    void drawAccessory();
    // ������Һ�ǽ
    void drawPlayerWall();
    // ����ѡ����Ϸ����
    void drawInConfig();
    // ����ѡ�����ֲ�
    void drawPickMask();
    // ȷ�ϴ���
    void drawConfirm();
    // ����ʤ������
    void drawVictory();
    // ������Ϸ����
    void resetGameData();
    // ����������ʱ����Ҫ����Ϸ����,������Ϸ�Ĺ���(�����������ʱ����Ӧ��)
    void playerActionRule();
    // ���������ʱ��ɳ��ģʽʹ�õģ������ƶ��Ĺ���
    void freeRuleSendBox();
    // ������Ϸʱ��������λ�ú����ɿ��ƶ���λ��
    void playerMovablePos(pos2d selected);
    // �ж��¼����ǽλ���Ƿ�Ϸ�
    bool judgeWallLegal();

    // ���Կ����ж�����
    void computer_AI();

    // �������������
    int random(double start, double end)
    {
        return (int)(start+(end-start)*rand()/(RAND_MAX + 1));
    };

    ////////////////////////////////////////////////////////////
    //data
    int iGameState;     // ��ǰ��Ϸ״̬
    int iMenu;          // ��ǰѡ��Ĳ˵���
    int iButton;        // ��ťѡ��Ľ��
    int win_flag;       // ��λ��һ����ʤ����0û��ʤ����1����ɫ....(����GD_ϵ������)
    bool g_debug_flag;  // ��ʾ������Ϣ
    
    //images
    unsigned int g_cactus[GAME_TEX_NUM];

    //pre ani
    //int param1;
    
    //���λ��
    //int xmouse;
    //int ymouse;
    //int xmouseOld;
    //int ymouseOld;

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
    int board_y;
    // ��Ϸ���̻��߿��
    int lace;

    //------------------------------------
    // ��Ϸ�㷨����
    // ��ֵ������ö��
    char gameData[sz][sz];
    // ʵʱ����Ļ�ȡ����������ϵ�����λ��
    // ע�⣬���ֵ������ֻ����road���㣬�ǰ���ǽ���ڵ�����
    pos2d arr;
    // �����Ϣ����ҵ������ǰ���road���λ���������ģ����Ϊ9-1
    // ���Ĭ�ϰ�����ɫ��Ϣ��0:��ɫ��1:��ɫ��2:��ɫ��3:��ɫ
    player plyer[4];
    // ���ѡ����λ��,���λ�ö�Ӧ���㷨������±��һ��
    pos2d pickup;
    // ��Ҫ���Ƶ�ǽ�Ķ���,��ѹ�������ߵĿ����±ߵĿ�(�������)
    // ǽ�����ݽṹ���������λ�õ����꣬�ֱ��ǿ��Ե�ס����·���������꣬
    // ��������λ��֮�����һС���м����ӵ�λ�ã������жϽ������
    std::vector<pos2d> wall_vec;

    // ������Ϸ��ʼʱ�����˳������,��ǰӦ��ִ�е����ָ��
    player* ply_head;
    // ��ʽ��Ϸʱ����ҿ��ж�Ԥѡλ��
    std::vector<pos2d> preselect_pos;

    //------------------------------------
    // ��Ϸ��ʱ������ı���
    unsigned int tcounter;

};

#endif