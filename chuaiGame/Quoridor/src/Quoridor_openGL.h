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
	// ���̰��������Ӧ
	void keyupproc(int keyparam);


	void showpreani();
	// �������˵�
	void showmenu();
	// cgm test
	void show_2D_test();
	void show_Font_test();
	
	void mouseproc(int lparam);
	
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

	////////////////////////////////////////////////////////////
	//data
	int iGameState;		// ��ǰ��Ϸ״̬
	int iMenu;			// ��ǰѡ��Ĳ˵���
	int iButton;
	int iShowHelp;

	//images
	unsigned int g_cactus[GAME_TEX_NUM];
	unsigned int TexBox[GAME_TEX_NUM];

	//pre ani
	int param1;
	
	//���λ��
	//int xmouse;
	//int ymouse;
	//int xmouseOld;
	//int ymouseOld;

	// ���˵���صļ������
	// ��Щ��������initviewʱ������opengl�ӿ�����پ��帳ֵ
	int x_menu;
	int y_menu;
	// �˵���ť�Ŀ�Ⱥ͸߶�
	int menu_w;
	int menu_h;
	// �������淵�ز˵���ť���
	int helpRetButtonW;
	// �������淵�ز˵���ť������
	int rButtonx;
	// �˵����¼��
	int menu_dis;

	// ��Ϸ��������
	// ������·�Ŀ��
	float roadw;
	// ǽ�ĳ��ȺͿ��
	float wall_l;
	float wall_w;
	// ���ָʾ��Ŀ�Ⱥ͸߶�
	int player_info_w;
	int player_info_h;
	// ���̵���㶨λ����,�������½�
	int board_x;
	int board_y;
	// ��Ϸ���̻��߿��
	int lace;

	// ��Ϸ�㷨����
	char gameData[17][17];
	int arr_x;
	int arr_y;
};

#endif