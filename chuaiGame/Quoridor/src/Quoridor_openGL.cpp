//=======================================
//
// 游戏：步步为营
//       Quoridor
// 作者：chuaiGM
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

//font
extern CGLFont myfont;
//clock
extern MYCLOCK c1;
//mouse
extern int Lbutdown;
// OpenGL环境对象指针在主函数文件中声明
extern COpenGLbase* g_OpenGL;
// 外部输入的窗口宽高
extern int WinWidth;
extern int WinHeight;
// 是否开启音乐标记
extern int g_sound;
// 配置文件操作
extern int ConfigGetKeyValue(const char *CFG_file, const char *section, const char *key, char *buf);
extern int ConfigSetKeyValue(const char *CFG_file, const char *section, const char *key, const char *buf);

/////////////////////////

CQuoridor::CQuoridor()
{
    // 当前游戏状态
    iGameState=GAME_MENU;
    // 当前菜单选择项
    iMenu=-1;
    // 菜单坐标位置
    menu.x=10;
    menu.y=10;
    // 菜单按钮的宽度和高度
    menu_w=10;
    menu_h=10;
    // 菜单上下间距
    menu_dis=10;
    // 游戏棋盘与窗口边界间距
    lace=6;

    iButton=-1;

    // 游戏数据初始化
    roadw = 10;
    wall_l=10;
    wall_w=10;
    player_info_h=10;
    player_info_w=10;
    board_x=10;

    arr.x=-1;
    arr.y=-1;
    // 玩家信息数据
    // 黄
    plyer[0].id=ID_CLOSED;
    plyer[0].color=GD_YELLOW;
    plyer[0].x=0;
    plyer[0].y=4;
    // 红
    plyer[1].id=ID_COMPUTER;
    plyer[1].color=GD_RED;
    plyer[1].x=4;
    plyer[1].y=8;
    // 绿
    plyer[2].id=ID_CLOSED;
    plyer[2].color=GD_GREEN;
    plyer[2].x=8;
    plyer[2].y=4;
    // 蓝
    plyer[3].id=ID_HUMAN;
    plyer[3].color=GD_BLUE;
    plyer[3].x=4;
    plyer[3].y=0;
    // 贴图数组初始化
    memset(g_cactus,0,sizeof(g_cactus));
    // 棋盘数据初始化
    memset(gameData,0,sizeof(gameData));
    // 这里的顺序需要注意，这里暂时按照先x后y的顺序去做，有问题再说
    //gameData[2*plyer[0].x][2*plyer[0].y]=GD_YELLOW;
    //gameData[2*plyer[1].x][2*plyer[1].y]=GD_RED;
    //gameData[2*plyer[2].x][2*plyer[2].y]=GD_GREEN;
    //gameData[2*plyer[3].x][2*plyer[3].y]=GD_BLUE;
    // 鼠标选取的位置
    pickup.x=-1;
    pickup.y=-1;
    // 正式游戏时，玩家轮流的顺序
    ply_head=NULL;
    // 玩家胜利标志
    win_flag=GD_BLANK;
    // 显示调试信息
    g_debug_flag=false;
    tcounter=0;

    // 网络相关
    n_TCPnet=NULL;
    n_port=0;
    memset(n_IP,0,sizeof(n_IP));
    memset(n_loaclIP,0,sizeof(n_loaclIP));
    memset(n_Name,0,sizeof(n_Name));
    n_netWorkStatus=0;
    memset(n_NameAll,0,sizeof(n_NameAll));
}

CQuoridor::~CQuoridor()
{
    
}
//游戏所需数据文件检查,如果缺少,程序中止
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
            ihave=0;//缺少文件
            break;
        }
        else
            fclose(fp);
    }

    //如果当前文件夹没有,返回上级文件夹,再检查
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

    return 1;
}

//加载各个图片,模型,初始化全局数据
void CQuoridor::init()
{
    //load images
    g_OpenGL->LoadBMP_aux("data/images/quoridor_cover.bmp", g_cactus[0]);		// 封面
    g_OpenGL->LoadBMP_aux("data/images/chess_board_shading.bmp", g_cactus[1]);	// 棋盘底纹
    g_OpenGL->LoadBMP_aux("data/images/road.bmp", g_cactus[2]);				// 玩家可站的位置
    g_OpenGL->LoadBMP_aux("data/images/bidiu1.bmp", g_cactus[3]);				// 玩家1形象
    g_OpenGL->LoadBMP_aux("data/images/huangshou1.bmp", g_cactus[4]);			// 玩家2形象
    g_OpenGL->LoadBMP_aux("data/images/diting1.bmp", g_cactus[5]);				// 玩家3形象
    g_OpenGL->LoadBMP_aux("data/images/xiaohei1.bmp", g_cactus[6]);			// 玩家4形象
    g_OpenGL->LoadBMP_aux("data/images/wall1.bmp", g_cactus[7]);				// 墙的贴图
    g_OpenGL->LoadBMP_aux("data/images/computer_logo.bmp", g_cactus[8]);		// 电脑图标
    //button
    g_OpenGL->LoadBMP_aux("data/images/button.bmp", g_cactus[9]);				// 按钮

    g_OpenGL->LoadBMP_aux("data/images/example_show_help.bmp", g_cactus[10]);  // 帮助中使用的展示图

    ///////////////////////////////////////////
    //启动贴图
    glEnable(GL_TEXTURE_2D);

    //游戏状态
//	iGameState=GAME_PRE_ANI;
    iGameState=GAME_MENU;
}

//设置投影方式
void CQuoridor::initView()
{
    //switch(iGameState)
    //{
    //case GAME_MENU:
    //	g_OpenGL->init_2D();
    //	break;

    //default:
    //	//g_OpenGL->init_3D();
    //	g_OpenGL->init_2D();
    //	break;
    //}

    // 创建二维的OpenGL视图模型
    g_OpenGL->init_2D();

    // 在这里创建一些从OpenGL中获取的窗口参数
    // 这里也可以单独立一个函数去实现
    // 菜单上下间距
    menu_dis=g_OpenGL->RCheight/15;
    // 菜单按钮的宽度和高度
    menu_w=g_OpenGL->RCwidth/9;
    menu_h=g_OpenGL->RCheight/20;
    // 帮助界面返回菜单按钮宽度
    //helpRetButtonW=g_OpenGL->RCwidth/5;
    //rButtonx=(g_OpenGL->RCwidth-helpRetButtonW)/2;
    // 菜单坐标位置
    menu.x=g_OpenGL->RCwidth-menu_dis/2-menu_w;
    menu.y=/*menu_dis/2 +*/ menu_h;

    // 游戏棋盘数据
    player_info_h=g_OpenGL->RCheight/4.0f;
    player_info_w=(g_OpenGL->RCwidth-g_OpenGL->RCheight)*3/8.0f;
    // 棋盘左下角，起点坐标
    board_x = (g_OpenGL->RCwidth-g_OpenGL->RCheight)/2;

    // 墙和路的宽度推算公式：
    //  x=roadw; y=wall_w
    //  9*x + 8*y = Height - lace*2
    //    x = 4 y
    roadw = (g_OpenGL->RCheight - lace*2)/11.0f;
    wall_w= (g_OpenGL->RCheight - lace*2)/44.0f;
    wall_l= 2*roadw+wall_w;
}

