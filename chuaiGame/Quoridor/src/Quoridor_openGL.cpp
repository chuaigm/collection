//=======================================
//
// ��Ϸ������ΪӪ
//       Quoridor
// ���ߣ�chuaiGM
// date: 2014-11-29
//
//=======================================

#include "stdafx.h"
#include "myclock.h"
#include "glfont.h"
#include "OpenGLbase.h"
#include "Quoridor_openGL.h"
#include "Quoridor_ComputerAI.h"
#include <stdlib.h>
#include "Quoridor_Network.h"

//font
extern CGLFont myfont;

// OpenGL��������ָ�����������ļ�������
extern COpenGLbase* g_OpenGL;
// �ⲿ����Ĵ��ڿ��
extern int WinWidth;
extern int WinHeight;
// �Ƿ������ֱ��
extern int g_sound;
// �����ļ�����
extern int ConfigGetKeyValue(const char *CFG_file, const char *section, const char *key, char *buf);
extern int ConfigSetKeyValue(const char *CFG_file, const char *section, const char *key, const char *buf);

/////////////////////////

CQuoridor::CQuoridor()
{
    // ��ǰ��Ϸ״̬
    iGameState=GAME_MENU;
    // ��ǰ�˵�ѡ����
    iMenu=-1;
    // �˵�����λ��
    menu.x=10;
    menu.y=10;
    // �˵���ť�Ŀ�Ⱥ͸߶�
    menu_w=10;
    menu_h=10;
    // �˵����¼��
    menu_dis=10;
    // ��Ϸ�����봰�ڱ߽���
    lace=6;

    iButton=-1;

    // ��Ϸ���ݳ�ʼ��
    roadw = 10;
    wall_l=10;
    wall_w=10;
    player_info_h=10;
    player_info_w=10;
    board_x=10;

    arr.x=-1;
    arr.y=-1;
    // �����Ϣ����
    // ��
    g_player[0].id=ID_CLOSED;
    g_player[0].color=GD_YELLOW;
    g_player[0].x=0;
    g_player[0].y=4;
    // ��
    g_player[1].id=ID_COMPUTER;
    g_player[1].color=GD_RED;
    g_player[1].x=4;
    g_player[1].y=8;
    // ��
    g_player[2].id=ID_CLOSED;
    g_player[2].color=GD_GREEN;
    g_player[2].x=8;
    g_player[2].y=4;
    // ��
    g_player[3].id=ID_HUMAN;
    g_player[3].color=GD_BLUE;
    g_player[3].x=4;
    g_player[3].y=0;
    // ��ͼ�����ʼ��
    memset(g_cactus,0,sizeof(g_cactus));
    // �������ݳ�ʼ��
    void* tmpp = (void*)gameData;
    memset(tmpp,0,sizeof(gameData));
    // ���ѡȡ��λ��
    pickup.x=-1;
    pickup.y=-1;
    // ��ʽ��Ϸʱ�����������˳��
    ply_head=NULL;
    // ���ʤ����־
    win_flag=GD_BLANK;
#ifdef __DEBUG__
    // ��ʾ������Ϣ
    g_debug_flag=false;
#endif
    // ��ʾ��Ϣ����
    HintFlag=HINT_NULL;

    // ʱ�������
    tcounter=0;

    // �������
    memset(n_loaclIP,0,sizeof(n_loaclIP));

    n_netWorkStatus=0;
    tmpp = (void*)n_NameAll;
    memset(tmpp,0,sizeof(n_NameAll));

    // �������Ӷ����ڹ���ʱ����
    n_net = NULL;
}

CQuoridor::~CQuoridor()
{
}
//��Ϸ���������ļ����,���ȱ��,������ֹ
int CQuoridor::haveDataFile()
{
    int ihave=1;
    FILE *fp;
    char *fdata[]={
        //bmp texture
        "data/images/quoridor_cover.bmp",
        "data/images/button.bmp",
        "data/images/road.bmp",
        "data/images/bidiu1.bmp",
        "data/images/huangshou1.bmp",
        "data/images/diting1.bmp",
        "data/images/xiaohei1.bmp",
        "data/images/wall1.bmp",
        "data/images/computer_logo.bmp",
        "data/images/example_show_help.bmp",
        //sound
        "data/sound/wall_set.wav",
        "data/sound/player_move.wav"
        };
    int i;
    
    for(i=0;i<sizeof(fdata)/sizeof(fdata[0]);i++)
    {
        fp=fopen(fdata[i],"r");
        if(!fp)
        {
#ifdef _UPPER_DIR_
            ihave=0;//ȱ���ļ�
#else
            return 0;
#endif
            break;
        }
        else
            fclose(fp);
    }
#ifdef _UPPER_DIR_
    //�����ǰ�ļ���û��,�����ϼ��ļ���,�ټ��
    if(!ihave)
        SetCurrentDirectory("..");

    for(i=0;i<sizeof(fdata)/sizeof(fdata[0]);i++)
    {
        fp=fopen(fdata[i],"r");
        if(!fp)
        {
            return 0;
        }
        else
            fclose(fp);
    }
#endif
    return 1;
}

//���ظ���ͼƬ,ģ��,��ʼ��ȫ������
void CQuoridor::init()
{
    //load images
    g_OpenGL->LoadBMP_aux("data/images/quoridor_cover.bmp", g_cactus[0]);       // ����
    g_OpenGL->LoadBMP_aux("data/images/chess_board_shading.bmp", g_cactus[1]);  // ���̵���
    g_OpenGL->LoadBMP_aux("data/images/road.bmp", g_cactus[2]);                 // ��ҿ�վ��λ��
    g_OpenGL->LoadBMP_aux("data/images/bidiu1.bmp", g_cactus[3]);               // ���1����
    g_OpenGL->LoadBMP_aux("data/images/huangshou1.bmp", g_cactus[4]);           // ���2����
    g_OpenGL->LoadBMP_aux("data/images/diting1.bmp", g_cactus[5]);              // ���3����
    g_OpenGL->LoadBMP_aux("data/images/xiaohei1.bmp", g_cactus[6]);             // ���4����
    g_OpenGL->LoadBMP_aux("data/images/wall1.bmp", g_cactus[7]);                // ǽ����ͼ
    g_OpenGL->LoadBMP_aux("data/images/computer_logo.bmp", g_cactus[8]);        // ����ͼ��
    //button
    g_OpenGL->LoadBMP_aux("data/images/button.bmp", g_cactus[9]);               // ��ť
    g_OpenGL->LoadBMP_aux("data/images/example_show_help.bmp", g_cactus[10]);   // ������ʹ�õ�չʾͼ

    ///////////////////////////////////////////
    //������ͼ
    glEnable(GL_TEXTURE_2D);

    //��Ϸ״̬
    iGameState=GAME_MENU;
}

//����ͶӰ��ʽ
void CQuoridor::initView()
{
    // ������ά��OpenGL��ͼģ��
    g_OpenGL->init_2D();

    // �����ﴴ��һЩ��OpenGL�л�ȡ�Ĵ��ڲ���
    // ����Ҳ���Ե�����һ������ȥʵ��
    // �˵����¼��
    menu_dis=g_OpenGL->RCheight/15;
    // �˵���ť�Ŀ�Ⱥ͸߶�
    menu_w=g_OpenGL->RCwidth/9;
    menu_h=g_OpenGL->RCheight/20;

    // �˵�����λ��
    menu.x=g_OpenGL->RCwidth-menu_dis/2-menu_w;
    menu.y=menu_h;

    // ��Ϸ��������
    player_info_h=g_OpenGL->RCheight/4.0f;
    player_info_w=(g_OpenGL->RCwidth-g_OpenGL->RCheight)*3/8.0f;
    // �������½ǣ��������
    board_x = (g_OpenGL->RCwidth-g_OpenGL->RCheight)/2;

    // ǽ��·�Ŀ�����㹫ʽ��
    //  x=roadw; y=wall_w
    //  9*x + 8*y = Height - lace*2
    //    x = 4 y
    roadw = (g_OpenGL->RCheight - lace*2)/11.0f;
    wall_w= (g_OpenGL->RCheight - lace*2)/44.0f;
    wall_l= 2*roadw+wall_w;
}

