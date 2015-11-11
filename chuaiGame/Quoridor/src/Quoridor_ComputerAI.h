#pragma once

class Quoridor_ComputerAI
{
public:
    Quoridor_ComputerAI(void);
    ~Quoridor_ComputerAI(void);

    // 随机数产生函数
    // 生成start至end之间的整数，
    // 包含start,不包含end
    int random(double start, double end)
    {
        return (int)(start+(end-start)*rand()/(RAND_MAX + 1));
    };
    void FoolAI();
    void AI_action();
    int TestBestPath(pos2d start, int end);
};