// 游戏主绘图函数
void CQuoridor::showMain()
{
    // 显示测试数据
    if (g_debug_flag)
    {
        show_Font_test();
    }

    switch(iGameState)
    {
    case GAME_PRE:
        //showpreani();
        break;
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
        // 注意此处，透明图层与绘制顺序有关
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
    //case GAME_NETWORK:
    //    break;
    case GAME_SENDBOX:
        drawAccessory();
        drawChessBorad();
        drawPlayerWall();
        // 注意此处，透明图层与绘制顺序有关
        drawPickMask();
        break;

    case GAME_HELP:
        showHelp();
        break;

    default:
        break;
    }
}
//实时检测
void CQuoridor::check()
{
    int i;
    int x,y;
    int it;
    float remain;
    // 检测鼠标当前移动的位置
    x=g_OpenGL->Xmouse;
    y=g_OpenGL->Ymouse;
    iMenu=-1;//初始化 没有选择
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
    //    //计时
    //    if(c1.clipcount())
    //    {
    //        iGameState=GAME_MENU;
    //        //设置投影 
    //        initView();
    //    }
    //    break;

    case GAME_MENU:
        if(x<menu.x	|| x>menu.x+menu_w || y<menu.y)
            break;

        for(i=0;i<MENU_NUM;i++)
        {
            if( y>menu.y+i*menu_dis && y<menu.y+i*menu_dis+menu_h )
            {
                // 注意，这里给菜单选取值，赋值的i，是与定义的枚举变量所对应的
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
            ai.SimpleComputer();
            break;
        }
        else if (ply_head->id==ID_NET_PLAYER)
        {
            break;
        }
    case GAME_SENDBOX:
        // 实时检查鼠标位置
        // 检查时，如果鼠标位于棋盘边界外
        if (x>board_x+lace && x<board_x+g_OpenGL->RCheight-lace)
        {
            // 当前位置除一个路的宽度和一个墙的宽度的整数部分
            it=(int)((x-board_x-lace)/(roadw+wall_w));
            // 余数部分
            remain = (x-board_x-lace)-it*(roadw+wall_w);
            if (remain>roadw)
            {	// 余数比路宽度大，这是墙的位置，奇数
                arr.x = it*2+1;
            } else {
                // 玩家可以放置的位置，路，偶数
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
        // 这里不进行break，继承返回菜单按钮
    case GAME_IN_CONFIG:
        // 这里检测返回菜单按钮和重置棋盘按钮
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
// 左键松开
void CQuoridor::lbuttonproc(int lparam)
{
    char tmpstr[32];
    if (iButton==BUTTON_RETURN)
    {
        iGameState=GAME_MENU;
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
            // 点击进入联网模式时，获取本机IP与端口一次
            {   // 这里也可以定义一个临时tcp类，调用函数去做。不过目前这样做效率更好
            WORD wVersionRequested;
            WSADATA wsaData;
            wVersionRequested=MAKEWORD(2,2);
            WSAStartup(wVersionRequested,&wsaData);

            char buf[256]="";
            struct hostent *ph = 0;
            gethostname(buf, 256);
            ph = gethostbyname(buf);
            const char *IP =inet_ntoa(*((struct in_addr *)ph->h_addr_list[0]));//此处获得本机IP
            strncpy(n_loaclIP, IP, 16);
            WSACleanup();
            }

            ConfigGetKeyValue("config.ini", "Net_work", "MyName", n_Name);
            ConfigGetKeyValue("config.ini", "Net_work", "IP", n_IP);
            ConfigGetKeyValue("config.ini", "Net_work", "port", tmpstr);
            n_port=atoi(tmpstr);

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
        // 在单人配置模式下，此按钮为确定
        if (iButton==BUTTON_INIT_OR_CONFIRM)
        {
            // 这里，相当于单人游戏的初始化部分
            int nn=0;
            ply_head=NULL;      //这句应该可以不要
            player* tail=NULL;
            // 这里也可以考虑清一下每个玩家的next指针
            for (int i=0; i<4; i++)
            {	// 如果当前的玩家状态是参与游戏的(可能是人控制，也可能是电脑)
                if (plyer[i].id!=ID_CLOSED)
                {
                    if (ply_head==NULL)
                    {
                        ply_head=&plyer[i];
                        tail=&plyer[i];
                    }
                    tail->next=&plyer[i];
                    tail=&plyer[i];
                    nn++;
                    switch (plyer[i].color)
                    {
                    case GD_YELLOW:
                        gameData[plyer[i].x*2][plyer[i].y*2]=GD_YELLOW;
                        break;
                    case GD_RED:
                        gameData[plyer[i].x*2][plyer[i].y*2]=GD_RED;
                        break;
                    case GD_GREEN:
                        gameData[plyer[i].x*2][plyer[i].y*2]=GD_GREEN;
                        break;
                    case GD_BLUE:
                        gameData[plyer[i].x*2][plyer[i].y*2]=GD_BLUE;
                        break;
                    default:
                        break;
                    }
                }
                // 如果当前玩家的状态是关闭的，不参与游戏的
                //else
                //{	// 因为初始化的时候，已经把玩家所应该站的初始位置在游戏数据中建立了
                //    // 如果不参与游戏，需要再清空这个游戏数据
                //    gameData[plyer[i].x*2][plyer[i].y*2]=GD_BLANK;
                //}
            }
            // 当可用玩家数量，少于2时，无法进行游戏
            if (nn<2)
            {   // TODO 以后添加弹窗提示, 玩家人数不能少于2人
                // 因为上面删了游戏数据，这里再补回来
                resetGameData();
                break;
            }
            // 构建环形链表
            tail->next=ply_head;
            // 为了统计人类玩家数，为了给玩家剩余墙数赋值
            player* tmp_head=ply_head;
            // 人类玩家数
            int human_n=0;
            // 循环给剩余墙数赋值
            do
            {   // 统计人类玩家数
                if (tmp_head->id==ID_HUMAN)
                {
                    human_n++;
                }
                // 整形数除法,21除2，取整=10
                //            21除3，取整=7
                //            21除4，取整=5
                tmp_head->wall_num_left=wall_total_num/nn;
                tmp_head=tmp_head->next;
            }while (ply_head!=tmp_head);
            // 如果存在人类玩家，让人类玩家先行
            if (human_n>0)
            {
                while (ply_head->id!=0)
                {
                    ply_head=ply_head->next;
                }
            }
            iGameState=GAME_SINGE;
        }
        // 鼠标选取不同玩家的三个选项时的处理，
        // 遍历玩家
        for (size_t j=0; j<4; j++)
        {
            // 决定，选的到底是，玩家，电脑，还是关闭
            for (size_t i=0; i< 3; i++)
            {
                if (g_OpenGL->Xmouse>(float)board_x+(i*2.3f+1)*(roadw+wall_w)
                    && g_OpenGL->Xmouse<(float)board_x+(i*2.3f+1)*(roadw+wall_w)+menu_w
                    && g_OpenGL->Ymouse>(player_info_h-menu_h)/2+player_info_h*j
                    && g_OpenGL->Ymouse<(player_info_h-menu_h)/2+menu_h+player_info_h*j
                    )
                {
                    plyer[3-j].id=i;
                }
            }
        }
        break;
    case GAME_SINGE:
        // 如果是玩家可控的角色
        if (ply_head->id==ID_HUMAN)
        {
            playerActionRule();
        }
        break;
    case GAME_NETWORK:
        // 如果是玩家可控的角色
        if (ply_head->id==ID_HUMAN)
        {
            playerActionRule_network();
        }
        break;
    case GAME_NET_CONFIG:
        switch (iButton)
        {
        case BUTTON_SERVER:
            // 这里创建网络对象
            n_TCPnet = new CTCPSocket(TCP_SOCKET_SERVER);
            // 创建服务
            if(!n_TCPnet->CreateServer(n_port))
            {
                int err=n_TCPnet->GetError();
                sprintf(tmpstr,"无法创建服务器，错误号: %d",err);
                MessageBox(NULL, tmpstr, "TcpNetwork",MB_OK);
                delete n_TCPnet;
                n_TCPnet=NULL;
            }
            // 开始服务器
            else if(!n_TCPnet->StartServer(NULL,OnReceiveNetData,NULL))
            {
                int err=n_TCPnet->GetError();
                sprintf(tmpstr,"无法开启服务器，错误号: %d",err);
                MessageBox(NULL, tmpstr, "TcpNetwork",MB_OK);
                delete n_TCPnet;
                n_TCPnet=NULL;
            }
            else 
            {
                // 服务成功开启
                strncpy(n_NameAll[0],n_Name,8);
                n_netWorkStatus=1;
            }
            // 如果是服务器，首先把玩家链表中加入自己
            // 服务器默认蓝色玩家
            plyer[3].id=ID_HUMAN;
            ply_head=&plyer[3];
            break;
        case BUTTON_CLIENT:
            // 这里创建网络对象
            n_TCPnet = new CTCPSocket(TCP_SOCKET_CLIENT);
            if(!n_TCPnet->Connect(n_IP,n_port))
            {
                int err=n_TCPnet->GetError();
                sprintf(tmpstr,"无法连接到服务器，错误号: %d",err);
                MessageBox(NULL, tmpstr, "TcpNetwork",MB_OK);
                delete n_TCPnet;
                n_TCPnet=NULL;
            }
            else if(!n_TCPnet->StartReceiving(NULL,OnReceiveNetData,NULL))
            {
                int err=n_TCPnet->GetError();
                sprintf(tmpstr,"无法开启客户端数据接收服务，错误号: %d",err);
                MessageBox(NULL, tmpstr, "TcpNetwork",MB_OK);
                delete n_TCPnet;
                n_TCPnet=NULL;
            }
            else
            {
                n_TCPnet->SendClient(n_Name,strlen(n_Name)+1);
                n_netWorkStatus=2;
            }
            break;
        case BUTTON_SERVER_TEST:
            break;
        case BUTTON_CLIENT_TEST:
            break;
        case BUTTON_SERVER_START:
            // 如果没有其他玩家连入
            if (n_TCPnet->GetConnectionNumber()<1)
            {   // 这里以后可以给出提示
                break;
            }
            player* tail=ply_head;
            // 点开始游戏，才给服务器玩家赋上棋盘数据值
            gameData[plyer[3].x*2][plyer[3].y*2]=GD_BLUE;
            int ConNum=n_TCPnet->GetConnectionNumber();
            // 由于网络游戏时，三个玩家的分配顺序，服务器必是蓝色
            // 第二个加入游戏的玩家为红色，如果再有玩家加入是绿色，最后是黄色
            for (int i=0; i<ConNum;i++)
            {
                switch (i)
                {
                case 0:// 设置为红色玩家
                    plyer[1].id=ID_NET_PLAYER;
                    gameData[plyer[1].x*2][plyer[1].y*2]=GD_RED;
                    tail->next=&plyer[1];
                    tail=&plyer[1];
                    break;
                case 1:// 设置为绿色玩家
                    plyer[2].id=ID_NET_PLAYER;
                    gameData[plyer[2].x*2][plyer[2].y*2]=GD_GREEN;
                    tail->next=&plyer[2];
                    tail=&plyer[2];
                    break;
                case 2:// 设置为黄色玩家
                    plyer[0].id=ID_NET_PLAYER;
                    gameData[plyer[0].x*2][plyer[0].y*2]=GD_YELLOW;
                    // 当存在黄色玩家时，比较特别
                    ply_head->next=&plyer[0];
                    plyer[0].next=&plyer[1];
                    break;
                default:
                    break;
                }
                sprintf(tmpstr,"READY%1dN%1d",i,ConNum);
                n_TCPnet->SendServer(i,tmpstr,strlen(tmpstr)+1);     // 这里注意size上要考虑\0问题
                //n_TCPnet->SendServer(i,"START",5+1);      // 连续调用貌似有问题
            }
            tail->next=ply_head;        // 形成环状
            // 为了统计人类玩家数，为了给玩家剩余墙数赋值
            player* tmp_head=ply_head;
            // 循环给剩余墙数赋值
            do
            {   // 统计人类玩家数
                // 整形数除法,21除2，取整=10
                //            21除3，取整=7
                //            21除4，取整=5
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
// 鼠标右键处理
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
            plyer[0].x=-1;
            plyer[0].y=-1;
            gameData[arr.x][arr.y]=0;
            break;
        case GD_RED:
            plyer[1].x=-1;
            plyer[1].y=-1;
            gameData[arr.x][arr.y]=0;
            break;
        case GD_GREEN:
            plyer[2].x=-1;
            plyer[2].y=-1;
            gameData[arr.x][arr.y]=0;
            break;
        case GD_BLUE:
            plyer[3].x=-1;
            plyer[3].y=-1;
            gameData[arr.x][arr.y]=0;
            break;
        case GD_WALL:
            {   // 从vector中，搜索当前右键选取的墙的位置
                std::vector<pos2d>::iterator itor=find(wall_vec.begin(),wall_vec.end(),arr);
                if (itor==wall_vec.end())
                {
                    //continue;
                    break;
                }
                int dist=distance(wall_vec.begin(),itor);
                // 如果间距是偶数，则下标也是偶数
                if (dist%2==0)
                {
                    // 注意避免迭代器失效的问题
                    gameData[itor->x][itor->y]=GD_BLANK;
                    gameData[(itor->x+(itor+1)->x)/2][(itor->y+(itor+1)->y)/2]=GD_BLANK;
                    gameData[(itor+1)->x][(itor+1)->y]=GD_BLANK;
                    wall_vec.erase(itor,itor+2);
                    break;
                }
                // 如果间距是奇数，则下标也是奇数
                else
                {
                    // 注意迭代器失效问题
                    gameData[itor->x][itor->y]=GD_BLANK;
                    gameData[(itor->x+(itor-1)->x)/2][(itor->y+(itor-1)->y)/2]=GD_BLANK;
                    gameData[(itor-1)->x][(itor-1)->y]=GD_BLANK;
                    wall_vec.erase(itor-1,itor+1);
                    break;
                }
                // 这个可加可不加，如果发生了删东西，选取的过程也重新来
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

//键盘处理
void CQuoridor::keyupproc(int keyparam)
{
    switch (keyparam)
    {
        //case KEY_F12:
        //	//直接过关
        //	if(iGameState=GAME_IN)
        //	{
        //		iEnemyNum=0;
        //	}		
        //	break;
    case VK_F9:
        g_debug_flag=!g_debug_flag;
        break;
    case VK_ESCAPE:
        //回到菜单
        iGameState=GAME_MENU;
        //initView();
        break;

    default:
        break;
    }
}

////构造位图贴图
//bool CQuoridor::g_OpenGL->LoadBMP_aux(char *filename, GLuint &texture)
//{
//    AUX_RGBImageRec *pImage = NULL;
//    
//    //加载图片内容
//    pImage = auxDIBImageLoad(filename);
//    if(pImage == NULL)
//        return false;
//
//    //创建纹理
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D,texture);
//    //生成2维纹理
//    gluBuild2DMipmaps(GL_TEXTURE_2D,4, 
//                      pImage->sizeX, 
//                      pImage->sizeY, 
//                      GL_RGB, GL_UNSIGNED_BYTE, 
//                      pImage->data 
//                     );
//    //释放内存
//    free(pImage->data);
//    free(pImage);
//    return true;
//}

//指定贴图
void CQuoridor::texture_select(UINT texture)
{
    glBindTexture  (GL_TEXTURE_2D, texture);

    // GL_TEXTURE_2D: 操作2D纹理.
    // GL_TEXTURE_WRAP_S: S方向上的贴图模式.
    // GL_CLAMP: 将纹理坐标限制在0.0,1.0的范围之内.如果超出了会如何呢.
    //           不会错误,只是会边缘拉伸填充.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    // 这里同上,只是它是T方向
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    // 这是纹理过滤
    // GL_TEXTURE_MIN_FILTER: 缩小过滤
    // GL_LINEAR: 线性过滤, 使用距离当前渲染像素中心最近的4个纹素加权平均值.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    
    // GL_TEXTURE_MAG_FILTER: 放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // GL_LINEAR_MIPMAP_NEAREST: 使用GL_NEAREST对最接近当前多边形的解析度
    //                  的两个层级贴图进行采样,然后用这两个值进行线性插值.
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

//画正方形 ，边长2e
void CQuoridor::tPic(float e)
{
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-e, -e,  0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( e, -e,  0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( e,  e,  0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-e,  e,  0.0f);
    glEnd();
}

//画矩形，左下角坐标xy，宽度w，高h
void CQuoridor::tPicRectangle(float x,float y,float w,float h,float deep)
{ 
    glPushMatrix();

    //画背景矩形
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
//画按钮
void CQuoridor::tPicButton(float x,float y,float w,float h,float ytex)
{ 
    glPushMatrix();

    //画背景矩形
    glTranslatef(x,y,-0.1f);	//z轴向后移动
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, ytex);
        glVertex3f(0.0, 0.0, 0.0f);
        
        glTexCoord2f(1.0f, ytex);
        glVertex3f(w, 0.0,  0.0f);

        //up
        glTexCoord2f(1.0f, ytex+0.5f);
        glVertex3f(w, h,  0.0f);
                
        glTexCoord2f(0.0f, ytex+0.5f);
        glVertex3f(0.0, h,  0.0f);
    glEnd();	

    glPopMatrix();
}

//设置灯光
//void CQuoridor::light0()
//{
//	GLfloat light_position[] = {10.0,10.0,3.0,1.0};
//
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//	
//	//glEnable(GL_LIGHTING);
//	//glEnable(GL_LIGHT0);
//	
//	//启动深度检测
//	glEnable(GL_DEPTH_TEST);
//	//启动颜色材质
//	glEnable(GL_COLOR_MATERIAL);
//}

//开屏动画
//void CQuoridor::showpreani()
//{
//	glPushMatrix();
//	//移动到当前位置
//	glTranslatef(0,0,-8.0);
//	texture_select(g_cactus[0]);	
//	tPic(2.0f);
//	//移动动画
//	tSquare(param1*0.2, 0.0, 0.1, 3.0);
//	glPopMatrix();
//}

//指定位置画一个正方形
void CQuoridor::tRectangle(float x, float y, float z, float w, float h, float r, float g, float b, float a)
{
    //属性进栈
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

//绘制鼠标
void CQuoridor::drawMouse()
{
    glPushMatrix();
    //glLoadIdentity();
    //属性进栈
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

//显示菜单
void CQuoridor::showmenu()
{
    //画背景图片
    glPushMatrix();
    glTranslatef(0.0,0.0,-0.2f);
    texture_select(g_cactus[0]);
    tPicRectangle((g_OpenGL->RCwidth-g_OpenGL->RCheight)/2.0f, 0, (float)g_OpenGL->RCheight, (float)g_OpenGL->RCheight);
    glPopMatrix();

    //  对应枚举关系     0          1          2          3          4
    char *menustr[]={"退    出","游戏说明","沙盒模式","联网游戏","单机游戏"};
    //char *menustr[]={"单机游戏","联网游戏","沙盒模式","游戏帮助","退    出"};

    // 在glOrtho模式下绘制菜单
    // 菜单的是从下网上绘制的，所以菜单序号如上所示
    for(int i=0;i<MENU_NUM;i++)
    {
        //文字
        myfont.Print2D(menu.x+10,menu.y+5+i*menu_dis,menustr[i],FONT4,1,1,1);

        //图片
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

//鼠标检测函数
//void CQuoridor::mouseproc(int lparam)
//{
//	xmouseOld=xmouse;
//	ymouseOld=ymouse;
//	xmouse=LOWORD(lparam);
//	ymouse=HIWORD(lparam);
//}

// 显示测试数据
void CQuoridor::show_Font_test()
{
    // Y /-------------\
    //   |600          |(800,600)
    //   |             |
    //   |0         800|
    //   \-------------/ X

    char tmpstr[64]={0};
    sprintf(tmpstr, "iMenu=%d",iMenu);
    myfont.Print2D(50,2,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);

    //for (int i=0; i<9;i++)
    //{
    //	for (int j=0; j<9;j++)
    //	{
    //		sprintf(tmpstr, "%d,",tmpflag[j][i]);
    //		myfont.Print2D(10*(j+1),260+10*i,tmpstr,FONT0,1.0f,1.0f,1.0f,0.5f);
    //	}
    //}

    for (int i=0; i<17;i++)
    {
        for (int j=0; j<17;j++)
        {
            sprintf(tmpstr, "%d,",gameData[j][i]);
            myfont.Print2D(10*(j+1),70+10*i,tmpstr,FONT0,1.0f,1.0f,1.0f,1.0f);
        }
    }

    sprintf(tmpstr, "pick_x=%d",pickup.x);
    myfont.Print2D(50,40,tmpstr,FONT0,1.0f,1.0f,1.0f,1.0f);
    sprintf(tmpstr, "pick_y=%d",pickup.y);
    myfont.Print2D(50,30,tmpstr,FONT0,1.0f,1.0f,1.0f,1.0f);
    sprintf(tmpstr, "arr.x=%d",arr.x);
    myfont.Print2D(50,20,tmpstr,FONT0,1.0f,1.0f,1.0f,1.0f);
    sprintf(tmpstr, "arr.y=%d",arr.y);
    myfont.Print2D(50,10,tmpstr,FONT0,1.0f,1.0f,1.0f,1.0f);

    //myfont.Print2D(100,2,"1,0",FONT4,1.0f,0.0f,0.0f);
    //myfont.Print2D(200,2,"2,0",FONT2,1.0f,1.0f,0.0f);
    //myfont.Print2D(300,2,"3,0",FONT4,1.0f,0.0f,1.0f);
    //myfont.Print2D(400,2,"4,0",FONT4,0.0f,0.0f,1.0f);
    //myfont.Print2D(500,2,"5,0",FONT5,0.0f,1.0f,0.0f);
}

void CQuoridor::showHelp()
{
    // 帮助画面中的装饰图片
    float cover_logo_len=(g_OpenGL->RCwidth - g_OpenGL->RCheight)/2.0f-2*lace;
    float player_logo_len=((g_OpenGL->RCwidth - g_OpenGL->RCheight)/2.0f-3*lace)/2.0f;

    // 封面图
    texture_select(g_cactus[0]);
    tPicRectangle((float)lace, g_OpenGL->RCheight-lace-cover_logo_len, cover_logo_len, cover_logo_len);
    // 玩家logo
    texture_select(g_cactus[3]);
    tPicRectangle((float)lace, (float)lace, player_logo_len, player_logo_len);
    texture_select(g_cactus[4]);
    tPicRectangle((float)lace*2+player_logo_len, (float)lace, player_logo_len, player_logo_len);
    texture_select(g_cactus[5]);
    tPicRectangle((float)lace, (float)lace*2+player_logo_len, player_logo_len, player_logo_len);
    texture_select(g_cactus[6]);
    tPicRectangle((float)lace*2+player_logo_len, (float)lace*2+player_logo_len, player_logo_len, player_logo_len);
    // 实物展示图
    texture_select(g_cactus[10]);
    tPicRectangle((g_OpenGL->RCwidth+g_OpenGL->RCheight)/2.0f, 0.45f*g_OpenGL->RCheight, (g_OpenGL->RCwidth-g_OpenGL->RCheight)/2.0f, (g_OpenGL->RCwidth-g_OpenGL->RCheight)*0.375f);

    // 半透明背景
    tRectangle((float)board_x,0,-0.3f,(float)g_OpenGL->RCheight,(float)g_OpenGL->RCheight,1,1,1,0.3f);

    char tmpstr[1024]="游戏简介";
    myfont.Print2D(g_OpenGL->RCwidth/2-60,g_OpenGL->RCheight-50,tmpstr,FONT5,1.0,1.0,0.0);
    sprintf(tmpstr,"游戏名称：《Quoridor》《墙棋》《步步为营》");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-80,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"         Quoridor是由游戏设计师 Mirko Marchesi，于1997年发明");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-110,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"         的2~4人对战棋类游戏");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-140,tmpstr,FONT3,0.8f,1.0,0.0);

    sprintf(tmpstr,"游戏规则");
    myfont.Print2D(g_OpenGL->RCwidth/2-60,g_OpenGL->RCheight-180,tmpstr,FONT5,1.0,1.0,0.0);
    sprintf(tmpstr,"游戏目标：谁先走到棋盘对面的一端谁就获得胜利");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-210,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"基本规则：玩家按顺序轮流行动，轮到自己时，只能从下面");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-240,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"         两种行动中选择一个执行：");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-270,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"       1.前后左右四个方向中选择一个，移动玩家所控制的棋子一步");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-300,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"       2.在可以放置墙的位置放置一堵墙");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-330,tmpstr,FONT3,0.8f,1.0,0.0);
    sprintf(tmpstr,"特殊规则：1. 如果要移动的位置存在其他玩家，则可以跳到其他玩家身后的位置");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-360,tmpstr,FONT2,1.0,0.8f,0.0);
    sprintf(tmpstr,"          2. 如果想跳到其他玩家身后时，有墙或者另外的玩家存在，");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-380,tmpstr,FONT2,1.0,0.8f,0.0);
    sprintf(tmpstr,"             则可以跳到其他玩家两边任意可走的位置上(棋盘边界官方也未说明)");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-400,tmpstr,FONT2,1.0,0.8f,0.0);
    sprintf(tmpstr,"          3. 在放置墙时，如果即将放置的一面墙，将导致场上有任何一个玩家");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-420,tmpstr,FONT2,1.0,0.8f,0.0);
    sprintf(tmpstr,"             不能到达彼岸，则该墙无法被放置");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-440,tmpstr,FONT2,1.0,0.8f,0.0);
    sprintf(tmpstr,"          4. 2人游戏时，每人有10个墙可以使用，3人游戏7个，4人游戏5个");
    myfont.Print2D(board_x+lace,g_OpenGL->RCheight-460,tmpstr,FONT2,1.0,0.8f,0.0);

    sprintf(tmpstr,"             (F4 最小化窗口，F9 显示调试信息)");
    myfont.Print2D(board_x+lace,lace+50,tmpstr,FONT2,1.0,0.8f,0.0);

    sprintf(tmpstr,"OpenGL信息：[厂商:%s] [渲染器:%s]",glGetString(GL_VENDOR),glGetString(GL_RENDERER));
    myfont.Print2D(board_x+lace,lace+32,tmpstr,FONT0,1.0,1.0,0.0);
    sprintf(tmpstr,"            [GL版本:%s][GLU版本:%s]",glGetString(GL_VERSION),gluGetString(GLU_VERSION));
    myfont.Print2D(board_x+lace,lace+20,tmpstr,FONT0,1.0,1.0,0.0);
    sprintf(tmpstr,"版本信息：[From 2014-11-29]-[%s][%s][_MSC_VER=%d][Ver= 0.8a]          作者：ChuaiGuoMing",__DATE__,__TIME__,_MSC_VER);
    myfont.Print2D(board_x+lace,lace,tmpstr,FONT0,1.0,1.0,0.0);

    //音乐控制按钮
    if (g_sound==1)
    {
        sprintf(tmpstr,"音乐: 开");
    } else {
        sprintf(tmpstr,"音乐: 关");
    }
    myfont.Print2D(menu.x+8,menu.y+menu_dis+6,tmpstr,FONT4,1,1,1);

    //图片
    texture_select(g_cactus[9]);
    if(iButton==BUTTON_MUSIC)
    {
        tPicButton((float)menu.x,(float)menu.y+menu_dis,(float)menu_w,(float)menu_h,0.0f);
    }
    else
    {
        tPicButton((float)menu.x,(float)menu.y+menu_dis,(float)menu_w,(float)menu_h,0.5f);
    }

    //文字
    sprintf(tmpstr,"按ESC返回");
    myfont.Print2D(menu.x+5,menu.y+5,tmpstr,FONT4,1,1,1);

    //图片
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
    // 所占图层深度
    float layer=-0.5;

    // 绘制棋盘底
    texture_select(g_cactus[1]);
    tPicRectangle((float)board_x,0,(float)g_OpenGL->RCheight,(float)g_OpenGL->RCheight,layer-0.1f);
    // 绘制棋盘
    texture_select(g_cactus[2]);
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            tPicRectangle(board_x+lace+i*(roadw+wall_w),lace+j*(roadw+wall_w),roadw,roadw,layer);
        }
    }

    // 绘制彩色边
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glTranslatef(0,0,layer);
    // 左边黄色边纹
    glColor3f(0, 1, 0);
    glBegin(GL_QUADS);
    glVertex3f( (float)board_x, 0,  0.0f);
    glVertex3f( (float)board_x, (float)g_OpenGL->RCheight,  0.0f);
    glVertex3f( (float)board_x+lace, (float)g_OpenGL->RCheight-lace,  0.0f);
    glVertex3f( (float)board_x+lace, (float)lace,  0.0f);
    glEnd();
    // 上边红色边纹
    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f( (float)board_x, (float)g_OpenGL->RCheight, 0.0f);
    glVertex3f( (float)board_x+g_OpenGL->RCheight, (float)g_OpenGL->RCheight,  0.0f);
    glVertex3f( (float)board_x+g_OpenGL->RCheight-lace, (float)g_OpenGL->RCheight-lace,  0.0f);
    glVertex3f( (float)board_x+lace, (float)g_OpenGL->RCheight-lace,  0.0f);
    glEnd();
    // 右边绿色边纹
    glColor3f(1, 1, 0);
    glBegin(GL_QUADS);
    glVertex3f((float) board_x+g_OpenGL->RCheight, (float)g_OpenGL->RCheight,  0.0f);
    glVertex3f((float) board_x+g_OpenGL->RCheight, 0,  0.0f);
    glVertex3f((float) board_x+g_OpenGL->RCheight-lace, (float)lace,  0.0f);
    glVertex3f((float) board_x+g_OpenGL->RCheight-lace, (float)g_OpenGL->RCheight-lace,  0.0f);
    glEnd();
    // 下边蓝色边纹
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
    // 所占图层深度
    float layer=-0.5;
    float alp=0.85f;
    // 绘制玩家信息指示标志区域
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
    // 下面这段代码是绘制轮到的玩家的指示三角形的
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
    // 绘制四块固定颜色固定位置的指示背景
    tRectangle(0,3*player_info_h,layer,player_info_w,player_info_h,1,1,0,alp);
    tRectangle(0,2*player_info_h,layer,player_info_w,player_info_h,1,0,0,alp);
    tRectangle(0,1*player_info_h,layer,player_info_w,player_info_h,0,1,0,alp);
    tRectangle(0,0,layer,(float)player_info_w,(float)player_info_h,0,0,1,alp);
    
    for (int i=0; i<4; i++)
    {
        if (plyer[i].id!=ID_CLOSED)
        {
            // 这里需要注意贴图的标号顺序
            texture_select(g_cactus[3+i]);
            tPicRectangle((float)lace,(3-i+1/2.0f)*player_info_h,player_info_w*0.5f,player_info_w*0.5f,layer+0.1f);
            if (plyer[i].id==ID_COMPUTER)
            {	// 绘制电脑的图标
                texture_select(g_cactus[8]);
                tPicRectangle((float)lace*2.5f+player_info_w*0.5f,(3-i+1/2.0f)*player_info_h,player_info_w*0.28f,player_info_w*0.28f,layer+0.1f);
            }
            sprintf(tmpstr,"墙剩余:%u",plyer[i].wall_num_left);
            myfont.Print2D(12,(int)((3-i+1/5.0f)*player_info_h),tmpstr,FONT3,1,1,1);
            if (iGameState==GAME_NETWORK)
            {
                if (i==3)
                {
                    sprintf(tmpstr,"%8s",n_NameAll[0]);
                    myfont.Print2D(12,(int)((3-i+1/3.0f)*player_info_h),tmpstr,FONT2,1,1,1);
                }
                /*sprintf(tmpstr,"%8s","XXXXXXXX");
                myfont.Print2D(12,(int)((3-i+1/3.0f)*player_info_h),tmpstr,FONT2,1,1,1);*/
            }
        }
    }

    // 按钮贴图
    texture_select(g_cactus[9]);
    sprintf(tmpstr,"返回菜单");
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
        sprintf(tmpstr,"确    认");
        break;
    case GAME_SENDBOX:
        sprintf(tmpstr,"重置棋盘");
        break;
    default:
        break;
    }
    myfont.Print2D(menu.x+10,menu.y+5,tmpstr,FONT4,1,1,1);
    // 按钮图片
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
    // 对应颜色的玩家，黄，红，绿，蓝
    for (int i=0; i<4; i++)
    {
        if (plyer[i].id!=ID_CLOSED)
        {
            texture_select(g_cactus[3+i]);
            if (plyer[i].x>-1 && plyer[i].y>-1)
            {
                tPicRectangle(board_x+lace+(roadw+wall_w)*plyer[i].x,lace+(roadw+wall_w)*plyer[i].y,roadw,roadw);
            }
        }
    }
    // 绘制墙
    for (size_t i=0; i<wall_vec.size();i+=2)
    {
        texture_select(g_cactus[7]);
        // 如果是横墙
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
        // 轮到谁走，在玩家图标上，给个动态提示
        float cx=board_x+lace+ply_head->x*(roadw+wall_w);
        float cy=        lace+ply_head->y*(roadw+wall_w);
        //属性进栈
        glPushAttrib(GL_CURRENT_BIT);
        glPushMatrix();
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        // 左下
        glVertex3f( cx-det,  cy-det,  0.5f);
        glVertex3f( cx-det,  cy-det+roadw/4.0f,  0.5f);	// 竖线
        glVertex3f( cx-det,  cy-det,  0.5f);
        glVertex3f( cx-det+roadw/4.0f,  cy-det,  0.5f);	//	横线
        // 左上
        glVertex3f( cx-det,  cy+roadw+det,  0.5f);
        glVertex3f( cx-det+roadw/4.0f,  cy+roadw+det,  0.5f);	// 横线
        glVertex3f( cx-det,  cy+roadw+det,  0.5f);
        glVertex3f( cx-det,  cy+roadw+det-roadw/4,  0.5f);	// 竖线
        // 右上
        glVertex3f( cx+roadw+det,  cy+roadw+det,  0.5f);
        glVertex3f( cx+roadw+det-roadw/4,  cy+roadw+det,  0.5f);	// 横线
        glVertex3f( cx+roadw+det,  cy+roadw+det,  0.5f);
        glVertex3f( cx+roadw+det,  cy+roadw+det-roadw/4,  0.5f);	// 竖线
        // 右下
        glVertex3f( cx+roadw+det,  cy-det,  0.5f);
        glVertex3f( cx+roadw+det-roadw/4,  cy-det,  0.5f);	// 横线
        glVertex3f( cx+roadw+det,  cy-det,  0.5f);
        glVertex3f( cx+roadw+det,  cy-det+roadw/4,  0.5f);	// 竖线
        glEnd();
        glEnable(GL_TEXTURE_2D);
        glPopMatrix();
        glPopAttrib();
        // 暂时注释掉动态显示
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
            // 这里以后换个贴图
            tRectangle(board_x+lace+pickup.x/2*(roadw+wall_w),lace+pickup.y/2*(roadw+wall_w),0,roadw,roadw,0.1f,0.5f,1,0.6f);
            // 绘制玩家可走预选区
            for (size_t i=0; i<preselect_pos.size();i++)
            {
                tRectangle(board_x+lace+preselect_pos[i].x/2*(roadw+wall_w),lace+preselect_pos[i].y/2*(roadw+wall_w),0,roadw,roadw,0.1f,0.5f,1,0.6f);
            }
#if 0
            // 单人模式时，提示玩家的目的地
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
            // 这里以后换个贴图
            tRectangle(board_x+lace+pickup.x/2*(roadw+wall_w),lace+pickup.y/2*(roadw+wall_w),0,roadw,roadw,0.1f,0.5f,1,0.6f);
        }
    }
    else if (pickup.x%2==0 && pickup.y%2!=0)
    {
        // 横坐标为偶数，纵坐标为奇数，为横墙
        tRectangle(board_x+lace+pickup.x/2*(roadw+wall_w),lace+(int)(pickup.y/2)*(roadw+wall_w)+roadw,0,(float)wall_l*4/9,(float)wall_w,1.0f,1,0,0.6f);
    }
    else if (pickup.x%2!=0 && pickup.y%2==0)
    {
        // 横坐标为奇数，纵坐标为偶数，为竖墙
        tRectangle(board_x+lace+(int)(pickup.x/2)*(roadw+wall_w)+roadw,lace+pickup.y/2*(roadw+wall_w),0,(float)wall_w,(float)wall_l*4/9,1.0f,1,0,0.6f);
    }

    return ;
}
// 绘制确认窗口
void CQuoridor::drawConfirm()
{
    float layer=0.3f;
    float tri_w=g_OpenGL->RCwidth/3.0f;
    float tri_h=g_OpenGL->RCheight/3.0f;
    //绘制背景半透明底纹窗口
    tRectangle(tri_w-menu_w,tri_h,layer,tri_w+2*menu_w,tri_h,0.0f,0.0f,0.0f,0.7f);
}