// ��Ϸ����ͼ����
void CQuoridor::showMain()
{
#ifdef __DEBUG__
    // ��ʾ��������
    if (g_debug_flag)
    {
        show_Font_test();
    }
#endif

    switch(iGameState)
    {
/*
    case GAME_PRE:
        //showpreani();
        break;
*/
    case GAME_MENU:
        showmenu();
        break;
    case GAME_IN_CONFIG:
        drawAccessory();
        drawChessBorad();
        drawInConfig();
        break;
    case GAME_NETWORK:
    case GAME_SINGE:
        drawAccessory();
        drawChessBorad();
        drawPlayerWall();
        // ע��˴���͸��ͼ�������˳���й�
        drawPickMask();
        break;
    case GAME_WIN:
        drawAccessory();
        drawChessBorad();
        drawPlayerWall();
        drawVictory();
        break;
    case GAME_NET_CONFIG:
        drawNetworkOp();
        break;
    case GAME_SENDBOX:
        drawAccessory();
        drawChessBorad();
        drawPlayerWall();
        // ע��˴���͸��ͼ�������˳���й�
        drawPickMask();
#ifdef __DEBUG__
        drawTestOptimalPath();
#endif
        break;

    case GAME_HELP:
        showHelp();
        break;

    default:
        break;
    }
    // ���Ƶ���ʽ��ʾ��Ϣ��ע��͸��ͼ����������
    drawHint();
}
//ʵʱ���
void CQuoridor::check()
{
    int i;
    int x,y;
    int it;
    float remain;
    // �����굱ǰ�ƶ���λ��
    x=g_OpenGL->Xmouse;
    y=g_OpenGL->Ymouse;
    iMenu=-1;//��ʼ�� û��ѡ��
    iButton=-1;
    arr.x=-1;
    arr.y=-1;

    switch(iGameState)
    {
    //case GAME_PRE:
    //    if(c1.iNumClip%2==0)
    //    {
    //        if(param1<30)
    //        {
    //            param1++;
    //        }
    //    }
    //    //��ʱ
    //    if(c1.clipcount())
    //    {
    //        iGameState=GAME_MENU;
    //        //����ͶӰ 
    //        initView();
    //    }
    //    break;

    case GAME_MENU:
        if(x<menu.x || x>menu.x+menu_w || y<menu.y)
            break;

        for(i=0;i<MENU_NUM;i++)
        {
            if( y>menu.y+i*menu_dis && y<menu.y+i*menu_dis+menu_h )
            {
                // ע�⣬������˵�ѡȡֵ����ֵ��i�����붨���ö�ٱ�������Ӧ��
                iMenu=i;
                break;
            }
        }
        break;
    case GAME_SINGE:
    case GAME_NETWORK:
        if (ply_head->id==ID_COMPUTER)
        {
            Quoridor_ComputerAI ai;
            // AI�㷨ѡ��
            //ai.FoolAI();
            ai.AI_action();
            break;
        }
        else if (ply_head->id==ID_NET_PLAYER)
        {
            break;
        }
    case GAME_SENDBOX:
        // ʵʱ������λ��
        // ���ʱ��������λ��������
        if (x>board_x+lace && x<board_x+g_OpenGL->RCheight-lace
            && y>lace && y<g_OpenGL->RCheight-lace)
        {
            // ��ǰ������λ�ó�һ��·�Ŀ�Ⱥ�һ��ǽ�Ŀ�ȵ���������
            it=(int)((x-board_x-lace)/(roadw+wall_w));
            // ��������
            remain = (x-board_x-lace)-it*(roadw+wall_w);
            if (remain>roadw)
            {   // ������·��ȴ�����ǽ��λ�ã�����
                arr.x = it*2+1;
            } else {
                // ��ҿ��Է��õ�λ�ã�·��ż��
                arr.x = it*2;
            }
            it = (int)((y-lace)/(roadw+wall_w));
            remain = (y-lace) - it*(roadw+wall_w);
            if (remain>roadw)
            {
                arr.y = it*2+1;
            } else {
                arr.y = it*2;
            }
        }
        // ���ﲻ����break���̳з��ز˵���ť
    case GAME_IN_CONFIG:
        // �����ⷵ�ز˵���ť���������̰�ť
        if (x>menu.x&&x<menu.x+menu_w)
        {
            if (y>menu.y&&y<menu.y+menu_h)
            {
                iButton=BUTTON_INIT_OR_CONFIRM;
            }
            else if (y>g_OpenGL->RCheight-menu.y-menu_h&&y<g_OpenGL->RCheight-menu.y)
            {
                iButton=BUTTON_RETURN;
            }
        }
        break;
    case GAME_WIN:
        if (x>menu.x&&x<menu.x+menu_w)
        {
            if (y>menu.y&&y<menu.y+menu_h)
            {
                iButton=BUTTON_INIT_OR_CONFIRM;
            }
            else if (y>g_OpenGL->RCheight-menu.y-menu_h&&y<g_OpenGL->RCheight-menu.y)
            {
                iButton=BUTTON_RETURN;
            }
        }
        break;
    case GAME_NET_CONFIG:
        if (x>menu.x&&x<menu.x+menu_w&&y>menu.y&&y<menu.y+menu_h)
        {
            iButton=BUTTON_RETURN;
        }
        switch (n_netWorkStatus)
        {
        case 0:
            if (x>g_OpenGL->RCwidth/4-menu_w/2&&x<g_OpenGL->RCwidth/4-menu_w/2+menu_w
                &&y>g_OpenGL->RCheight*2/3&&y<g_OpenGL->RCheight*2/3+menu_h)
            {
                iButton=BUTTON_SERVER;
            }
            if (x>g_OpenGL->RCwidth*3/4-menu_w/2&&x<g_OpenGL->RCwidth*3/4-menu_w/2+menu_w
                &&y>g_OpenGL->RCheight*2/3&&y<g_OpenGL->RCheight*2/3+menu_h)
            {
                iButton=BUTTON_CLIENT;
            }
            break;
        case 1:
            if (x>(g_OpenGL->RCwidth-menu_w)/2&&x<(g_OpenGL->RCwidth-menu_w)/2+menu_w
                &&y>menu.y&&y<menu.y+menu_h)
            {
                iButton=BUTTON_SERVER_START;
            }
            if (x>g_OpenGL->RCwidth/4-menu_w/2&&x<g_OpenGL->RCwidth/4-menu_w/2+menu_w
                &&y>g_OpenGL->RCheight*1/6&&y<g_OpenGL->RCheight*1/6+menu_h)
            {
                iButton=BUTTON_SERVER_TEST;
            }
            break;
        case 2:
            if (x>g_OpenGL->RCwidth*3/4-menu_w/2&&x<g_OpenGL->RCwidth*3/4-menu_w/2+menu_w
                &&y>g_OpenGL->RCheight/6&&y<g_OpenGL->RCheight/6+menu_h)
            {
                iButton=BUTTON_CLIENT_TEST;
            }
            break;
        default:
            break;
        }
        break;
    case GAME_HELP:
        if (x>menu.x&&x<menu.x+menu_w)
        {
            if (y>menu.y+menu_dis&&y<menu.y+menu_h+menu_dis)
            {
                iButton=BUTTON_MUSIC;
            }
            else if (y>menu.y&&y<menu.y+menu_h)
            {
                iButton=BUTTON_RETURN;
            }
        }
        break;

    default:
        break;
    }
}
// ����ɿ���������ѡ��ȷ�ϴ���
void CQuoridor::lbuttonproc(int lparam)
{
    char tmpstr[32]={0};
    if (iButton==BUTTON_RETURN)
    {
        if (iGameState==GAME_SINGE || iGameState==GAME_NETWORK)
        {
            HintFlag=HintFlag==HINT_EXIT?HINT_NULL:HINT_EXIT;
        }
        else
        {
            if (iGameState==GAME_NET_CONFIG)
            {
                n_net->closeNetWork();
            }
            HintFlag=HINT_NULL;
            iGameState=GAME_MENU;
        }
        return;
    }
    else if (iButton==BUTTON_INIT_OR_CONFIRM)
    {
        if (HintFlag==HINT_EXIT)
        {
            if (iGameState==GAME_NET_CONFIG)
            {
                n_net->closeNetWork();
            }
            HintFlag=HINT_NULL;
            iGameState=GAME_MENU;
            return;
        } else if (HintFlag==HINT_PLAY_NUMBER_INVALID)
        {
            HintFlag=HINT_NULL;
            return;
        }
    }
    // ������ʾ����ʾ��Ϣʱ��ֻ����ȷ���뷵�ذ�ť�����������������Ϸ���ݵ��ж�
    if (HintFlag!=HINT_NULL)
    {
        return;
    }

    switch(iGameState)
    {
    case GAME_MENU:
        if(iMenu<0)
            break;
        switch(iMenu)
        {
        case MENU_SINGE:
            iGameState=GAME_IN_CONFIG;
            resetGameData();
            break;
        case MENU_NETWORK:
            {   // �����������ģʽʱ����ȡ����IP��˿�һ��
            WORD wVersionRequested;
            WSADATA wsaData;
            wVersionRequested=MAKEWORD(2,2);
            WSAStartup(wVersionRequested,&wsaData);

            char buf[256]="";
            struct hostent *ph = 0;
            gethostname(buf, 256);
            ph = gethostbyname(buf);
            const char *IP =inet_ntoa(*((struct in_addr *)ph->h_addr_list[0]));//�˴���ñ���IP
            strncpy(n_loaclIP, IP, 16);
            WSACleanup();
            }

            // �����������Ӷ���
            if (n_net == NULL)
            {
                n_net = new Quoridor_Network;
            }

            ConfigGetKeyValue("config.ini", "Net_work", "MyName", n_net->Name);
            ConfigGetKeyValue("config.ini", "Net_work", "IP", n_net->IP);
            ConfigGetKeyValue("config.ini", "Net_work", "port", tmpstr);
            n_net->port=atoi(tmpstr);

            iGameState=GAME_NET_CONFIG;
            resetGameData();
            break;
        case MENU_SENDBOX:
            iGameState=GAME_SENDBOX;
            resetGameData();
            break;
        case MENU_HELP:
            iGameState=GAME_HELP;
            break;
        case MENU_QUIT:
            g_OpenGL->CleanUp();
            PostQuitMessage(0);
            exit(0);
            break;
        default:
            break;
        }
        break;
    case GAME_IN_CONFIG:
        // �ڵ�������ģʽ�£��˰�ťΪȷ��
        if (iButton==BUTTON_INIT_OR_CONFIRM)
        {
            // ����൱�ڵ�����Ϸ�ĳ�ʼ������
            int nn=0;
            ply_head=NULL;      //���Ӧ�ÿ��Բ�Ҫ
            player* tail=NULL;
            // ����Ҳ���Կ�����һ��ÿ����ҵ�nextָ��
            for (int i=0; i<4; i++)
            {   // �����ǰ�����״̬�ǲ�����Ϸ��(�������˿��ƣ�Ҳ�����ǵ���)
                if (g_player[i].id!=ID_CLOSED)
                {
                    if (ply_head==NULL)
                    {
                        ply_head=&g_player[i];
                        tail=&g_player[i];
                    }
                    tail->next=&g_player[i];
                    tail=&g_player[i];
                    nn++;
                    switch (g_player[i].color)
                    {
                    case GD_YELLOW:
                        gameData[g_player[i].x*2][g_player[i].y*2]=GD_YELLOW;
                        break;
                    case GD_RED:
                        gameData[g_player[i].x*2][g_player[i].y*2]=GD_RED;
                        break;
                    case GD_GREEN:
                        gameData[g_player[i].x*2][g_player[i].y*2]=GD_GREEN;
                        break;
                    case GD_BLUE:
                        gameData[g_player[i].x*2][g_player[i].y*2]=GD_BLUE;
                        break;
                    default:
                        break;
                    }
                }
                // ����else��֧����resetGameData()������ʵ��
            }
            // �������������������2ʱ���޷�������Ϸ
            if (nn<2)
            {
                HintFlag=HINT_PLAY_NUMBER_INVALID;
                // ��Ϊ����ɾ����Ϸ���ݣ������ٲ�����
                resetGameData();
                break;
            }
            // ������������
            tail->next=ply_head;
            // Ϊ��ͳ�������������Ϊ�˸����ʣ��ǽ����ֵ
            player* tmp_head=ply_head;
            // ���������
            int human_n=0;
            // ѭ����ʣ��ǽ����ֵ
            do
            {   // ͳ�����������
                if (tmp_head->id==ID_HUMAN)
                {
                    human_n++;
                }
                // ����������,21��2��ȡ��=10
                //            21��3��ȡ��=7
                //            21��4��ȡ��=5
                tmp_head->wall_num_left=wall_total_num/nn;
                tmp_head=tmp_head->next;
            }while (ply_head!=tmp_head);
            // �������������ң��������������
            if (human_n>0)
            {
                while (ply_head->id!=0)
                {
                    ply_head=ply_head->next;
                }
            }
            iGameState=GAME_SINGE;
        }
        // ���ѡȡ��ͬ��ҵ�����ѡ��ʱ�Ĵ���
        // �������
        for (size_t j=0; j<4; j++)
        {
            // ������ѡ�ĵ����ǣ���ң����ԣ����ǹر�
            for (size_t i=0; i< 3; i++)
            {
                if (g_OpenGL->Xmouse>(float)board_x+(i*2.3f+1)*(roadw+wall_w)
                    && g_OpenGL->Xmouse<(float)board_x+(i*2.3f+1)*(roadw+wall_w)+menu_w
                    && g_OpenGL->Ymouse>(player_info_h-menu_h)/2+player_info_h*j
                    && g_OpenGL->Ymouse<(player_info_h-menu_h)/2+menu_h+player_info_h*j
                    )
                {
                    g_player[3-j].id=i;
                }
            }
        }
        break;
    case GAME_SINGE:
        // �������ҿɿصĽ�ɫ
        if (ply_head->id==ID_HUMAN)
        {
            playerActionRule(false);
        }
        break;
    case GAME_NETWORK:
        // �������ҿɿصĽ�ɫ
        if (ply_head->id==ID_HUMAN)
        {
            playerActionRule(true);
        }
        break;
    case GAME_NET_CONFIG:
        switch (iButton)
        {
        case BUTTON_SERVER:
            // ���ﴴ���������
            if (n_net->startServer())
            {
                // ����ɹ�����
                n_netWorkStatus=1;
                // ����Ƿ����������Ȱ���������м����Լ�
                // ������Ĭ����ɫ���
                g_player[3].id=ID_HUMAN;
                ply_head=&g_player[3];
            }
            break;
        case BUTTON_CLIENT:
            // ���ﴴ���������
            if(n_net->startClient())
            {
                n_netWorkStatus=2;
            }
            break;
        case BUTTON_SERVER_TEST:
            break;
        case BUTTON_CLIENT_TEST:
            break;
        case BUTTON_SERVER_START:
            // ���û�������������
            if (n_net->GetConnectionNumber()<1)
            {   // �����Ժ���Ը�����ʾ
                break;
            }
            player* tail=ply_head;
            // �㿪ʼ��Ϸ���Ÿ���������Ҹ�����������ֵ
            gameData[g_player[3].x*2][g_player[3].y*2]=GD_BLUE;
            int ConNum=n_net->GetConnectionNumber();
            // ����������Ϸʱ��������ҵķ���˳�򣬷�����������ɫ
            // �ڶ���������Ϸ�����Ϊ��ɫ�����������Ҽ�������ɫ������ǻ�ɫ
            for (int i=0; i<ConNum;i++)
            {
                switch (i)
                {
                case 0:// ����Ϊ��ɫ���
                    g_player[1].id=ID_NET_PLAYER;
                    gameData[g_player[1].x*2][g_player[1].y*2]=GD_RED;
                    tail->next=&g_player[1];
                    tail=&g_player[1];
                    break;
                case 1:// ����Ϊ��ɫ���
                    g_player[2].id=ID_NET_PLAYER;
                    gameData[g_player[2].x*2][g_player[2].y*2]=GD_GREEN;
                    tail->next=&g_player[2];
                    tail=&g_player[2];
                    break;
                case 2:// ����Ϊ��ɫ���
                    g_player[0].id=ID_NET_PLAYER;
                    gameData[g_player[0].x*2][g_player[0].y*2]=GD_YELLOW;
                    // �����ڻ�ɫ���ʱ���Ƚ��ر�
                    ply_head->next=&g_player[0];
                    g_player[0].next=&g_player[1];
                    break;
                default:
                    break;
                }
                sprintf(tmpstr,"READY%1dN%1d",i,ConNum);
                n_net->n_TCPnet->SendServer(i,tmpstr,strlen(tmpstr)+1);     // ����ע��size��Ҫ����\0����
                //n_TCPnet->SendServer(i,"START",5+1);      // ��������ò��������
#ifdef __DEBUG__
                {
                    char stmp[16];
                    static int s_send=0;
                    sprintf(stmp, "%04d|Send%04d",__LINE__,s_send++);
                    ConfigSetKeyValue("debugLog.txt", "Server", stmp, tmpstr);
                }
#endif
            }
            tail->next=ply_head;        // �γɻ�״
            // Ϊ��ͳ�������������Ϊ�˸����ʣ��ǽ����ֵ
            player* tmp_head=ply_head;
            // ѭ����ʣ��ǽ����ֵ
            do
            {   // ͳ�����������
                // ����������,21��2��ȡ��=10
                //            21��3��ȡ��=7
                //            21��4��ȡ��=5
                tmp_head->wall_num_left=wall_total_num/(ConNum+1);
                tmp_head=tmp_head->next;
            }while (ply_head!=tmp_head);
            iGameState=GAME_NETWORK;
            break;
        }
        break;
    case GAME_SENDBOX:
        freeSendBoxRule();
        break;
    case GAME_HELP:
        if (iButton==BUTTON_MUSIC)
        {
            g_sound=!g_sound;
            if (g_sound!=1)
            {
                g_sound=0;
            }
            char tmpstr[4];
            sprintf(tmpstr,"%d",g_sound);
            ConfigSetKeyValue("config.ini", "Sound", "sound", tmpstr);
            return;
        }
        break;

    default:
        break;
    }
}
// ����Ҽ�����
void CQuoridor::rbuttonproc( int lparam )
{
    switch(iGameState)
    {
    case GAME_SINGE:
        break;
    case GAME_NETWORK:
        break;
    case GAME_SENDBOX:
        switch (gameData[arr.x][arr.y])
        {
        case GD_BLANK:
            break;
        case GD_YELLOW:
            g_player[0].x=-1;
            g_player[0].y=-1;
            gameData[arr.x][arr.y]=0;
            break;
        case GD_RED:
            g_player[1].x=-1;
            g_player[1].y=-1;
            gameData[arr.x][arr.y]=0;
            break;
        case GD_GREEN:
            g_player[2].x=-1;
            g_player[2].y=-1;
            gameData[arr.x][arr.y]=0;
            break;
        case GD_BLUE:
            g_player[3].x=-1;
            g_player[3].y=-1;
            gameData[arr.x][arr.y]=0;
            break;
        case GD_WALL:
            {   // ��vector�У�������ǰ�Ҽ�ѡȡ��ǽ��λ��
                std::vector<pos2d>::iterator itor=find(wall_vec.begin(),wall_vec.end(),arr);
                if (itor==wall_vec.end())
                {
                    //continue;
                    break;
                }
                int dist=distance(wall_vec.begin(),itor);
                // ��������ż�������±�Ҳ��ż��
                if (dist%2==0)
                {
                    // ע����������ʧЧ������
                    gameData[itor->x][itor->y]=GD_BLANK;
                    gameData[(itor->x+(itor+1)->x)/2][(itor->y+(itor+1)->y)/2]=GD_BLANK;
                    gameData[(itor+1)->x][(itor+1)->y]=GD_BLANK;
                    wall_vec.erase(itor,itor+2);
                    break;
                }
                // �����������������±�Ҳ������
                else
                {
                    // ע�������ʧЧ����
                    gameData[itor->x][itor->y]=GD_BLANK;
                    gameData[(itor->x+(itor-1)->x)/2][(itor->y+(itor-1)->y)/2]=GD_BLANK;
                    gameData[(itor-1)->x][(itor-1)->y]=GD_BLANK;
                    wall_vec.erase(itor-1,itor+1);
                    break;
                }
                // ����ɼӿɲ��ӣ����������ɾ������ѡȡ�Ĺ���Ҳ������
                pickup.x=-1;
                pickup.y=-1;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

//���̴���
void CQuoridor::keyupproc(int keyparam)
{
    switch (keyparam)
    {
#ifdef __DEBUG__
    case VK_F9:
        g_debug_flag=!g_debug_flag;
        break;
#endif
    case VK_ESCAPE:
        //�ص��˵�
        if (iGameState==GAME_SINGE || iGameState==GAME_NETWORK)
        {
            HintFlag=HintFlag==HINT_EXIT?HINT_NULL:HINT_EXIT;
        }
        else{iGameState=GAME_MENU;}
        break;
    //case 81:        // 'Q'
    //    if (iGameState==GAME_SINGE || iGameState==GAME_NETWORK)
    //    {
    //        
    //    }
    //    break;
    case 77:        // 'M'
        if (iGameState==GAME_SINGE || iGameState==GAME_NETWORK)
        {
            pickup.x = 2*ply_head->x;
            pickup.y = 2*ply_head->y;
            playerMovablePos(pickup);
        }
        break;
    default:
        break;
    }
}

//ָ����ͼ
void CQuoridor::texture_select(UINT texture)
{
    glBindTexture  (GL_TEXTURE_2D, texture);

    // GL_TEXTURE_2D: ����2D����.
    // GL_TEXTURE_WRAP_S: S�����ϵ���ͼģʽ.
    // GL_CLAMP: ����������������0.0,1.0�ķ�Χ֮��.��������˻������.
    //           �������,ֻ�ǻ��Ե�������.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    // ����ͬ��,ֻ������T����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    // �����������
    // GL_TEXTURE_MIN_FILTER: ��С����
    // GL_LINEAR: ���Թ���, ʹ�þ��뵱ǰ��Ⱦ�������������4�����ؼ�Ȩƽ��ֵ.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    
    // GL_TEXTURE_MAG_FILTER: �Ŵ����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // GL_LINEAR_MIPMAP_NEAREST: ʹ��GL_NEAREST����ӽ���ǰ����εĽ�����
    //                  �������㼶��ͼ���в���,Ȼ����������ֵ�������Բ�ֵ.
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

//�����Σ����½�����xy�����w����h
void CQuoridor::tPicRectangle(float x,float y,float w,float h,float deep)
{ 
    glPushMatrix();

    //����������
    glTranslatef(x,y,deep);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(w, 0.0f,  0.0f);

        //up
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(w, h,  0.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0.0f, h,  0.0f);
    glEnd();

    glPopMatrix();
}
//����ť
void CQuoridor::tPicButton(float x,float y,float w,float h,float ytex)
{ 
    glPushMatrix();

    //����������
    glTranslatef(x,y,-0.1f);    //z������ƶ�
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, ytex);
        glVertex3f(0.0, 0.0, 0.0f);
        
        glTexCoord2f(1.0f, ytex);
        glVertex3f(w, 0.0, 0.0f);

        // up
        glTexCoord2f(1.0f, ytex+0.5f);
        glVertex3f(w, h, 0.0f);

        glTexCoord2f(0.0f, ytex+0.5f);
        glVertex3f(0.0, h, 0.0f);
    glEnd();

    glPopMatrix();
}

//��������
//void CQuoridor::showpreani()
//{
//	glPushMatrix();
//	//�ƶ�����ǰλ��
//	glTranslatef(0,0,-8.0);
//	texture_select(g_cactus[0]);
//	tPic(2.0f);
//	//�ƶ�����
//	tSquare(param1*0.2, 0.0, 0.1, 3.0);
//	glPopMatrix();
//}

//ָ��λ�û�һ��������
void CQuoridor::tRectangle(float x, float y, float z, float w, float h, float r, float g, float b, float a)
{
    //���Խ�ջ
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    glTranslatef(x,y,z);
    glColor4f(r, g, b, a);
    //glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex3f( 0,  0,  0.0f);
        glVertex3f( w,  0,  0.0f);
        glVertex3f( w,  h,  0.0f);
        glVertex3f( 0,  h,  0.0f);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopAttrib();
}

//�������
void CQuoridor::drawMouse()
{
    glPushMatrix();
    //glLoadIdentity();
    //���Խ�ջ
    //glPushAttrib(GL_CURRENT_BIT);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_TEXTURE_2D);
    //glDisable(GL_LIGHTING);

    glTranslatef((GLfloat)g_OpenGL->Xmouse,(GLfloat)g_OpenGL->Ymouse,0.1f);

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(30.0f, -15.0f,0.0f);

    glColor3f(0.0f, 0.8f, 0.0f);
    glVertex3f(12.0f, -12.0f,0.0f);
    glVertex3f(6.0f,-20.0f,0.0f);

    glColor3f(0.0f, 0.0f, 0.0f); 
    glVertex3f(3.0f,-20.0f,0.0f);
    glEnd();

    /////////////////////////
    //glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glPopAttrib();
    glPopMatrix();
}

//��ʾ�˵�
void CQuoridor::showmenu()
{
    //������ͼƬ
    glPushMatrix();
    glTranslatef(0.0,0.0,-0.2f);
    texture_select(g_cactus[0]);
    tPicRectangle((g_OpenGL->RCwidth-g_OpenGL->RCheight)/2.0f, 0, (float)g_OpenGL->RCheight, (float)g_OpenGL->RCheight);
    glPopMatrix();

#ifdef __DEBUG__
    myfont.Print2D(5,5,"���԰�  ,"__DATE__,FONT0,1,0.9f,0.1f);
#endif

    //  ��Ӧö�ٹ�ϵ     0          1          2          3          4
    char *menustr[]={"��    ��","��Ϸ˵��","ɳ��ģʽ","������Ϸ","������Ϸ"};
    //char *menustr[]={"������Ϸ","������Ϸ","ɳ��ģʽ","��Ϸ����","��    ��"};

    // ��glOrthoģʽ�»��Ʋ˵�
    // �˵����Ǵ������ϻ��Ƶģ����Բ˵����������ʾ
    for(int i=0;i<MENU_NUM;i++)
    {
        //����
        myfont.Print2D(menu.x+10,menu.y+5+i*menu_dis,menustr[i],FONT4,1,1,1);
        //ͼƬ
        texture_select(g_cactus[9]);
        if(iMenu==i)
        {
            tPicButton((float)menu.x,(float)(menu.y+i*menu_dis),(float)menu_w,(float)menu_h,0.0f);
        }
        else
        {
            tPicButton((float)menu.x,(float)(menu.y+i*menu_dis),(float)menu_w,(float)menu_h,0.5f);
        }
    }
}
#ifdef __DEBUG__
// ��ʾ��������
void CQuoridor::show_Font_test()
{
    // Y /-------------\
    //   |600          |(800,600)
    //   |             |
    //   |0         800|
    //   \-------------/ X

    char tmpstr[64]={0};
    sprintf(tmpstr, "iMenu=%d",iMenu);
    myfont.Print2D(menu.x+10,menu.y+2*menu_h,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);

    for (int i=0; i<17;i++)
    {
        for (int j=0; j<17;j++)
        {
            sprintf(tmpstr, "%d,",gameData[j][i]);
            //myfont.Print2D(10*(j+1),70+10*i,tmpstr,FONT0,1.0f,0.0f,0.0f,1.0f);
            myfont.Print2D(board_x+g_OpenGL->RCheight+10*j,menu.y+2*menu_h+60+10*i,tmpstr,FONT0,1.0f,0.0f,0.0f,1.0f);
        }
    }

    sprintf(tmpstr, "pick_x=%d",pickup.x);
    myfont.Print2D(menu.x+10,menu.y+2*menu_h+40,tmpstr,FONT0,1.0f,1.0f,1.0f,1.0f);
    sprintf(tmpstr, "pick_y=%d",pickup.y);
    myfont.Print2D(menu.x+10,menu.y+2*menu_h+30,tmpstr,FONT0,1.0f,1.0f,1.0f,1.0f);
    sprintf(tmpstr, "arr.x=%d",arr.x);
    myfont.Print2D(menu.x+10,menu.y+2*menu_h+20,tmpstr,FONT0,1.0f,1.0f,1.0f,1.0f);
    sprintf(tmpstr, "arr.y=%d",arr.y);
    myfont.Print2D(menu.x+10,menu.y+2*menu_h+10,tmpstr,FONT0,1.0f,1.0f,1.0f,1.0f);

    sprintf(tmpstr, "best_path_length=%d",best_path.size());
    myfont.Print2D(menu.x+10,menu.y+2*menu_h+50,tmpstr,FONT0,1.0f,1.0f,1.0f,1.0f);
}
#endif

void CQuoridor::showHelp()
{
    // ���������е�װ��ͼƬ
    float cover_logo_len=(g_OpenGL->RCwidth - g_OpenGL->RCheight)/2.0f-2*lace;
    float player_logo_len=((g_OpenGL->RCwidth - g_OpenGL->RCheight)/2.0f-3*lace)/2.0f;

    // ����ͼ
    texture_select(g_cactus[0]);
    tPicRectangle((float)lace, g_OpenGL->RCheight-lace-cover_logo_len, cover_logo_len, cover_logo_len);
    // ���logo
    texture_select(g_cactus[3]);
    tPicRectangle((float)lace, (float)lace, player_logo_len, player_logo_len);
    texture_select(g_cactus[4]);
    tPicRectangle((float)lace*2+player_logo_len, (float)lace, player_logo_len, player_logo_len);
    texture_select(g_cactus[5]);
    tPicRectangle((float)lace, (float)lace*2+player_logo_len, player_logo_len, player_logo_len);
    texture_select(g_cactus[6]);
    tPicRectangle((float)lace*2+player_logo_len, (float)lace*2+player_logo_len, player_logo_len, player_logo_len);
    // ʵ��չʾͼ
    texture_select(g_cactus[10]);
    tPicRectangle((g_OpenGL->RCwidth+g_OpenGL->RCheight)/2.0f, 0.45f*g_OpenGL->RCheight, (g_OpenGL->RCwidth-g_OpenGL->RCheight)/2.0f, (g_OpenGL->RCwidth-g_OpenGL->RCheight)*0.375f);

    // ��͸������
    tRectangle((float)board_x,0,-0.3f,(float)g_OpenGL->RCheight,(float)g_OpenGL->RCheight,1,1,1,0.3f);

    char tmpstr[1024]="��Ϸ���";
    myfont.Print2D(g_OpenGL->RCwidth/2-60,g_OpenGL->RCheight-50,tmpstr,FONT5,1.0,1.0,0.0);
    sprintf(tmpstr,"��Ϸ���ƣ���Quoridor����ǽ�塷������ΪӪ��");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-80,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"         Quoridor������Ϸ���ʦ Mirko Marchesi����1997�귢��");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-110,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"         ��2~4�˶�ս������Ϸ");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-140,tmpstr,FONT3,0.8f,1.0,0.0);

    sprintf(tmpstr,"��Ϸ����");
    myfont.Print2D(g_OpenGL->RCwidth/2-60,g_OpenGL->RCheight-180,tmpstr,FONT5,1.0,1.0,0.0);
    sprintf(tmpstr,"��ϷĿ�꣺˭���ߵ����̶����һ��˭�ͻ��ʤ��");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-210,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"����������Ұ�˳�������ж����ֵ��Լ�ʱ��ֻ�ܴ�����");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-240,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"         �����ж���ѡ��һ��ִ�У�");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-270,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"       1.ǰ�������ĸ�������ѡ��һ�����ƶ���������Ƶ�����һ��");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-300,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"       2.�ڿ��Է���ǽ��λ�÷���һ��ǽ");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-330,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"�������1. ���Ҫ�ƶ���λ�ô���������ң���������������������λ��");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-360,tmpstr,FONT2,1.0,0.8f,0.0);
    sprintf(tmpstr,"          2. �������������������ʱ����ǽ�����������Ҵ��ڣ�");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-380,tmpstr,FONT2,1.0,0.8f,0.0);
    sprintf(tmpstr,"             ��������������������������ߵ�λ����(���̱߽�ٷ�Ҳδ˵��)");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-400,tmpstr,FONT2,1.0,0.8f,0.0);
    sprintf(tmpstr,"          3. �ڷ���ǽʱ������������õ�һ��ǽ�������³������κ�һ�����");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-420,tmpstr,FONT2,1.0,0.8f,0.0);
    sprintf(tmpstr,"             ���ܵ���˰������ǽ�޷�������");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-440,tmpstr,FONT2,1.0,0.8f,0.0);
    sprintf(tmpstr,"          4. 2����Ϸʱ��ÿ����10��ǽ����ʹ�ã�3����Ϸ7����4����Ϸ5��");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-460,tmpstr,FONT2,1.0,0.8f,0.0);

    sprintf(tmpstr,"             (F4 ��С�����ڣ�F9 ��ʾ������Ϣ)");
    myfont.Print2D(board_x+lace,lace+62,tmpstr,FONT2,1.0,0.8f,0.0);
    sprintf(tmpstr,"             (������ʹ����ͼ�����������磬����ͷ��ȡ�ԡ���С��ս�ǡ�����)");
    myfont.Print2D(board_x+lace,lace+46,tmpstr,FONT1,1.0,0.8f,0.0);

    sprintf(tmpstr,"OpenGL��Ϣ��[����:%s] [��Ⱦ��:%s]",glGetString(GL_VENDOR),glGetString(GL_RENDERER));
    myfont.Print2D(board_x+lace,lace+32,tmpstr,FONT0,1.0,1.0,0.0);
    sprintf(tmpstr,"            [GL�汾:%s][GLU�汾:%s]",glGetString(GL_VERSION),gluGetString(GLU_VERSION));
    myfont.Print2D(board_x+lace,lace+20,tmpstr,FONT0,1.0,1.0,0.0);
    sprintf(tmpstr,"�汾��Ϣ��[From 2014-11-29]-[%s][%s][_MSC_VER=%d][Ver= 0.8a]          ���ߣ�ChuaiGuoMing",__DATE__,__TIME__,_MSC_VER);
    myfont.Print2D(board_x+lace,lace,tmpstr,FONT0,1.0,1.0,0.0);

    //���ֿ��ư�ť
    if (g_sound==1)
    {
        sprintf(tmpstr,"��Ч: ��");
    } else {
        sprintf(tmpstr,"��Ч: ��");
    }
    myfont.Print2D(menu.x+8,menu.y+menu_dis+6,tmpstr,FONT4,1,1,1);

    //ͼƬ
    texture_select(g_cactus[9]);
    if(iButton==BUTTON_MUSIC)
    {
        tPicButton((float)menu.x,(float)menu.y+menu_dis,(float)menu_w,(float)menu_h,0.0f);
    }
    else
    {
        tPicButton((float)menu.x,(float)menu.y+menu_dis,(float)menu_w,(float)menu_h,0.5f);
    }

    //����
    sprintf(tmpstr,"��ESC����");
    myfont.Print2D(menu.x+5,menu.y+5,tmpstr,FONT4,1,1,1);

    //ͼƬ
    texture_select(g_cactus[9]);
    if(iButton==BUTTON_RETURN)
    {
        tPicButton((float)menu.x,(float)menu.y,(float)menu_w,(float)menu_h,0.0f);
    }
    else
    {
        tPicButton((float)menu.x,(float)menu.y,(float)menu_w,(float)menu_h,0.5f);
    }
}

