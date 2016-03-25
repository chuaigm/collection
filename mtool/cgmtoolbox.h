#ifndef __CHUAI_TOOL_BOX_H__
#define __CHUAI_TOOL_BOX_H__
/* ---------- Organized by chuaiGM ----------*/
// !
// environment       |    compiler
// CentOS 6.3(linux) |    g++ 4.4.6
// HP-UX             |    aCC A.06.05
// windows 7         |    VS2005
// history:  2013-03-19   add delaySf,RandomSeed function
// history:  2013-03-21   redo some method, order
// history:  2013-11-28   tidy up, make menu, func trace

/* -----[Option]------ MENU -----------------*/
#define __C1_  // print Debugging information(file,line,func...)
#define __C2_  // print when you Compile, version display
#define __C3_  // Time statistics
#define __C4_  // Use select to sleep a while(LINUX only)
#define __C5_  // set random seed
#define __C6_  // Color character output
#define __C7_  // judge endian
#define __C8_  // function tracer
//#define __C9_  // back tracer (LINUX only)

/* -----[Option]------ INCLUDE --------------*/
// you can't open both
//#define __WIN32__
#define __LINUX__

/* ----------------------------------------- */
// standard lib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#ifdef __WIN32__
#include <windows.h>
#include <time.h>
#endif

#ifdef __LINUX__
#include <sys/time.h>
#include <unistd.h>
#endif

/*=================================================*\
 *    C1. this macro is for DEBUGing
\*=================================================*/
#ifdef __C1_
// change the Macro name if you like
// in windows, no __func__ macro, so use __FUNCTION__ instead...
#define MMMM \
	do {\
	printf ("|%24s|%8d|%24s|\n",__FILE__,__LINE__,__FUNCTION__); \
	} while (0)
#define MMM printf("|%24s|%8d|%24s|\n",__FILE__,__LINE__,__FUNCTION__)
#define MMTOFILE \
	{\
		FILE *cgmtfp;\
		if((cgmtfp=fopen("cgmDebugFile.txt","a"))==NULL) exit(0);\
		fprintf(cgmtfp,"|%24s|%8d|%24s|\n",__FILE__,__LINE__,__FUNCTION__);\
		if(cgmtfp != NULL) fclose(cgmtfp);\
	}
// some times, you just want to execute once
// you may write like this to debug
	/*
	static int ctsi=0;
	(0==ctsi)?({MMTOFILE;ctsi=0;}):(ctsi=ctsi);
	*/
#endif // for __C1_

/*=================================================*\
 *    C2. this is for debug, give version info
\*=================================================*/
#ifdef __C2_
// display when you compile
#define CGM_VER_INFO \
	do {\
	printf ("|========== cgm ver ==========|\n"); \
	printf ("|== Compile Date : %s\n", __DATE__); \
	printf ("|== Compile Time : %s\n", __TIME__); \
	printf ("|========== cgm ver ==========|\n"); \
	} while (0)
#endif // end of __C2_

/*=================================================*\
 *    C3. this charpter is for TIME CALCULTE
\*=================================================*/
#ifdef __C3_ 
#ifdef __WIN32__
int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year    = wtm.wYear - 1900;
	tm.tm_mon     = wtm.wMonth - 1;
	tm.tm_mday    = wtm.wDay;
	tm.tm_hour    = wtm.wHour;
	tm.tm_min     = wtm.wMinute;
	tm.tm_sec     = wtm.wSecond;
	tm.tm_isdst   = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	// this func is used in windows,may cause Accuracy lost
	return (0);
}
#endif // end of __WIN32__

// static var to record time mark
static struct timeval tm_begin, tm_end;

// use this!!
#define BEGIN_CALC_TIME gettimeofday (&tm_begin, NULL)
#define END_CALC_TIME gettimeofday (&tm_end, NULL)
#define SHOW_TIME_US \
	do {\
	printf (" time (us): %.0f\n", 1000000.0 * (tm_end.tv_sec - tm_begin.tv_sec) + (tm_end.tv_usec - tm_begin.tv_usec)); \
	} while (0)
