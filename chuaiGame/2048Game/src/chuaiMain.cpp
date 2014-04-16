// chuaiGM's test cpp file
// environment: cygwin(win32) HPUX Ubuntu(linux)

///////////////*****include & define*****///////////////
#if 0
#define WIN_32				// win32 Environment
#else
#define LINUX_UNIX			// Linux & Unix Environment
#endif

// Win32 Include
#ifdef WIN_32
#include "windows.h"
#endif
// Linux & Unix Include
#ifdef LINUX_UNIX
#include <sys/time.h>
#include <unistd.h>
#endif

// C++ standard header
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
// include class
#include "alg2048.h"

using namespace std;

///////////////*****const & global*****///////////////

struct testStruct
{
	unsigned int sn;
	char kword[100];
};
// Define some Fixed
const char OutfileName[] = "Outfile_ChuaiTest.txt";

///////////////*****Functions*****///////////////
//

///////////////*****Main() Func*****///////////////
// main in legend
int main(int argc, char *argv[])
{
	printf("\n************chuaiTest-Begin************\n\n");
	std::cout<<"Greetings! let' Rock!!!"<<std::endl<<std::endl;
	//////////////////////////////////////////////////////////////////////////
	
	int matrix[4][4]={0};

	Calg2048 game;
	game.init();

	memcpy(matrix, game.GetMatrix(), 4*4*sizeof(int));
		cout << "-------------------------------------" << endl;
		cout << "得分Socre => " << game.GetScore() << endl;
		cout << "-------------------------------------" << endl;
		for (int i = 0; i < MAX; i++)
		{
			cout <<"|    |    |    |    |"<<endl;
			cout <<"|";
			for (int j = 0; j < MAX; j++)
			{
				if (matrix[i][j])
					cout << setw(4) << matrix[i][j] << "|";
				else
					cout << "    |";
			}
			cout << endl;
			cout <<"-----------------------"<< endl;
		}
		cout << "-------------------------------------" << endl << endl;;

	char in[8]={0};
	cin>>in;
	printf("--input:%s --\n",in);
	for(;;)
	{
		if(game.IsGameOver()==true || 'q'==in[0])
		{
			break;
		}
		game.Game2048Alg(in[0]);
		memcpy(matrix, game.GetMatrix(), 4*4*sizeof(int));
		cout << "-------------------------------------" << endl;
		cout << "得分Socre => " << game.GetScore() << endl;
		cout << "-------------------------------------" << endl;
		for (int i = 0; i < MAX; i++)
		{
			cout <<"|    |    |    |    |"<<endl;
			cout <<"|";
			for (int j = 0; j < MAX; j++)
			{
				if (matrix[i][j])
				{
					cout << setw(4) << matrix[i][j] << "|";
				}
				else
				{
					cout << "    |";
				}
			}
			cout << endl;
			cout <<"-----------------------"<< endl;
		}
		cout << "-------------------------------------" << endl << endl;;

		cin>>in;
		printf("--input:%s --\n",in);
	}

	//////////////////////////////////////////////////////////////////////////
	printf("\n");
	printf("\n************chuaiTest- End ************\n\n");
	
}

