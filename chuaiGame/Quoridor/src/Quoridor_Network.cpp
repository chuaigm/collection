#include "stdafx.h"
#include "Quoridor_Network.h"
#include "Quoridor_openGL.h"
#include "TCPSocket.h"

extern CQuoridor* pgm;
// �Ƿ������ֱ��
extern int g_sound;

CTCPSocket* Quoridor_Network::n_TCPnet=NULL;

Quoridor_Network::Quoridor_Network(void)
{
    port=0;
    memset(IP,0,sizeof(IP));
    memset(Name,0,sizeof(Name));
    memset(pgm->n_NameAll,0,sizeof(pgm->n_NameAll));
}

Quoridor_Network::~Quoridor_Network(void)
{
}

int Quoridor_Network::GetConnectionNumber()
{
    if (n_TCPnet==NULL)
    {
        return -1;
    }
    return n_TCPnet->GetConnectionNumber();
}

bool Quoridor_Network::startServer()
{
    char tmpstr[32];
    // ���ﴴ���������
    n_TCPnet = new CTCPSocket(TCP_SOCKET_SERVER);
    // ��������
    if(!n_TCPnet->CreateServer(port))
    {
        int err=n_TCPnet->GetError();
        sprintf(tmpstr,"�޷������������������: %d",err);
        MessageBox(NULL, tmpstr, "TcpNetwork",MB_OK);
        delete n_TCPnet;
        n_TCPnet=NULL;
        return false;
    }
    // ��ʼ������
    else if(!n_TCPnet->StartServer(NULL,OnReceiveNetData,NULL))
    {
        int err=n_TCPnet->GetError();
        sprintf(tmpstr,"�޷������������������: %d",err);
        MessageBox(NULL, tmpstr, "TcpNetwork",MB_OK);
        delete n_TCPnet;
        n_TCPnet=NULL;
        return false;
    }
    strncpy(pgm->n_NameAll[0],Name,8);
    return true;
}

bool Quoridor_Network::startClient()
{
    char tmpstr[32];
    // ���ﴴ���������
    n_TCPnet = new CTCPSocket(TCP_SOCKET_CLIENT);
    if(!n_TCPnet->Connect(IP,port))
    {
        int err=n_TCPnet->GetError();
        sprintf(tmpstr,"�޷����ӵ��������������: %d",err);
        MessageBox(NULL, tmpstr, "TcpNetwork",MB_OK);
        delete n_TCPnet;
        n_TCPnet=NULL;
        return false;
    }
    else if(!n_TCPnet->StartReceiving(NULL,OnReceiveNetData,NULL))
    {
        int err=n_TCPnet->GetError();
        sprintf(tmpstr,"�޷������ͻ������ݽ��շ��񣬴����: %d",err);
        MessageBox(NULL, tmpstr, "TcpNetwork",MB_OK);
        delete n_TCPnet;
        n_TCPnet=NULL;
        return false;
    }
    else
    {
        n_TCPnet->SendClient(Name,strlen(Name)+1);
        return true;
    }
}

void Quoridor_Network::closeNetWork()
{
    // ���������󱻴�������ôɾ��
    if (n_TCPnet!=NULL)
    {
        delete n_TCPnet;
        n_TCPnet=NULL;
    }
}

void Quoridor_Network::OnServerReceive()
{

}

void Quoridor_Network::OnClientReceive()
{

}

void Quoridor_Network::NetWorkSendData( int netWorkStat, char* data, int length )
{
    switch(netWorkStat)
    {
    case 1:
        for (int i=0;i<n_TCPnet->GetConnectionNumber();i++)
        {
            n_TCPnet->SendServer(i,data,length);
        }
        break;
    case 2:
        n_TCPnet->SendClient(data,length);
        break;
    default:
        break;
    }
}

