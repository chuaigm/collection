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
	// ��ʼ����
	void init();
	// ��ȡ����
	int GetScore(){return score;};
	// ��ȡ��Ϸ���ݾ���
	int* GetMatrix(){return (int*)_matrix;};
	// �Ƿ���Ϸ����
	bool IsGameOver(){return b_GameOver;};
	// ��Ϸ�����㷨����Ҫ�ŵ�һ��ѭ����
	int Game2048Alg(char input);
private:
	// ��ӡ����
	void test_print();
	// ����˵��
	void help();
	// ���λ�� => �������2/4
	bool random_data_one();
	// ��������
	bool b_up();
	bool b_left();
	bool b_down();
	bool b_right();
	// ����ִ��
	void up_do();
	void left_do();
	void down_do();
	void right_do();
private:
	// �����С
	int MAXMAX;
	// ��ʼ���ݸ���
	int INIT_SIZE;
	// ���ݼ���
	int _count;
	// �������� => ��ʹ��char... => ch[0]����...
	//string ch;
	// �÷�...
	int score;
	// ��Ϸ�Ƿ������־ 
	bool b_GameOver;
	// �ϲ���ʱ����
	int curline[4];
	// ���ݾ���
	int _matrix[4][4];
};

////////////////////////////////////////////////////////////////////////////////////
#endif
