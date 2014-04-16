
/*!
 *		Copyright(C),2010-2010
 * \file:		SCOpenGLmodel.cpp	,	SCOpenGLmodel.h
 * \brief:		啜国明封装的OpenGL模型类，利用OpenGL相关库，依赖文件Glext.h
 * \author:		啜国明
 * \version:	0.9
 * \Create:		2011-03-05
 * \modify：	2011-03-05
 * \History:	
 * \Run:			
 * \Environment	MicroSoft windows XP; Windows 7; Visual Studio 2005
 *
 *  <啜国明><2011-03-05><0.9><初步建立模型类库，主要用于创建纹理图形>
 *  <啜国明><2011-03-11><1.0><建立常用模型，加入两种载入位图方式>
 *	
 */

#pragma once
// 屏蔽C4996字符串类警告
#pragma warning (disable : 4996)

// 纹理载入采用位图方式BMP
//#define TEXTURE_LOAD_BMP
// 纹理载入采用TGA图片
#define TEXTURE_LOAD_TGA


#include "stdio.h"
#include "stdlib.h"
#include "math.h"
// openglut中包含了常用gl基本文件
#include "openglut.h"
//#include "glut.h"

#ifdef TEXTURE_LOAD_BMP
	// 支持多纹理
	#include "Glext.h"
	// 纹理格式应用了AUX库
	#include "gl\glaux.h"
	#pragma comment(lib, "GLAUX.LIB")
#endif


//////////////////////////////////////////////////////////////////////////
// 宏定义

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
	// 加载纹理图片的方式

	// BMP位图方式
#ifdef TEXTURE_LOAD_BMP
	unsigned int texture_BMP[50];				// 定义一个OpenGL纹理数组，用于纹理序号标识

	PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;		// 
	PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;			//

	// 加载纹理，调用CreateTexture,此函数一定需要等主窗口创建结束之后，OpenGL初始化全部完成之后才能执行
	void LoadTextures();
	// 加载一个纹理到纹理数组中
	void CreateTexture(unsigned int textureArray[], LPSTR strFileName, int textureID);
	// 画出一个纹理平面
	void TextureSquareBMP(double posX, double posY, double posZ,double length,double width,unsigned int i_texture = 0);
#endif
    //----------------------------------------------------------------------//

	// TGA图片方式
#ifdef TEXTURE_LOAD_TGA
	// 用户定义的变量
	GLuint		base;								// 字体显示列表

	// 定义材质贴图结构体
	typedef struct
	{
		GLubyte	*imageData;							// 图片数据 (最大 32 位)
		GLuint	bpp;								// 图片颜色深度 每象素
		GLuint	width;								// 图片宽度
		GLuint	height;								// 图片高度
		GLuint	texID;								// 贴图材质 ID 用来选择一个材质
	} TextureImage;									// 结构 名称

	TextureImage texture_TGA[50];						// 定义10个材质

	// 加载所有TGA图像素材
	bool LoadTextureTGA();
	// 加载TGA图片文件
	bool LoadTGA(TextureImage *texture, char *filename);
	// 建立字体显示列表
	void BuildFont(GLvoid);
	// 将字体显示在屏幕中
	void glPrint(GLint x, GLint y, const char *string, ...);

	// 画出一个纹理平面
	void TextureSquareTGA(double posX, double posY, double posZ,double length,double width,unsigned int i_texture = 1);

#endif
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// 声明变量
public:



	//////////////////////////////////////////////////////////////////////////
	// 函数声明
public:

	// 画出文字
	void DrawLetter(char* string, float x, float y , float z);
	void DrawLetter(int string, float x, float y , float z);
	void DrawLetter(float string, float x, float y , float z);
	void DrawLetter(double string, float x, float y , float z);

	// 画出一条直线
	void DrawLine(float x1, float y1, float z1,float x2, float y2, float z2, float r = 1);
	// 画出一个实心圆
	void DrawSolidCircle(float x, float y, float z, float r = 1);
	// 画出一个彩色方盒
	void DrawColorCube(float x, float y, float z, DWORD inc);
	void DrawMenuCur(float x, float y, float z, DWORD inc);

	// 画出地面网格
	void DrawGrid(GLfloat unit, int degree, DWORD inc);

};