void Quoridor_Network::OnReceiveNetData( char* data, int length, DWORD userdata )
{
    if (pgm->n_netWorkStatus==1)
    {   // �������������ݸ�ʽ��
        // 00000000001111111111222222
        // 01234567890123456789012345
        // S127.  0.  0.  1_  2_unamed
        // S[     IP       ][cn][ data ]
        char recMsg[128]={0};
        memcpy(recMsg,data+17,3);
        size_t clientID=atoi(recMsg);
        memcpy(recMsg,data+21,length-21);
        if (pgm->iGameState==GAME_NETWORK)
        {
            if (*recMsg=='P')
            {   // CP1M46
                char tmpC=*(recMsg+1);
                int color=atoi(&tmpC);      // ��������ҵ���ɫ
                tmpC=*(recMsg+2);           // ��������ҵ��ж������ƶ���ɫ(M)���Ƿ�ǽ(W)
                if ('W'==tmpC)          // �������յ�������ҷ���ǽ����Ϣ
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
                    // ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
                    pgm->wall_vec.push_back(wall1);
                    pgm->wall_vec.push_back(wall2);
                    // ������Ϸ�㷨����
                    pgm->gameData[wall1.x][wall1.y]=GD_WALL;
                    pgm->gameData[mid.x][mid.y]=GD_WALL;
                    pgm->gameData[wall2.x][wall2.y]=GD_WALL;
                    // color-1 ʵ���Ͽ��Ա�ʾ����������Ӧ���±�
                    pgm->plyer[color-1].wall_num_left--;
                    if (g_sound==1)
                    {
                        // ����ǽ����Ч
                        sndPlaySound("data/sound/wall_set.wav",SND_ASYNC);
                    }
                }
                else if ('M'==tmpC)
                {
                    tmpC=*(recMsg+3);
                    int plX=atoi(&tmpC);
                    tmpC=*(recMsg+4);
                    int plY=atoi(&tmpC);
                    // �������ɫ�����λ�õĶ�Ӧ��ϵ
                    pgm->gameData[pgm->plyer[color-1].x*2][pgm->plyer[color-1].y*2]=GD_BLANK;
                    pgm->gameData[plX*2][plY*2]=color;
                    pgm->plyer[color-1].x=plX;
                    pgm->plyer[color-1].y=plY;
                    if (g_sound==1)
                    {
                        // ��������ƶ���Ч
                        sndPlaySound("data/sound/player_move.wav",SND_ASYNC);
                    }
                    if (*(recMsg+5)=='F')
                    {
                        switch (color)
                        {
                        case GD_YELLOW:
                            pgm->win_flag=GD_YELLOW;
                            break;
                        case GD_RED:
                            pgm->win_flag=GD_RED;
                            break;
                        case GD_GREEN:
                            pgm->win_flag=GD_GREEN;
                            break;
                        case GD_BLUE:
                            pgm->win_flag=GD_BLUE;
                            break;
                        default:
                            break;
                        }
                        pgm->iGameState=GAME_WIN;
                    }
                }
                // �����������пͻ���ת���յ������ݣ�������Ϣ��Դ
                for (int i=0;i<n_TCPnet->GetConnectionNumber();i++)
                {
                    if (i==clientID)
                    {
                        continue;
                    }
                    n_TCPnet->SendServer(i,recMsg,strlen(recMsg)+1);
                }
                pgm->ply_head=pgm->ply_head->next;
            }
        }
        else if (pgm->iGameState==GAME_NET_CONFIG)
        {
            if (clientID<=3)
            {   // ���������տͻ��˷����������
                strncpy(pgm->n_NameAll[clientID+1],recMsg,8);
                pgm->n_NameAll[clientID+1][8]='\0';
                // �����������пͻ���ת���յ�������������б�
                char namelist[64]={0};
                strncpy(namelist   ,"namelist",8);
                strncpy(namelist+8 ,pgm->n_NameAll[0],8);
                strncpy(namelist+16,pgm->n_NameAll[1],8);
                strncpy(namelist+24,pgm->n_NameAll[2],8);
                strncpy(namelist+32,pgm->n_NameAll[3],8);
                for (int i=0;i<n_TCPnet->GetConnectionNumber();i++)
                {
                    n_TCPnet->SendServer(i,namelist,64);
                }
            }
        }
    }
    else if (pgm->n_netWorkStatus==2)
    {   // �ͻ��˽������ݸ�ʽ��
        // 00000000001111111111222222
        // 01234567890123456789012345
        // CXXXXXXXXX
        // C[ data ]
        //if (strcmp(data,"CSTART")==0)
        //{
        //    pgm->iGameState=GAME_NETWORK;
        //}
        if (strncmp(data,"CP",2)==0)
        {   // CP1M46
            char tmpC=*(data+2);
            int color=atoi(&tmpC);      // ��������ҵ���ɫ
            tmpC=*(data+3);             // ��������ҵ��ж������ƶ���ɫ(M)���Ƿ�ǽ(W)
            if ('W'==tmpC)          // �ͻ����յ�������������ĳ��ҷ�ǽ����Ϣ
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
                // ѹ��ǽ���ƶ��У�һ����ѹ�������һ��
                pgm->wall_vec.push_back(wall1);
                pgm->wall_vec.push_back(wall2);
                // ������Ϸ�㷨����
                pgm->gameData[wall1.x][wall1.y]=GD_WALL;
                pgm->gameData[mid.x][mid.y]=GD_WALL;
                pgm->gameData[wall2.x][wall2.y]=GD_WALL;
                // color-1 ʵ���Ͽ��Ա�ʾ����������Ӧ���±�
                pgm->plyer[color-1].wall_num_left--;
                if (g_sound==1)
                {
                    // ����ǽ����Ч
                    sndPlaySound("data/sound/wall_set.wav",SND_ASYNC);
                }
            }
            else if ('M'==tmpC)
            {
                tmpC=*(data+4);
                int plX=atoi(&tmpC);
                tmpC=*(data+5);
                int plY=atoi(&tmpC);
                // �������ɫ�����λ�õĶ�Ӧ��ϵ
                pgm->gameData[pgm->plyer[color-1].x*2][pgm->plyer[color-1].y*2]=GD_BLANK;
                pgm->gameData[plX*2][plY*2]=color;
                pgm->plyer[color-1].x=plX;
                pgm->plyer[color-1].y=plY;
                if (g_sound==1)
                {
                    // ��������ƶ���Ч
                    sndPlaySound("data/sound/player_move.wav",SND_ASYNC);
                }
                if (*(data+6)=='F')
                {
                    switch (color)
                    {
                    case GD_YELLOW:
                        pgm->win_flag=GD_YELLOW;
                        break;
                    case GD_RED:
                        pgm->win_flag=GD_RED;
                        break;
                    case GD_GREEN:
                        pgm->win_flag=GD_GREEN;
                        break;
                    case GD_BLUE:
                        pgm->win_flag=GD_BLUE;
                        break;
                    default:
                        break;
                    }
                    pgm->iGameState=GAME_WIN;
                }
            }
            pgm->ply_head=pgm->ply_head->next;
        }
        else if (strncmp(data,"Cnamelist",9)==0)
        {
            strncpy(pgm->n_NameAll[0],data+9 ,8);
            strncpy(pgm->n_NameAll[1],data+17,8);
            strncpy(pgm->n_NameAll[2],data+25,8);
            strncpy(pgm->n_NameAll[3],data+33,8);
        }
        else if (strncmp(data,"CREADY",6)==0)   // ǰ�����ֽ�
        {   // ������Ҹ�ʽ�����磬CREADY1N3
            // Nǰ�����0,1,2��ʾ�ڷ������еĿͻ����б��������ڱ����
            char tmpC=*(data+6);
            int clientID=atoi(&tmpC);     // �ڷ������Ŀͻ����б��еı��
            tmpC=*(data+8);
            int playerNum=atoi(&tmpC);    // �ͻ���������������������Ϸ�����������һ
            pgm->plyer[3].id=ID_NET_PLAYER;
            pgm->ply_head=&pgm->plyer[3];

            player* tail=pgm->ply_head;
            for (int i=0; i<playerNum;i++)
            {
                switch (i)
                {
                case 0:// ����Ϊ��ɫ���
                    if (0==clientID)
                    {
                        pgm->plyer[1].id=ID_HUMAN;
                    } else {
                        pgm->plyer[1].id=ID_NET_PLAYER;
                    }
                    pgm->gameData[pgm->plyer[1].x*2][pgm->plyer[1].y*2]=GD_RED;
                    tail->next=&pgm->plyer[1];
                    tail=&pgm->plyer[1];
                    break;
                case 1:// ����Ϊ��ɫ���
                    if (1==clientID)
                    {
                        pgm->plyer[2].id=ID_HUMAN;
                    } else {
                        pgm->plyer[2].id=ID_NET_PLAYER;
                    }
                    pgm->gameData[pgm->plyer[2].x*2][pgm->plyer[2].y*2]=GD_GREEN;
                    tail->next=&pgm->plyer[2];
                    tail=&pgm->plyer[2];
                    break;
                case 2:// ����Ϊ��ɫ���
                    if (2==clientID)
                    {
                        pgm->plyer[0].id=ID_HUMAN;
                    } else {
                        pgm->plyer[0].id=ID_NET_PLAYER;
                    }
                    pgm->gameData[pgm->plyer[0].x*2][pgm->plyer[0].y*2]=GD_YELLOW;
                    // �����ڻ�ɫ���ʱ���Ƚ��ر�
                    pgm->ply_head->next=&pgm->plyer[0];
                    pgm->plyer[0].next=&pgm->plyer[1];
                    break;
                default:
                    break;
                }
            }
            tail->next=pgm->ply_head;        // �γɻ�״
            // Ϊ��ͳ�������������Ϊ�˸����ʣ��ǽ����ֵ
            player* tmp_head=pgm->ply_head;
            // ѭ����ʣ��ǽ����ֵ
            do
            {   // ͳ�����������
                // ����������,21��2��ȡ��=10
                //            21��3��ȡ��=7
                //            21��4��ȡ��=5
                tmp_head->wall_num_left=wall_total_num/(playerNum+1);
                tmp_head=tmp_head->next;
            }while (pgm->ply_head!=tmp_head);

            pgm->iGameState=GAME_NETWORK;
        }
    }
}
