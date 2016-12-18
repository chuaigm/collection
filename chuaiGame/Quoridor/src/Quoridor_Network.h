#pragma once

#include "gData.h"
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

    // ����ֻʵ��һ�������Ұ�S��C,������һ��
    // �����netWorkStat�Ķ�����Լ������Quoridor_OpenGL��Լ����
    void NetWorkSendData(int netWorkStat, char* data, int length);

    // TODO��������δ�����Գ��Ըĳɲ����̺߳����е���ȫ�ֱ��������Խ���������
    //       ���浽�����ڱ����У�Ȼ��ͨ�����Ʋ�(�ϲ�)��ѭ���ڣ�
    // ����check�����ڱ�����������һ�������ĺ���

    // �������ݵĺ����������ݣ���Ϊʵ�ִ���̫�࣬�������
    static void OnServerReceive(char* data, int length);
    static void OnClientReceive(char* data, int length);

    // �������ݺ���������ڣ���һ���ص�����
    // **ע��
    // �������粿���漰���̣߳���Ŀǰ���������ܿ����Ż���
    // ���ԣ��˺���������һ���̴߳������޸Ļ�ͼ�ຯ���Ĳ��֣�
    // �漰����ر�������Ҫ�����volatile���ԣ��������Ų�������⣬����������Ԥ��
    static void OnReceiveNetData(char* data, int length, DWORD userdata);

    // ����״̬�ص�����
    static void OnServerStatus(char* data, int length);
    static void OnClientStatus(char* data, int length);
    // ����״̬�ص�����ں���
    static void OnNetStatus(char* data, int length, DWORD userdata);


    // �������
    DWORD port;               // ͨѶ�˿�
    char IP[16];              // �����ļ��е�IP
    //char n_loaclIP[16];         // ����IP
    char Name[16];            // �����ļ���д�����û���
    //int n_netWorkStatus;        // ��������ʱ��״̬��0:δѡ��1:��������2:�ͻ���
    //char NameAll[4][16];      // Ŀǰ�������ӵ������
    static CTCPSocket *n_TCPnet;       // tcp��������

};
