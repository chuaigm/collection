#pragma once

class Quoridor_ComputerAI
{
public:
    Quoridor_ComputerAI(void);
    ~Quoridor_ComputerAI(void);

    // �������������
    int random(double start, double end)
    {
        return (int)(start+(end-start)*rand()/(RAND_MAX + 1));
    };
    void SimpleComputer();
    void TestBestPath(pos2d start, int end);
};
