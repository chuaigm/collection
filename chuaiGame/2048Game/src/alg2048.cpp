#include <cstdio>
#include "alg2048.h"
// 2�ĸ��ʴ���4
#define random_2_4 (rand()%5==4 ? 4:2)
#define random_x(x) (rand()%x)
using namespace std;

Calg2048::Calg2048()
{
	// �����С
	MAXMAX = MAX*MAX;
	// ��ʼ���ݸ���
	INIT_SIZE = 2;
	// ���ݼ���
	_count=0;
	// �÷�...
	score=0;
	// ��Ϸ��ʼ
	b_GameOver=false;
	// �ϲ���ʱ����
	memset(curline,0,MAX*sizeof(int));
	// ��ʼ������
	memset(_matrix,0,MAXMAX*sizeof(int));
}
Calg2048::~Calg2048()
{
}

int Calg2048::Game2048Alg(char input)
{
	//init();
	//print();
	if (b_up() || b_left() || b_down() || b_right())
	{
	//	help();
		switch (input)
		{
		case 'w':
			up_do();
			break;
		case 'a':
			left_do();
			break;
		case 's':
			down_do();
			break;
		case 'd':
			right_do();
			break;
		default:
			cout <<"!!!!!!!!!!!!��Ч����..."<< endl << endl;
			break;
		}
	//	print();
	}
	else
	{
		b_GameOver=true;
	}
	
	return 0;
}

void Calg2048::test_print()
{
	cout << "-------------------------------------" << endl;
	cout << "�÷�Socre => " << score << endl;
	cout << "-------------------------------------" << endl;
	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			if (_matrix[i][j])
				cout << setw(5) << _matrix[i][j] << " |";
			else
				cout << setw(7) << " |";
		}
		cout << endl;
	}
	cout << "-------------------------------------" << endl << endl;;
}

void Calg2048::help()
{
	cout << "wasd => ��������" << endl;
	cout << "������: ";
	//cin >> ch;
}
void Calg2048::init()
{
	_count = 0;
	score = 0;
	srand((int)time(0));
	for (int i = 0; i < INIT_SIZE; i++)
		random_data_one();
}

bool Calg2048::random_data_one()
{
	if (_count == MAXMAX)
		return false;
	int left = MAXMAX - _count;
	int tmp = random_x(left);
	int num = random_2_4;
	int k = 0;
	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			if (_matrix[i][j] == 0)
			{
				if (k++ == tmp)
				{
					_matrix[i][j] = num;
					break;
				}
			}
		}
	}
	++_count;
	return true;
}

bool Calg2048::b_up()
{
	for (int k = 0; k < MAX; k++)
	{
		// �������...��һ���п�Ԫ��...
		bool flag = false;
		for (int i = 0; i < MAX - 1; i++)
		{
			if (_matrix[i][k] == 0)
				flag = true;
			else
			{
				int j = i + 1;
				while (j < MAX)
				{
					if (_matrix[j][k])
					{
						if (_matrix[i][k] == _matrix[j][k])
							return true;
						else
							break;
					}
					else{
						++j;
					}
				}
			}
		}
		if (flag)
		{
			// ��Ԫ���Ϸ���Ԫ����ΪTrue...
			// ʹ�����Ҽл���...
			int i = 0, j = MAX - 1;
			while (i < MAX)
			{
				if (_matrix[i][k])
					++i;
				else
					break;
			}
			while (j >= 0)
			{
				if (_matrix[j][k] == 0)
					--j;
				else
					break;
			}
			if (i < j)
				return true;
		}
	}
	return false;
}
bool Calg2048::b_left()
{
	for (int k = 0; k < MAX; k++)
	{
		// �������...��һ���п�Ԫ��...
		bool flag = false;
		for (int i = 0; i < MAX - 1; i++)
		{
			if (_matrix[k][i] == 0)
				flag = true;
			else
			{
				int j = i + 1;
				while (j < MAX)
				{
					if (_matrix[k][j])
					{
						if (_matrix[k][i] == _matrix[k][j])
							return true;
						else
							break;
					}
					else{
						++j;
					}
				}
			}
		}
		if (flag)
		{
			// ��Ԫ���ұ���Ԫ����ΪTrue...
			// ʹ�����Ҽл���...
			int i = 0, j = MAX - 1;
			// i �ǿ�Ԫ��λ�ã�j�Ƿǿ�Ԫ��λ��
			while (i < MAX)
			{
				if (_matrix[k][i])
					++i;
				else
					break;
			}
			while (j >= 0)
			{
				if (_matrix[k][j] == 0)
					--j;
				else
					break;
			}
			if (i < j)
				return true;
		}
	}
	return false;
}
bool Calg2048::b_down()
{
	for (int k = 0; k < MAX; k++)
	{
		// �������...��һ���п�Ԫ��...
		bool flag = false;
		for (int i = MAX - 1; i > 0; i--)
		{
			if (_matrix[i][k] == 0)
				flag = true;
			else
			{
				int j = i - 1;
				while (j >= 0)
				{
					if (_matrix[j][k])
					{
						if (_matrix[i][k] == _matrix[j][k])
							return true;
						else
							break;
					}
					else{
						--j;
					}
				}
			}
		}
		if (flag)
		{
			// ��Ԫ���Ϸ���Ԫ����ΪTrue... => �±ߵ�һ����Ԫ�����ϱߵ�һ���ǿ�Ԫ���±߼���...
			// ʹ�����Ҽл���...
			int i = 0, j = MAX - 1;
			// i �Ƿǿ�λ�ã�j�ǿ�Ԫ��λ��
			while (i < MAX)
			{
				if (_matrix[i][k] == 0)
					++i;
				else
					break;
			}
			while (j >= 0)
			{
				if (_matrix[j][k])
					--j;
				else
					break;
			}
			if (i < j)
				return true;
		}
	}
	return false;
}
bool Calg2048::b_right()
{
	for (int k = 0; k < MAX; k++)
	{
		// �������...��һ����һ���п�Ԫ��...
		bool flag = false;
		for (int i = MAX - 1; i > 0; i--)
		{
			if (_matrix[k][i] == 0)
				flag = true;
			else
			{
				int j = i - 1;
				while (j >= 0)
				{
					if (_matrix[k][j])
					{
						if (_matrix[k][i] == _matrix[k][j])
							return true;
						else
							break;
					}
					else{
						--j;
					}
				}
			}
		}
		if (flag)
		{
			// ��Ԫ�������Ԫ����ΪTrue... => �ұߵ�һ����Ԫ������ߵ�һ���ǿ�Ԫ���ұ߼���...
			// ʹ�����Ҽл���...
			int i = 0, j = MAX - 1;
			// i �Ƿǿ�λ�ã�j�ǿ�Ԫ��λ��
			while (i < MAX)
			{
				if (_matrix[k][i] == 0)
					++i;
				else
					break;
			}
			while (j >= 0)
			{
				if (_matrix[k][j])
					--j;
				else
					break;
			}
			if (i < j)
				return true;
		}
	}
	return false;
}