void CQuoridor::drawChessBorad()
{
    // ��ռͼ�����
    float layer=-0.5;

    // �������̵�
    texture_select(g_cactus[1]);
    tPicRectangle((float)board_x,0,(float)g_OpenGL->RCheight,(float)g_OpenGL->RCheight,layer-0.1f);
    // ��������
    texture_select(g_cactus[2]);
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            tPicRectangle(board_x+lace+i*(roadw+wall_w),lace+j*(roadw+wall_w),roadw,roadw,layer);
        }
    }

    // ���Ʋ�ɫ��
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glTranslatef(0,0,layer);
    // ��߻�ɫ����
    glColor3f(0, 1, 0);
    glBegin(GL_QUADS);
    glVertex3f( (float)board_x, 0,  0.0f);
    glVertex3f( (float)board_x, (float)g_OpenGL->RCheight,  0.0f);
    glVertex3f( (float)board_x+lace, (float)g_OpenGL->RCheight-lace,  0.0f);
    glVertex3f( (float)board_x+lace, (float)lace,  0.0f);
    glEnd();
    // �ϱߺ�ɫ����
    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f( (float)board_x, (float)g_OpenGL->RCheight, 0.0f);
    glVertex3f( (float)board_x+g_OpenGL->RCheight, (float)g_OpenGL->RCheight,  0.0f);
    glVertex3f( (float)board_x+g_OpenGL->RCheight-lace, (float)g_OpenGL->RCheight-lace,  0.0f);
    glVertex3f( (float)board_x+lace, (float)g_OpenGL->RCheight-lace,  0.0f);
    glEnd();
    // �ұ���ɫ����
    glColor3f(1, 1, 0);
    glBegin(GL_QUADS);
    glVertex3f((float) board_x+g_OpenGL->RCheight, (float)g_OpenGL->RCheight,  0.0f);
    glVertex3f((float) board_x+g_OpenGL->RCheight, 0,  0.0f);
    glVertex3f((float) board_x+g_OpenGL->RCheight-lace, (float)lace,  0.0f);
    glVertex3f((float) board_x+g_OpenGL->RCheight-lace, (float)g_OpenGL->RCheight-lace,  0.0f);
    glEnd();
    // �±���ɫ����
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f((float)board_x,  0,  0.0f);
    glVertex3f((float)board_x+lace, (float)lace,  0.0f);
    glVertex3f((float)board_x+g_OpenGL->RCheight-lace, (float)lace,  0.0f);
    glVertex3f((float)board_x+g_OpenGL->RCheight,  0,  0.0f);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopAttrib();
}

