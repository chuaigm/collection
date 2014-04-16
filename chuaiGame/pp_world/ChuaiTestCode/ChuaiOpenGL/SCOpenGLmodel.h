
/*!
 *		Copyright(C),2010-2010
 * \file:		SCOpenGLmodel.cpp	,	SCOpenGLmodel.h
 * \brief:		ਹ�����װ��OpenGLģ���࣬����OpenGL��ؿ⣬�����ļ�Glext.h
 * \author:		ਹ���
 * \version:	0.9
 * \Create:		2011-03-05
 * \modify��	2011-03-05
 * \History:	
 * \Run:			
 * \Environment	MicroSoft windows XP; Windows 7; Visual Studio 2005
 *
 *  <ਹ���><2011-03-05><0.9><��������ģ����⣬��Ҫ���ڴ�������ͼ��>
 *  <ਹ���><2011-03-11><1.0><��������ģ�ͣ�������������λͼ��ʽ>
 *	
 */

#pragma once
// ����C4996�ַ����ྯ��
#pragma warning (disable : 4996)

// �����������λͼ��ʽBMP
//#define TEXTURE_LOAD_BMP
// �����������TGAͼƬ
#define TEXTURE_LOAD_TGA


#include "stdio.h"
#include "stdlib.h"
#include "math.h"
// openglut�а����˳���gl�����ļ�
#include "openglut.h"
//#include "glut.h"

#ifdef TEXTURE_LOAD_BMP
	// ֧�ֶ�����
	#include "Glext.h"
	// �����ʽӦ����AUX��
	#include "gl\glaux.h"
	#pragma comment(lib, "GLAUX.LIB")
#endif


//////////////////////////////////////////////////////////////////////////
// �궨��

#ifndef PI
#define PI 3.1415926535
#endif

//////////////////////////////////////////////////////////////////////////

class CSCOpenGLmodel
{
public:
	CSCOpenGLmodel(void);
	~CSCOpenGLmodel(void);
	//////////////////////////////////////////////////////////////////////////
	// ��������ͼƬ�ķ�ʽ

	// BMPλͼ��ʽ
#ifdef TEXTURE_LOAD_BMP
	unsigned int texture_BMP[50];				// ����һ��OpenGL�������飬����������ű�ʶ

	PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;		// 
	PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;			//

	// ������������CreateTexture,�˺���һ����Ҫ�������ڴ�������֮��OpenGL��ʼ��ȫ�����֮�����ִ��
	void LoadTextures();
	// ����һ����������������
	void CreateTexture(unsigned int textureArray[], LPSTR strFileName, int textureID);
	// ����һ������ƽ��
	void TextureSquareBMP(double posX, double posY, double posZ,double length,double width,unsigned int i_texture = 0);
#endif
    //----------------------------------------------------------------------//

	// TGAͼƬ��ʽ
#ifdef TEXTURE_LOAD_TGA
	// �û�����ı���
	GLuint		base;								// ������ʾ�б�

	// ���������ͼ�ṹ��
	typedef struct
	{
		GLubyte	*imageData;							// ͼƬ���� (��� 32 λ)
		GLuint	bpp;								// ͼƬ��ɫ��� ÿ����
		GLuint	width;								// ͼƬ���
		GLuint	height;								// ͼƬ�߶�
		GLuint	texID;								// ��ͼ���� ID ����ѡ��һ������
	} TextureImage;									// �ṹ ����

	TextureImage texture_TGA[50];						// ����10������

	// ��������TGAͼ���ز�
	bool LoadTextureTGA();
	// ����TGAͼƬ�ļ�
	bool LoadTGA(TextureImage *texture, char *filename);
	// ����������ʾ�б�
	void BuildFont(GLvoid);
	// ��������ʾ����Ļ��
	void glPrint(GLint x, GLint y, const char *string, ...);

	// ����һ������ƽ��
	void TextureSquareTGA(double posX, double posY, double posZ,double length,double width,unsigned int i_texture = 1);

#endif
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// ��������
public:



	//////////////////////////////////////////////////////////////////////////
	// ��������
public:

	// ��������
	void DrawLetter(char* string, float x, float y , float z);
	void DrawLetter(int string, float x, float y , float z);
	void DrawLetter(float string, float x, float y , float z);
	void DrawLetter(double string, float x, float y , float z);

	// ����һ��ֱ��
	void DrawLine(float x1, float y1, float z1,float x2, float y2, float z2, float r = 1);
	// ����һ��ʵ��Բ
	void DrawSolidCircle(float x, float y, float z, float r = 1);
	// ����һ����ɫ����
	void DrawColorCube(float x, float y, float z, DWORD inc);
	void DrawMenuCur(float x, float y, float z, DWORD inc);

	// ������������
	void DrawGrid(GLfloat unit, int degree, DWORD inc);

};