void Calg2048::up_do()
{
	cout << "�������ϼ�" << endl << endl;
	if (b_up())
	{
		cout << "�������Ϻϲ�" << endl;
		for (int i = 0; i < MAX; i++)
		{
			memset(curline, 0, sizeof(int)*MAX);
			int ii = 0;
			for (int j = 0; j < MAX; j++)
			{
				if (_matrix[j][i])
					curline[ii++] = _matrix[j][i];
			}
			for (int k = 0; k < ii - 1; k++)
			{
				if (curline[k] == curline[k + 1])
				{
					curline[k] *= 2;
					score += curline[k];
					curline[k + 1] = 0;
					++k;
					--_count;
				}
			}
			ii = 0;
			for (int j = 0; j < MAX; j++)
			{
				if (curline[j])
					_matrix[ii++][i] = curline[j];
			}
			for (; ii < MAX; ii++)
				_matrix[ii][i] = 0;
		}
		random_data_one();
	}
	else{
		cout << "!!!!!!!!!!!������Ч" << endl << endl;
	}
}

void Calg2048::left_do()
{
	cout << "���������" << endl << endl;
	if (b_left())
	{
		cout << "��������ϲ�" << endl << endl;
		for (int i = 0; i < MAX; i++)
		{
			memset(curline, 0, sizeof(int)*MAX);
			int ii = 0;
			for (int j = 0; j < MAX; j++)
			{
				if (_matrix[i][j])
					curline[ii++] = _matrix[i][j];
			}
			for (int k = 0; k < ii - 1; k++)
			{
				if (curline[k] == curline[k + 1])
				{
					curline[k] *= 2;
					score += curline[k];
					curline[k + 1] = 0;
					++k;
					--_count;
				}
			}
			ii = 0;
			for (int j = 0; j < MAX; j++)
			{
				if (curline[j])
					_matrix[i][ii++] = curline[j];
			}
			for (; ii < MAX; ii++)
				_matrix[i][ii] = 0;
		}
		random_data_one();
	}
	else{
		cout << "!!!!!!!!!!!������Ч" << endl << endl;
	}
}

void Calg2048::down_do()
{
	cout << "�������¼�" << endl << endl;
	if (b_down())
	{
		cout << "�������ºϲ�" << endl;
		for (int i = 0; i < MAX; i++)
		{
			memset(curline, 0, sizeof(int)*MAX);
			int ii = 0;
			for (int j = MAX - 1; j >= 0; j--)
			{
				if (_matrix[j][i])
					curline[ii++] = _matrix[j][i];
			}
			for (int k = 0; k < ii - 1; k++)
			{
				if (curline[k] == curline[k + 1])
				{
					curline[k] *= 2;
					score += curline[k];
					curline[k + 1] = 0;
					++k;
					--_count;
				}
			}
			ii = MAX - 1;
			for (int j = 0; j < MAX; j++)
			{
				if (curline[j])
					_matrix[ii--][i] = curline[j];
			}
			for (; ii >= 0; ii--)
				_matrix[ii][i] = 0;
		}
		random_data_one();
	}
	else{
		cout << "!!!!!!!!!!!������Ч" << endl << endl;
	}
}

void Calg2048::right_do()
{
	cout << "�������Ҽ�" << endl << endl;
	if (b_right())
	{
		cout << "�������Һϲ�" << endl;
		for (int i = 0; i < MAX; i++)
		{
			memset(curline, 0, sizeof(int)*MAX);
			int ii = 0;
			for (int j = MAX - 1; j >= 0; j--)
			{
				if (_matrix[i][j])
					curline[ii++] = _matrix[i][j];
			}
			for (int k = 0; k < ii - 1; k++)
			{
				if (curline[k] == curline[k + 1])
				{
					curline[k] *= 2;
					score += curline[k];
					curline[k + 1] = 0;
					++k;
					--_count;
				}
			}
			ii = MAX - 1;
			for (int j = 0; j < MAX; j++)
			{
				if (curline[j])
					_matrix[i][ii--] = curline[j];
			}
			for (; ii >= 0; ii--)
				_matrix[i][ii] = 0;
		}
		random_data_one();
	}
	else{
		cout << "!!!!!!!!!!!������Ч" << endl << endl;
	}
}