void CQuoridor::resetGameData()
{
    // 清空vector
    wall_vec.swap(std::vector<pos2d>());
    // 棋盘数据初始化
    memset(gameData,0,sizeof(gameData));
    // 重置玩家位置数据
    plyer[0].x=0;
    plyer[0].y=4;
    plyer[0].wall_num_left=0;
    plyer[0].next=NULL;
    plyer[1].x=4;
    plyer[1].y=8;
    plyer[1].wall_num_left=0;
    plyer[1].next=NULL;
    plyer[2].x=8;
    plyer[2].y=4;
    plyer[2].wall_num_left=0;
    plyer[2].next=NULL;
    plyer[3].x=4;
    plyer[3].y=0;
    plyer[3].wall_num_left=0;
    plyer[3].next=NULL;
    switch (iGameState)
    {
    case GAME_IN_CONFIG:
        plyer[0].id=ID_CLOSED;
        plyer[1].id=ID_COMPUTER;
        plyer[2].id=ID_CLOSED;
        plyer[3].id=ID_HUMAN;
        break;
    case GAME_SENDBOX:
        plyer[0].id=ID_HUMAN;
        plyer[1].id=ID_HUMAN;
        plyer[2].id=ID_HUMAN;
        plyer[3].id=ID_HUMAN;
        gameData[2*plyer[0].x][2*plyer[0].y]=GD_YELLOW;
        gameData[2*plyer[1].x][2*plyer[1].y]=GD_RED;
        gameData[2*plyer[2].x][2*plyer[2].y]=GD_GREEN;
        gameData[2*plyer[3].x][2*plyer[3].y]=GD_BLUE;
        break;
    case GAME_NET_CONFIG:
        plyer[0].id=ID_CLOSED;
        plyer[1].id=ID_CLOSED;
        plyer[2].id=ID_CLOSED;
        plyer[3].id=ID_CLOSED;
        break;
    default:
        break;
    }
    // 老代码玩家初始位置一次全都初始化，这里暂时注释掉
    //gameData[2*plyer[0].x][2*plyer[0].y]=GD_YELLOW;
    //gameData[2*plyer[1].x][2*plyer[1].y]=GD_RED;
    //gameData[2*plyer[2].x][2*plyer[2].y]=GD_GREEN;
    //gameData[2*plyer[3].x][2*plyer[3].y]=GD_BLUE;
    // 鼠标选取的位置
    pickup.x=-1;
    pickup.y=-1;
    // 玩家环装链表头指针
    ply_head=NULL;
    // 玩家胜利标志
    win_flag=GD_BLANK;
    // 如果网络对象被创建，那么删除
    if (n_TCPnet!=NULL)
    {
        n_netWorkStatus=0;
        delete n_TCPnet;
        n_TCPnet=NULL;
    }
}

