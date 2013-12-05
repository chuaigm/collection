#ifndef __FILEREPORT
#define __FILEREPORT

class FILEREPORT
{
public:
	FILEREPORT();
	FILEREPORT(char *p);
	~FILEREPORT();
	
	void put(char *p);
	void put(char *p,int a);
	void put(char *p,int a,int b);
	void putnum(int i,char *p);
	void put(char *p,double a,double b, double c);
	void put(char *p,double a,double b, double c,double d);
	void putOnce(char *p,double a,double b, double c);
	void putOnceStop();
	
#ifdef WIN_REPORT
	//////////	
	void CheckDC(HDC h,char *p);
	//show
	void SetDevice(HDC h,int x,int y);
	void Output(char *p,int a,int b);
	void Show();
#endif 

private:
	FILE *fp;
	//¼ÆÊýÆ÷×é
	int num[5];
	//
	int iPut;

#ifdef WIN_REPORT
	//show
	HDC hshow;
	int xpos;
	int ypos;
	char info[50];
#endif 

};

#endif