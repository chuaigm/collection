#pragma once

class Quoridor_Network
{
public:
    Quoridor_Network(void);
    ~Quoridor_Network(void);

    void startServer();
    void startClient();

    void OnServerReceive();
    void OnClientReceive();
};