void CQuoridor::drawInConfig()
{
#if 0
    // 原来方形的背景区，暂时不用这个样式
    // 绘制玩家信息指示标志区域
    tRectangle(0,3.1f*player_info_h,-0.2f,player_info_w*7,player_info_h*0.8f,1,1,0,0.4f);
    tRectangle(0,2.1f*player_info_h,-0.2f,player_info_w*7,player_info_h*0.8f,1,0,0,0.4f);
    tRectangle(0,1.1f*player_info_h,-0.2f,player_info_w*7,player_info_h*0.8f,0,1,0,0.4f);
    tRectangle(0,0.1f*player_info_h,-0.2f,player_info_w*7,player_info_h*0.8f,0,0,1,0.4f);
#endif
    float layer=-0.2f;
    // 绘制选择时的背景区
    //属性进栈
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
    // 此处注意与枚举值的对应关系顺序
    char *cfgstr[]={" 玩  家 "," 电  脑 "," 关  闭 "};
    // 按钮贴图
    texture_select(g_cactus[9]);
    for (int j=0; j<4; j++)
    {
        for (int i=0; i<3; i++)
        {
            if(i==plyer[3-j].id)
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

void CQuoridor::playerActionRule()
{
    // 已经存在的墙的位置，是不能被选取的
    if (gameData[arr.x][arr.y]==GD_WALL)
    {
        return ;
    }
    // 如果之前没有选取任何位置
    if ( pickup.x <0 && pickup.y <0 )
    {
        // 空白的玩家位，或者不能控制的玩家位
        if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]!=ply_head->color)
        {
            return;
        }
        pickup.x=arr.x;
        pickup.y=arr.y;
        // 如果选完是当前玩家位置
        if (gameData[pickup.x][pickup.y]==ply_head->color)
        {
            playerMovablePos(pickup);
        }
        // 这里是if else 后面有return，这里相当于return
    }
    // 之前有选取的位置
    else
    {   // 存在已选取的位置,连续点两次相同位置，在最开始过滤
        if (arr == pickup)
        {
            pickup.x = -1;
            pickup.y = -1;
            // 清除玩家可走待选位置(虽然不清也是可以的)
            preselect_pos.clear();
            return ;    // 连续点击两次相同坐标视为重选
        }
        // 并且鼠标再次点击时，选取的位置是在玩家棋子可以移动的位置
        // 要走的位置在可走位置的向量中
        //else if (0==arr.x%2 && 0==arr.y%2 && 0==pickup.x%2 && 0==pickup.y%2)
        else if (find(preselect_pos.begin(),preselect_pos.end(),arr)!=preselect_pos.end())
        {
            // 这种情况，进入到人物棋子处理阶段
            char tmp=0;
            // 交换
            tmp=gameData[arr.x][arr.y];
            gameData[arr.x][arr.y]=gameData[pickup.x][pickup.y];
            gameData[pickup.x][pickup.y]=tmp;
            // 在目标位置上，更新玩家变量
            switch (gameData[arr.x][arr.y])
            {
            case GD_BLANK:
                break;
            case GD_YELLOW:
                plyer[0].x=arr.x/2;
                plyer[0].y=arr.y/2;
                if (plyer[0].x==8)
                {
                    win_flag=GD_YELLOW;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_RED:
                plyer[1].x=arr.x/2;
                plyer[1].y=arr.y/2;
                if (plyer[1].y==0)
                {
                    win_flag=GD_RED;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_GREEN:
                plyer[2].x=arr.x/2;
                plyer[2].y=arr.y/2;
                if (plyer[2].x==0)
                {
                    win_flag=GD_GREEN;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_BLUE:
                plyer[3].x=arr.x/2;
                plyer[3].y=arr.y/2;
                if (plyer[3].y==8)
                {
                    win_flag=GD_BLUE;
                    iGameState=GAME_WIN;
                }
                break;
            default:
                break;
            }
            // 这里，是重要的算法流转。控制玩家移动后
            if (g_sound==1)
            {
                // 玩家棋子移动音效
                sndPlaySound("data/sound/player_move.wav",SND_ASYNC);
            }
            goto ACTION_RULE_EXIT;
        }
        // 能走到这里，应该是只有已经选了墙的情况，
        // 这里单独过滤，如果又点击了当前玩家，则按选中玩家处理
        else if (gameData[arr.x][arr.y]==ply_head->color)
        {
            pickup.x=arr.x;
            pickup.y=arr.y;
            playerMovablePos(pickup);
        }
        // 在选墙的过程中，跳过所有非墙的位置
        else if ((arr.x+arr.y)%2!=1)
        {
            return ;
        }
        // 如果上次选取的位置也是墙，这次选取的也是墙，并且这次选取的位置是空
        // 连续选取相同位置已经在最开始过滤
        // 这里是产生实际墙位置的唯一入口，所以在这里判定玩家墙剩余数是否可用
        else if ((pickup.x+pickup.y)%2==1 
            && (arr.x+arr.y)%2==1 
            && GD_BLANK == gameData[arr.x][arr.y]
            && ply_head->wall_num_left>0)
        {
            // 如果是横墙,并且这次选的和上次选的在同一行上
            if (pickup.x%2==0&&arr.y==pickup.y)
            {	// 如果这次选的在上一次选的左边一块位置
                // 并且两块连接的中间位置是可用的
                if(arr.x==pickup.x-2&&gameData[arr.x+1][arr.y]==GD_BLANK)
                {
                    // 压入墙绘制队列,一定先压入左边的块
                    wall_vec.push_back(arr);
                    wall_vec.push_back(pickup);
                    // 更新游戏算法数据,这里注意，把相连两墙位置的中间连接处也赋值
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x+1][arr.y]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto RULE_WALL_EXIT;
                }
                // 如果在这次选的在上一次右边一块位置
                // 并且两块连接的中间位置是可用的
                else if(arr.x==pickup.x+2&&gameData[arr.x-1][arr.y]==GD_BLANK)
                {
                    // 压入墙绘制队列,一定先压入左边的块
                    wall_vec.push_back(pickup);
                    wall_vec.push_back(arr);
                    // 更新游戏算法数据
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x-1][arr.y]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto RULE_WALL_EXIT;
                }
            }
            // 如果是竖墙，并且这次选的和上次选的在同一列上
            else if(pickup.y%2==0&&arr.x==pickup.x)
            {	// 如果这次选择的在上一次选择的下面一块
                // 并且两块连接的中间位置是可用的
                if (arr.y==pickup.y-2&&gameData[arr.x][arr.y+1]==GD_BLANK)
                {
                    // 压入墙绘制队列，一定先压入下面的一块
                    wall_vec.push_back(arr);
                    wall_vec.push_back(pickup);
                    // 更新游戏算法数据
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x][arr.y+1]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto RULE_WALL_EXIT;
                }
                // 如果这次选择的在上一次选择的上面一块
                // 并且两块连接的中间位置是可用的
                else if (arr.y==pickup.y+2&&gameData[arr.x][arr.y-1]==GD_BLANK)
                {
                    // 压入墙绘制队列，一定先压入下面的一块
                    wall_vec.push_back(pickup);
                    wall_vec.push_back(arr);
                    // 更新游戏算法数据
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x][arr.y-1]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto RULE_WALL_EXIT;
                }
            }
            // 上面使用goto语句，如果能够成功放置墙，那么已经跳出去了，
            // 这里，如果不满足一个可放墙的条件，视为重新选取
            pickup.x=arr.x;
            pickup.y=arr.y;
        }
        // 其他情况，一律视为重新选取墙
        else if ( gameData[arr.x][arr.y]!=GD_WALL )
        {
            pickup.x=arr.x;
            pickup.y=arr.y;
            // 清除玩家可走待选位置
            preselect_pos.clear();
        }
    }
    return ;
RULE_WALL_EXIT:
    // 这里判断，加入的墙是否合法的判定，如果新放的一堵墙，使得任何一个玩家无解，那么此墙非法
    // 如果不合法，那么需要还原原来的游戏数据
    if (!judgeWallLegal())
    {
        // 还原游戏算法数据
        gameData[arr.x][arr.y]=GD_BLANK;
        gameData[(arr.x+pickup.x)/2][(arr.y+pickup.y)/2]=GD_BLANK;
        gameData[pickup.x][pickup.y]=GD_BLANK;
        // 弹出刚压入的墙坐标(两个点)
        wall_vec.pop_back();
        wall_vec.pop_back();
        // 清空预选位置
        pickup.x=-1;
        pickup.y=-1;
        // 清除玩家可走待选位置
        preselect_pos.clear();
        return ;
    }
    if (g_sound==1)
    {
        // 放置墙的音效
        sndPlaySound("data/sound/wall_set.wav",SND_ASYNC);
    }
    // 当前玩家的可用墙数减1
    ply_head->wall_num_left--;
ACTION_RULE_EXIT:
    // 当玩家选择了移动人物，那么此玩家的行动就结束了，控制权应该交由下一位玩家
    ply_head=ply_head->next;
    // 清空预选位置
    pickup.x=-1;
    pickup.y=-1;
    // 清除玩家可走待选位置
    preselect_pos.clear();
    return ;
}
// 此函数的前提是，selected是在棋盘上选取的可用位置
// 此函数的出口是，preselect_pos玩家可走位置的向量
void CQuoridor::playerMovablePos( pos2d selected )
{
    // 玩家可走的位置，有个规则值得商榷，官方规则，对于棋盘边缘是否可跳的情况没有说明
    // 下面的代码中，有一个地方的注释写明，两种判断的区别
    pos2d tmppos;
    // 清空vector
    preselect_pos.swap(std::vector<pos2d>());
    // 优先判断上下左右四个方向是否可走，包括可跳的情况
    // 如果直接的四个方向可走，那么斜向四角可以直接不判断
    // 为true时，需要进行判断，四个方向时，找到了就置为false
    //bool left = true;
    //bool right = true;
    //bool up = true;
    //bool bottom = true;

    // 分情况判断玩家可走的可能位置
    // OOOOO
    // O@0OO
    // OOOOO
    // 如果不在最左边一列，且左边没有墙，且左边一格可走
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
    // 如果不是位于左边第二列或第一列，且左边有玩家，且左边玩家左边没有墙，且左边位置可跳
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
    // 如果不在最右边一列，且右边没有墙，且右边一格可走
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
    // 如果不是位于右边第二列或第一列，且右边有玩家，且右边玩家左边没有墙，且右边位置可跳
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
    // 如果不在最下边一列，且下边没有墙，且下边一格可走
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
    // 如果不是位于下边第二列或第一列，且下边有玩家，且下边玩家下边没有墙，且下边位置可跳
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
    // 如果不在最上边一列，且上边没有墙，且上边一格可走
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
    // 如果不是位于上边第二列或第一列，且上边有玩家，且上边玩家上边没有墙，且上边位置可跳
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
    // 对左上角可走的情况进行判断(不包含边界值)
    if ( selected.x > 2 && selected.y < 14 && gameData[selected.x-2][selected.y+2]==GD_BLANK )
    {   // 如果左边有敌对玩家且左边敌人的左边是墙，或，上面有敌对玩家且上面敌人的上边是墙
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
        // 或者左边的玩家左边还有玩家，或，上边的上边还有敌人
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
    // 如果玩家位于第二列,左上角可跳的情况(单独讨论)
    else if ( selected.x == 2 && selected.y < 14 
        && gameData[0][selected.y+2]==GD_BLANK )
    {   // 如果边界可跳，则把第一个条件打开 (option)
        if ( (gameData[1][selected.y]!=GD_WALL && gameData[0][selected.y]!=GD_BLANK && gameData[0][selected.y+1]!=GD_WALL) || 
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
    // 如果玩家位于第二行,左上角可跳的情况(单独讨论)
    else if ( selected.x > 2 && selected.y == 14 
        && gameData[selected.x-2][16]==GD_BLANK )
    {   // 如果边界可跳，则把第一个条件打开 (option)
        if ( (gameData[selected.x][15]!=GD_WALL && gameData[selected.x][16]!=GD_BLANK && gameData[selected.x-1][16]!=GD_WALL) || 
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
    // 对右上角可走的情况进行判断(不包含边界值)
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
    // 如果玩家位于右数第二列,右上角可跳的情况(单独讨论)
    else if ( selected.x == 14 && selected.y < 14 && gameData[16][selected.y+2]==GD_BLANK )
    {
        if ( (gameData[15][selected.y]!=GD_WALL && gameData[16][selected.y]!=GD_BLANK && gameData[16][selected.y+1]!=GD_WALL) || 
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
    // 如果玩家位于第二行,右上角可跳的情况(单独讨论)
    else if ( selected.x < 14 && selected.y == 14 && gameData[selected.x+2][16]==GD_BLANK )
    {
        if ( (gameData[selected.x][15]!=GD_WALL && gameData[selected.x][16]!=GD_BLANK && gameData[selected.x+1][16]!=GD_WALL ) || 
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
    // 对左下角可走的情况进行判断(不包含边界值)
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
    // 如果玩家位于第二列,左下角可跳的情况(单独讨论)
    else if ( selected.x == 2 && selected.y > 2 && gameData[0][selected.y-2]==GD_BLANK )
    {   // 如果边界可跳，则把第一个条件打开
        if ( (gameData[1][selected.y]!=GD_WALL && gameData[0][selected.y]!=GD_BLANK && gameData[0][selected.y-1]!=GD_WALL ) || 
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
    // 如果玩家位于第二行,左下角可跳的情况(单独讨论)
    else if ( selected.x > 2 && selected.y == 2 
        && gameData[selected.x-2][0]==GD_BLANK )
    {   // 如果边界可跳，则把第一个条件打开
        if ( (gameData[selected.x][1]!=GD_WALL && gameData[selected.x][0]!=GD_BLANK && gameData[selected.x-1][0]!=GD_WALL ) || 
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
    // 对右下角可走的情况进行判断(不包含边界值)
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
    // 如果玩家位于右数第二列,右下角可跳的情况(单独讨论)
    else if ( selected.x == 14 && selected.y >2 && gameData[16][selected.y-2]==GD_BLANK )
    {
        if ( (gameData[15][selected.y]!=GD_WALL && gameData[16][selected.y]!=GD_BLANK && gameData[16][selected.y-1]!=GD_WALL ) || 
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
    // 如果玩家位于下数第二行,右下角可跳的情况(单独讨论)
    else if ( selected.x < 14 && selected.y == 2 && gameData[selected.x+2][0]==GD_BLANK )
    {
        if ( (gameData[selected.x][1]!=GD_WALL && gameData[selected.x][0]!=GD_BLANK && gameData[selected.x+1][0]!=GD_WALL ) || 
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
}

void CQuoridor::freeSendBoxRule()
{
    // 沙盒模式下，初始化棋盘的按钮
    if (iButton==BUTTON_INIT_OR_CONFIRM)
    {
        resetGameData();
        return ;
    }
    // 已经存在的墙的位置，是不能被选取的
    if (gameData[arr.x][arr.y]==GD_WALL)
    {
        return ;
    }
    // 如果之前没有选取任何位置
    if ( pickup.x <0 && pickup.y <0 )
    {
        // 空白的玩家位
        if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]==GD_BLANK)
        {
            return ;
        }
        pickup.x=arr.x;
        pickup.y=arr.y;
#ifdef __DEBUG__
        // 方便测试，在沙盒模式里，显示鼠标选取可走区
        if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]!=GD_BLANK)
        {
            playerMovablePos(pickup);
        }
#endif
    }
    else
    {   // 存在已选取的位置,连续点两次相同位置，在最开始过滤
        if (arr == pickup)
        {
            pickup.x = -1;
            pickup.y = -1;
            return ;
        }
        // 并且鼠标再次点击时，前后两次都是玩家位置，即都为双偶数位
        else if (0==arr.x%2 && 0==arr.y%2 && 0==pickup.x%2 && 0==pickup.y%2)
        {
            // 这种情况，进入到人物棋子处理阶段
            char tmp=0;
            // 交换
            tmp=gameData[arr.x][arr.y];
            gameData[arr.x][arr.y]=gameData[pickup.x][pickup.y];
            gameData[pickup.x][pickup.y]=tmp;
            // 在目标位置上，更新玩家变量
            switch (gameData[arr.x][arr.y])
            {
            case GD_BLANK:
                break;
            case GD_YELLOW:
                plyer[0].x=arr.x/2;
                plyer[0].y=arr.y/2;
                break;
            case GD_RED:
                plyer[1].x=arr.x/2;
                plyer[1].y=arr.y/2;
                break;
            case GD_GREEN:
                plyer[2].x=arr.x/2;
                plyer[2].y=arr.y/2;
                break;
            case GD_BLUE:
                plyer[3].x=arr.x/2;
                plyer[3].y=arr.y/2;
                break;
            default:
                break;
            }
            // 对于旧的位置上，更新玩家变量
            switch (gameData[pickup.x][pickup.y])
            {
            case GD_BLANK:
                break;
            case GD_YELLOW:
                plyer[0].x=pickup.x/2;
                plyer[0].y=pickup.y/2;
                break;
            case GD_RED:
                plyer[1].x=pickup.x/2;
                plyer[1].y=pickup.y/2;
                break;
            case GD_GREEN:
                plyer[2].x=pickup.x/2;
                plyer[2].y=pickup.y/2;
                break;
            case GD_BLUE:
                plyer[3].x=pickup.x/2;
                plyer[3].y=pickup.y/2;
                break;
            default:
                break;
            }
            // 将选取位置赋值为无效值
            goto SEND_BOX_EXIT;
        }
        // 在选墙的过程中，跳过玩家位置
        else if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]==GD_BLANK)
        {
            return ;
        }
        // 如果上次选取的位置也是墙，这次选取的也是墙，并且这次选取的位置是空
        // 连续选取相同位置已经在最开始过滤
        else if ((pickup.x+pickup.y)%2==1 
            && (arr.x+arr.y)%2==1 
            && GD_BLANK == gameData[arr.x][arr.y])
        {
            // 如果是横墙,并且这次选的和上次选的在同一行上
            if (pickup.x%2==0&&arr.y==pickup.y)
            {	// 如果这次选的在上一次选的左边一块位置
                // 并且两块连接的中间位置是可用的
                if(arr.x==pickup.x-2&&gameData[arr.x+1][arr.y]==GD_BLANK)
                {
                    // 压入墙绘制队列,一定先压入左边的块
                    wall_vec.push_back(arr);
                    wall_vec.push_back(pickup);
                    // 更新游戏算法数据,这里注意，把相连两墙位置的中间连接处也赋值
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x+1][arr.y]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto SEND_BOX_EXIT;
                }
                // 如果在这次选的在上一次右边一块位置
                // 并且两块连接的中间位置是可用的
                else if(arr.x==pickup.x+2&&gameData[arr.x-1][arr.y]==GD_BLANK)
                {
                    // 压入墙绘制队列,一定先压入左边的块
                    wall_vec.push_back(pickup);
                    wall_vec.push_back(arr);
                    // 更新游戏算法数据
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x-1][arr.y]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto SEND_BOX_EXIT;
                }
            }
            // 如果是竖墙，并且这次选的和上次选的在同一列上
            else if(pickup.y%2==0&&arr.x==pickup.x)
            {	// 如果这次选择的在上一次选择的下面一块
                // 并且两块连接的中间位置是可用的
                if (arr.y==pickup.y-2&&gameData[arr.x][arr.y+1]==GD_BLANK)
                {
                    // 压入墙绘制队列，一定先压入下面的一块
                    wall_vec.push_back(arr);
                    wall_vec.push_back(pickup);
                    // 更新游戏算法数据
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x][arr.y+1]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto SEND_BOX_EXIT;
                }
                // 如果这次选择的在上一次选择的上面一块
                // 并且两块连接的中间位置是可用的
                else if (arr.y==pickup.y+2&&gameData[arr.x][arr.y-1]==GD_BLANK)
                {
                    // 压入墙绘制队列，一定先压入下面的一块
                    wall_vec.push_back(pickup);
                    wall_vec.push_back(arr);
                    // 更新游戏算法数据
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x][arr.y-1]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    goto SEND_BOX_EXIT;
                }
            }
        }
        if ( gameData[arr.x][arr.y]!=GD_WALL )
        {
            // 其他情况，一律视为重新选取
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
    // 清空预选位置
    pickup.x=-1;
    pickup.y=-1;
    return ;
}

bool CQuoridor::judgeWallLegal()
{
    /*
     注意：这里，为了游戏规则的严格性，判断一个墙是否被允许放下时，
           指的是，每判断到一个玩家时，棋盘上不计任何玩家的位置，再计算可走位置
           所以，这里保护一下游戏数据，在出函数的时候，再还原
    */
    char backupGD[sz][sz]={0};
    memcpy(backupGD,gameData,sizeof(gameData));
    // 将游戏数据中，去除所有玩家所在的位置
    player* pp=ply_head;
    do 
    {
        gameData[pp->x*2][pp->y*2]=GD_BLANK;
        pp=pp->next;
    } while (pp!=ply_head);
    // 定义一个临时的存放玩家可走位置的队列
    std::deque<pos2d> que;
    // 9x9的临时标记，记录玩家可走的位置,0为空，1为遍历过，可走，2为已经处理过
    char tmpflag[9][9];
    bool jump_flag=false;
    for (int i=0; i<4; i++)
    {   // 清空当前玩家可走位置的标记
        memset(tmpflag,0,sizeof(tmpflag));
        // 清空待选队列
        que.clear();
        // 证明可以找到出口的跳转标志置为false
        jump_flag=false;
        // 只要不是关闭的玩家
        if (plyer[i].id!=ID_CLOSED)
        {   // 先将当前玩家的位置传入，注意，玩家坐标是0~8的范围，转换为0~16的范围
            // 因为，选点算法返回的是0~16的范围
            pos2d tmpp;
            tmpp.x=plyer[i].x*2;
            tmpp.y=plyer[i].y*2;
            // 可走点标记二维数组是按照0~8设计的
            tmpflag[plyer[i].x][plyer[i].y]=1;
            que.push_back(tmpp);
            // 如果可走队列里还有内容，且没有找到终点的可走位置
            while (que.size()>0 && jump_flag == false)
            {   // 使用队列的头重新搜索可走位置
                playerMovablePos(*que.begin());
                // 其实正常来说，选可走点算法是不可能返回空向量的
                if (preselect_pos.size()>0)
                {
                    for (size_t j=0;j<preselect_pos.size();j++)
                    {   // 不同玩家不同的到达终点的条件
                        switch(plyer[i].color)
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
                        // 如果这一位置还没有被考虑过,没有被标记
                        if (tmpflag[preselect_pos[j].x/2][preselect_pos[j].y/2]==0)
                        {
                            // 可走点标记二维数组是按照0~8设计的, 将预选位置转为标记位置
                            tmpflag[preselect_pos[j].x/2][preselect_pos[j].y/2]=1;
                            que.push_back(preselect_pos[j]);
                        }
                    }
                }
                // 由这一点去计算的可走位置已经计算结束，弹出当前点
                que.pop_front();
            }
            if (jump_flag==false /*&& que.size()==0*/)
            {   // 如果当前玩家没有检测到可走的终点
                // 还原被修改过的游戏数据
                memcpy(gameData,backupGD,sizeof(gameData));
                return false;
            }
        }
    }
    // 还原被修改过的游戏数据
    memcpy(gameData,backupGD,sizeof(gameData));
    // 参与游戏的玩家全都有解
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
    //绘制背景半透明底纹窗口
    tRectangle(tri_w-menu_w,tri_h,layer,tri_w+2*menu_w,tri_h,0.0f,0.0f,0.0f,0.8f);

    glPushMatrix();
    glTranslatef(0,0,0.5f);
    switch (win_flag)
    {
    case GD_YELLOW:
        sprintf(tmpstr,"黄 色 玩 家");
        myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT8,1,1,0);
        break;
    case GD_RED:
        sprintf(tmpstr,"红 色 玩 家");
        myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT8,1,0,0);
        break;
    case GD_GREEN:
        sprintf(tmpstr,"绿 色 玩 家");
        myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT8,0,1,0);
        break;
    case GD_BLUE:
        sprintf(tmpstr,"蓝 色 玩 家");
        myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+5/7.0)),tmpstr,FONT8,0,0,1);
        break;
    default:
        break;
    }
    sprintf(tmpstr,"  胜  利 ");
    myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+3/7.0)),tmpstr,FONT8,1,1,1);
    sprintf(tmpstr," 好 厉 害！");
    myfont.Print2D(g_OpenGL->RCwidth/2,(int)(tri_h*(1+1/8.0)),tmpstr,FONT8,1,1,1);
    glPopMatrix();

    // 这里可以写一个烟花函数 
}

