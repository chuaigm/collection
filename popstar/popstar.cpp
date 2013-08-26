//***************【POPstar】***************
//
//	AppName:    popstar
//  Author:     啜神chuaiGod
//  brief:      A game ref from android
//  Enviroment: VS2005,WIN32, Droid Sans mono(12)
//  Date：      2012-10-18    begin to do
//	History:    2012-11-25    实现了消去临近的方块
//              2012-11-27    修正了消去方法的bug
//*********************************************

// 如果是VS编译器
#if _MSC_VER > 1000
#define __WIN32__
//#pragma warning (disable : 4996)
#else
#define __LINUX__
#endif // _MSC_VER > 1000

#ifdef __WIN32__
#include <windows.h>
#include <conio.h>
//#include <time.h>
#endif

#include <stdio.h>
#include <stdlib.h>

// 如果不调试，发布版本，打开这个宏
#define NO_DEBUG
#include <assert.h>

#include <string>
#include <iostream>
#include <iomanip>  // cout 控制位宽所用，这里可能没有使用
#include <limits>
#include <vector>

#include "cgmtoolbox.h"

using namespace std;

#ifdef __WIN32__
//显示颜色方法,默认为graywhite
void SetColor(unsigned short c=7)
{ 
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h,c);
}

/*各种带颜色和时间停顿效果的输出流*/
class SingleColorOutStream
{
public:
	int color;				// 颜色
	static int sleepms;		// 为了实现时间停顿效果，单位毫秒(ms，1S=1000ms)
	// 构造函数初始化颜色与时间
	SingleColorOutStream(int a=7)
	{
		color=a;
		sleepms=0;
	}
	const SingleColorOutStream& SingleColorOutStream::operator<<(char str) const
	{
		SetColor(color);
		Sleep(sleepms);
		cout<<str;
		SetColor();
		return *this;
	}
	const SingleColorOutStream& SingleColorOutStream::operator<<(char str[]) const
	{
		SetColor(color);
		for(int i=0;i<strlen(str);i++)
		{
			Sleep(sleepms);
			cout<<str[i];
		}
		SetColor();
		return *this;
	}
	const SingleColorOutStream& SingleColorOutStream::operator<<(int value) const
	{
		SetColor(color);
		Sleep(sleepms);
		cout<<value;
		SetColor();
		return *this;
	}  
};
int SingleColorOutStream::sleepms = 0;
int &sleep=SingleColorOutStream::sleepms;

SingleColorOutStream white(15);			//亮白色
SingleColorOutStream green(10);			//绿色
SingleColorOutStream blue(9);			//蓝色
SingleColorOutStream red(12);			//红色
SingleColorOutStream yellow(14);		//黄色
SingleColorOutStream gray(8);			//灰色
SingleColorOutStream crimson(4);		//深红色
SingleColorOutStream yellow_under(14|COMMON_LVB_UNDERSCORE);//黄色加下划线


//////////////////////////////////////////////////////////////////////////
// 为了popstar项目定制

