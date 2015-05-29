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

    // 发送只实现一个，而且把S和C,整合在一起
    void NetWorkSendData(int netWorkStat, char* data, int length);

    // 接收数据的函数，做两份，因为实现代码太多，方便管理
    static void OnServerReceive(char* data, int length);
    static void OnClientReceive(char* data, int length);
    // 接收数据函数的主入口，是一个回调函数
    static void OnReceiveNetData(char* data, int length, DWORD userdata);

    // 网络相关
    DWORD port;               // 通讯端口
    char IP[16];              // 配置文件中的IP
    //char n_loaclIP[16];         // 本机IP
    char Name[16];            // 配置文件中写明的用户名
    //int n_netWorkStatus;        // 网络联机时的状态，0:未选择，1:服务器，2:客户端
    //char NameAll[4][16];      // 目前所有连接的玩家名
    static CTCPSocket *n_TCPnet;       // tcp网络连接
};
