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
    // 这里的netWorkStat的定义与约定是在Quoridor_OpenGL中约定的
    void NetWorkSendData(int netWorkStat, char* data, int length);

    // TODO，这里在未来可以尝试改成不在线程函数中调用全局变量，可以将网络数据
    //       缓存到本类内变量中，然后通过绘制层(上层)的循环内，
    // 调用check此类内变量或此类的另一个单独的函数

    // 接收数据的函数，做两份，因为实现代码太多，方便管理
    static void OnServerReceive(char* data, int length);
    static void OnClientReceive(char* data, int length);

    // 接收数据函数的主入口，是一个回调函数
    // **注意
    // 由于网络部分涉及多线程，而目前编译器可能开启优化，
    // 所以，此函数由另外一个线程触发，修改绘图类函数的部分，
    // 涉及的相关变量，需要定义成volatile属性，这样，才不会出问题，否则结果不可预料
    static void OnReceiveNetData(char* data, int length, DWORD userdata);

    // 网络状态回调函数
    static void OnServerStatus(char* data, int length);
    static void OnClientStatus(char* data, int length);
    // 网络状态回调总入口函数
    static void OnNetStatus(char* data, int length, DWORD userdata);


    // 网络相关
    DWORD port;               // 通讯端口
    char IP[16];              // 配置文件中的IP
    //char n_loaclIP[16];         // 本机IP
    char Name[16];            // 配置文件中写明的用户名
    //int n_netWorkStatus;        // 网络联机时的状态，0:未选择，1:服务器，2:客户端
    //char NameAll[4][16];      // 目前所有连接的玩家名
    static CTCPSocket *n_TCPnet;       // tcp网络连接

};