#define SHOW_TIME_MS \
	do {\
	printf (" time (ms): %.3f\n", 1000.0 * (tm_end.tv_sec - tm_begin.tv_sec) + (tm_end.tv_usec - tm_begin.tv_usec)/1000.0); \
	} while (0)
#define SHOW_TIME_S \
	do {\
	printf (" time ( s): %lf\n", (tm_end.tv_sec - tm_begin.tv_sec) + (tm_end.tv_usec - tm_begin.tv_usec)/1000000.0); \
	} while (0)

// support class
#ifdef __cplusplus
class timer_xxx
{
public:
	timer_xxx ()
	{
		BEGIN_CALC_TIME;
	}

	~timer_xxx ()
	{
		END_CALC_TIME;
		SHOW_TIME_S;
	}
};
#endif // for cplusplus
#endif // for __C3_

/*=================================================*\
 *    C4. this function is for sleep a while
\*=================================================*/
#ifdef __C4_
// Func | delay function creat by chuaiGM
// secf = the "seconds" you want to delay(double)
void delaySf(double secf)
{
	unsigned long int_s = (unsigned long) secf;
	unsigned long int_us = (unsigned long)((secf-(double)int_s)*1000000.0);

	timeval tv;
	tv.tv_sec = int_s;
	tv.tv_usec = int_us;

	select(0,NULL,NULL,NULL,&tv);
}
#endif	// for __C4_

/*=================================================*\
 *    C5. this function is for initial random seed
\*=================================================*/
#ifdef __C5_
// Func | Random seed by chuaiGM
long plantRandomSeed(bool useuser=0, long usrseed=0, bool bHint=0)
{
	long seed =0;
	if(useuser) {
		// use user random seed give out
		srand(usrseed);
		seed = usrseed;
	} else {
		// time struct def
		struct timeval tval;
		// get now time
		gettimeofday(&tval,NULL);
		// a ref val for give user the seed number
		seed = long(tval.tv_sec*tval.tv_usec);
		// set random number seed according to the real time value
		srand(seed);
	}
	// Hint!!!
	if(bHint)
	{
		printf("Random seed has been planted! ---- [%ld]\n",seed);
	}
	return seed;
}
#endif	// for __C5_

