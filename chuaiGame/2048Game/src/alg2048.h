/*!
 * \file:		alg2048.h
 * \Create:		2013-08-22
 */
#ifndef CALG_2048
#define CALG_2048
 
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <memory>
 
////////////////////////////////////////////////////////////////////////////////////

const int MAX=4;

class Calg2048
{
public:
	Calg2048();
	~Calg2048();
public:
	// 初始操作
	void init();
	// 获取分数
	int GetScore(){return score;};
	// 获取游戏数据矩阵
	int* GetMatrix(){return (int*)_matrix;};
	// 是否游戏结束
	bool IsGameOver(){return b_GameOver;};
	// 游戏核心算法，需要放到一个循环中
	int Game2048Alg(char input);
private:
	// 打印数组
	void test_print();
	// 操作说明
	void help();
	// 随机位置 => 随机数据2/4
	bool random_data_one();
	// 上左下右
	bool b_up();
	bool b_left();
	bool b_down();
	bool b_right();
	// 方向执行
	void up_do();
	void left_do();
	void down_do();
	void right_do();
private:
	// 矩阵大小
	int MAXMAX;
	// 初始数据个数
	int INIT_SIZE;
	// 数据计数
	int _count;
	// 按键操作 => 不使用char... => ch[0]即可...
	//string ch;
	// 得分...
	int score;
	// 游戏是否结束标志 
	bool b_GameOver;
	// 合并临时数组
	int curline[4];
	// 数据矩阵
	int _matrix[4][4];
};

////////////////////////////////////////////////////////////////////////////////////
#endif