void CQuoridor::drawAccessory()
{
    char tmpstr[64]={0};
    // ��ռͼ�����
    float layer=-0.5;
    float alp=0.85f;
    // ���������Ϣָʾ��־����
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
    // ������δ����ǻ����ֵ�����ҵ�ָʾ�����ε�
    if (ply_head!=NULL)
    {
        switch (ply_head->color)
        {
        case GD_YELLOW:
            glColor4f(1, 1, 0, alp);
            glVertex3f( player_info_w, 4.0f*player_info_h, layer);
            glVertex3f( (float)board_x, 3.5f*player_info_h, layer);
            glVertex3f( player_info_w, 3.0f*player_info_h, layer);
            break;
        case GD_RED:
            glColor4f(1, 0, 0, alp);
            glVertex3f( player_info_w, 3.0f*player_info_h, layer);
            glVertex3f( (float)board_x, 2.5f*player_info_h, layer);
            glVertex3f( player_info_w, 2.0f*player_info_h, layer);
            break;
        case GD_GREEN:
            glColor4f(0, 1, 0, alp);
            glVertex3f( player_info_w, 2.0f*player_info_h, layer);
            glVertex3f( (float)board_x, 1.5f*player_info_h, layer);
            glVertex3f( player_info_w, 1.0f*player_info_h, layer);
            break;
        case GD_BLUE:
            glColor4f(0, 0, 1, alp);
            glVertex3f( player_info_w, 1.0f*player_info_h, layer);
            glVertex3f( (float)board_x, 0.5f*player_info_h, layer);
            glVertex3f( player_info_w, 0.0f*player_info_h, layer);
            break;
        default:
            break;
        }
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopAttrib();
    // �����Ŀ�̶���ɫ�̶�λ�õ�ָʾ����
    tRectangle(0,3*player_info_h,layer,player_info_w,player_info_h,1,1,0,alp);
    tRectangle(0,2*player_info_h,layer,player_info_w,player_info_h,1,0,0,alp);
    tRectangle(0,1*player_info_h,layer,player_info_w,player_info_h,0,1,0,alp);
    tRectangle(0,0,layer,(float)player_info_w,(float)player_info_h,0,0,1,alp);
    
    for (int i=0; i<4; i++)
    {
        if (g_player[i].id!=ID_CLOSED)
        {
            // ������Ҫע����ͼ�ı��˳��
            texture_select(g_cactus[3+i]);
            tPicRectangle((float)lace,(3-i+1/2.0f)*player_info_h,player_info_w*0.5f,player_info_w*0.5f,layer+0.1f);
            if (g_player[i].id==ID_COMPUTER)
            {   // ���Ƶ��Ե�ͼ��
                texture_select(g_cactus[8]);
                tPicRectangle((float)lace*2.5f+player_info_w*0.5f,(3-i+1/2.0f)*player_info_h,player_info_w*0.28f,player_info_w*0.28f,layer+0.1f);
            }
            sprintf(tmpstr,"ǽʣ��:%u",g_player[i].wall_num_left);
            myfont.Print2D(12,(int)((3-i+1/5.0f)*player_info_h),tmpstr,FONT3,1,1,1);
            if (iGameState==GAME_NETWORK)
            {
                // i ��ɫ ����
                // 0  ��  3
                // 1  ��  1
                // 2  ��  2
                // 3  ��  0
                if (i==1 || i==2)
                {
                    sprintf(tmpstr,"%8s",n_NameAll[i]);
                }
                else 
                {
                    sprintf(tmpstr,"%8s",n_NameAll[3-i]);
                }
                myfont.Print2D(12,(int)((3-i+1/3.0f)*player_info_h),tmpstr,FONT2,1,1,1);
            }
        }
    }

    // ��ť��ͼ
    texture_select(g_cactus[9]);
    sprintf(tmpstr,"���ز˵�");
    myfont.Print2D(menu.x+10,g_OpenGL->RCheight-menu.y-menu_h+5,tmpstr,FONT4,1,1,1);
    if(BUTTON_RETURN==iButton)
    {
        tPicButton((float)menu.x,(float)(g_OpenGL->RCheight-menu.y-menu_h),(float)menu_w,(float)menu_h,0.0f);
    }
    else
    {
        tPicButton((float)menu.x,(float)(g_OpenGL->RCheight-menu.y-menu_h),(float)menu_w,(float)menu_h,0.5f);
    }

    switch (iGameState)
    {
    case GAME_IN_CONFIG:
    case GAME_SINGE:
    case GAME_NETWORK:
    case GAME_WIN:
        sprintf(tmpstr,"ȷ    ��");
        break;
    case GAME_SENDBOX:
        sprintf(tmpstr,"��������");
        break;
    default:
        break;
    }
    myfont.Print2D(menu.x+10,menu.y+5,tmpstr,FONT4,1,1,1);
    // ��ťͼƬ
    if(BUTTON_INIT_OR_CONFIRM==iButton)
    {
        tPicButton((float)menu.x,(float)(menu.y),(float)menu_w,(float)menu_h,0.0f);
    }
    else
    {
        tPicButton((float)menu.x,(float)(menu.y),(float)menu_w,(float)menu_h,0.5f);
    }
}

void CQuoridor::drawPlayerWall()
{
    // ��Ӧ��ɫ����ң��ƣ��죬�̣���
    for (int i=0; i<4; i++)
    {
        if (g_player[i].id!=ID_CLOSED)
        {
            texture_select(g_cactus[3+i]);
            if (g_player[i].x>-1 && g_player[i].y>-1)
            {
                tPicRectangle(board_x+lace+(roadw+wall_w)*g_player[i].x,lace+(roadw+wall_w)*g_player[i].y,roadw,roadw);
            }
        }
    }
    // ����ǽ
    for (size_t i=0; i<wall_vec.size();i+=2)
    {
        texture_select(g_cactus[7]);
        // ����Ǻ�ǽ
        if (wall_vec[i].x%2==0)
        {
            tPicRectangle(board_x+lace+wall_vec[i].x/2*(roadw+wall_w),lace+(int)(wall_vec[i].y/2)*(roadw+wall_w)+roadw,(float)wall_l,(float)wall_w);
        }
        else
        {
            tPicRectangle(board_x+lace+(int)(wall_vec[i].x/2)*(roadw+wall_w)+roadw,lace+wall_vec[i].y/2*(roadw+wall_w),(float)wall_w,(float)wall_l);
        }
    }
}

void CQuoridor::drawPickMask()
{
    //static int det=1;
    int det = 2;
    //static int ctick=0;

    if (ply_head!=NULL)
    {
        // �ֵ�˭�ߣ������ͼ���ϣ�������̬��ʾ
        float cx=board_x+lace+ply_head->x*(roadw+wall_w);
        float cy=        lace+ply_head->y*(roadw+wall_w);
        //���Խ�ջ
        glPushAttrib(GL_CURRENT_BIT);
        glPushMatrix();
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        // ����
        glVertex3f( cx-det,  cy-det,  0.5f);
        glVertex3f( cx-det,  cy-det+roadw/4.0f,  0.5f);	// ����
        glVertex3f( cx-det,  cy-det,  0.5f);
        glVertex3f( cx-det+roadw/4.0f,  cy-det,  0.5f);	// ����
        // ����
        glVertex3f( cx-det,  cy+roadw+det,  0.5f);
        glVertex3f( cx-det+roadw/4.0f,  cy+roadw+det,  0.5f);	// ����
        glVertex3f( cx-det,  cy+roadw+det,  0.5f);
        glVertex3f( cx-det,  cy+roadw+det-roadw/4,  0.5f);	// ����
        // ����
        glVertex3f( cx+roadw+det,  cy+roadw+det,  0.5f);
        glVertex3f( cx+roadw+det-roadw/4,  cy+roadw+det,  0.5f);	// ����
        glVertex3f( cx+roadw+det,  cy+roadw+det,  0.5f);
        glVertex3f( cx+roadw+det,  cy+roadw+det-roadw/4,  0.5f);	// ����
        // ����
        glVertex3f( cx+roadw+det,  cy-det,  0.5f);
        glVertex3f( cx+roadw+det-roadw/4,  cy-det,  0.5f);	// ����
        glVertex3f( cx+roadw+det,  cy-det,  0.5f);
        glVertex3f( cx+roadw+det,  cy-det+roadw/4,  0.5f);	// ����
        glEnd();
        glEnable(GL_TEXTURE_2D);
        glPopMatrix();
        glPopAttrib();
        // ��ʱע�͵���̬��ʾ
        //if (ctick>g_refresh_rate/10)
        //{
        //	det++;
        //	if (det>3)
        //	{
        //		det=1;
        //	}
        //	ctick=0;
        //}
        //ctick++;
    }
    if (pickup.x < 0 && pickup.y < 0)
    {
        return ;
    }
    else if (pickup.x%2==0 && pickup.y%2==0)
    {
#ifdef __DEBUG__
        if (iGameState==GAME_SINGE || iGameState==GAME_NETWORK || iGameState==GAME_SENDBOX)
#else
        if (iGameState==GAME_SINGE || iGameState==GAME_NETWORK)
#endif
        {
            // �����Ժ󻻸���ͼ
            tRectangle(board_x+lace+pickup.x/2*(roadw+wall_w),lace+pickup.y/2*(roadw+wall_w),0,roadw,roadw,0.1f,0.5f,1,0.6f);
            // ������ҿ���Ԥѡ��
            for (size_t i=0; i<preselect_pos.size();i++)
            {
                tRectangle(board_x+lace+preselect_pos[i].x/2*(roadw+wall_w),lace+preselect_pos[i].y/2*(roadw+wall_w),0,roadw,roadw,0.1f,0.5f,1,0.6f);
            }
#if 0
            // ����ģʽʱ����ʾ��ҵ�Ŀ�ĵ�
            for (int i=0; i<9; i++)
            {
                switch(ply_head->color)
                {
                case GD_YELLOW:
                    tRectangle(board_x+lace+8*(roadw+wall_w)+roadw-wall_w, lace+i*(roadw+wall_w),0,(float)wall_w,roadw,0.9f,0.9f,0.0f,0.6f);
                    break;
                case GD_RED:
                    tRectangle(board_x+lace+i*(roadw+wall_w), (float)lace,0,roadw,(float)wall_w,1.0f,0.0f,0.0f,0.8f);
                    break;
                case GD_GREEN:
                    tRectangle((float)board_x+lace, lace+i*(roadw+wall_w),0,(float)wall_w,roadw,0.0f,0.9f,0,0.6f);
                    break;
                case GD_BLUE:
                    tRectangle(board_x+lace+i*(roadw+wall_w),lace+8*(roadw+wall_w)+roadw-wall_w,0,roadw,(float)wall_w,0.0f,0.0f,0.9f,0.6f);
                    break;
                default:
                    break;
                }
            }
#endif
        }
        else if (iGameState==GAME_SENDBOX)
        {
            // �����Ժ󻻸���ͼ
            tRectangle(board_x+lace+pickup.x/2*(roadw+wall_w),lace+pickup.y/2*(roadw+wall_w),0,roadw,roadw,0.1f,0.5f,1,0.6f);
        }
    }
    else if (pickup.x%2==0 && pickup.y%2!=0)
    {
        // ������Ϊż����������Ϊ������Ϊ��ǽ
        tRectangle(board_x+lace+pickup.x/2*(roadw+wall_w),lace+(int)(pickup.y/2)*(roadw+wall_w)+roadw,0,(float)wall_l*4/9,(float)wall_w,1.0f,1,0,0.6f);
    }
    else if (pickup.x%2!=0 && pickup.y%2==0)
    {
        // ������Ϊ������������Ϊż����Ϊ��ǽ
        tRectangle(board_x+lace+(int)(pickup.x/2)*(roadw+wall_w)+roadw,lace+pickup.y/2*(roadw+wall_w),0,(float)wall_w,(float)wall_l*4/9,1.0f,1,0,0.6f);
    }

    return ;
}
// ����ȷ�ϴ���
//void CQuoridor::drawConfirm()
//{
//    float layer=0.3f;
//    float tri_w=g_OpenGL->RCwidth/3.0f;
//    float tri_h=g_OpenGL->RCheight/3.0f;
//    //���Ʊ�����͸�����ƴ���
//    tRectangle(tri_w-menu_w,tri_h,layer,tri_w+2*menu_w,tri_h,0.0f,0.0f,0.0f,0.7f);
//}

void CQuoridor::resetGameData()
{
    // ���vector
    wall_vec.swap(std::vector<pos2d>());
    // �������ݳ�ʼ��
    void* tmpp = (void*)gameData;
    memset(tmpp,0,sizeof(gameData));
    // �������λ������
    g_player[0].color=GD_YELLOW;
    g_player[0].x=0;
    g_player[0].y=4;
    g_player[0].wall_num_left=0;
    g_player[0].next=NULL;
    g_player[1].color=GD_RED;
    g_player[1].x=4;
    g_player[1].y=8;
    g_player[1].wall_num_left=0;
    g_player[1].next=NULL;
    g_player[2].color=GD_GREEN;
    g_player[2].x=8;
    g_player[2].y=4;
    g_player[2].wall_num_left=0;
    g_player[2].next=NULL;
    g_player[3].color=GD_BLUE;
    g_player[3].x=4;
    g_player[3].y=0;
    g_player[3].wall_num_left=0;
    g_player[3].next=NULL;
    switch (iGameState)
    {
    case GAME_IN_CONFIG:
        g_player[0].id=ID_CLOSED;
        g_player[1].id=ID_COMPUTER;
        g_player[2].id=ID_CLOSED;
        g_player[3].id=ID_HUMAN;
        break;
    case GAME_SENDBOX:
        g_player[0].id=ID_HUMAN;
        g_player[1].id=ID_HUMAN;
        g_player[2].id=ID_HUMAN;
        g_player[3].id=ID_HUMAN;
        gameData[2*g_player[0].x][2*g_player[0].y]=GD_YELLOW;
        gameData[2*g_player[1].x][2*g_player[1].y]=GD_RED;
        gameData[2*g_player[2].x][2*g_player[2].y]=GD_GREEN;
        gameData[2*g_player[3].x][2*g_player[3].y]=GD_BLUE;
        break;
    case GAME_NET_CONFIG:
        g_player[0].id=ID_CLOSED;
        g_player[1].id=ID_CLOSED;
        g_player[2].id=ID_CLOSED;
        g_player[3].id=ID_CLOSED;
        break;
    default:
        break;
    }
    // ���ѡȡ��λ��
    pickup.x=-1;
    pickup.y=-1;
    // ��һ�װ����ͷָ��
    ply_head=NULL;
    // ���ʤ����־
    win_flag=GD_BLANK;
    // �ر��������
    n_net->closeNetWork();
    n_netWorkStatus=0;
    //�������·��
    best_path.swap(std::vector<pos2d>());
}

void CQuoridor::drawInConfig()
{
    float layer=-0.2f;
    // ����ѡ��ʱ�ı�����
    //���Խ�ջ
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    glColor4f(1, 1, 0, 0.6f);
    glBegin(GL_POLYGON);
    glVertex3f( player_info_w, 3.5f*player_info_h, layer);
    glVertex3f( (float)board_x, 3.9f*player_info_h, layer);
    glVertex3f( (float)board_x+g_OpenGL->RCheight, 3.9f*player_info_h, layer);
    glVertex3f( (float)board_x+g_OpenGL->RCheight, 3.1f*player_info_h, layer);
    glVertex3f( (float)board_x, 3.1f*player_info_h, layer);
    glEnd();
    glColor4f(1, 0, 0, 0.6f);
    glBegin(GL_POLYGON);
    glVertex3f( player_info_w, 2.5f*player_info_h, layer);
    glVertex3f( (float)board_x, 2.9f*player_info_h, layer);
    glVertex3f( (float)board_x+g_OpenGL->RCheight, 2.9f*player_info_h, layer);
    glVertex3f( (float)board_x+g_OpenGL->RCheight, 2.1f*player_info_h, layer);
    glVertex3f( (float)board_x, 2.1f*player_info_h, layer);
    glEnd();
    glColor4f(0, 1, 0, 0.6f);
    glBegin(GL_POLYGON);
    glVertex3f( player_info_w, 1.5f*player_info_h, layer);
    glVertex3f( (float)board_x, 1.9f*player_info_h, layer);
    glVertex3f( (float)board_x+g_OpenGL->RCheight, 1.9f*player_info_h, layer);
    glVertex3f( (float)board_x+g_OpenGL->RCheight, 1.1f*player_info_h, layer);
    glVertex3f( (float)board_x, 1.1f*player_info_h, layer);
    glEnd();
    glColor4f(0, 0, 1, 0.6f);
    glBegin(GL_POLYGON);
    glVertex3f( player_info_w, 0.5f*player_info_h, layer);
    glVertex3f( (float)board_x, 0.9f*player_info_h, layer);
    glVertex3f( (float)board_x+g_OpenGL->RCheight, 0.9f*player_info_h, layer);
    glVertex3f( (float)board_x+g_OpenGL->RCheight, 0.1f*player_info_h, layer);
    glVertex3f( (float)board_x, 0.1f*player_info_h, layer);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopAttrib();
    // �˴�ע����ö��ֵ�Ķ�Ӧ��ϵ˳��
    char *cfgstr[]={" ��  �� "," ��  �� "," ��  �� "};
    // ��ť��ͼ
    texture_select(g_cactus[9]);
    for (int j=0; j<4; j++)
    {
        for (int i=0; i<3; i++)
        {
            if(i==g_player[3-j].id)
            {
                myfont.Print2D((int)(board_x+(i*2.3+1)*(roadw+wall_w))+10,(int)((player_info_h-menu_h)/2)+5+(int)player_info_h*j,cfgstr[i],FONT4,1,1,1);
                tPicButton((float)board_x+(i*2.3f+1)*(roadw+wall_w),(player_info_h-menu_h)/2+player_info_h*j,(float)menu_w,(float)menu_h,0.0f);
            }
            else
            {
                myfont.Print2D((int)(board_x+(i*2.3+1)*(roadw+wall_w))+10,(int)((player_info_h-menu_h)/2)+5+(int)player_info_h*j,cfgstr[i],FONT4,0,0,0);
                tPicButton((float)board_x+(i*2.3f+1)*(roadw+wall_w),(player_info_h-menu_h)/2+player_info_h*j,(float)menu_w,(float)menu_h,0.5f);
            }
        }
    }
}
// ����������ʱ����Ҫ����Ϸ����,��Ϸ����Ŀ���ʵ��(�����������ʱ����Ӧ��)
// ��ڲ���Ϊ�Ƿ�������ģʽ���Ƿ�������緢��
void CQuoridor::playerActionRule(bool network)
{
    char netstr[16]={0};        // ���緢�����û���
    // �Ѿ����ڵ�ǽ��λ�ã��ǲ��ܱ�ѡȡ��
    if (gameData[arr.x][arr.y]==GD_WALL)
    {
        return ;
    }
    // ���֮ǰû��ѡȡ�κ�λ��
    if ( pickup.x <0 && pickup.y <0 )
    {
        // �հ׵����λ�����߲��ܿ��Ƶ����λ
        if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]!=ply_head->color)
        {
            return;
        }
        pickup.x=arr.x;
        pickup.y=arr.y;
        // ���ѡ���ǵ�ǰ���λ��
        if (gameData[pickup.x][pickup.y]==ply_head->color)
        {
            playerMovablePos(pickup);
        }
        // ������if else ������return�������൱��return
    }
    // ֮ǰ��ѡȡ��λ��
    else
    {
        // ������ѡȡ��λ��,������������ͬλ�ã����ʼ����
        if (arr == pickup)
        {
            pickup.x = -1;
            pickup.y = -1;
            // �����ҿ��ߴ�ѡλ��(��Ȼ����Ҳ�ǿ��Ե�)
            preselect_pos.clear();
            return ;    // �������������ͬ������Ϊ��ѡ
        }
        // ��������ٴε��ʱ��ѡȡ��λ������������ӿ����ƶ���λ��
        // Ҫ�ߵ�λ���ڿ���λ�õ�������
        //else if (0==arr.x%2 && 0==arr.y%2 && 0==pickup.x%2 && 0==pickup.y%2)
        else if (find(preselect_pos.begin(),preselect_pos.end(),arr)!=preselect_pos.end())
        {
            // ������������뵽�������Ӵ���׶�
            char tmp=0;
            // ����
            tmp=gameData[arr.x][arr.y];
            gameData[arr.x][arr.y]=gameData[pickup.x][pickup.y];
            gameData[pickup.x][pickup.y]=tmp;
            // ��Ŀ��λ���ϣ�������ұ���
            switch (gameData[arr.x][arr.y])
            {
            case GD_BLANK:
                break;
            case GD_YELLOW:
                g_player[0].x=arr.x/2;
                g_player[0].y=arr.y/2;
                if (g_player[0].x==8)
                {
                    win_flag=GD_YELLOW;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_RED:
                g_player[1].x=arr.x/2;
                g_player[1].y=arr.y/2;
                if (g_player[1].y==0)
                {
                    win_flag=GD_RED;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_GREEN:
                g_player[2].x=arr.x/2;
                g_player[2].y=arr.y/2;
                if (g_player[2].x==0)
                {
                    win_flag=GD_GREEN;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_BLUE:
                g_player[3].x=arr.x/2;
                g_player[3].y=arr.y/2;
                if (g_player[3].y==8)
                {
                    win_flag=GD_BLUE;
                    iGameState=GAME_WIN;
                }
                break;
            default:
                break;
            }
            // �������Ҫ���㷨��ת����������ƶ���
            if (g_sound==1)
            {
                // ��������ƶ���Ч
                sndPlaySound("data/sound/player_move.wav",SND_ASYNC);
            }
        if (network)
        {
            // ��������ƶ��������緢������ƶ��İ�
            // �����пͻ�������P1M45,P������Ϊ�����ɫ���룬
            // M��������ƶ��������ִ����ƶ�������꣬���һλF������Ϸ��������ǰ���ʤ��
            if (iGameState==GAME_WIN)
            {
                sprintf(netstr,"P%1dM%1d%1dF",ply_head->color,arr.x/2,arr.y/2);
            }
            else
            {
                sprintf(netstr,"P%1dM%1d%1d_",ply_head->color,arr.x/2,arr.y/2);
            }
            n_net->NetWorkSendData(n_netWorkStatus,netstr,strlen(netstr)+1);
        }   // end if network
            goto ACTION_RULE_EXIT;
        }
        // ���ߵ����Ӧ����ֻ���Ѿ�ѡ��ǽ�������
        // ���ﵥ�����ˣ�����ֵ���˵�ǰ��ң���ѡ����Ҵ���
        else if (gameData[arr.x][arr.y]==ply_head->color)
        {
            pickup.x=arr.x;
            pickup.y=arr.y;
            playerMovablePos(pickup);
        }
        // ��ѡǽ�Ĺ����У��������з�ǽ��λ��
        else if ((arr.x+arr.y)%2!=1)
        {
            return ;
        }
        // ����ϴ�ѡȡ��λ��Ҳ��ǽ�����ѡȡ��Ҳ��ǽ���������ѡȡ��λ���ǿ�
        // ����ѡȡ��ͬλ���Ѿ����ʼ����
        // �����ǲ���ʵ��ǽλ�õ�Ψһ��ڣ������������ж����ǽʣ�����Ƿ����
        else if ((pickup.x+pickup.y)%2==1 
            && (arr.x+arr.y)%2==1 
            && GD_BLANK == gameData[arr.x][arr.y] 
            && ply_head->wall_num_left>0)
        {
            // ����Ǻ�ǽ,�������ѡ�ĺ��ϴ�ѡ����ͬһ����
            if (pickup.x%2==0&&arr.y==pickup.y)
            {   // ������ѡ������һ��ѡ�����һ��λ��
                // �����������ӵ��м�λ���ǿ��õ�
                if(arr.x==pickup.x-2&&gameData[arr.x+1][arr.y]==GD_BLANK)
                {
                    // ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
                    wall_vec.push_back(arr);
                    wall_vec.push_back(pickup);
                    // ������Ϸ�㷨����,����ע�⣬��������ǽλ�õ��м����Ӵ�Ҳ��ֵ
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x+1][arr.y]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
            if (network)
            {
                // �����пͻ�������P1W070807090710,P������Ϊ�����ɫ���룬
                // W������ǽ��������Ϣ������д�����������ߣ���ʾǽ��λ�ã����ڻ���ԭ����Ҫ�ϸ���˳��
                sprintf(netstr,"P%1dW%2d%2d%2d%2d%2d%2d",ply_head->color,arr.x,arr.y,(arr.x+pickup.x)/2,(arr.y+pickup.y)/2,pickup.x,pickup.y);
            }
                    goto RULE_WALL_EXIT;
                }
                // ��������ѡ������һ���ұ�һ��λ��
                // �����������ӵ��м�λ���ǿ��õ�
                else if(arr.x==pickup.x+2&&gameData[arr.x-1][arr.y]==GD_BLANK)
                {
                    // ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
                    wall_vec.push_back(pickup);
                    wall_vec.push_back(arr);
                    // ������Ϸ�㷨����
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x-1][arr.y]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
            if (network)
            {
                // �����пͻ�������P1W070807090710,P������Ϊ�����ɫ���룬
                // W������ǽ��������Ϣ������д�����������ߣ���ʾǽ��λ�ã����ڻ���ԭ����Ҫ�ϸ���˳��
                sprintf(netstr,"P%1dW%2d%2d%2d%2d%2d%2d",ply_head->color,pickup.x,pickup.y,(arr.x+pickup.x)/2,(arr.y+pickup.y)/2,arr.x,arr.y);
            }
                    goto RULE_WALL_EXIT;
                }
            }
            // �������ǽ���������ѡ�ĺ��ϴ�ѡ����ͬһ����
            else if(pickup.y%2==0&&arr.x==pickup.x)
            {   // ������ѡ�������һ��ѡ�������һ��
                // �����������ӵ��м�λ���ǿ��õ�
                if (arr.y==pickup.y-2&&gameData[arr.x][arr.y+1]==GD_BLANK)
                {
                    // ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
                    wall_vec.push_back(arr);
                    wall_vec.push_back(pickup);
                    // ������Ϸ�㷨����
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x][arr.y+1]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
            if (network)
            {
                // �����пͻ�������P1W070807090710,P������Ϊ�����ɫ���룬
                // W������ǽ��������Ϣ������д�����������ߣ���ʾǽ��λ�ã����ڻ���ԭ����Ҫ�ϸ���˳��
                sprintf(netstr,"P%1dW%2d%2d%2d%2d%2d%2d",ply_head->color,arr.x,arr.y,(arr.x+pickup.x)/2,(arr.y+pickup.y)/2,pickup.x,pickup.y);
            }
                    goto RULE_WALL_EXIT;
                }
                // ������ѡ�������һ��ѡ�������һ��
                // �����������ӵ��м�λ���ǿ��õ�
                else if (arr.y==pickup.y+2&&gameData[arr.x][arr.y-1]==GD_BLANK)
                {
                    // ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
                    wall_vec.push_back(pickup);
                    wall_vec.push_back(arr);
                    // ������Ϸ�㷨����
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x][arr.y-1]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
            if (network)
            {
                // �����пͻ�������P1W070807090710,P������Ϊ�����ɫ���룬
                // W������ǽ��������Ϣ������д�����������ߣ���ʾǽ��λ�ã����ڻ���ԭ����Ҫ�ϸ���˳��
                sprintf(netstr,"P%1dW%2d%2d%2d%2d%2d%2d",ply_head->color,pickup.x,pickup.y,(arr.x+pickup.x)/2,(arr.y+pickup.y)/2,arr.x,arr.y);
            }
                    goto RULE_WALL_EXIT;
                }
            }
            // ����ʹ��goto��䣬����ܹ��ɹ�����ǽ����ô�Ѿ�����ȥ�ˣ�
            // ������������һ���ɷ�ǽ����������Ϊ����ѡȡ
            pickup.x=arr.x;
            pickup.y=arr.y;
        }
        // ���������һ����Ϊ����ѡȡǽ
        else if ( gameData[arr.x][arr.y]!=GD_WALL )
        {
            pickup.x=arr.x;
            pickup.y=arr.y;
            // �����ҿ��ߴ�ѡλ��
            preselect_pos.clear();
        }
    }
    return ;
