
#include <stdio.h>
#include <stdlib.h>
#include <string>
// 捕获信号所需的头文件
#include "signal.h" 

using namespace std;

int	SignalShield(int nSig, void (* SigFunc)(int))
{
	int nRet;
	struct sigaction sigAct;

	sigemptyset( &(sigAct.sa_mask) );
	sigaddset(&(sigAct.sa_mask), nSig);
	sigAct.sa_handler  = SigFunc;
	sigAct.sa_flags    = 0;

	nRet = sigaction( nSig, &sigAct, (struct sigaction *)0 );
	return (nRet);
}
void SignalUSR( int nSigNo)
{
	// SIGINT 定义在signal.h中，表示interrupt
	if(SIGINT == nSigNo)
	{
		printf("get signal!,--%d\n",nSigNo);
		exit(-1);
	}
}

int main()
{
	// SIGINT 定义在signal.h中，表示interrupt
	if(SignalShield(SIGINT, SignalUSR))
		return -1;
	while(1)
	{
		sleep(1);
		printf("sleep 1s\n");
	}
}

