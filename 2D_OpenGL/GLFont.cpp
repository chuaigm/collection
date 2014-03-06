//
// ����  programking 2009��3��
// ����  http://blog.csdn.net/programking
//=======================================
// GLFont.cpp: implementation of the CGLFont class.
// ѧ�������Ϸϵ�д���
// ������ E_mail: cqtmL@163.com
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

//�ڹ�դλ��xy�����������
// ���ڹ�դ��λ��z=0������gluPerspectiveͶӰ����Ҫ����
void CGLFont::settextTest(int x,int y,const char* str,int ifont,float r,float g,float b)
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);    
	//glDisable(GL_LIGHTING); 
	/////////////////////////
	//ָ����ɫ
	glColor3f(r,g,b);             
	//�������
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

	//���Խ�ջ
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_LIGHTING);
	
	//ָ����ɫ
	glColor3f(r,g,b);             
	//����ת�����ƶ�
	//glTranslatef(-(420-x)/800,(260-y)/600,-1.0f);
	glTranslatef(-(400-x)/600,(300-y)/600,-1.0f);
	//�������
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

	//���Խ�ջ
	//glPushAttrib(GL_CURRENT_BIT);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_TEXTURE_2D);

	//ָ����ɫ
	glColor3f(r,g,b);             
	//����ת�����ƶ�
	//glTranslatef(x, y, -0.5f);
	//�������
	Printftext ((int)x,(int)y, str,hFontAr[ifont]);   

	/////////////////////////
	//glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();

	glPopMatrix();

	//glEnable(GL_DEPTH_TEST);
}

// ����Ļ��դ����ַ���Ĭ�ϻ�ɫ����
// int x, y [�������½�Ϊ���λ�õ�����]
// LPCTSTR lpszText  [Ҫ������ַ���]
// HFONT hFont [����]
void CGLFont:: Printftext (int x, int y, LPCTSTR lpszText,HFONT hFont)
{
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	// ����Ļ�����ƽ���ַ�
	CBitmap bitmap;						
	BITMAP bm;                           
	SIZE size;                          
	
	//������ǰ��Ļ��ص�DC
	HDC MDC = ::CreateCompatibleDC(0);
	//ѡ������
	SelectObject(MDC,hFont);		
	//�õ����
	::GetTextExtentPoint32(MDC,lpszText,strlen(lpszText),&size);
	
	//����λͼ
	bitmap.CreateBitmap(size.cx, size.cy, 1, 1, NULL);
	//ѡ��λͼ
	HBITMAP oldBmp=(HBITMAP)SelectObject(MDC,bitmap);
	//�������ֱ���ɫ
	SetBkColor  (MDC, RGB(0,     0,   0));	
	//������ɫ
	SetTextColor(MDC, RGB(255, 255, 255));	
	//DC���������
	TextOut(MDC, 0, 0, lpszText, strlen(lpszText));
	
	bitmap.GetBitmap(&bm);				
	//�õ�32�ı���
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
	//�����ڴ�
	UCHAR* Bits = new UCHAR[bufsize];			
	//�õ�ͼ��
	::GetDIBits(MDC,bitmap,0,bm.bmHeight,Bits,binf,DIB_RGB_COLORS); 
 
	//��RC���������
	glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
	//���ù�դλ��
	glRasterPos2i(x,y);                  
	glBitmap(size.cx,size.cy,0,0,0,0,Bits);
	
	delete Bits;                            
	SelectObject(MDC, oldBmp);           
	::DeleteDC(MDC);

	glPopAttrib();
	glPopMatrix();
}