RULE_WALL_EXIT:
    // �����жϣ������ǽ�Ƿ�Ϸ����ж�������·ŵ�һ��ǽ��ʹ���κ�һ������޽⣬��ô��ǽ�Ƿ�
    // ������Ϸ�����ô��Ҫ��ԭԭ������Ϸ����
    if (!judgeWallLegal())
    {
        // ��ԭ��Ϸ�㷨����
        gameData[arr.x][arr.y]=GD_BLANK;
        gameData[(arr.x+pickup.x)/2][(arr.y+pickup.y)/2]=GD_BLANK;
        gameData[pickup.x][pickup.y]=GD_BLANK;
        // ������ѹ���ǽ����(������)
        wall_vec.pop_back();
        wall_vec.pop_back();
        // ���Ԥѡλ��
        pickup.x=-1;
        pickup.y=-1;
        // �����ҿ��ߴ�ѡλ��
        preselect_pos.clear();
        return ;
    }
    if (g_sound==1)
    {
        // ����ǽ����Ч
        sndPlaySound("data/sound/wall_set.wav",SND_ASYNC);
    }
    if (network)
    {
        n_net->NetWorkSendData(n_netWorkStatus,netstr,strlen(netstr)+1);
    }
    // ��ǰ��ҵĿ���ǽ����1
    ply_head->wall_num_left--;
