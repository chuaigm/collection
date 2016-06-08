#pragma once

class Quoridor_ComputerAI
{
public:
    Quoridor_ComputerAI(void);
    ~Quoridor_ComputerAI(void);

    // 随机数产生函数
    // 注意使用前使用 srand(unsigned(time(0)))
    // 生成start至end之间的整数，
    // 近似等概率生成start至end之间的整数
    // 包含start,不包含end
    // 如，random(0,3.0)  相对均匀生成 0，1，2，
    // 如果是random(0,2.2)，则生成2的数量会比较少
    int random(double start, double end)
    {
        return (int)(start+(end-start)*rand()/(RAND_MAX + 1));
    };
    // 只进行随机走棋子，需要考虑网络处理
    void RandomMove();
    // 很简单的AI
    void FoolAI();
    // 正常的AI动作设计
    void AI_action();
    // 测试最优路径，开始的棋盘位置，以及玩家序号
    int TestBestPath(pos2d start, int end);
};
