#pragma once
#include "TCPSocket.h"

class Quoridor_Network
{
public:
    Quoridor_Network(void);
    ~Quoridor_Network(void);

    int GetConnectionNumber();

    bool startServer();
    bool startClient();
    void closeNetWork();

    void OnServerReceive();
    void OnClientReceive();

    void NetWorkSendData(int netWorkStat, char* data, int length);

    static void OnReceiveNetData(char* data, int length, DWORD userdata);

    // �������
    DWORD port;               // ͨѶ�˿�
    char IP[16];              // �����ļ��е�IP
    //char n_loaclIP[16];         // ����IP
    char Name[16];            // �����ļ���д�����û���
    //int n_netWorkStatus;        // ��������ʱ��״̬��0:δѡ��1:��������2:�ͻ���
    //char NameAll[4][16];      // Ŀǰ�������ӵ������
    static CTCPSocket *n_TCPnet;       // tcp��������
};