ACTION_RULE_EXIT:
    // �����ѡ�����ƶ������ô����ҵ��ж��ͽ����ˣ�����ȨӦ�ý�����һλ���
    ply_head=ply_head->next;
    // ���Ԥѡλ��
    pickup.x=-1;
    pickup.y=-1;
    // �����ҿ��ߴ�ѡλ��
    preselect_pos.clear();
    return ;
}

// �˺�����ǰ���ǣ�selected����������ѡȡ�Ŀ���λ��
// �˺����ĳ����ǣ�preselect_pos��ҿ���λ�õ�����
void CQuoridor::playerMovablePos( pos2d selected )
{
    // ��ҿ��ߵ�λ�ã��и�����ֵ����ȶ���ٷ����򣬶������̱�Ե�Ƿ���������û��˵��
    // ����Ĵ����У���һ���ط���ע��д���������жϵ�����
    pos2d tmppos;
    // ���vector
    preselect_pos.swap(std::vector<pos2d>());
    // �����ж����������ĸ������Ƿ���ߣ��������������
    // ���ֱ�ӵ��ĸ�������ߣ���ôб���Ľǿ���ֱ�Ӳ��ж�
    // Ϊtrueʱ����Ҫ�����жϣ��ĸ�����ʱ���ҵ��˾���Ϊfalse
    //bool left = true;
    //bool right = true;
    //bool up = true;
    //bool bottom = true;

    // ������ж���ҿ��ߵĿ���λ��
    // OOOOO
    // O@0OO
    // OOOOO
    // ������������һ�У������û��ǽ�������һ�����
    if ( selected.x > 0 
        && gameData[selected.x-1][selected.y]!=GD_WALL 
        && gameData[selected.x-2][selected.y]==GD_BLANK )
    {
        tmppos.x=selected.x-2;
        tmppos.y=selected.y;
        preselect_pos.push_back(tmppos);
    }
    // OOOOO
    // @X0OO
    // OOOOO
    // �������λ����ߵڶ��л��һ�У����������ң������������û��ǽ�������λ�ÿ���
    else if (selected.x > 2
        && gameData[selected.x-1][selected.y]!=GD_WALL 
        && gameData[selected.x-2][selected.y]!=GD_BLANK 
        && gameData[selected.x-3][selected.y]!=GD_WALL 
        && gameData[selected.x-4][selected.y]==GD_BLANK )
    {
        tmppos.x=selected.x-4;
        tmppos.y=selected.y;
        preselect_pos.push_back(tmppos);
    }
    // OOOOO
    // OO0@O
    // OOOOO
    // ����������ұ�һ�У����ұ�û��ǽ�����ұ�һ�����
    if ( selected.x < 16 
        && gameData[selected.x+1][selected.y]!=GD_WALL 
        && gameData[selected.x+2][selected.y]==GD_BLANK )
    {
        tmppos.x=selected.x+2;
        tmppos.y=selected.y;
        preselect_pos.push_back(tmppos);
    }
    // OOOOO
    // OO0X@
    // OOOOO
    // �������λ���ұߵڶ��л��һ�У����ұ�����ң����ұ�������û��ǽ�����ұ�λ�ÿ���
    else if (selected.x < 14
        && gameData[selected.x+1][selected.y]!=GD_WALL 
        && gameData[selected.x+2][selected.y]!=GD_BLANK 
        && gameData[selected.x+3][selected.y]!=GD_WALL 
        && gameData[selected.x+4][selected.y]==GD_BLANK )
    {
        tmppos.x=selected.x+4;
        tmppos.y=selected.y;
        preselect_pos.push_back(tmppos);
    }
    // OOOOO
    // OOOOO
    // OO0OO
    // OO@OO
    // OOOOO
    // ����������±�һ�У����±�û��ǽ�����±�һ�����
    if ( selected.y > 0 
        && gameData[selected.x][selected.y-1]!=GD_WALL 
        && gameData[selected.x][selected.y-2]==GD_BLANK )
    {
        tmppos.x=selected.x;
        tmppos.y=selected.y-2;
        preselect_pos.push_back(tmppos);
    }
    // OOOOO
    // OOOOO
    // OO0OO
    // OOXOO
    // OO@OO
    // �������λ���±ߵڶ��л��һ�У����±�����ң����±�����±�û��ǽ�����±�λ�ÿ���
    else if (selected.y > 2
        && gameData[selected.x][selected.y-1]!=GD_WALL 
        && gameData[selected.x][selected.y-2]!=GD_BLANK 
        && gameData[selected.x][selected.y-3]!=GD_WALL 
        && gameData[selected.x][selected.y-4]==GD_BLANK )
    {
        tmppos.x=selected.x;
        tmppos.y=selected.y-4;
        preselect_pos.push_back(tmppos);
    }
    // OOOOO
    // OO@OO
    // OO0OO
    // OOOOO
    // OOOOO
    // ����������ϱ�һ�У����ϱ�û��ǽ�����ϱ�һ�����
    if ( selected.y < 16 
        && gameData[selected.x][selected.y+1]!=GD_WALL 
        && gameData[selected.x][selected.y+2]==GD_BLANK )
    {
        tmppos.x=selected.x;
        tmppos.y=selected.y+2;
        preselect_pos.push_back(tmppos);
    }
    // OO@OO
    // OOXOO
    // OO0OO
    // OOOOO
    // OOOOO
    // �������λ���ϱߵڶ��л��һ�У����ϱ�����ң����ϱ�����ϱ�û��ǽ�����ϱ�λ�ÿ���
    else if (selected.y < 14
        && gameData[selected.x][selected.y+1]!=GD_WALL 
        && gameData[selected.x][selected.y+2]!=GD_BLANK 
        && gameData[selected.x][selected.y+3]!=GD_WALL 
        && gameData[selected.x][selected.y+4]==GD_BLANK )
    {
        tmppos.x=selected.x;
        tmppos.y=selected.y+4;
        preselect_pos.push_back(tmppos);
    }
//-----------------
    // OO-OO
    // O@XOO
    // |X0OO
    // OOOOO
    // �����Ͻǿ��ߵ���������ж�(�������߽�ֵ)
    if ( selected.x > 2 && selected.y < 14 && gameData[selected.x-2][selected.y+2]==GD_BLANK )
    {   // �������еж��������ߵ��˵������ǽ���������еж������������˵��ϱ���ǽ
        if ( (gameData[selected.x-1][selected.y]!=GD_WALL 
            && gameData[selected.x-2][selected.y]!=GD_BLANK 
            && gameData[selected.x-3][selected.y]==GD_WALL 
            && gameData[selected.x-2][selected.y+1]!=GD_WALL) 
        || (gameData[selected.x][selected.y+1]!=GD_WALL 
            && gameData[selected.x][selected.y+2]!=GD_BLANK 
            && gameData[selected.x][selected.y+3]==GD_WALL 
            && gameData[selected.x-1][selected.y+2]!=GD_WALL) )
        {
            tmppos.x=selected.x-2;
            tmppos.y=selected.y+2;
            preselect_pos.push_back(tmppos);
        }
        // ������ߵ������߻�����ң����ϱߵ��ϱ߻��е���
        else if ( (gameData[selected.x-1][selected.y]!=GD_WALL 
            && gameData[selected.x-2][selected.y]!=GD_BLANK 
            && gameData[selected.x-4][selected.y]!=GD_BLANK 
            && gameData[selected.x-2][selected.y+1]!=GD_WALL) 
        || (gameData[selected.x][selected.y+1]!=GD_WALL 
            && gameData[selected.x][selected.y+2]!=GD_BLANK 
            && gameData[selected.x][selected.y+4]!=GD_BLANK 
            && gameData[selected.x-1][selected.y+2]!=GD_WALL) )
        {
            tmppos.x=selected.x-2;
            tmppos.y=selected.y+2;
            preselect_pos.push_back(tmppos);
        }
    }
    // O-OO
    // @XOO
    // X0OO
    // OOOO
    // ������λ�ڵڶ���,���Ͻǿ��������(��������)
    else if ( selected.x == 2 && selected.y < 14 
        && gameData[0][selected.y+2]==GD_BLANK )
    {   // ����߽��������ѵ�һ�������� (option)
        if ( /*(gameData[1][selected.y]!=GD_WALL && gameData[0][selected.y]!=GD_BLANK && gameData[0][selected.y+1]!=GD_WALL) || */
            (gameData[2][selected.y+1]!=GD_WALL 
            && gameData[2][selected.y+2]!=GD_BLANK 
            && gameData[2][selected.y+3]==GD_WALL 
            && gameData[1][selected.y+2]!=GD_WALL ) )
        {
            tmppos.x=selected.x-2;
            tmppos.y=selected.y+2;
            preselect_pos.push_back(tmppos);
        }
    }
    // O@XOO
    // |X0OO
    // OOOOO
    // ������λ�ڵڶ���,���Ͻǿ��������(��������)
    else if ( selected.x > 2 && selected.y == 14 
        && gameData[selected.x-2][16]==GD_BLANK )
    {   // ����߽��������ѵ�һ�������� (option)
        if ( /*(gameData[selected.x][15]!=GD_WALL && gameData[selected.x][16]!=GD_BLANK && gameData[selected.x-1][16]!=GD_WALL) || */
            (gameData[selected.x-1][14]!=GD_WALL 
            && gameData[selected.x-2][14]!=GD_BLANK 
            && gameData[selected.x-3][14]==GD_WALL 
            && gameData[selected.x-2][15]!=GD_WALL ) )
        {
            tmppos.x=selected.x-2;
            tmppos.y=selected.y+2;
            preselect_pos.push_back(tmppos);
        }
    }
    // OO-OO
    // OOX@O
    // OO0X|
    // OOOOO
    // �����Ͻǿ��ߵ���������ж�(�������߽�ֵ)
    if ( selected.x < 14 && selected.y < 14 && gameData[selected.x+2][selected.y+2]==GD_BLANK )
    {
        if ( (gameData[selected.x+1][selected.y]!=GD_WALL 
            && gameData[selected.x+2][selected.y]!=GD_BLANK 
            && gameData[selected.x+3][selected.y]==GD_WALL 
            && gameData[selected.x+2][selected.y+1]!=GD_WALL) 
        || (gameData[selected.x][selected.y+1]!=GD_WALL 
            && gameData[selected.x][selected.y+2]!=GD_BLANK 
            && gameData[selected.x][selected.y+3]==GD_WALL 
            && gameData[selected.x+1][selected.y+2]!=GD_WALL) )
        {
            tmppos.x=selected.x+2;
            tmppos.y=selected.y+2;
            preselect_pos.push_back(tmppos);
        }
        else if ( (gameData[selected.x+1][selected.y]!=GD_WALL 
            && gameData[selected.x+2][selected.y]!=GD_BLANK 
            && gameData[selected.x+4][selected.y]!=GD_BLANK 
            && gameData[selected.x+2][selected.y+1]!=GD_WALL) 
        || (gameData[selected.x][selected.y+1]!=GD_WALL 
            && gameData[selected.x][selected.y+2]!=GD_BLANK 
            && gameData[selected.x][selected.y+4]!=GD_BLANK 
            && gameData[selected.x+1][selected.y+2]!=GD_WALL) )
        {
            tmppos.x=selected.x+2;
            tmppos.y=selected.y+2;
            preselect_pos.push_back(tmppos);
        }
    }
    // OO-O
    // OOX@
    // OO0X
    // OOOO
    // ������λ�������ڶ���,���Ͻǿ��������(��������)
    else if ( selected.x == 14 && selected.y < 14 && gameData[16][selected.y+2]==GD_BLANK )
    {
        if ( /*(gameData[15][selected.y]!=GD_WALL && gameData[16][selected.y]!=GD_BLANK && gameData[16][selected.y+1]!=GD_WALL) || */
            (gameData[14][selected.y+1]!=GD_WALL 
            && gameData[14][selected.y+2]!=GD_BLANK 
            && gameData[14][selected.y+3]==GD_WALL 
            && gameData[15][selected.y+2]!=GD_WALL ) )
        {
            tmppos.x=selected.x+2;
            tmppos.y=selected.y+2;
            preselect_pos.push_back(tmppos);
        }
    }
    // OOX@O
    // OO0X|
    // OOOOO
    // ������λ�ڵڶ���,���Ͻǿ��������(��������)
    else if ( selected.x < 14 && selected.y == 14 && gameData[selected.x+2][16]==GD_BLANK )
    {
        if ( /*(gameData[selected.x][15]!=GD_WALL && gameData[selected.x][16]!=GD_BLANK && gameData[selected.x+1][16]!=GD_WALL ) || */
            (gameData[selected.x+1][14]!=GD_WALL 
            && gameData[selected.x+2][14]!=GD_BLANK 
            && gameData[selected.x+3][14]==GD_WALL 
            && gameData[selected.x+2][15]!=GD_WALL ) )
        {
            tmppos.x=selected.x+2;
            tmppos.y=selected.y+2;
            preselect_pos.push_back(tmppos);
        }
    }
    // OOOOO
    // |X0OO
    // O@XOO
    // OO-OO
    // �����½ǿ��ߵ���������ж�(�������߽�ֵ)
    if ( selected.x > 2 && selected.y >2 && gameData[selected.x-2][selected.y-2]==GD_BLANK )
    {
        if ( (gameData[selected.x-1][selected.y]!=GD_WALL 
            && gameData[selected.x-2][selected.y]!=GD_BLANK 
            && gameData[selected.x-3][selected.y]==GD_WALL 
            && gameData[selected.x-2][selected.y-1]!=GD_WALL ) 
        || (gameData[selected.x][selected.y-1]!=GD_WALL 
            && gameData[selected.x][selected.y-2]!=GD_BLANK 
            && gameData[selected.x][selected.y-3]==GD_WALL 
            && gameData[selected.x-1][selected.y-2]!=GD_WALL ) )
        {
            tmppos.x=selected.x-2;
            tmppos.y=selected.y-2;
            preselect_pos.push_back(tmppos);
        }
        else if ( (gameData[selected.x-1][selected.y]!=GD_WALL 
            && gameData[selected.x-2][selected.y]!=GD_BLANK 
            && gameData[selected.x-4][selected.y]!=GD_BLANK 
            && gameData[selected.x-2][selected.y-1]!=GD_WALL ) 
        || (gameData[selected.x][selected.y-1]!=GD_WALL 
            && gameData[selected.x][selected.y-2]!=GD_BLANK 
            && gameData[selected.x][selected.y-4]!=GD_BLANK 
            && gameData[selected.x-1][selected.y-2]!=GD_WALL ) )
        {
            tmppos.x=selected.x-2;
            tmppos.y=selected.y-2;
            preselect_pos.push_back(tmppos);
        }
    }
    // OOOO
    // X0OO
    // @XOO
    // O-OO
    // ������λ�ڵڶ���,���½ǿ��������(��������)
    else if ( selected.x == 2 && selected.y > 2 && gameData[0][selected.y-2]==GD_BLANK )
    {   // ����߽��������ѵ�һ��������
        if ( /*(gameData[1][selected.y]!=GD_WALL && gameData[0][selected.y]!=GD_BLANK && gameData[0][selected.y-1]!=GD_WALL ) || */
            (gameData[2][selected.y-1]!=GD_WALL 
            && gameData[2][selected.y-2]!=GD_BLANK 
            && gameData[2][selected.y-3]==GD_WALL 
            && gameData[1][selected.y-2]!=GD_WALL ) )
        {
            tmppos.x=selected.x-2;
            tmppos.y=selected.y-2;
            preselect_pos.push_back(tmppos);
        }
    }
    // OOOOO
    // |X0OO
    // O@XOO
    // ������λ�ڵڶ���,���½ǿ��������(��������)
    else if ( selected.x > 2 && selected.y == 2 
        && gameData[selected.x-2][0]==GD_BLANK )
    {   // ����߽��������ѵ�һ��������
        if ( /*(gameData[selected.x][1]!=GD_WALL && gameData[selected.x][0]!=GD_BLANK && gameData[selected.x-1][0]!=GD_WALL ) || */
            (gameData[selected.x-1][2]!=GD_WALL 
            && gameData[selected.x-2][2]!=GD_BLANK 
            && gameData[selected.x-3][2]==GD_WALL 
            && gameData[selected.x-2][1]!=GD_WALL ) )
        {
            tmppos.x=selected.x-2;
            tmppos.y=selected.y-2;
            preselect_pos.push_back(tmppos);
        }
    }
    // OOOOO
    // OO0X|
    // OOX@O
    // OO-OO
    // �����½ǿ��ߵ���������ж�(�������߽�ֵ)
    if ( selected.x < 14 && selected.y > 2 && gameData[selected.x+2][selected.y-2]==GD_BLANK )
    {
        if ( (gameData[selected.x+1][selected.y]!=GD_WALL 
            && gameData[selected.x+2][selected.y]!=GD_BLANK 
            && gameData[selected.x+3][selected.y]==GD_WALL 
            && gameData[selected.x+2][selected.y-1]!=GD_WALL ) 
        || (gameData[selected.x][selected.y-1]!=GD_WALL 
            && gameData[selected.x][selected.y-2]!=GD_BLANK 
            && gameData[selected.x][selected.y-3]==GD_WALL 
            && gameData[selected.x+1][selected.y-2]!=GD_WALL ) )
        {
            tmppos.x=selected.x+2;
            tmppos.y=selected.y-2;
            preselect_pos.push_back(tmppos);
        }
        else if ( (gameData[selected.x+1][selected.y]!=GD_WALL 
            && gameData[selected.x+2][selected.y]!=GD_BLANK 
            && gameData[selected.x+4][selected.y]!=GD_BLANK 
            && gameData[selected.x+2][selected.y-1]!=GD_WALL ) 
        || (gameData[selected.x][selected.y-1]!=GD_WALL 
            && gameData[selected.x][selected.y-2]!=GD_BLANK 
            && gameData[selected.x][selected.y-4]!=GD_BLANK 
            && gameData[selected.x+1][selected.y-2]!=GD_WALL ) )
        {
            tmppos.x=selected.x+2;
            tmppos.y=selected.y-2;
            preselect_pos.push_back(tmppos);
        }
    }
    // OOOO
    // OO0X
    // OOX@
    // OO-O
    // ������λ�������ڶ���,���½ǿ��������(��������)
    else if ( selected.x == 14 && selected.y >2 && gameData[16][selected.y-2]==GD_BLANK )
    {
        if ( /*(gameData[15][selected.y]!=GD_WALL && gameData[16][selected.y]!=GD_BLANK && gameData[16][selected.y-1]!=GD_WALL ) || */
            (gameData[14][selected.y-1]!=GD_WALL 
            && gameData[14][selected.y-2]!=GD_BLANK 
            && gameData[14][selected.y-3]==GD_WALL 
            && gameData[15][selected.y-2]!=GD_WALL ) )
        {
            tmppos.x=selected.x+2;
            tmppos.y=selected.y-2;
            preselect_pos.push_back(tmppos);
        }
    }
    // OOOOO
    // OO0X|
    // OOX@O
    // ������λ�������ڶ���,���½ǿ��������(��������)
    else if ( selected.x < 14 && selected.y == 2 && gameData[selected.x+2][0]==GD_BLANK )
    {
        if ( /*(gameData[selected.x][1]!=GD_WALL && gameData[selected.x][0]!=GD_BLANK && gameData[selected.x+1][0]!=GD_WALL ) || */
            (gameData[selected.x+1][2]!=GD_WALL 
            && gameData[selected.x+2][2]!=GD_BLANK 
            && gameData[selected.x+3][2]==GD_WALL 
            && gameData[selected.x+2][1]!=GD_WALL ) )
        {
            tmppos.x=selected.x+2;
            tmppos.y=selected.y-2;
            preselect_pos.push_back(tmppos);
        }
    }
    // ����߽����������ô�����Ľǵ��������Ҫ�������Ĵ���
}

