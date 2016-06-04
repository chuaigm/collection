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
// 这里修改最大连接数
#define MAX_CONNECTION 3

enum TCP_SOCKET_TYPE
{
    TCP_SOCKET_SERVER=0,
    TCP_SOCKET_CLIENT
};

typedef void (*LPStatusProc)(char *data,int length,DWORD userdata); //服务器状态的回调函数
typedef void (*LPDataArriveProc)(char *data,int length,DWORD userdata); //数据到达的回调函数

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

    // 公共函数
    char* GetLocalIP();                       // 获取本机的IP
    const int GetError(){return error;};      // 取得错误
    SOCKET GetSocket(){return m_sSocket;};    // 取得套接字
    int GetType(){return m_nType;};           // 取得类型
    BOOL IsConnected(SOCKET s);               // 判断一个socket是否连接

    void Close(); // 全部关闭

    //==============================
    // 服务器相关函数

    // 1. 先执行CreateServer，初始化socket，bind(), listen()
    BOOL CreateServer(int nPort,int backlog=5);
    // 2. 再执行StartServer，第一个参数可以传入服务器状态回调函数，检测连接与断开。
    //    第二个参数需要传入静态的回调函数，外部定义来获取数据。此函数创建ServerThread不断accept
    //    accept默认是阻塞的，当accept成功后，对新的连接创建DataThread线程
    BOOL StartServer(LPStatusProc proc1=NULL,LPDataArriveProc proc2=NULL,DWORD userdata=NULL);
    // 3. 服务器端调用，对客户端发送指定数据，第一个参数为客户端编号与m_sServer[]套接字数组对应
    int SendServer(int nNo,char* data, int length);

    // 服务器获取客户端总数
    int GetConnectionNumber(){return m_nConnections;};    // 获取连接数
    // 服务器端获取指定客户IP
    const char* GetClientIP(size_t i){return m_cIp[i];};   // 获取客户端IP列表
    // 停止服务器
    BOOL StopServer(); //停止服务
    // 其他
    SOCKET Listen(char* ClientIP=NULL); //监听单个IP的连接
    int ReceiveServer(int nNo,char* data, int length,int timeout); //接收指定字节的数据
    void Disconnect(int nNo);       // 单独断开一个socket

    // 服务器线程流程：
    //                主线程
    //                  |
    //                  |      ServerThread
    //     StartServer  >------------|
    //                  |            |
    //                  | accept阻塞 X        DataThread(有多个连接，
    //                  | 有新连接时 >------------|      就有多个DataThread)
    //                  |            |            |
    //                  |       循环 U            |
    //                  |                         U 循环判断当前连接，
    //                  |                           接收数据回调，

    //==============================
    // 客户端相关函数

    // 1. 客户端先执行Connect()，第一个参数传入IP
    BOOL Connect(LPCSTR pstrHost, int nPort); //连接一个IP
    // 2. 再执行StartReceiving，第二个参数是传入的静态回调函数
    BOOL StartReceiving(LPStatusProc proc1=NULL,LPDataArriveProc proc2=NULL,DWORD userdata=NULL);
    // 3. 客户端调用，对服务器发送数据
    int SendClient(char* data, int length); //发送指定字节的数据

    // 停止客户端自动接收
    BOOL StopReceiving(); //停止自动接收

    // 其他
    int ReceiveClient(char* data, int length,int timeout); //接收指定字节的数据

protected:

    // 公用变量
    int error; //错误类型
    int m_nType; //类型
    SOCKET m_sSocket; //套接字
    BOOL m_bAvailable; //能否使用
    BOOL m_bCreated; //是否建立,就是CreateServer和Connect之后的状态
    BOOL m_bAuto; //是否自动收发,就是StartServer和StartReceiving之后的状态
    DWORD m_dwUserData; //用户数据
    char local_ip[16]; // 记录本机的IP
    int m_nPort; //服务器端口

    // 服务器端变量
    HANDLE m_hServerThread; //服务器监听连接的线程
    HANDLE m_hServerDataThread[MAX_CONNECTION]; //服务器数据收发的线程
    SOCKET m_sServer[MAX_CONNECTION]; //每个客户的连接,初期为了方便给连接回送才这样设置
    char m_cIp[MAX_CONNECTION][16]; //每个连接的IP
    int m_nConnections; //连接总数
    int m_nCurrent; //当前正要建立的连接
    BOOL m_bConnected[MAX_CONNECTION]; //服务器判断每个能够使用的连接的状态

    LPDataArriveProc m_lpServerDataArriveProc; //服务器数据收发回调
    LPStatusProc m_lpServerStatusProc; //服务器状态回复回调

    // 客户端变量
    BOOL m_bconnect;               //客户端判断连接的状态
    HANDLE m_hClientThread; //客户端数据收发的线程
    LPDataArriveProc m_lpClientDataArriveProc; //客户端数据收发回调
    LPStatusProc m_lpClientStatusProc; //客户端状态回复回调

    // 其他函数
    BOOL Initwinsock(); // 初始化套接字，共用
    BOOL NewConnect(int nNo); // 服务器端使用，创建接收数据线程
    // 服务器不断的accpet，监听接收客户端
    static DWORD WINAPI ServerThread(LPVOID lpParmameter); // 服务器监听线程
    static DWORD WINAPI DataThread(LPVOID lpParameter); // 服务器数据收发线程(核心)
    static DWORD WINAPI ClientThread(LPVOID lpParameter); //客户端接收线程
    static DWORD WINAPI TimeOutControl(LPVOID lpParameter); //超时控制线程

};

#endif      // _TCPSOCKET_H_