void CQuoridor::drawNetworkOp()
{
    char tmpstr[128]="";
    if ((2!=n_netWorkStatus && g_OpenGL->Xmouse<g_OpenGL->RCwidth/2) || 1==n_netWorkStatus)
    {
        // 左边服务器一侧的底图
        tRectangle(0,0,-0.5f,g_OpenGL->RCwidth/2.0f,(float)g_OpenGL->RCheight,0.8f,0,0,0.6f);
        // 右边客户端一侧的底图
        tRectangle(g_OpenGL->RCwidth/2.0f,0,-0.5f,g_OpenGL->RCwidth/2.0f,(float)g_OpenGL->RCheight,0,0.8f,0,0.4f);
        // 显示本机IP以及服务器信息
        sprintf(tmpstr,"本 机 IP: %s", n_loaclIP);
        myfont.Print2D(g_OpenGL->RCwidth/8,(int)(g_OpenGL->RCheight*0.6),tmpstr,FONT4,1,1,1);
        sprintf(tmpstr,"监听端口: %u", n_port);
        myfont.Print2D(g_OpenGL->RCwidth/8,(int)(g_OpenGL->RCheight*0.6)-30,tmpstr,FONT4,1,1,1);
    }
    else
    {
        // 左边服务器一侧的底图
        tRectangle(0,0,-0.5f,g_OpenGL->RCwidth/2.0f,(float)g_OpenGL->RCheight,0.8f,0,0,0.4f);
        // 右边客户端一侧的底图
        tRectangle(g_OpenGL->RCwidth/2.0f,0,-0.5f,g_OpenGL->RCwidth/2.0f,(float)g_OpenGL->RCheight,0,0.8f,0,0.6f);
        // 显示连接服务器IP以及端口信息
        sprintf(tmpstr,"服务器IP: %s", n_IP);
        myfont.Print2D((int)(g_OpenGL->RCwidth*0.62),(int)(g_OpenGL->RCheight*0.6),tmpstr,FONT4,1,1,1);
        sprintf(tmpstr,"服务端口: %u", n_port);
        myfont.Print2D((int)(g_OpenGL->RCwidth*0.62),(int)(g_OpenGL->RCheight*0.6)-30,tmpstr,FONT4,1,1,1);
        // 如果已经是客户端状态了，这里显示本机IP
        if (2==n_netWorkStatus)
        {
            sprintf(tmpstr,"本 机 IP: %s", n_loaclIP);
            myfont.Print2D((int)(g_OpenGL->RCwidth*0.62),(int)(g_OpenGL->RCheight*0.6)-80,tmpstr,FONT4,1,1,1);
            sprintf(tmpstr,"本机名称: %8s", n_Name);
            myfont.Print2D((int)(g_OpenGL->RCwidth*0.62),(int)(g_OpenGL->RCheight*0.6)-110,tmpstr,FONT4,1,1,1);
        }
    }
    //------------------------------------
    switch (n_netWorkStatus)
    {
    case 0:
        sprintf(tmpstr,"建立主机");
        myfont.Print2D(g_OpenGL->RCwidth/4-menu_w/2+10,g_OpenGL->RCheight*2/3+5,tmpstr,FONT4,1,1,1);
        texture_select(g_cactus[9]);
        tPicButton((float)(g_OpenGL->RCwidth/4-menu_w/2),(float)(g_OpenGL->RCheight*2/3),
            (float)menu_w,(float)menu_h,(iButton==BUTTON_SERVER)?0.0f:0.5f);
        sprintf(tmpstr,"连接主机");
        myfont.Print2D(g_OpenGL->RCwidth*3/4-menu_w/2+10,g_OpenGL->RCheight*2/3+5,tmpstr,FONT4,1,1,1);
        tPicButton((float)(g_OpenGL->RCwidth*3/4-menu_w/2),(float)(g_OpenGL->RCheight*2/3),
            (float)menu_w,(float)menu_h,(iButton==BUTTON_CLIENT)?0.0f:0.5f);
        break;
    case 1:
        sprintf(tmpstr,"建立主机");
        myfont.Print2D(g_OpenGL->RCwidth/4-menu_w/2+10,g_OpenGL->RCheight*2/3+5,tmpstr,FONT4,0.0f,1.0f,0.0f);
        texture_select(g_cactus[9]);
        tPicButton((float)(g_OpenGL->RCwidth/4-menu_w/2),(float)(g_OpenGL->RCheight*2/3),
            (float)menu_w,(float)menu_h,0.5f);
        //sprintf(tmpstr,"主机测试");
        //myfont.Print2D(g_OpenGL->RCwidth/4-menu_w/2+10,g_OpenGL->RCheight*1/6+5,tmpstr,FONT4,1,1,1);
        //tPicButton((float)(g_OpenGL->RCwidth/4-menu_w/2),(float)(g_OpenGL->RCheight*1/6),
        //    (float)menu_w,(float)menu_h,(iButton==BUTTON_SERVER_TEST)?0:0.5f);
        sprintf(tmpstr,"开始游戏");
        myfont.Print2D((g_OpenGL->RCwidth-menu_w)/2+10,menu.y+5,tmpstr,FONT4,1,1,1);
        tPicButton((float)(g_OpenGL->RCwidth-menu_w)/2,(float)menu.y,
            (float)menu_w,(float)menu_h,(iButton==BUTTON_SERVER_START)?0:0.5f);
        // 显示当前链接的IP列表
        // 先固定显示主机IP
        sprintf(tmpstr,"[1] %8s (%16s)",n_NameAll[0], n_loaclIP);
        myfont.Print2D(g_OpenGL->RCwidth/12,(int)(g_OpenGL->RCheight*0.43),tmpstr,FONT4,1,1,1);
        tRectangle(g_OpenGL->RCwidth/12-2.0f,g_OpenGL->RCheight*0.43f-3,-0.3f,380,27,0,0,1,1);
        // 再显示已连接的客户端IP列表
        for (int i=0; i<3; i++)
        {
            if (strlen(n_TCPnet->GetClientIP(i))==0)
            {
                sprintf(tmpstr,"[%1d] %8s (%16s)",i+2,"--[空]--","---.---.---.---");
            }
            else
            {
                sprintf(tmpstr,"[%1d] %8s (%16s)",i+2,n_NameAll[i+1],n_TCPnet->GetClientIP(i));
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
        sprintf(tmpstr,"连接主机");
        myfont.Print2D(g_OpenGL->RCwidth*3/4-menu_w/2+10,g_OpenGL->RCheight*2/3+5,tmpstr,FONT4,0.1f,0.1f,0.1f);
        tPicButton((float)(g_OpenGL->RCwidth*3/4-menu_w/2),(float)(g_OpenGL->RCheight*2/3),
            (float)menu_w,(float)menu_h,0.5f);
        //sprintf(tmpstr,"客户测试");
        //myfont.Print2D(g_OpenGL->RCwidth*3/4-menu_w/2+10,g_OpenGL->RCheight*1/6+5,tmpstr,FONT4,1,1,1);
        //tPicButton((float)(g_OpenGL->RCwidth*3/4-menu_w/2),(float)(g_OpenGL->RCheight*1/6),(float)menu_w,(float)menu_h,(iButton==BUTTON_CLIENT_TEST)?0:0.5f);
        break;
    default:
        break;
    }
    // 返回按钮
    sprintf(tmpstr,"按ESC返回");
    myfont.Print2D(menu.x+4,menu.y+5,tmpstr,FONT4,1,1,1);
    tPicButton((float)menu.x,(float)menu.y,(float)menu_w,(float)menu_h,(iButton==BUTTON_RETURN)?0:0.5f);
}

void CQuoridor::OnReceiveNetData( char* data, int length, DWORD userdata )
{
    if (pThis->n_netWorkStatus==1)
    {   // 服务器接收数据格式：
        // 00000000001111111111222222
        // 01234567890123456789012345
        // S127.  0.  0.  1_  2_unamed
        // S[     IP       ][cn][ data ]
        char recMsg[128]={0};
        memcpy(recMsg,data+17,3);
        size_t clientID=atoi(recMsg);
        memcpy(recMsg,data+21,length-21);
        if (pThis->iGameState==GAME_NETWORK)
        {
            if (*recMsg=='P')
            {   // CP1M46
                char tmpC=*(recMsg+1);
                int color=atoi(&tmpC);      // 解析出玩家的颜色
                tmpC=*(recMsg+2);           // 解析出玩家的行动，是移动角色(M)还是放墙(W)
                if ('W'==tmpC)          // 服务器收到其他玩家放置墙的消息
                {
                    char snum[3]={0};
                    pos2d wall1,wall2,mid;
                    memcpy(snum,recMsg+3,2);
                    wall1.x=atoi(snum);
                    memcpy(snum,recMsg+5,2);
                    wall1.y=atoi(snum);
                    memcpy(snum,recMsg+7,2);
                    mid.x=atoi(snum);
                    memcpy(snum,recMsg+9,2);
                    mid.y=atoi(snum);
                    memcpy(snum,recMsg+11,2);
                    wall2.x=atoi(snum);
                    memcpy(snum,recMsg+13,2);
                    wall2.y=atoi(snum);
                    // 压入墙绘制队列，一定先压入下面的一块
                    pThis->wall_vec.push_back(wall1);
                    pThis->wall_vec.push_back(wall2);
                    // 更新游戏算法数据
                    pThis->gameData[wall1.x][wall1.y]=GD_WALL;
                    pThis->gameData[mid.x][mid.y]=GD_WALL;
                    pThis->gameData[wall2.x][wall2.y]=GD_WALL;
                    // color-1 实际上可以表示，玩家数组对应的下标
                    pThis->plyer[color-1].wall_num_left--;
                    if (g_sound==1)
                    {
                        // 放置墙的音效
                        sndPlaySound("data/sound/wall_set.wav",SND_ASYNC);
                    }
                }
                else if ('M'==tmpC)
                {
                    tmpC=*(recMsg+3);
                    int plX=atoi(&tmpC);
                    tmpC=*(recMsg+4);
                    int plY=atoi(&tmpC);
                    // 这基于颜色与玩家位置的对应关系
                    pThis->gameData[pThis->plyer[color-1].x*2][pThis->plyer[color-1].y*2]=GD_BLANK;
                    pThis->gameData[plX*2][plY*2]=color;
                    pThis->plyer[color-1].x=plX;
                    pThis->plyer[color-1].y=plY;
                    if (g_sound==1)
                    {
                        // 玩家棋子移动音效
                        sndPlaySound("data/sound/player_move.wav",SND_ASYNC);
                    }
                    if (*(recMsg+5)=='F')
                    {
                        switch (color)
                        {
                        case GD_YELLOW:
                                pThis->win_flag=GD_YELLOW;
                            break;
                        case GD_RED:
                                pThis->win_flag=GD_RED;
                            break;
                        case GD_GREEN:
                                pThis->win_flag=GD_GREEN;
                            break;
                        case GD_BLUE:
                                pThis->win_flag=GD_BLUE;
                            break;
                        default:
                            break;
                        }
                        pThis->iGameState=GAME_WIN;
                    }
                }
                // 服务器向所有客户端转发收到的内容，跳过消息来源
                for (int i=0;i<pThis->n_TCPnet->GetConnectionNumber();i++)
                {
                    if (i==clientID)
                    {
                        continue;
                    }
                    pThis->n_TCPnet->SendServer(i,recMsg,strlen(recMsg)+1);
                }
                pThis->ply_head=pThis->ply_head->next;
            }
        }
        else if (pThis->iGameState==GAME_NET_CONFIG)
        {
            if (clientID<=3)
            {   // 服务器接收客户端发来的玩家名
                strncpy(pThis->n_NameAll[clientID+1],recMsg,8);
                pThis->n_NameAll[clientID+1][8]='\0';
            }
        }
    }
    else if (pThis->n_netWorkStatus==2)
    {   // 客户端接收数据格式：
        // 00000000001111111111222222
        // 01234567890123456789012345
        // CXXXXXXXXX
        // C[ data ]
        //if (strcmp(data,"CSTART")==0)
        //{
        //    pThis->iGameState=GAME_NETWORK;
        //}
        if (strncmp(data,"CP",2)==0)
        {   // CP1M46
            char tmpC=*(data+2);
            int color=atoi(&tmpC);      // 解析出玩家的颜色
            tmpC=*(data+3);             // 解析出玩家的行动，是移动角色(M)还是放墙(W)
            if ('W'==tmpC)          // 客户端收到服务器发来的某玩家放墙的消息
            {
                char snum[3]={0};
                pos2d wall1,wall2,mid;
                memcpy(snum,data+4,2);
                wall1.x=atoi(snum);
                memcpy(snum,data+6,2);
                wall1.y=atoi(snum);
                memcpy(snum,data+8,2);
                mid.x=atoi(snum);
                memcpy(snum,data+10,2);
                mid.y=atoi(snum);
                memcpy(snum,data+12,2);
                wall2.x=atoi(snum);
                memcpy(snum,data+14,2);
                wall2.y=atoi(snum);
                // 压入墙绘制队列，一定先压入下面的一块
                pThis->wall_vec.push_back(wall1);
                pThis->wall_vec.push_back(wall2);
                // 更新游戏算法数据
                pThis->gameData[wall1.x][wall1.y]=GD_WALL;
                pThis->gameData[mid.x][mid.y]=GD_WALL;
                pThis->gameData[wall2.x][wall2.y]=GD_WALL;
                // color-1 实际上可以表示，玩家数组对应的下标
                pThis->plyer[color-1].wall_num_left--;
                if (g_sound==1)
                {
                    // 放置墙的音效
                    sndPlaySound("data/sound/wall_set.wav",SND_ASYNC);
                }
            }
            else if ('M'==tmpC)
            {
                tmpC=*(data+4);
                int plX=atoi(&tmpC);
                tmpC=*(data+5);
                int plY=atoi(&tmpC);
                // 这基于颜色与玩家位置的对应关系
                pThis->gameData[pThis->plyer[color-1].x*2][pThis->plyer[color-1].y*2]=GD_BLANK;
                pThis->gameData[plX*2][plY*2]=color;
                pThis->plyer[color-1].x=plX;
                pThis->plyer[color-1].y=plY;
                if (g_sound==1)
                {
                    // 玩家棋子移动音效
                    sndPlaySound("data/sound/player_move.wav",SND_ASYNC);
                }
                if (*(data+6)=='F')
                {
                    switch (color)
                    {
                    case GD_YELLOW:
                        pThis->win_flag=GD_YELLOW;
                        break;
                    case GD_RED:
                        pThis->win_flag=GD_RED;
                        break;
                    case GD_GREEN:
                        pThis->win_flag=GD_GREEN;
                        break;
                    case GD_BLUE:
                        pThis->win_flag=GD_BLUE;
                        break;
                    default:
                        break;
                    }
                    pThis->iGameState=GAME_WIN;
                }
            }
            pThis->ply_head=pThis->ply_head->next;
        }
        else if (strncmp(data,"CREADY",6)==0)   // 前六个字节
        {   // 三个玩家格式，例如，CREADY1N3
            // N前面的数0,1,2表示在服务器中的客户端列表，主机不在编号内
            char tmpC=*(data+6);
            int clientID=atoi(&tmpC);     // 在服务器的客户端列表中的编号
            tmpC=*(data+8);
            int playerNum=atoi(&tmpC);    // 客户端连接数量，即参与游戏的玩家总数减一
            pThis->plyer[3].id=ID_NET_PLAYER;
            pThis->ply_head=&pThis->plyer[3];

            player* tail=pThis->ply_head;
            for (int i=0; i<playerNum;i++)
            {
                switch (i)
                {
                case 0:// 设置为红色玩家
                    if (0==clientID)
                    {
                        pThis->plyer[1].id=ID_HUMAN;
                    } else {
                        pThis->plyer[1].id=ID_NET_PLAYER;
                    }
                    pThis->gameData[pThis->plyer[1].x*2][pThis->plyer[1].y*2]=GD_RED;
                    tail->next=&pThis->plyer[1];
                    tail=&pThis->plyer[1];
                    break;
                case 1:// 设置为绿色玩家
                    if (1==clientID)
                    {
                        pThis->plyer[2].id=ID_HUMAN;
                    } else {
                        pThis->plyer[2].id=ID_NET_PLAYER;
                    }
                    pThis->gameData[pThis->plyer[2].x*2][pThis->plyer[2].y*2]=GD_GREEN;
                    tail->next=&pThis->plyer[2];
                    tail=&pThis->plyer[2];
                    break;
                case 2:// 设置为黄色玩家
                    if (2==clientID)
                    {
                        pThis->plyer[0].id=ID_HUMAN;
                    } else {
                        pThis->plyer[0].id=ID_NET_PLAYER;
                    }
                    pThis->gameData[pThis->plyer[0].x*2][pThis->plyer[0].y*2]=GD_YELLOW;
                    // 当存在黄色玩家时，比较特别
                    pThis->ply_head->next=&pThis->plyer[0];
                    pThis->plyer[0].next=&pThis->plyer[1];
                    break;
                default:
                    break;
                }
            }
            tail->next=pThis->ply_head;        // 形成环状
            // 为了统计人类玩家数，为了给玩家剩余墙数赋值
            player* tmp_head=pThis->ply_head;
            // 循环给剩余墙数赋值
            do
            {   // 统计人类玩家数
                // 整形数除法,21除2，取整=10
                //            21除3，取整=7
                //            21除4，取整=5
                tmp_head->wall_num_left=wall_total_num/(playerNum+1);
                tmp_head=tmp_head->next;
            }while (pThis->ply_head!=tmp_head);

            pThis->iGameState=GAME_NETWORK;
        }
    }
}

void CQuoridor::playerActionRule_network()
{
    char tmpstr[16];        // 网络发包所用缓存
    // 已经存在的墙的位置，是不能被选取的
    if (gameData[arr.x][arr.y]==GD_WALL)
    {
        return ;
    }
    // 如果之前没有选取任何位置
    if ( pickup.x <0 && pickup.y <0 )
    {
        // 空白的玩家位，或者不能控制的玩家位
        if (0==arr.x%2 && 0==arr.y%2 && gameData[arr.x][arr.y]!=ply_head->color)
        {
            return;
        }
        pickup.x=arr.x;
        pickup.y=arr.y;
        // 如果选完是当前玩家位置
        if (gameData[pickup.x][pickup.y]==ply_head->color)
        {
            playerMovablePos(pickup);
        }
        // 这里是if else 后面有return，这里相当于return
    }
    // 之前有选取的位置
    else
    {   // 存在已选取的位置,连续点两次相同位置，在最开始过滤
        if (arr == pickup)
        {
            pickup.x = -1;
            pickup.y = -1;
            // 清除玩家可走待选位置(虽然不清也是可以的)
            preselect_pos.clear();
            return ;	// 连续点击两次相同坐标视为重选
        }
        // 并且鼠标再次点击时，选取的位置是在玩家棋子可以移动的位置
        // 要走的位置在可走位置的向量中
        //else if (0==arr.x%2 && 0==arr.y%2 && 0==pickup.x%2 && 0==pickup.y%2)
        else if (find(preselect_pos.begin(),preselect_pos.end(),arr)!=preselect_pos.end())
        {
            // 这种情况，进入到人物棋子处理阶段
            char tmp=0;
            // 交换
            tmp=gameData[arr.x][arr.y];
            gameData[arr.x][arr.y]=gameData[pickup.x][pickup.y];
            gameData[pickup.x][pickup.y]=tmp;
            // 在目标位置上，更新玩家变量
            switch (gameData[arr.x][arr.y])
            {
            case GD_BLANK:
                break;
            case GD_YELLOW:
                plyer[0].x=arr.x/2;
                plyer[0].y=arr.y/2;
                if (plyer[0].x==8)
                {
                    win_flag=GD_YELLOW;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_RED:
                plyer[1].x=arr.x/2;
                plyer[1].y=arr.y/2;
                if (plyer[1].y==0)
                {
                    win_flag=GD_RED;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_GREEN:
                plyer[2].x=arr.x/2;
                plyer[2].y=arr.y/2;
                if (plyer[2].x==0)
                {
                    win_flag=GD_GREEN;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_BLUE:
                plyer[3].x=arr.x/2;
                plyer[3].y=arr.y/2;
                if (plyer[3].y==8)
                {
                    win_flag=GD_BLUE;
                    iGameState=GAME_WIN;
                }
                break;
            default:
                break;
            }
            // 这里，是重要的算法流转。控制玩家移动后
            if (g_sound==1)
            {
                // 玩家棋子移动音效
                sndPlaySound("data/sound/player_move.wav",SND_ASYNC);
            }
            // 控制玩家移动后，向网络发送玩家移动的包
            // 向所有客户机发出P1M45,P后数字为玩家颜色代码，
            // M代表玩家移动，后数字代表移动后的坐标，最后一位F代表游戏结束，当前玩家胜利
            if (iGameState==GAME_WIN)
            {
                sprintf(tmpstr,"P%1dM%1d%1dF",ply_head->color,arr.x/2,arr.y/2);
            }
            else
            {
                sprintf(tmpstr,"P%1dM%1d%1d_",ply_head->color,arr.x/2,arr.y/2);
            }
            if (n_netWorkStatus==1)     // 如果是服务器端
            {
                for (int i=0;i<n_TCPnet->GetConnectionNumber();i++)
                {
                    n_TCPnet->SendServer(i,tmpstr,strlen(tmpstr)+1);
                }
            }
            else if (n_netWorkStatus==2)
            {
                n_TCPnet->SendClient(tmpstr,strlen(tmpstr)+1);
            }
            goto ACTION_RULE_EXIT;
        }
        // 能走到这里，应该是只有已经选了墙的情况，
        // 这里单独过滤，如果又点击了当前玩家，则按选中玩家处理
        else if (gameData[arr.x][arr.y]==ply_head->color)
        {
            pickup.x=arr.x;
            pickup.y=arr.y;
            playerMovablePos(pickup);
        }
        // 在选墙的过程中，跳过所有非墙的位置
        else if ((arr.x+arr.y)%2!=1)
        {
            return ;
        }
        // 如果上次选取的位置也是墙，这次选取的也是墙，并且这次选取的位置是空
        // 连续选取相同位置已经在最开始过滤
        // 这里是产生实际墙位置的唯一入口，所以在这里判定玩家墙剩余数是否可用
        else if ((pickup.x+pickup.y)%2==1 
            && (arr.x+arr.y)%2==1 
            && GD_BLANK == gameData[arr.x][arr.y]
        && ply_head->wall_num_left>0)
        {
            // 如果是横墙,并且这次选的和上次选的在同一行上
            if (pickup.x%2==0&&arr.y==pickup.y)
            {	// 如果这次选的在上一次选的左边一块位置
                // 并且两块连接的中间位置是可用的
                if(arr.x==pickup.x-2&&gameData[arr.x+1][arr.y]==GD_BLANK)
                {
                    // 压入墙绘制队列,一定先压入左边的块
                    wall_vec.push_back(arr);
                    wall_vec.push_back(pickup);
                    // 更新游戏算法数据,这里注意，把相连两墙位置的中间连接处也赋值
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x+1][arr.y]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    // 向所有客户机发出P1W070807090710,P后数字为玩家颜色代码，
                    // W代表是墙的坐标信息，连续写入三个点的左边，表示墙的位置，由于绘制原因，需要严格按照顺序
                    sprintf(tmpstr,"P%1dW%2d%2d%2d%2d%2d%2d",ply_head->color,arr.x,arr.y,(arr.x+pickup.x)/2,(arr.y+pickup.y)/2,pickup.x,pickup.y);
                    goto RULE_WALL_EXIT;
                }
                // 如果在这次选的在上一次右边一块位置
                // 并且两块连接的中间位置是可用的
                else if(arr.x==pickup.x+2&&gameData[arr.x-1][arr.y]==GD_BLANK)
                {
                    // 压入墙绘制队列,一定先压入左边的块
                    wall_vec.push_back(pickup);
                    wall_vec.push_back(arr);
                    // 更新游戏算法数据
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x-1][arr.y]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    // 向所有客户机发出P1W070807090710,P后数字为玩家颜色代码，
                    // W代表是墙的坐标信息，连续写入三个点的左边，表示墙的位置，由于绘制原因，需要严格按照顺序
                    sprintf(tmpstr,"P%1dW%2d%2d%2d%2d%2d%2d",ply_head->color,pickup.x,pickup.y,(arr.x+pickup.x)/2,(arr.y+pickup.y)/2,arr.x,arr.y);
                    goto RULE_WALL_EXIT;
                }
            }
            // 如果是竖墙，并且这次选的和上次选的在同一列上
            else if(pickup.y%2==0&&arr.x==pickup.x)
            {	// 如果这次选择的在上一次选择的下面一块
                // 并且两块连接的中间位置是可用的
                if (arr.y==pickup.y-2&&gameData[arr.x][arr.y+1]==GD_BLANK)
                {
                    // 压入墙绘制队列，一定先压入下面的一块
                    wall_vec.push_back(arr);
                    wall_vec.push_back(pickup);
                    // 更新游戏算法数据
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x][arr.y+1]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    // 向所有客户机发出P1W070807090710,P后数字为玩家颜色代码，
                    // W代表是墙的坐标信息，连续写入三个点的左边，表示墙的位置，由于绘制原因，需要严格按照顺序
                    sprintf(tmpstr,"P%1dW%2d%2d%2d%2d%2d%2d",ply_head->color,arr.x,arr.y,(arr.x+pickup.x)/2,(arr.y+pickup.y)/2,pickup.x,pickup.y);
                    goto RULE_WALL_EXIT;
                }
                // 如果这次选择的在上一次选择的上面一块
                // 并且两块连接的中间位置是可用的
                else if (arr.y==pickup.y+2&&gameData[arr.x][arr.y-1]==GD_BLANK)
                {
                    // 压入墙绘制队列，一定先压入下面的一块
                    wall_vec.push_back(pickup);
                    wall_vec.push_back(arr);
                    // 更新游戏算法数据
                    gameData[arr.x][arr.y]=GD_WALL;
                    gameData[arr.x][arr.y-1]=GD_WALL;
                    gameData[pickup.x][pickup.y]=GD_WALL;
                    // 向所有客户机发出P1W070807090710,P后数字为玩家颜色代码，
                    // W代表是墙的坐标信息，连续写入三个点的左边，表示墙的位置，由于绘制原因，需要严格按照顺序
                    sprintf(tmpstr,"P%1dW%2d%2d%2d%2d%2d%2d",ply_head->color,pickup.x,pickup.y,(arr.x+pickup.x)/2,(arr.y+pickup.y)/2,arr.x,arr.y);
                    goto RULE_WALL_EXIT;
                }
            }
            // 上面使用goto语句，如果能够成功放置墙，那么已经跳出去了，
            // 这里，如果不满足一个可放墙的条件，视为重新选取
            pickup.x=arr.x;
            pickup.y=arr.y;
        }
        // 其他情况，一律视为重新选取墙
        else if ( gameData[arr.x][arr.y]!=GD_WALL )
        {
            pickup.x=arr.x;
            pickup.y=arr.y;
            // 清除玩家可走待选位置
            preselect_pos.clear();
        }
    }
    return ;
RULE_WALL_EXIT:
    // 这里判断，加入的墙是否合法的判定，如果新放的一堵墙，使得任何一个玩家无解，那么此墙非法
    // 如果不合法，那么需要还原原来的游戏数据
    if (!judgeWallLegal())
    {
        // 还原游戏算法数据
        gameData[arr.x][arr.y]=GD_BLANK;
        gameData[(arr.x+pickup.x)/2][(arr.y+pickup.y)/2]=GD_BLANK;
        gameData[pickup.x][pickup.y]=GD_BLANK;
        // 弹出刚压入的墙坐标(两个点)
        wall_vec.pop_back();
        wall_vec.pop_back();
        // 清空预选位置
        pickup.x=-1;
        pickup.y=-1;
        // 清除玩家可走待选位置
        preselect_pos.clear();
        return ;
    }
    if (g_sound==1)
    {
        // 放置墙的音效
        sndPlaySound("data/sound/wall_set.wav",SND_ASYNC);
    }
    // 放置了墙后，向网络发送放置墙位置的包
    if (n_netWorkStatus==1)     // 如果是服务器端
    {
        for (int i=0;i<n_TCPnet->GetConnectionNumber();i++)
        {
            n_TCPnet->SendServer(i,tmpstr,strlen(tmpstr)+1);
        }
    }
    else if (n_netWorkStatus==2)    // 如果是客户端
    {
        n_TCPnet->SendClient(tmpstr,strlen(tmpstr)+1);
    }
    // 当前玩家的可用墙数减1
    ply_head->wall_num_left--;
ACTION_RULE_EXIT:
    // 当玩家选择了移动人物，那么此玩家的行动就结束了，控制权应该交由下一位玩家
    ply_head=ply_head->next;
    // 清空预选位置
    pickup.x=-1;
    pickup.y=-1;
    // 清除玩家可走待选位置
    preselect_pos.clear();
    return ;
}