/*=================================================*\
 *    C6. this charpter is for COLOR output 
\*=================================================*/
#ifdef __C6_
#ifdef __LINUX__
// color word
#define gm_NONE      "\033[m"
#define gm_BLACK     "\033[0;30m"
#define LT_BLACK     "\033[1;30m"
#define gm_RED       "\033[0;31m"
#define LT_RED       "\033[1;31m"
#define gm_GREEN     "\033[0;32m"
#define LT_GREEN     "\033[1;32m"
#define gm_YELLOW    "\033[0;33m"
#define LT_YELLOW    "\033[1;33m"
#define gm_BLUE      "\033[0;34m"
#define LT_BLUE      "\033[1;34m"
#define gm_PURPLE    "\033[0;35m"
#define LT_PURPLE    "\033[1;35m"
#define gm_CYAN      "\033[0;36m"
#define LT_CYAN      "\033[1;36m"
#define gm_GRAY      "\033[0;37m"
#define LT_WHITE     "\033[1;37m"
// color background
#define BLACK_BKO    "\033[40m"
#define RED_BKO      "\033[41m"
#define GREEN_BKO    "\033[42m"
#define YELLOW_BKO   "\033[43m"
#define BLUE_BKO     "\033[44m"
#define PURPLE_BKO   "\033[45m"
#define CYAN_BKO     "\033[46m"
#define WHITE_BKO    "\033[47m"
#define BLACK_BK     "\033[1;30;7m"
#define RED_BK       "\033[1;31;7m"
#define GREEN_BK     "\033[1;32;7m"
#define YELLOW_BK    "\033[1;33;7m"
#define BLUE_BK      "\033[1;34;7m"
#define PURPLE_BK    "\033[1;35;7m"
#define CYAN_BK      "\033[1;36;7m"
#define WHITE_BK     "\033[1;37;7m"
// use like: printf(YELLOW"this is yellow!"NONE"\n");
// and these color can be combine like: "\033[1;31;44m"
// but should be define by yourself
#define RED_ON_BLUE  "\033[0;31;44m"
#endif // for __LINUX__
////////////////////////////////////////////////////////////////
// next I packaging some class and ostream
class SingleColorOutStream
{
//#define __USE_DELAY__
public:
	int color;
	static int sleepms;
	// construct
#ifdef __LINUX__
	SingleColorOutStream( int c = 0 )
#endif
#ifdef __WIN32__
	SingleColorOutStream( unsigned int c = 7 )
#endif
	{
		color = c;
		sleepms = 0;
	}
	const SingleColorOutStream& operator<<(char str) const
	{
		SetColor(color);
#ifdef __USE_DELAY__
		delaySf(sleepms);
#endif
		std::cout<<str;
		SetColor();
		return *this;
	}
	const SingleColorOutStream& operator<<(const char str[]) const
	{
		SetColor(color);
#ifdef __USE_DELAY__
		for(int i=0; i<strlen(str); ++i) {
		delaySf(sleepms);
		std::cout<<str[i];
		}
#else
		std::cout<<str;
#endif
		SetColor();
		return *this;
	}
	const SingleColorOutStream& operator<<(char str[]) const
	{
		SetColor(color);
#ifdef __USE_DELAY__
		for(int i=0; i<strlen(str); ++i) {
		delaySf(sleepms);
		std::cout<<str[i];
		}
#else
		std::cout<<str;
#endif
		SetColor();
		return *this;
	}
	const SingleColorOutStream& operator<<(int value) const
	{
		SetColor(color);
#ifdef __USE_DELAY__
		delaySf(sleepms);
#endif
		std::cout<<value;
		SetColor();
		return *this;
	}
	const SingleColorOutStream& operator<<(double value) const
	{
		SetColor(color);
#ifdef __USE_DELAY__
		delaySf(sleepms);
#endif
		std::cout<<value;
		SetColor();
		return *this;
	}
private:
#ifdef __LINUX__
	void SetColor( int c = 0 ) const
	{
		switch(c) {
		case 0:
			printf(gm_NONE);
			break;
		case 1:
			printf(LT_RED);
			break;
		case 2:
			printf(LT_GREEN);
			break;
		case 3:
			printf(LT_YELLOW);
			break;
		case 4:
			printf(LT_BLUE);
			break;
		case 5:
			printf(LT_PURPLE);
			break;
		case 6:
			printf(LT_CYAN);
			break;
		case 7:
			printf(LT_WHITE);
			break;
		case 11:
			printf(RED_BK);
			break;
		case 12:
			printf(GREEN_BK);
			break;
		case 13:
			printf(YELLOW_BK);
			break;
		case 14:
			printf(BLUE_BK);
			break;
		case 15:
			printf(PURPLE_BK);
			break;
		case 16:
			printf(CYAN_BK);
			break;
		case 17:
			printf(WHITE_BK);
			break;
		default:
			printf(gm_NONE);
			break;
		}
	}
#endif

#ifdef __WIN32__
	void SetColor( unsigned int c = 7 ) const
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h,c);
	}
#endif
};
int SingleColorOutStream::sleepms = 0;

#ifdef __LINUX__
// front color
SingleColorOutStream red(1);
SingleColorOutStream green(2);
SingleColorOutStream yellow(3);
SingleColorOutStream blue(4);
SingleColorOutStream purple(5);
SingleColorOutStream cyan(6);
SingleColorOutStream white(7);
// background color
SingleColorOutStream red_bk(11);
SingleColorOutStream green_bk(12);
SingleColorOutStream yellow_bk(13);
SingleColorOutStream blue_bk(14);
SingleColorOutStream purple_bk(15);
SingleColorOutStream cyan_bk(16);
SingleColorOutStream white_bk(17);
#endif // for __LINUX__

