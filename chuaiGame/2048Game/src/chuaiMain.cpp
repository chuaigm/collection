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
	
	char input[8];
	vfk_btn in_btn;
	int matrix[4][4]={0};
	const char blank[]="        ";

	Calg2048 game;
	game.init();

	for(;;)
	{
		if(game.IsGameOver()==true || 'q'==input[0])
		{
			break;
		}
		game.Game2048Alg(in_btn);
		memcpy(matrix, game.GetMatrix(), 4*4*sizeof(int));
		system("clear");
		cout<<endl<<endl;
		cout<<blank << "-------------------------------------" << endl;
		cout<<blank << "µÃ·ÖSocre => " << game.GetScore() << endl;
		cout<<blank << "-------------------------------------" << endl;
		for (int i = 0; i < MAX; i++)
		{
			cout<<blank <<"    |    |    |    |    |"<<endl;
			cout<<blank <<"    |";
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
			cout <<blank<<"    -----------------------"<< endl;
		}
		cout <<blank<< "-------------------------------------" << endl << endl;;

		system("stty raw -echo");
		input[0]=getchar();
		if(input[0]!='a'&&input[0]!='d'&&
		   input[0]!='s'&&input[0]!='w'&&
		   input[0]!='j'&&input[0]!='k'&&
		   input[0]!='l'&&input[0]!='i'&&
		   input[0]!='q'&&input[0]!='Q'
				)
		{
			input[0]=0;
		}
		system("stty -raw echo");
		switch(input[0])
		{
			case 'a':
			case 'j':
				in_btn=vfk_left;
				break;
			case 's':
			case 'k':
				in_btn=vfk_down;
				break;
			case 'd':
			case 'l':
				in_btn=vfk_right;
				break;
			case 'w':
			case 'i':
				in_btn=vfk_up;
				break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	printf("\n");
	printf("\n************chuaiTest- End ************\n\n");
	
}

