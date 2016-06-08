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
    // ֻ������������ӣ���Ҫ�������紦��
    void RandomMove();
    // �ܼ򵥵�AI
    void FoolAI();
    // ������AI�������
    void AI_action();
    // ��������·������ʼ������λ�ã��Լ�������
    int TestBestPath(pos2d start, int end);
};