#ifdef __WIN32__
// front color
SingleColorOutStream red(FOREGROUND_RED|FOREGROUND_INTENSITY);
SingleColorOutStream green(FOREGROUND_GREEN|FOREGROUND_INTENSITY);
SingleColorOutStream yellow(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
SingleColorOutStream blue(FOREGROUND_BLUE|FOREGROUND_INTENSITY);
SingleColorOutStream purple(FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
SingleColorOutStream cyan(FOREGROUND_INTENSITY);
SingleColorOutStream white(15);
// background color
SingleColorOutStream red_bk(BACKGROUND_RED|BACKGROUND_INTENSITY);
SingleColorOutStream green_bk(BACKGROUND_GREEN|BACKGROUND_INTENSITY);
SingleColorOutStream yellow_bk(BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_INTENSITY);
SingleColorOutStream blue_bk(BACKGROUND_BLUE|BACKGROUND_INTENSITY);
SingleColorOutStream purple_bk(BACKGROUND_RED|BACKGROUND_BLUE|BACKGROUND_INTENSITY);
SingleColorOutStream cyan_bk(BACKGROUND_INTENSITY);
SingleColorOutStream white_bk(0x10|0x20|0x40|0x80);
#endif // for __WIN32__
/*!
 * caution! do not use "std::endl" follow the end of the line
 * write another line, like: std::cout<<std::endl;
 */
#endif // for __C6_

/*=================================================*\
 *    C7. judge little endian or big endian
\*=================================================*/
#ifdef __C7_
void endianJdg(int hint=0)
{
	int x = 0x0102;
	char x0 = ((char*)&x)[0];
	char x1 = ((char*)&x)[1];
	if (x0 == 0x02 && x1 == 0x01) {
		0==hint?NULL:printf("\n[ Little Endian ]\n");
	} else {
		0==hint?NULL:printf("\n[   Big  Endian ]\n");
	}
}
#endif // for __C7_

/*=================================================*\
 *    C8. function tracer
\*=================================================*/
#ifdef __C8_
#include <stdarg.h>
#define FUNC_TRACER CFuncTracer log(__FUNCTION__)
// linux gcc pattern
//#define INFO_LOG(format, args...)  log.print("[INFO_]", format, ##args)
// for compatibility of windows, write like this...
#define INFO_LOG(format, ...)  log.print("[INFO_]", format, ##__VA_ARGS__)
#define ERROR_LOG(format, ...) log.print("[ERROR]", format, ##__VA_ARGS__)
class CFuncTracer
{
public:
	CFuncTracer(const char* func_name)
	{
		++indent;
		memset(_name, 0, 256);
		memcpy(_name, func_name, strlen(func_name));
		printf("%d: ", indent);
		for(unsigned int i=0; i<indent; ++i) {
			printf("  ");
		}
		printf("-->> %s()\n", _name);
	}
	~CFuncTracer()
	{
		printf("%d: ", indent);
		for(unsigned int i=0; i<indent; ++i) {
			printf("  ");
		}
		printf("<<-- %s()\n", _name);
		--indent;
	}
	void print(const char* level, const char *fmt, ...)
	{
		char buf[1024]={0};
		va_list ap;
		va_start(ap, fmt);
		vsnprintf(buf, 1024, fmt, ap);
		va_end(ap);
		printf("%d: ", indent);
		for(unsigned int i=0; i<indent; ++i) {
			printf("  ");
		}
		printf("%s: %s", level, buf);
	}
private:
	CFuncTracer();
	CFuncTracer(const CFuncTracer &);
	const CFuncTracer &operator=(const CFuncTracer &);

	char _name[256]; //length of function name should not longer than 256
	static unsigned int indent;
};
// initial
unsigned int CFuncTracer::indent=0;
#endif // for __C8_

/*=================================================*\
 *    C9. back trace
\*=================================================*/
#ifdef __C9_
// remember add -rdynamic when you compile, like:
// g++ -rdynamic prog.c
#include <execinfo.h>
void backTracer()
{
   int j, nptrs;
   void *buffer[100];
   char **strings;
   nptrs = backtrace(buffer, 100);
   printf("backtrace() returned %d addresses\n", nptrs);
   /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
    *  would produce similar output to the following: */
 
   strings = backtrace_symbols(buffer, nptrs);
   if (strings == NULL) {
       perror("backtrace_symbols");
       exit(EXIT_FAILURE);
   }
 
   for (j = 0; j < nptrs; j++)
       printf("%s\n", strings[j]);
   free(strings);
}
#endif // for __C9_

#endif	// for __CHUAI_TOOL_BOX_H__

