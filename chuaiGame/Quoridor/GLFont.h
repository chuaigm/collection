// GLFont.h: interface for the CGLFont class.

#if !defined(AFX_GLFONT_H__88F1F000_50F5_452A_B95E_60ED83712FA5__INCLUDED_)
#define AFX_GLFONT_H__88F1F000_50F5_452A_B95E_60ED83712FA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGLFont  
{
public:
	// 文字栅格化的实现
	void Printftext(int x, int y, LPCTSTR lpszText,HFONT hFont);
	//根据字体输出文字
	void settext(float x,float y,const char* str,int ifont,float r,float g,float b);
	void Print2D(int x,int y,const char* str,int ifont,float r,float g,float b,float a=1.0);
	//
	void settextTest(int x,int y,const char* str,int ifont,float r,float g,float b);

	CGLFont();
	virtual ~CGLFont();

public:
	// 创建字体
	HFONT	hFontAr[16];
};

#endif // !defined(AFX_GLFONT_H__88F1F000_50F5_452A_B95E_60ED83712FA5__INCLUDED_)
