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
#include "platform_code.h"

using namespace std;

///////////////*****const & global*****///////////////

struct testStruct
{
	unsigned int sn;
	char kword[100];
};
// Define some Fixed
const char InfileName[] = "Infile_ChuaiTest.txt";
const char OutfileName[] = "Outfile_ChuaiTest.txt";

///////////////*****Functions*****///////////////
//
// Func | Safe scanf
void absorbEOF()
{
	int c;
	while((c=getchar())!='\n'&&c!= EOF){}
}

///////////////*****Main() Func*****///////////////
// main in legend
int main(int argc, char *argv[])
{
	printf("\n************chuaiTest-Begin************\n\n");
//	std::cout<<"Greetings! let' Rock!!!"<<std::endl<<std::endl;

	//////////////////////////////////////////////////////////////////////////
	
	Cplatform_code* plat = new Cplatform_code();

	plat->run();

	//////////////////////////////////////////////////////////////////////////

#if 0
	// safty method to absorb scanf buffer
	int i,c;
	while (1)
	{
		printf("input:");
		if(scanf("%d",&i)!=EOF)
		{
			while((c=getchar())!='\n'&&c!= EOF){}
		}
		printf("out:%d\n",i);
		//i here hold the last value
	}
#endif
#if 0
	// safty method to absorb cin buffer
	int i;
	while(1)
	{
		cout<<"input:";
		cin>>i;
		cin.clear();
		cin.ignore( std::numeric_limits<std::streamsize>::max( ), '\n' );
		cout<<"out:"<<i<<endl;
		//i here hold the last value
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	printf("\n");
	printf("\n************chuaiTest- End ************\n\n");
	
}

