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

    // 网络相关
    DWORD port;               // 通讯端口
    char IP[16];              // 配置文件中的IP
    //char n_loaclIP[16];         // 本机IP
    char Name[16];            // 配置文件中写明的用户名
    //int n_netWorkStatus;        // 网络联机时的状态，0:未选择，1:服务器，2:客户端
    //char NameAll[4][16];      // 目前所有连接的玩家名
    static CTCPSocket *n_TCPnet;       // tcp网络连接
};
