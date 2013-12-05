// GLFont.h: interface for the CGLFont class.

#if !defined(AFX_GLFONT_H__88F1F000_50F5_452A_B95E_60ED83712FA5__INCLUDED_)
#define AFX_GLFONT_H__88F1F000_50F5_452A_B95E_60ED83712FA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGLFont  
{
public:
	void Printftext(int x, int y, LPCTSTR lpszText,HFONT hFont);
	//根据字体输出文字
	void settext(float x,float y,CString str,int ifont,float r,float g,float b);
	//
	void settextTest(int x,int y,CString str,int ifont,float r,float g,float b);

	CGLFont();
	virtual ~CGLFont();

public:
	HFONT	hFontAr[6];
};

#endif // !defined(AFX_GLFONT_H__88F1F000_50F5_452A_B95E_60ED83712FA5__INCLUDED_)
