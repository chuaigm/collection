#pragma once

class Quoridor_ComputerAI
{
public:
    Quoridor_ComputerAI(void);
    ~Quoridor_ComputerAI(void);

    // �������������
    // ����start��end֮���������
    // ����start,������end
    int random(double start, double end)
    {
        return (int)(start+(end-start)*rand()/(RAND_MAX + 1));
    };
    void FoolAI();
    void AI_action();
    int TestBestPath(pos2d start, int end);
};
