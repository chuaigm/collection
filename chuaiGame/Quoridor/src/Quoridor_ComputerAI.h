#pragma once

class Quoridor_ComputerAI
{
public:
    Quoridor_ComputerAI(void);
    ~Quoridor_ComputerAI(void);

    // �������������
    // ע��ʹ��ǰʹ�� srand(unsigned(time(0)))
    // ����start��end֮���������
    // ���Ƶȸ�������start��end֮�������
    // ����start,������end
    // �磬random(0,3.0)  ��Ծ������� 0��1��2��
    // �����random(0,2.2)��������2��������Ƚ���
    int random(double start, double end)
    {
        return (int)(start+(end-start)*rand()/(RAND_MAX + 1));
    };
    void FoolAI();
    void AI_action();
    int TestBestPath(pos2d start, int end);
};
