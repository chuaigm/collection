//
// 制作  programking 2009年3月
// 博客  http://blog.csdn.net/programking
//=======================================
// GLFont.cpp: implementation of the CGLFont class.
// 学程序编游戏系列丛书
// 唐明理 E_mail: cqtmL@163.com
//====================================================================
#include "stdafx.h"

#include "GLFont.h"

#include "filereport.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//unsigned int Base;
//////////////////////////////////////////////////////////////////////

CGLFont::CGLFont()
{
	//inti font
	int i=0;

	for(i=0;i<6;i++)
	{
		hFontAr[i]=CreateFont(-12-i*12, 0, 0, 0, 800, 0, 0, 0,
			GB2312_CHARSET,0,0,0,FF_MODERN,"Arial");
	}

	hFontAr[6]=CreateFont(20,0,0,0,800,0,0,0,
			GB2312_CHARSET,0,0,0,FF_MODERN,"Arial");
}
CGLFont::~CGLFont()
{
}

//在光栅位置xy处，输出文字
// 由于光栅定位到z=0处，在gluPerspective投影下需要后移
void CGLFont::settextTest(int x,int y,const char* str,int ifont,float r,float g,float b)
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);    
	//glDisable(GL_LIGHTING); 
	/////////////////////////
	//指定颜色
	glColor3f(r,g,b);             
	//输出文字
	Printftext (x,y, str,hFontAr[ifont]);   
	/////////////////////////
	//glEnable(GL_LIGHTING);         
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();
	glPopMatrix();
}

void CGLFont::settext(float x,float y,const char* str,int ifont,float r,float g,float b)
{
	glPushMatrix();

	glLoadIdentity();

	//属性进栈
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_LIGHTING);
	
	//指定颜色
	glColor3f(r,g,b);             
	//坐标转换，移动
	//glTranslatef(-(420-x)/800,(260-y)/600,-1.0f);
	glTranslatef(-(400-x)/600,(300-y)/600,-1.0f);
	//输出文字
	Printftext (0,0, str,hFontAr[ifont]);   

	/////////////////////////
	//glEnable(GL_LIGHTING);         
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();

	glPopMatrix();
}

void CGLFont::Print2D(float x,float y,const char* str,int ifont,float r,float g,float b)
{
	//glDisable(GL_DEPTH_TEST);

	glPushMatrix();

	//属性进栈
	//glPushAttrib(GL_CURRENT_BIT);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_TEXTURE_2D);

	//指定颜色
	glColor3f(r,g,b);             
	//坐标转换，移动
	//glTranslatef(x, y, -0.5f);
	//输出文字
	Printftext ((int)x,(int)y, str,hFontAr[ifont]);   

	/////////////////////////
	//glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();

	glPopMatrix();

	//glEnable(GL_DEPTH_TEST);
}

// 在屏幕光栅输出字符，默认灰色字体
// int x, y [窗口左下角为起点位置的坐标]
// LPCTSTR lpszText  [要输出的字符串]
// HFONT hFont [字体]
void CGLFont:: Printftext (int x, int y, LPCTSTR lpszText,HFONT hFont)
{
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	// 在屏幕上输出平面字符
	CBitmap bitmap;						
	BITMAP bm;                           
	SIZE size;                          
	
	//建立当前屏幕相关的DC
	HDC MDC = ::CreateCompatibleDC(0);
	//选入字体
	SelectObject(MDC,hFont);		
	//得到宽高
	::GetTextExtentPoint32(MDC,lpszText,strlen(lpszText),&size);
	
	//创建位图
	bitmap.CreateBitmap(size.cx, size.cy, 1, 1, NULL);
	//选入位图
	HBITMAP oldBmp=(HBITMAP)SelectObject(MDC,bitmap);
	//设置文字背景色
	SetBkColor  (MDC, RGB(0,     0,   0));	
	//文字颜色
	SetTextColor(MDC, RGB(255, 255, 255));	
	//DC中输出文字
	TextOut(MDC, 0, 0, lpszText, strlen(lpszText));
	
	bitmap.GetBitmap(&bm);				
	//得到32的倍数
	size.cx = (bm.bmWidth + 31) & (~31);		

	int bufsize =size.cy * size.cx;	
	
	struct {
		BITMAPINFOHEADER bih;
		RGBQUAD col[2];
 		 }bic;                                 
	BITMAPINFO *binf = (BITMAPINFO *)&bic;       
	binf->bmiHeader.biSize     = sizeof(binf->bmiHeader);
	binf->bmiHeader.biWidth    = bm.bmWidth;	
	binf->bmiHeader.biHeight   = bm.bmHeight;	
	binf->bmiHeader.biPlanes   = 1;         
	binf->bmiHeader.biBitCount = 1;             
	binf->bmiHeader.biCompression = BI_RGB;     
	binf->bmiHeader.biSizeImage   = bufsize; 
	//分配内存
	UCHAR* Bits = new UCHAR[bufsize];			
	//得到图像
	::GetDIBits(MDC,bitmap,0,bm.bmHeight,Bits,binf,DIB_RGB_COLORS); 
 
	//在RC中输出文字
	glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
	//设置光栅位置
	glRasterPos2i(x,y);                  
	glBitmap(size.cx,size.cy,0,0,0,0,Bits);
	
	delete Bits;                            
	SelectObject(MDC, oldBmp);           
	::DeleteDC(MDC);

	glPopAttrib();
	glPopMatrix();
}