// 灰色前景字
SingleColorOutStream blue_bk(8|BACKGROUND_BLUE|BACKGROUND_INTENSITY);
SingleColorOutStream blue_bk_dark(8|BACKGROUND_BLUE);	
SingleColorOutStream green_bk(8|BACKGROUND_GREEN|BACKGROUND_INTENSITY);
SingleColorOutStream green_bk_dark(8|BACKGROUND_GREEN);
SingleColorOutStream red_bk(8|BACKGROUND_RED|BACKGROUND_INTENSITY);
SingleColorOutStream red_bk_dark(8|BACKGROUND_RED);
SingleColorOutStream yellow_bk(8|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
SingleColorOutStream yellow_bk_dark(8|BACKGROUND_GREEN|BACKGROUND_RED);
SingleColorOutStream white_bk(8|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_BLUE|BACKGROUND_INTENSITY);
SingleColorOutStream white_bk_dark(8|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_BLUE);

#endif

//////////////////////////////////////////////////////////////////////////
// 全局变量,常量
const int gbWidth = 10;				// 游戏内容的宽度
const int gbHeight = 10;			// 游戏内容的高度
// 核心游戏数据，矩阵,+2是为了寻找邻域算法时，方便，不越界
// 具体可参考下面调试数据图
#ifdef NO_DEBUG
int coreGameData[gbWidth+2][gbHeight+2]={{0}};
#else
int coreGameData[12][12]={{0,0,0,0,0,0,0,0,0,0,0,0},
                          {0,1,2,1,1,1,1,1,5,5,5,0},
    					  {0,1,2,1,1,1,1,1,1,1,5,0},
	    				  {0,1,1,1,1,1,1,1,1,1,5,0},
		    			  {0,1,1,1,1,1,1,1,1,1,1,0},
			    		  {0,1,1,1,1,3,3,1,1,1,1,0},
						  {0,1,1,1,1,1,3,1,1,1,1,0},
						  {0,1,1,1,1,1,1,1,1,1,1,0},
						  {0,1,1,1,1,1,1,1,1,1,1,0},
						  {0,1,1,1,1,1,1,1,1,1,1,0},
						  {0,1,1,1,1,1,1,1,1,1,1,0},
						  {0,0,0,0,0,0,0,0,0,0,0,0}
						  };
#endif
char usrInput[32];						// 为了接收用户输入

bool g_beGame = true;					// 是否可以继续游戏(!退出)
bool g_bePlay = false;					// 是否在消去游戏状态
bool g_beTurn = true;					// 本局游戏是否可以继续
// 需要显示的数据----------------
unsigned int g_highScore = 0;			// 最高得分
unsigned int g_score = 0;				// 本局得分
unsigned int g_elimi_score = 0;			// 本次消去得分
unsigned int g_elimination = 0;			// 本次消去了多少块
unsigned int g_level = 1;				// 关卡
unsigned int g_target = 1000;			// 过关目标分数
unsigned int g_turnF_award = 0;			// 本局结束未消去块的奖励分数
unsigned int g_RandomSeed = 0;			// 随机数种子

//////////////////////////////////////////////////////////////////////////
// 绘制游戏屏幕
void GamePrintScreen()
{
	//0, 1, 2, 3, ....9
	//1, ...
	//2, ...
	//.  ...
	//9, ...

	// 左边的空格数(17个)
	char blanknum[]="                 ";

	cout<<endl;
	cout<<"<----------------------------------------------------------------------------->"<<endl;
	//printf("      Level: %6d      target: %6d\n",g_level,g_target);
	//printf("      Score: %6d       block: %6d\n",g_score,g_elimination);
	//cout<<"      Score = "<<g_score<<"      block = "<<g_elimination<<endl;
	cout<<blanknum<<"  0 0 1 1 2 2 3 3 4 4 5 5 6 6 7 7 8 8 9 9"<<endl;

	// 高度
	for (int i=1; i<gbHeight+1; i++)
	{
		// 显示提示数据
		switch (i-1)
		{
		case 2:
			printf("      Block      ");
			break;
		case 4:
			printf("    Get_score    ");
			break;
		case 6:
			printf("      Score      ");
			break;
		default:
			// 空出几个字符的位置
			cout<<blanknum;
			break;
		}
		// 输出提示的坐标
		cout<<i-1;
		// 宽度
		for (int j=1; j<gbWidth+1; j++)
		{
			// 用数字做状态游戏数据的状态控制，1,2,3,4,5代表五种颜色
			// 0代表空白的位置，9代表刚刚被消去的块
			switch (coreGameData[i][j])
			{
			case 0:
				cout<<"    ";
				break;
			case 1:
				white_bk<<"#"<<j-1<<i-1<<"#";
				break;
			case 2:
				yellow_bk<<"@"<<j-1<<i-1<<"@";
				break;
			case 3:
				green_bk<<"*"<<j-1<<i-1<<"*";
				break;
			case 4:
				red_bk<<"+"<<j-1<<i-1<<"+";
				break;
			case 5:
				blue_bk<<"&"<<j-1<<i-1<<"&";
				break;
			case 9:
				cout<<"xxxx";
				break;
			default:
				cout<<"????";
			}
		}
		// 提示坐标
		cout<<i-1;
		// 显示提示数据
		switch (i-1)
		{
		case 2:
			printf("  Highest Score\n");
			break;
		case 4:
			printf("      Level\n");
			break;
		case 6:
			printf("      Target\n");
			break;
		default:
			// 空出几个字符的位置
			printf("\n");
			break;
		}

		// 显示提示数据
		switch (i-1)
		{
		case 2:
			printf("      %06d     ",g_elimination);
			break;
		case 4:
			printf("      %06d     ",g_elimi_score);
			break;
		case 6:
			if(g_score>g_target) {
				printf(LT_GREEN"      %06d     "gm_NONE,g_score);
			} else {
				printf(LT_YELLOW"      %06d     "gm_NONE,g_score);
			}
			break;
		default:
			// 空出几个字符的位置
			cout<<blanknum;
			break;
		}
		// 因为一个字符版太小，所以，这里连续绘制两行相同内容
		cout<<i-1;
		// 宽度
		for (int j=1; j<gbWidth+1; j++)
		{
			switch (coreGameData[i][j])
			{
			case 0:
				cout<<"    ";
				break;
			case 1:
				white_bk<<"####";
				break;
			case 2:
				yellow_bk<<"@@@@";
				break;
			case 3:
				green_bk<<"****";
				break;
			case 4:
				red_bk<<"++++";
				break;
			case 5:
				blue_bk<<"&&&&";
				break;
			case 9:
				cout<<"xxxx";
				break;
			default:
				cout<<"????";
			}
		}
		// 提示坐标
		cout<<i-1;
		// 显示提示数据
		switch (i-1)
		{
		case 2:
			printf("      %06d\n",g_highScore);
			break;
		case 4:
			printf("      %06d\n",g_level);
			break;
		case 6:
			printf(LT_RED"      %06d"gm_NONE"\n",g_target);
			break;
		default:
			// 换行
			printf("\n");
			break;
		}
	}
	cout<<blanknum<<"  0 0 1 1 2 2 3 3 4 4 5 5 6 6 7 7 8 8 9 9"<<endl;
	cout<<"<----------------------------------------------------------------------------->"<<endl;
	// 刷缓冲区
	//fflush(stdout);
}

// 游戏核心算法函数
int GameAlgorithem()
{
	// 判断输入的值是否在合法范围
	if (usrInput[0]<'0'||
		usrInput[0]>'9'||
		usrInput[1]<'0'||
		usrInput[1]>'9')
	{
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////
	// 消去方块算法
	// 初始化消去块的数量
	g_elimination = 0;
	// 初始化本次得分，一个与本次消去数量同步的变量
	g_elimi_score = 0;
	// 转换输入值
	int x = usrInput[1]-'0'+1;
	int y = usrInput[0]-'0'+1;
	// 要消去的类型
	int type = coreGameData[x][y];
	// 如果方块的类型是不能被消去的类型
	if ((type != 1)&&
		(type != 2)&&
		(type != 3)&&
		(type != 4)&&
		(type != 5))
	{
		// 什么都不做
		return 1;
	}
	// 如果是独立的方块，没有同色邻域
	if ((type != coreGameData[x+1][y])&&
		(type != coreGameData[x][y+1])&&
		(type != coreGameData[x-1][y])&&
		(type != coreGameData[x][y-1]))
	{
		// 什么都不做
		return 1;
	}
	// 有同色邻域
	bool haveNei = true;
	// 初始化消去块的数量
	g_elimination = 1;
	// 将值改为需要消去的值
	coreGameData[x][y] = 9;
	// 循环找到邻域相同的值
	while (haveNei) {
		// 高度
		for (int i=1; i<gbHeight+1; i++) {
			// 宽度
			for (int j=1; j<gbWidth+1; j++) {
				// 如果是同属性
				if (type == coreGameData[i][j])
				{
					if ((9 == coreGameData[i+1][j])||
						(9 == coreGameData[i][j+1])||
						(9 == coreGameData[i-1][j])||
						(9 == coreGameData[i][j-1]))
					{
						coreGameData[i][j] = 9;
						haveNei = true;
						g_elimination++;
						break;
					}
				}
				else
				{
					haveNei = false;
				}
			}
			if (haveNei)
			{
				break;
			}
		}
	}
	// 计算消去得分
	g_elimi_score = 5 * g_elimination * g_elimination;
	// 累计本局得分
	g_score += g_elimi_score;
	if(g_score > g_highScore)
	{
		g_highScore = g_score;
	}
	// 重绘屏幕，中间消去过程
	GamePrintScreen();
	//////////////////////////////////////////////////////////////////////////
	// 计算消去后的游戏数据
	// 垂直计算
	bool have9=true; // 存在消去的块,消去块的临时属性是9
	// 宽度
	for (int j=1; j<gbWidth+1; j++)
	{
		// 每一列的遍历，都认为是一次新的开始
		have9=true;
		while (have9)
		{
			// 先把标志位置为false，找到才变true
			have9 = false;
			// 高度
			for (int i=gbHeight; i>0; i--)
			{
				if (9 == coreGameData[i][j])
				{
					for (int t=i; coreGameData[t][j]!=0; t--)
					{
						coreGameData[t][j] = coreGameData[t-1][j];
					}
					have9 = true;
					break;
				}
			}
		}
	}
	// 水平计算(这里认为，下落一定是成功的)
	// 宽度 Brow -- blank row -_-
	bool haveBrow = true;
	while(haveBrow)
	{
		haveBrow = false;
		// 遍历列
		for (int j=1; j<gbWidth+1; j++)
		{
			int tmpresult = 0;
			// 统计当前底部这一块至最右侧是否都为0
			for (int tmpj = j; tmpj<gbWidth+1; tmpj++)
			{
				tmpresult += coreGameData[9+1][tmpj];
			}
			// 当前块为0，切后面还有不为0的块存在
			if (0 == coreGameData[9+1][j] && 0!=tmpresult)
			{
				for (int tmpj = j; tmpj<gbWidth+1; tmpj++)
				{
					for (int i = 1; i<gbHeight+1; i++)
					{
						coreGameData[i][tmpj] = coreGameData[i][tmpj+1];
					}
				}
				haveBrow = true;
				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 判定游戏是否结束，遍历核心数据，存在邻域，游戏就未结束
	// 是否有邻域，游戏可以继续下去
	bool bHaveSame = false;
	// 高度
	for (int i=1; i<gbHeight+1; i++)
	{
		// 宽度
		for (int j=1; j<gbWidth+1; j++)
		{
			// 判断这个块的类型
			type = coreGameData[i][j];
			// 如果不是0，并且有相邻的同色
			if ((0 != type)&&
				((type == coreGameData[i+1][j])||
				 (type == coreGameData[i][j+1])||
				 (type == coreGameData[i-1][j])||
				 (type == coreGameData[i][j-1])))
			{
				// 有同色邻域的存在
				bHaveSame = true;
				break;
			}
		}
		if (bHaveSame)
		{
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 没有可以消除的块，本局游戏结束，游戏结算
	if (!bHaveSame)
	{
		int cnt = 0;
		for (int i=1; i<gbHeight+1; i++)
		{
			// 宽度
			for (int j=1; j<gbWidth+1; j++)
			{
				// 判断这个块的类型
				type = coreGameData[i][j];
				// 如果不是0，并且有相邻的同色
				if (0 != type)
				{
					cnt++;
				}
			}
		}
		if (cnt<11)
		{
			g_turnF_award = 2000 - 20*cnt*cnt;
			g_score +=g_turnF_award;
		}
		else
		{
			g_turnF_award = 0;
		}

		// 本局游戏结束
		g_beTurn = false;
	}

	return 0;
}
// 初始化随机数据, 参数一，是否有用户指定的种子，参数二，指定种子值
void initialRandomData(bool usr, long seed)
{
	// 随机种子
#ifdef __WIN__
	DWORD timeseed = GetTickCount();
	srand(timeseed);
#endif
#ifdef __LINUX__
	if(usr) {
		g_RandomSeed = plantRandomSeed(1,seed,0);
	} else {
		g_RandomSeed = plantRandomSeed();
	}
#endif
	// 数据是否可用
	bool b_use = false;
	// 存储随机数容器
	vector<int> vec_init;
	// 这个循环，要保证，生产的
	while(!b_use)
	{
		// 认为数据是可用的
		b_use = true;
		// 清空vector
	//	vec_init.swap(std::vector<int>());
		vec_init.clear();
		// 初始化随机数组
		for (int i=0; i<gbWidth*gbHeight; ++i)
		{
			vec_init.push_back(rand()%5+1);
		}
		for (int t=1; t< 5+1; ++t)
		{
			int cnt = 0;
			for (int i=0; i<gbWidth*gbHeight; ++i)
			{
				if (t == vec_init[i])
				{
					cnt++;
				}
			}
/* 调试用 */
//			cout<<t<<": "<<cnt<<endl;

			// 这个类型的块数小于8
			if (cnt <8)
			{
				b_use =false;
				break;
			}
		}
	}
	// 给游戏核心赋初值
	// 高度
	for (int j=1; j<gbHeight+1; j++)
	{
		// 宽度
		for (int i=1; i<gbWidth+1; i++)
		{
			coreGameData[i][j] = vec_init[(i-1)*gbWidth+(j-1)];
		}
	}
}
void PrintPassScreen()
{
	// 左边的空格数(80/2 - 24/2 = 28个)
	char blanknum[]="                          ";

	cout<<endl;
	green<<"*******************************************************************************";
	cout<<endl;
	cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
	red<<blanknum<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	cout<<endl;
	yellow<<blanknum<<"     **Congratulation!**";
	cout<<endl;
	purple<<blanknum<<"    You pass: ";
	printf(gm_GREEN"%06d"gm_NONE,g_level);
	purple<<" stage";
	cout<<endl;
	purple<<blanknum<<"  remanent award: ";
	printf(gm_GREEN"%06d"gm_NONE,g_turnF_award);
	cout<<endl;
	red<<blanknum<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	cout<<endl;
	cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
	green<<"*******************************************************************************";
	cout<<endl;
	//cout<<endl;
	//delaySf(2.5);
	cout<<"Press Enter to Continue...";

	cin.clear();
	cin.ignore( std::numeric_limits<std::streamsize>::max( ), '\n' );
	cin.get(usrInput,2);;
}
void PrintMenu()
{
	// 左边的空格数(80/2 - 24/2 = 28个)
	char blanknum[]="                          ";

	cout<<endl;
	green<<"*******************************************************************************";
	cout<<endl;
	cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
	red<<blanknum<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	cout<<endl;
	yellow<<blanknum<<"1. play game";
	cout<<endl;
	yellow<<blanknum<<"2. Achievement";
	cout<<endl;
	yellow<<blanknum<<"3. exit";
	cout<<endl;
	red<<blanknum<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	cout<<endl;
	cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
	green<<"*******************************************************************************";
	cout<<endl;
	cout<<"choose you way: ";
	
	cin.clear();
	cin.ignore( std::numeric_limits<std::streamsize>::max( ), '\n' );
	cin.get(usrInput,2);
	// 根据输入判断模式
	switch(usrInput[0])
	{
		case '1':
			 g_bePlay = true;
			 break;
		case '2':
			 printf("\n\n\n ==== not finish ===\n\n\n");
			 break;
		case '3':
			 printf("\n == Exit Game ==\n");
			 exit(0);
	}
}
void ad()
{
	cout<<endl;
	red<<"OO***************************************************************************OO";
	cout<<endl<<endl;
	printf("\tchuai God's POPstar Game!!! ------ have fun |OvO|!\n");
	cout<<endl;
	red<<"VV***************************************************************************VV";
	cout<<endl;
	cout<<endl;
	cout<<"Press Enter to continue...";
	char tp[8];
	cin.get(tp, 1);
	cout<<endl;
	cout<<endl;
}
// 游戏主函数
int GameMain()
{
	ad();
#ifdef NO_DEBUG
	// 初始化随机块
	initialRandomData(0,0);
#endif
	g_beGame = true;
	g_bePlay = false;
	g_beTurn = true;
	// 游戏主循环
	while (g_beGame)
	{
		if(g_bePlay)
		{
			if(g_beTurn)
			{
				GamePrintScreen();
				//cout<<usrInput[0]<<usrInput[1]<<endl;
				//cout<<endl;
				cout<<"Please Input the aix(xy): ";
				cin.clear();
				// 正确使用这句话，需要包含limits
				cin.ignore( std::numeric_limits<std::streamsize>::max( ), '\n' );
				cin.get(usrInput,2+1);
				// 加入对字母的支持
				for(int i=0; i<2; ++i)
				{
					switch(usrInput[i]) {
					case 'u':
					case 'U':
						usrInput[i] = '4';
						break;
					case 'i':
					case 'I':
						usrInput[i] = '5';
						break;
					case 'o':
					case 'O':
						usrInput[i] = '6';
						break;
					case 'j':
					case 'J':
						usrInput[i] = '1';
						break;
					case 'k':
					case 'K':
						usrInput[i] = '2';
						break;
					case 'l':
					case 'L':
						usrInput[i] = '3';
						break;
					case 'm':
					case 'M':
						usrInput[i] = '0';
						break;
					default:
						break;
					}
				}
				GameAlgorithem();
			}
			else
			{
				// 刷新最高分
				if(g_score > g_highScore)
				{
					g_highScore = g_score;
				}
				// 如果分数不能达到过关要求
				if(g_score < g_target)
				{
					g_bePlay = false;
					// save
					break;
				}
				// 打印通关画面
				PrintPassScreen();
				// 对游戏难度做增量
				// 1000, 3000, 6000, 8000
				// 10000, 13000, 16000, 18000
				// 原本应该先自增level再算分数，但是由于生成方法，还需要减一，所以用
				// 上一个的level先生产分数
				g_target = g_level/4*10000 + ((g_level%4==3)?8000:(g_level%4*3000));
				g_level++;
				g_elimi_score = 0;
				g_elimination = 0;
				initialRandomData(0,0);
				g_beTurn = true;
			}
		}
		else
		{
			// 初始化游戏数据
			g_target = 1000;
			g_level = 1;
			g_elimi_score = 0;
			g_elimination = 0;
			//g_highScore = 0;
			g_score = 0;
			g_turnF_award = 0;
			
			PrintMenu();	
		}
	}

	return 0;
}

// 程序主函数
int main(int argc, char* argv[])
{
	GameMain();

	return 0;
}
