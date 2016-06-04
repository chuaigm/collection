// TCPSocket.h: interface for the CTCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_TCPSOCKET_H_)
#define _TCPSOCKET_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock2.h"
#ifdef UNDER_CE
# pragma comment(lib,"winsock.lib")
#else
# pragma comment(lib,"ws2_32.lib")
#endif

#define SD_RECEIVE 0x00
#define SD_SEND 0x01
#define SD_BOTH 0x02
// �����޸����������
#define MAX_CONNECTION 3

enum TCP_SOCKET_TYPE
{
    TCP_SOCKET_SERVER=0,
    TCP_SOCKET_CLIENT
};

typedef void (*LPStatusProc)(char *data,int length,DWORD userdata); //������״̬�Ļص�����
typedef void (*LPDataArriveProc)(char *data,int length,DWORD userdata); //���ݵ���Ļص�����

struct TimeOutParameter
{
    int EndTime;
    SOCKET s;
    int nNo;
    BOOL bFinished;
    BOOL bExit;
    BOOL* pbConnected;
    HANDLE* phDataThread;
    int* pnConnections;
};

class CTCPSocket
{
public:
    CTCPSocket(int nType=TCP_SOCKET_CLIENT);
    virtual ~CTCPSocket();

    // ��������
    char* GetLocalIP();                       // ��ȡ������IP
    const int GetError(){return error;};      // ȡ�ô���
    SOCKET GetSocket(){return m_sSocket;};    // ȡ���׽���
    int GetType(){return m_nType;};           // ȡ������
    BOOL IsConnected(SOCKET s);               // �ж�һ��socket�Ƿ�����

    void Close(); // ȫ���ر�

    //==============================
    // ��������غ���

    // 1. ��ִ��CreateServer����ʼ��socket��bind(), listen()
    BOOL CreateServer(int nPort,int backlog=5);
    // 2. ��ִ��StartServer����һ���������Դ��������״̬�ص����������������Ͽ���
    //    �ڶ���������Ҫ���뾲̬�Ļص��������ⲿ��������ȡ���ݡ��˺�������ServerThread����accept
    //    acceptĬ���������ģ���accept�ɹ��󣬶��µ����Ӵ���DataThread�߳�
    BOOL StartServer(LPStatusProc proc1=NULL,LPDataArriveProc proc2=NULL,DWORD userdata=NULL);
    // 3. �������˵��ã��Կͻ��˷���ָ�����ݣ���һ������Ϊ�ͻ��˱����m_sServer[]�׽��������Ӧ
    int SendServer(int nNo,char* data, int length);

    // ��������ȡ�ͻ�������
    int GetConnectionNumber(){return m_nConnections;};    // ��ȡ������
    // �������˻�ȡָ���ͻ�IP
    const char* GetClientIP(size_t i){return m_cIp[i];};   // ��ȡ�ͻ���IP�б�
    // ֹͣ������
    BOOL StopServer(); //ֹͣ����
    // ����
    SOCKET Listen(char* ClientIP=NULL); //��������IP������
    int ReceiveServer(int nNo,char* data, int length,int timeout); //����ָ���ֽڵ�����
    void Disconnect(int nNo);       // �����Ͽ�һ��socket

    // �������߳����̣�
    //                ���߳�
    //                  |
    //                  |      ServerThread
    //     StartServer  >------------|
    //                  |            |
    //                  | accept���� X        DataThread(�ж�����ӣ�
    //                  | ��������ʱ >------------|      ���ж��DataThread)
    //                  |            |            |
    //                  |       ѭ�� U            |
    //                  |                         U ѭ���жϵ�ǰ���ӣ�
    //                  |                           �������ݻص���

    //==============================
    // �ͻ�����غ���

    // 1. �ͻ�����ִ��Connect()����һ����������IP
    BOOL Connect(LPCSTR pstrHost, int nPort); //����һ��IP
    // 2. ��ִ��StartReceiving���ڶ��������Ǵ���ľ�̬�ص�����
    BOOL StartReceiving(LPStatusProc proc1=NULL,LPDataArriveProc proc2=NULL,DWORD userdata=NULL);
    // 3. �ͻ��˵��ã��Է�������������
    int SendClient(char* data, int length); //����ָ���ֽڵ�����

    // ֹͣ�ͻ����Զ�����
    BOOL StopReceiving(); //ֹͣ�Զ�����

    // ����
    int ReceiveClient(char* data, int length,int timeout); //����ָ���ֽڵ�����

protected:

    // ���ñ���
    int error; //��������
    int m_nType; //����
    SOCKET m_sSocket; //�׽���
    BOOL m_bAvailable; //�ܷ�ʹ��
    BOOL m_bCreated; //�Ƿ���,����CreateServer��Connect֮���״̬
    BOOL m_bAuto; //�Ƿ��Զ��շ�,����StartServer��StartReceiving֮���״̬
    DWORD m_dwUserData; //�û�����
    char local_ip[16]; // ��¼������IP
    int m_nPort; //�������˿�

    // �������˱���
    HANDLE m_hServerThread; //�������������ӵ��߳�
    HANDLE m_hServerDataThread[MAX_CONNECTION]; //�����������շ����߳�
    SOCKET m_sServer[MAX_CONNECTION]; //ÿ���ͻ�������,����Ϊ�˷�������ӻ��Ͳ���������
    char m_cIp[MAX_CONNECTION][16]; //ÿ�����ӵ�IP
    int m_nConnections; //��������
    int m_nCurrent; //��ǰ��Ҫ����������
    BOOL m_bConnected[MAX_CONNECTION]; //�������ж�ÿ���ܹ�ʹ�õ����ӵ�״̬

    LPDataArriveProc m_lpServerDataArriveProc; //�����������շ��ص�
    LPStatusProc m_lpServerStatusProc; //������״̬�ظ��ص�

    // �ͻ��˱���
    BOOL m_bconnect;               //�ͻ����ж����ӵ�״̬
    HANDLE m_hClientThread; //�ͻ��������շ����߳�
    LPDataArriveProc m_lpClientDataArriveProc; //�ͻ��������շ��ص�
    LPStatusProc m_lpClientStatusProc; //�ͻ���״̬�ظ��ص�

    // ��������
    BOOL Initwinsock(); // ��ʼ���׽��֣�����
    BOOL NewConnect(int nNo); // ��������ʹ�ã��������������߳�
    // ���������ϵ�accpet���������տͻ���
    static DWORD WINAPI ServerThread(LPVOID lpParmameter); // �����������߳�
    static DWORD WINAPI DataThread(LPVOID lpParameter); // �����������շ��߳�(����)
    static DWORD WINAPI ClientThread(LPVOID lpParameter); //�ͻ��˽����߳�
    static DWORD WINAPI TimeOutControl(LPVOID lpParameter); //��ʱ�����߳�

};

#endif      // _TCPSOCKET_H_