void CQuoridor::freeSendBoxRule()
{
    // ɳ��ģʽ�£���ʼ�����̵İ�ť
    if (iButton==BUTTON_INIT_OR_CONFIRM)
    {
        resetGameData();
        return ;
    }
    // �Ѿ����ڵ�ǽ��λ�ã��ǲ��ܱ�ѡȡ��
    if (gameData[arr.x][arr.y]==GD_WALL)
    {
        return ;
    }
    // ���֮ǰû��ѡȡ�κ�λ��
    if ( pickup.x <0 && pickup.y <0 )
    {
        // �հ׵����λ
        if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]==GD_BLANK)
        {
            return ;
        }
        pickup.x=arr.x;
        pickup.y=arr.y;
#ifdef __DEBUG__
        // ������ԣ���ɳ��ģʽ���ʾ���ѡȡ������
        if (0==pickup.x%2 && 0==pickup.y%2 && gameData[pickup.x][pickup.y]!=GD_BLANK)
        {
            playerMovablePos(pickup);

            // �����������·��ѡȡ
            Quoridor_ComputerAI ai;
            ai.TestBestPath(pickup, gameData[pickup.x][pickup.y]);
        }
#endif
    }
    else
    {   // ������ѡȡ��λ��,������������ͬλ�ã����ʼ����
        if (arr == pickup)
        {
            pickup.x = -1;
            pickup.y = -1;
            return ;
        }
        // ��������ٴε��ʱ��ǰ�����ζ������λ�ã�����Ϊ˫ż��λ
        else if (0==arr.x%2 && 0==arr.y%2 && 0==pickup.x%2 && 0==pickup.y%2)
        {
            // ������������뵽�������Ӵ���׶�
            char tmp=0;
            // ����
            tmp=gameData[arr.x][arr.y];
            gameData[arr.x][arr.y]=gameData[pickup.x][pickup.y];
            gameData[pickup.x][pickup.y]=tmp;
            // ��Ŀ��λ���ϣ�������ұ���
            switch (gameData[arr.x][arr.y])
            {
            case GD_BLANK:
                break;
            case GD_YELLOW:
                g_player[0].x=arr.x/2;
                g_player[0].y=arr.y/2;
                break;
            case GD_RED:
                g_player[1].x=arr.x/2;
                g_player[1].y=arr.y/2;
                break;
            case GD_GREEN:
                g_player[2].x=arr.x/2;
                g_player[2].y=arr.y/2;
                break;
            case GD_BLUE:
                g_player[3].x=arr.x/2;
                g_player[3].y=arr.y/2;
                break;
            default:
                break;
            }
            // ���ھɵ�λ���ϣ�������ұ���
            switch (gameData[pickup.x][pickup.y])
            {
            case GD_BLANK:
                break;
            case GD_YELLOW:
                g_player[0].x=pickup.x/2;
                g_player[0].y=pickup.y/2;
                break;
            case GD_RED:
                g_player[1].x=pickup.x/2;
                g_player[1].y=pickup.y/2;
                break;
            case GD_GREEN:
                g_player[2].x=pickup.x/2;
                g_player[2].y=pickup.y/2;
                break;
            case GD_BLUE:
                g_player[3].x=pickup.x/2;
                g_player[3].y=pickup.y/2;
                break;
            default:
                break;
            }
            // ��ѡȡλ�ø�ֵΪ��Чֵ
            goto SEND_BOX_EXIT;
        }
        // ��ѡǽ�Ĺ����У��������λ��
        else if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]==GD_BLANK)
        {
            return ;
        }
        // ����ϴ�ѡȡ��λ��Ҳ��ǽ�����ѡȡ��Ҳ��ǽ���������ѡȡ��λ���ǿ�
        // ����ѡȡ��ͬλ���Ѿ����ʼ����
        else if ((pickup.x+pickup.y)%2==1 
            && (arr.x+arr.y)%2==1 
            && GD_BLANK == gameData[arr.x][arr.y])
        {
            // ����Ǻ�ǽ,�������ѡ�ĺ��ϴ�ѡ����ͬһ����
            if (pickup.x%2==0&&arr.y==pickup.y)
            {   // ������ѡ������һ��ѡ�����һ��λ��
                // �����������ӵ��м�λ���ǿ��õ�
                if(arr.x==pickup.x-2&&gameData[arr.x+1][arr.y]==GD_BLANK)
                {
                    // ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
                    wall_vec.push_back(arr);
                    wall_vec.push_back(pickup);
                    // ������Ϸ�㷨����,����ע�⣬��������ǽλ�õ��м����Ӵ�Ҳ��ֵ
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x+1][arr.y]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto SEND_BOX_EXIT;
                }
                // ��������ѡ������һ���ұ�һ��λ��
                // �����������ӵ��м�λ���ǿ��õ�
                else if(arr.x==pickup.x+2&&gameData[arr.x-1][arr.y]==GD_BLANK)
                {
                    // ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
                    wall_vec.push_back(pickup);
                    wall_vec.push_back(arr);
                    // ������Ϸ�㷨����
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x-1][arr.y]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto SEND_BOX_EXIT;
                }
            }
            // �������ǽ���������ѡ�ĺ��ϴ�ѡ����ͬһ����
            else if(pickup.y%2==0&&arr.x==pickup.x)
            {	// ������ѡ�������һ��ѡ�������һ��
                // �����������ӵ��м�λ���ǿ��õ�
                if (arr.y==pickup.y-2&&gameData[arr.x][arr.y+1]==GD_BLANK)
                {
                    // ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
                    wall_vec.push_back(arr);
                    wall_vec.push_back(pickup);
                    // ������Ϸ�㷨����
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x][arr.y+1]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto SEND_BOX_EXIT;
                }
                // ������ѡ�������һ��ѡ�������һ��
                // �����������ӵ��м�λ���ǿ��õ�
                else if (arr.y==pickup.y+2&&gameData[arr.x][arr.y-1]==GD_BLANK)
                {
                    // ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
                    wall_vec.push_back(pickup);
                    wall_vec.push_back(arr);
                    // ������Ϸ�㷨����
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x][arr.y-1]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto SEND_BOX_EXIT;
                }
            }
        }
        if ( gameData[arr.x][arr.y]!=GD_WALL )
        {
            // ���������һ����Ϊ����ѡȡ
            pickup.x=arr.x;
            pickup.y=arr.y;
#ifdef __DEBUG__
            if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]!=GD_BLANK)
            {
                playerMovablePos(pickup);
            }
#endif
        }
    }
    return ;
SEND_BOX_EXIT:
    // ���Ԥѡλ��
    pickup.x=-1;
    pickup.y=-1;
    return ;
}

bool CQuoridor::judgeWallLegal()
{
    /*
     ע�⣺���Ϊ����Ϸ������ϸ��ԣ��ж�һ��ǽ�Ƿ��������ʱ��
           ָ���ǣ�ÿ�жϵ�һ�����ʱ�������ϲ����κ���ҵ�λ�ã��ټ������λ��
           ���ԣ����ﱣ��һ����Ϸ���ݣ��ڳ�������ʱ���ٻ�ԭ
    */
    char backupGD[GDSIZE][GDSIZE]={0};
    void* tmpp = (void*)gameData;
    memcpy(backupGD,tmpp,sizeof(gameData));
    // ����Ϸ�����У�ȥ������������ڵ�λ��
    player* pp=ply_head;
    do 
    {
        gameData[pp->x*2][pp->y*2]=GD_BLANK;
        pp=pp->next;
    } while (pp!=ply_head);
    // ����һ����ʱ�Ĵ����ҿ���λ�õĶ���
    std::deque<pos2d> que;
    // 9x9����ʱ��ǣ���¼��ҿ��ߵ�λ��,0Ϊ�գ�1Ϊ�����������ߣ�2Ϊ�Ѿ������
    char tmpflag[9][9];
    bool jump_flag=false;
    for (int i=0; i<4; i++)
    {   // ��յ�ǰ��ҿ���λ�õı��
        memset(tmpflag,0,sizeof(tmpflag));
        // ��մ�ѡ����
        que.clear();
        // ֤�������ҵ����ڵ���ת��־��Ϊfalse
        jump_flag=false;
        // ֻҪ���ǹرյ����
        if (g_player[i].id!=ID_CLOSED)
        {   // �Ƚ���ǰ��ҵ�λ�ô��룬ע�⣬���������0~8�ķ�Χ��ת��Ϊ0~16�ķ�Χ
            // ��Ϊ��ѡ���㷨���ص���0~16�ķ�Χ
            pos2d tmpp;
            tmpp.x=g_player[i].x*2;
            tmpp.y=g_player[i].y*2;
            // ���ߵ��Ƕ�ά�����ǰ���0~8��Ƶ�
            tmpflag[g_player[i].x][g_player[i].y]=1;
            que.push_back(tmpp);
            // ������߶����ﻹ�����ݣ���û���ҵ��յ�Ŀ���λ��
            while (que.size()>0 && jump_flag == false)
            {   // ʹ�ö��е�ͷ������������λ��
                playerMovablePos(*que.begin());
                // ��ʵ������˵��ѡ���ߵ��㷨�ǲ����ܷ��ؿ�������
                if (preselect_pos.size()>0)
                {
                    for (size_t j=0;j<preselect_pos.size();j++)
                    {   // ��ͬ��Ҳ�ͬ�ĵ����յ������
                        switch(g_player[i].color)
                        {
                        case GD_YELLOW:
                            if (preselect_pos[j].x==16)
                                jump_flag=true;
                            break;
                        case GD_RED:
                            if (preselect_pos[j].y==0)
                                jump_flag=true;
                            break;
                        case GD_GREEN:
                            if (preselect_pos[j].x==0)
                                jump_flag=true;
                            break;
                        case GD_BLUE:
                            if (preselect_pos[j].y==16)
                                jump_flag=true;
                            break;
                        default:
                            break;
                        }
                        if (jump_flag)
                            break;
                        // �����һλ�û�û�б����ǹ�,û�б����
                        if (tmpflag[preselect_pos[j].x/2][preselect_pos[j].y/2]==0)
                        {
                            // ���ߵ��Ƕ�ά�����ǰ���0~8��Ƶ�, ��Ԥѡλ��תΪ���λ��
                            tmpflag[preselect_pos[j].x/2][preselect_pos[j].y/2]=1;
                            que.push_back(preselect_pos[j]);
                        }
                    }
                }
                // ����һ��ȥ����Ŀ���λ���Ѿ����������������ǰ��
                que.pop_front();
            }
            if (jump_flag==false /*&& que.size()==0*/)
            {   // �����ǰ���û�м�⵽���ߵ��յ�
                // ��ԭ���޸Ĺ�����Ϸ����
                void* tmpp = (void*)gameData;
                memcpy(tmpp,backupGD,sizeof(gameData));
                return false;
            }
        }
    }
    // ��ԭ���޸Ĺ�����Ϸ����
    tmpp = (void*)gameData;
    memcpy(tmpp,backupGD,sizeof(gameData));
    // ������Ϸ�����ȫ���н�
    return true;
}

