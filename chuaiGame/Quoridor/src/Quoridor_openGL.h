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
#include <algorithm>
// Game state
enum {
	GAME_PRE,
	GAME_MENU,
	GAME_SINGE,
	GAME_IN_CONFIG,
	GAME_MULTIP,
	GAME_SENDBOX,
	GAME_HELP
};
// �˵�����
#define MENU_NUM 5
// Game Menu state
// �����˳����ע������Ʋ˵�ʱ�Ķ�Ӧ��ϵ
enum {
	MENU_QUIT,
	MENU_HELP,
	MENU_SENDBOX,
	MENU_MULTIP,
	MENU_SINGE
};
// ��������������ĺ���
enum {
	GD_BLANK,
	GD_YELLOW,
	GD_RED,
	GD_GREEN,
	GD_BLUE,
	GD_WALL
};

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
//struct walls{
//	pos2d w[2];
//};
// ��Ϸ�������ݵ�����ߴ�
const int sz=17;

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
	// ��괦������
	//void mouseproc(int lparam);
	
	void showmapBox(float *ppos,float *psize,float *ptex,int itex,int iHastop);
	void light0();

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
	void showChessBorad();
	// ������Ϸ�����еİ�ť
	void showInGameBotton();
	// ������Һ�ǽ
	void showPlayerWall();
	// ����ѡ�����ֲ�
	void drawPickMask();

	////////////////////////////////////////////////////////////
	//data
	int iGameState;		// ��ǰ��Ϸ״̬
	int iMenu;			// ��ǰѡ��Ĳ˵���
	int iButton;

	//images
	unsigned int g_cactus[GAME_TEX_NUM];

	//pre ani
	int param1;
	
	//���λ��
	//int xmouse;
	//int ymouse;
	//int xmouseOld;
	//int ymouseOld;

	// ���˵���صļ������
	// ��Щ��������initviewʱ������opengl�ӿ�����پ��帳ֵ
	pos2d menu;
	// �˵���ť�Ŀ��Ⱥ͸߶�
	int menu_w;
	int menu_h;
	// �������淵�ز˵���ť����
	int helpRetButtonW;
	// �������淵�ز˵���ť������
	int rButtonx;
	// �˵����¼��
	int menu_dis;

	// ��Ϸ��������
	// ������·�Ŀ���
	float roadw;
	// ǽ�ĳ��ȺͿ���
	float wall_l;
	float wall_w;
	// ���ָʾ��Ŀ��Ⱥ͸߶�
	int player_info_w;
	int player_info_h;
	// ���̵���㶨λ����,�������½�
	// |-----------------|
	// |   |         |   |
	// |   |         |   |
	// |   |         |   |
	// |---*-------------|
	int board_x;
	int board_y;
	// ��Ϸ���̻��߿���
	int lace;

	// ��Ϸ�㷨����
	// ��ֵ������ö��
	char gameData[sz][sz];
	// ʵʱ����Ļ�ȡ����������ϵ�����λ��
	// ע�⣬���ֵ������ֻ����road���㣬�ǰ���ǽ���ڵ�����
	pos2d arr;

	// ���λ�ã���ҵ������ǰ���road���λ���������ģ����Ϊ9-1
	pos2d yellow_ply;
	pos2d red_ply;
	pos2d green_ply;
	pos2d blue_ply;

	// ���ѡ�������λ��,���λ�ö�Ӧ���㷨������±��һ��
	// Ҳ����˵��ֻ�������λ�õ�����(ͬply)�����Ϊ9-1
	pos2d pickup;

	// ��ǵ�ǽ�ĵ�һ��ѡȡλ�ã����������㷨����ͬ�±�
	pos2d wall_pick;
	// ��Ҫ���Ƶ�ǽ�Ķ���,��ѹ�������ߵĿ����±ߵĿ�(�������)
	// ǽ�����ݽṹ���������λ�õ����꣬�ֱ��ǿ��Ե�ס����·���������꣬
	// ��������λ��֮�����һС���м����ӵ�λ�ã������жϽ������
	std::vector<pos2d> wall_vec;
};

#endif