void CQuoridor::drawVictory()
{
    if (win_flag==GD_BLANK)
    {
        return ;
    }
    char tmpstr[64]={0};
    float layer=0.3f;
    float tri_w=g_OpenGL->RCwidth/3.0f;
    float tri_h=g_OpenGL->RCheight/3.0f;
    //���Ʊ�����͸�����ƴ���
    tRectangle(tri_w-menu_w,tri_h,layer,tri_w+2*menu_w,tri_h,0.0f,0.0f,0.0f,0.8f);

    glPushMatrix();
    glTranslatef(0,0,0.5f);
    switch (win_flag)
    {
    case GD_YELLOW:
        sprintf(tmpstr,"�� ɫ �� ��");
        myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT8,1,1,0);
        break;
    case GD_RED:
        sprintf(tmpstr,"�� ɫ �� ��");
        myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT8,1,0,0);
        break;
    case GD_GREEN:
        sprintf(tmpstr,"�� ɫ �� ��");
        myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT8,0,1,0);
        break;
    case GD_BLUE:
        sprintf(tmpstr,"�� ɫ �� ��");
        myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT8,0,0,1);
        break;
    default:
        break;
    }
    sprintf(tmpstr,"  ʤ  �� ");
    myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+3/7.0)),tmpstr,FONT8,1,1,1);
    sprintf(tmpstr," �� �� ����");
    myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+1/8.0)),tmpstr,FONT8,1,1,1);
    glPopMatrix();

    // �������дһ���̻����� 
}

void CQuoridor::drawNetworkOp()
{
    char tmpstr[128]="";
    // Ҫô�ǿͻ��������ͣ������ߣ�Ҫô���ڷ�����״̬
    if ((2!=n_netWorkStatus && g_OpenGL->Xmouse<g_OpenGL->RCwidth/2) || 1==n_netWorkStatus)
    {
        // ��߷�����һ��ĵ�ͼ
        tRectangle(0,0,-0.5f,g_OpenGL->RCwidth/2.0f,(float)g_OpenGL->RCheight,0.8f,0,0,0.6f);
        // �ұ߿ͻ���һ��ĵ�ͼ
        tRectangle(g_OpenGL->RCwidth/2.0f,0,-0.5f,g_OpenGL->RCwidth/2.0f,(float)g_OpenGL->RCheight,0,0.8f,0,0.4f);
        // ��ʾ����IP�Լ���������Ϣ
        sprintf(tmpstr,"�� �� IP: %s", n_loaclIP);
        myfont.Print2D(g_OpenGL->RCwidth/8,(int)(g_OpenGL->RCheight*0.6),tmpstr,FONT4,1,1,1);
        sprintf(tmpstr,"�����˿�: %u", n_net->port);
        myfont.Print2D(g_OpenGL->RCwidth/8,(int)(g_OpenGL->RCheight*0.6)-30,tmpstr,FONT4,1,1,1);
    }
    // �ͻ���״̬�������ͣ�����Ҳ�(�����߼�֮����߼�)
    else
    {
        // ��߷�����һ��ĵ�ͼ
        tRectangle(0,0,-0.5f,g_OpenGL->RCwidth/2.0f,(float)g_OpenGL->RCheight,0.8f,0,0,0.4f);
        // �ұ߿ͻ���һ��ĵ�ͼ
        tRectangle(g_OpenGL->RCwidth/2.0f,0,-0.5f,g_OpenGL->RCwidth/2.0f,(float)g_OpenGL->RCheight,0,0.8f,0,0.6f);
        // ��ʾ���ӷ�����IP�Լ��˿���Ϣ
        sprintf(tmpstr,"������IP: %s", n_net->IP);
        myfont.Print2D((int)(g_OpenGL->RCwidth*0.62),(int)(g_OpenGL->RCheight*0.6),tmpstr,FONT4,1,1,1);
        sprintf(tmpstr,"����˿�: %u", n_net->port);
        myfont.Print2D((int)(g_OpenGL->RCwidth*0.62),(int)(g_OpenGL->RCheight*0.6)-30,tmpstr,FONT4,1,1,1);
        // ����Ѿ��ǿͻ���״̬�ˣ�������ʾ����IP
        if (2==n_netWorkStatus)
        {
            // �ͻ�����ʾ��֪�������
            for (int i=0;i<4;i++)
            {
                if (strlen(const_cast<char*>(n_NameAll[i]))==0)
                {
                    sprintf(tmpstr,"[%1d] %8s ",i+1,"--[��]--");
                }
                else
                {
                    sprintf(tmpstr,"[%1d] %8s ",i+1,n_NameAll[i]);
                    switch (i)
                    {
                    case 0:
                        tRectangle(g_OpenGL->RCwidth/12-2.0f,g_OpenGL->RCheight*0.43f-3 ,-0.3f,380,27,0,0,1,1);
                        break;
                    case 1:
                        tRectangle(g_OpenGL->RCwidth/12-2.0f,g_OpenGL->RCheight*0.43f-33,-0.3f,380,27,1,0,0,1);
                        break;
                    case 2:
                        tRectangle(g_OpenGL->RCwidth/12-2.0f,g_OpenGL->RCheight*0.43f-63,-0.3f,380,27,0,1,0,1);
                        break;
                    case 3:
                        tRectangle(g_OpenGL->RCwidth/12-2.0f,g_OpenGL->RCheight*0.43f-93,-0.3f,380,27,1,1,0,1);
                        break;
                    default:
                        break;
                    }
                }
                myfont.Print2D(g_OpenGL->RCwidth/12,(int)(g_OpenGL->RCheight*0.43)-30*(i),tmpstr,FONT4,1,1,1);
            }
            sprintf(tmpstr,"   [ ��ȴ�������ʼ��Ϸ��]");
            myfont.Print2D(g_OpenGL->RCwidth/12,(int)(g_OpenGL->RCheight*0.43)-30*5,tmpstr,FONT4,1,1,1);

            sprintf(tmpstr,"�� �� IP: %s", n_loaclIP);
            myfont.Print2D((int)(g_OpenGL->RCwidth*0.62),(int)(g_OpenGL->RCheight*0.6)-80,tmpstr,FONT4,1,1,1);
            sprintf(tmpstr,"��������: %8s", n_net->Name);
            myfont.Print2D((int)(g_OpenGL->RCwidth*0.62),(int)(g_OpenGL->RCheight*0.6)-110,tmpstr,FONT4,1,1,1);
        }
    }
    //------------------------------------
    switch (n_netWorkStatus)
    {
    case 0:
        sprintf(tmpstr,"��������");
        myfont.Print2D(g_OpenGL->RCwidth/4-menu_w/2+10,g_OpenGL->RCheight*2/3+5,tmpstr,FONT4,1,1,1);
        texture_select(g_cactus[9]);
        tPicButton((float)(g_OpenGL->RCwidth/4-menu_w/2),(float)(g_OpenGL->RCheight*2/3),
            (float)menu_w,(float)menu_h,(iButton==BUTTON_SERVER)?0.0f:0.5f);
        sprintf(tmpstr,"��������");
        myfont.Print2D(g_OpenGL->RCwidth*3/4-menu_w/2+10,g_OpenGL->RCheight*2/3+5,tmpstr,FONT4,1,1,1);
        tPicButton((float)(g_OpenGL->RCwidth*3/4-menu_w/2),(float)(g_OpenGL->RCheight*2/3),
            (float)menu_w,(float)menu_h,(iButton==BUTTON_CLIENT)?0.0f:0.5f);
        break;
    case 1:
        sprintf(tmpstr,"��������");
        myfont.Print2D(g_OpenGL->RCwidth/4-menu_w/2+10,g_OpenGL->RCheight*2/3+5,tmpstr,FONT4,0.0f,1.0f,0.0f);
        texture_select(g_cactus[9]);
        tPicButton((float)(g_OpenGL->RCwidth/4-menu_w/2),(float)(g_OpenGL->RCheight*2/3),
            (float)menu_w,(float)menu_h,0.5f);
        //sprintf(tmpstr,"��������");
        //myfont.Print2D(g_OpenGL->RCwidth/4-menu_w/2+10,g_OpenGL->RCheight*1/6+5,tmpstr,FONT4,1,1,1);
        //tPicButton((float)(g_OpenGL->RCwidth/4-menu_w/2),(float)(g_OpenGL->RCheight*1/6),
        //    (float)menu_w,(float)menu_h,(iButton==BUTTON_SERVER_TEST)?0:0.5f);
        sprintf(tmpstr,"��ʼ��Ϸ");
        myfont.Print2D((g_OpenGL->RCwidth-menu_w)/2+10,menu.y+5,tmpstr,FONT4,1,1,1);
        tPicButton((float)(g_OpenGL->RCwidth-menu_w)/2,(float)menu.y,
            (float)menu_w,(float)menu_h,(iButton==BUTTON_SERVER_START)?0:0.5f);
        // ��ʾ��ǰ���ӵ�IP�б�
        // �ȹ̶���ʾ����IP
        sprintf(tmpstr,"[1] %8s (%16s)",n_NameAll[0], n_loaclIP);
        myfont.Print2D(g_OpenGL->RCwidth/12,(int)(g_OpenGL->RCheight*0.43),tmpstr,FONT4,1,1,1);
        tRectangle(g_OpenGL->RCwidth/12-2.0f,g_OpenGL->RCheight*0.43f-3,-0.3f,380,27,0,0,1,1);
        // ����ʾ�����ӵĿͻ���IP�б�
        for (int i=0; i<3; i++)
        {
            if (strlen(n_net->n_TCPnet->GetClientIP(i))==0)
            {
                sprintf(tmpstr,"[%1d] %8s (%16s)",i+2,"--[��]--","---.---.---.---");
            }
            else
            {
                sprintf(tmpstr,"[%1d] %8s (%16s)",i+2,n_NameAll[i+1],n_net->n_TCPnet->GetClientIP(i));
                switch (i)
                {
                case 0:
                    tRectangle(g_OpenGL->RCwidth/12-2.0f,g_OpenGL->RCheight*0.43f-33,-0.3f,380,27,1,0,0,1);
                    break;
                case 1:
                    tRectangle(g_OpenGL->RCwidth/12-2.0f,g_OpenGL->RCheight*0.43f-63,-0.3f,380,27,0,1,0,1);
                    break;
                case 2:
                    tRectangle(g_OpenGL->RCwidth/12-2.0f,g_OpenGL->RCheight*0.43f-93,-0.3f,380,27,1,1,0,1);
                    break;
                default:
                    break;
                }
            }
            myfont.Print2D(g_OpenGL->RCwidth/12,(int)(g_OpenGL->RCheight*0.43)-30*(i+1),tmpstr,FONT4,1,1,1);
        }
        break;
    case 2:
        sprintf(tmpstr,"��������");
        myfont.Print2D(g_OpenGL->RCwidth*3/4-menu_w/2+10,g_OpenGL->RCheight*2/3+5,tmpstr,FONT4,0.1f,0.1f,0.1f);
        tPicButton((float)(g_OpenGL->RCwidth*3/4-menu_w/2),(float)(g_OpenGL->RCheight*2/3),
            (float)menu_w,(float)menu_h,0.5f);
        //sprintf(tmpstr,"�ͻ�����");
        //myfont.Print2D(g_OpenGL->RCwidth*3/4-menu_w/2+10,g_OpenGL->RCheight*1/6+5,tmpstr,FONT4,1,1,1);
        //tPicButton((float)(g_OpenGL->RCwidth*3/4-menu_w/2),(float)(g_OpenGL->RCheight*1/6),(float)menu_w,(float)menu_h,(iButton==BUTTON_CLIENT_TEST)?0:0.5f);
        break;
    default:
        break;
    }
    // ���ذ�ť
    sprintf(tmpstr,"��ESC����");
    myfont.Print2D(menu.x+4,menu.y+5,tmpstr,FONT4,1,1,1);
    tPicButton((float)menu.x,(float)menu.y,(float)menu_w,(float)menu_h,(iButton==BUTTON_RETURN)?0:0.5f);
}
#ifdef __DEBUG__
void CQuoridor::drawTestOptimalPath()
{
    if (pickup.x%2!=0 || pickup.y%2!=0)
    {
        return;
    }
    char tmpstr[128]="";
    for (size_t i=0; i<best_path.size();i++)
    {
        tRectangle(board_x+lace+best_path[i].x/2*(roadw+wall_w),lace+best_path[i].y/2*(roadw+wall_w),0.1f,roadw/2,roadw/2,1.0f,1.0f,0.1f,0.6f);
        sprintf(tmpstr,"%02d",i);
        glPushMatrix();
        glTranslatef(0,0,0.5f);
        myfont.Print2D((int)(board_x+lace+best_path[i].x/2*(roadw+wall_w)),(int)(lace+best_path[i].y/2*(roadw+wall_w)),tmpstr,FONT4,1,1,1);
        glPopMatrix();
    }
}
// ���Ƶ���ʽ����Ϣ��ʾ
void CQuoridor::drawHint()
{
    switch (HintFlag)
    {
    case HINT_NULL:
        break;
    case HINT_EXIT:
        Hint2Line("        [ע    ��]  ����   ", "   ��ȷ��Ҫ�˳���ǰ��Ϸ��");
        break;
    case HINT_PLAY_NUMBER_INVALID:
        Hint2Line("        [��    ��]  ����   ", "   ��Ϸ�������������������");
    default:
        break;
    }
}
// ��Ϣ��ʾ������
void CQuoridor::Hint2Line(const char* line1, const char* line2)
{
    //char tmpstr[64]={0};
    float layer=0.3f;
    float tri_w=g_OpenGL->RCwidth/3.0f;
    float tri_h=g_OpenGL->RCheight/3.0f;
    //���Ʊ�����͸�����ƴ���
    tRectangle(tri_w-menu_w,tri_h,layer,tri_w+2*menu_w,tri_h,0.0f,0.0f,0.0f,0.8f);

    glPushMatrix();
    glTranslatef(0,0,0.5f);
    myfont.Print2D((int)(tri_w-menu_w+20),(int)(tri_h*1.628),line1,FONT8,1,0,0);
    myfont.Print2D((int)(tri_w-menu_w+20),(int)(tri_h*1.325),line2,FONT8,1,1,0);
    glPopMatrix();
}

#endif
