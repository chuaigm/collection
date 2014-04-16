
#include "SCOpenGLmodel.h"

CSCOpenGLmodel::CSCOpenGLmodel(void)
{

}

CSCOpenGLmodel::~CSCOpenGLmodel(void)
{
	glDeleteLists(base,95);					// 删掉所有95 字体显示列表
}


//////////////////////////////////////////////////////////////////////////
// 纹理加载部分
#ifdef TEXTURE_LOAD_BMP

// 加载纹理BMP图像，调用时一定要等主窗体创建完毕时
void CSCOpenGLmodel::LoadTextures()
{
	glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");

	CreateTexture(texture_BMP, ".\\GameData\\texture\\menu1.bmp", 0);				//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\menu2.bmp", 1);				//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\menu3.bmp", 2);				//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\menu4.bmp", 3);				//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\menu5.bmp", 4);				//

	CreateTexture(texture_BMP, ".\\GameData\\texture\\SubMenu11.bmp", 5);			//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\SubMenu12.bmp", 6);			//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\SubMenu13.bmp", 7);			//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\SubMenu14.bmp", 8);			//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\SubMenu15.bmp", 9);			//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\SubMenu21.bmp", 10);			//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\SubMenu22.bmp", 11);			//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\SubMenu23.bmp", 12);			//

	CreateTexture(texture_BMP, ".\\GameData\\texture\\SubMenu31.bmp", 13);			//
	CreateTexture(texture_BMP, ".\\GameData\\texture\\SubMenu32.bmp", 14);			//
}
// 创建一个纹理
void CSCOpenGLmodel::CreateTexture( unsigned int textureArray[], LPSTR strFileName, int textureID )
{
	AUX_RGBImageRec *pBitmap = NULL;

	if(!strFileName)									// Return from the function if no file name was passed in
		return;
	// We need to load the texture data, so we use a cool API that the glaux.lib offers.

	pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data

	if(pBitmap == NULL)									// If we can't load the file, quit!
		exit(0);
	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &textureArray[textureID]);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though.

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}
		free(pBitmap);									// Free the bitmap structure
	}
}
// 画出一个带纹理的盒子，定位点在中心
void CSCOpenGLmodel::TextureSquareBMP( double posX, double posY, double posZ,double length,double width,unsigned int i_texture /*= 0*/ )
{
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,  texture_BMP[i_texture]);

	// Set the second texture ID to be active, then bind our light map texture to this ID.
	/*glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,  g_Texture[1]);*/

	// Because we want to translate the textured QUADS to different locations, we push
	// on a matrix so that the other QUAD is not effect by our translation of the previous one.
	// We do not have to push and pop on another matrix for our other QUAD because there is no
	// geometry rendered after it.  The matrix will be nixed anyRoamWay.
	glPushMatrix();

	// This translates the current matrix to move the first QUAD over to the left
	// of the screen.
	glTranslatef(posX, posY, posZ);
	glScalef(length,width,1);

	// Display a multitextured quad to the left side of the screen
	glBegin(GL_QUADS);

	// Now, instead of using glTexCoord2f() we use glMultiTextCoord2fARB() to
	// specify the texture coordinates for each texture.  This allows us to move
	// one texture to a totally different place on the geometry.  A perfect example
	// of this is shadows.  glMultiTexCoord2fARB() takes the texture ID we wish
	// to specify, with it's texture coordinates afterwards.  We just use the same
	// texture coordinates for each texture.

	// Display the top left vertice with the texture coordinates for both textures
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
	glVertex3f(-0.5, 0.5, 0);

	// Display the bottom left vertice with the textures for both textures
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
	glVertex3f(-0.5, -0.5, 0);

	// Display the bottom right vertice with the textures for both textures
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
	glVertex3f(0.5, -0.5, 0);

	// Display the top right vertice with the textures for both textures
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
	glVertex3f(0.5, 0.5, 0);

	// Stop drawing 
	glEnd();											

	// Here we pop the last matrix so that it does not effect our next QUAD.  Now we
	// are rendering with the original identity matrix.
	glPopMatrix();
}
#endif
//////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////
// 加载TGA方式的纹理

// 加载所有
bool CSCOpenGLmodel::LoadTextureTGA()
{
	if ((!LoadTGA(&texture_TGA[0], "GameData/texture/font.tga"))  ||				// 调入 材质
		(!LoadTGA(&texture_TGA[1], "GameData/texture/menu1.tga")) ||				// 调入 材质
		(!LoadTGA(&texture_TGA[2], "GameData/texture/menu2.tga")) ||				// 调入 材质
		(!LoadTGA(&texture_TGA[3], "GameData/texture/menu3.tga")) ||				// 调入 材质
		(!LoadTGA(&texture_TGA[4], "GameData/texture/menu4.tga")) ||				// 调入 材质
		(!LoadTGA(&texture_TGA[5], "GameData/texture/menu5.tga")) ||				// 调入 材质

		(!LoadTGA(&texture_TGA[6], "GameData/texture/SubMenu11.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[7], "GameData/texture/SubMenu12.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[8], "GameData/texture/SubMenu13.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[9], "GameData/texture/SubMenu14.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[10],"GameData/texture/SubMenu15.tga")) ||				// 调入  材质

		(!LoadTGA(&texture_TGA[11],"GameData/texture/SubMenu21.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[12],"GameData/texture/SubMenu22.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[13],"GameData/texture/SubMenu23.tga")) ||				// 调入  材质

		(!LoadTGA(&texture_TGA[14],"GameData/texture/SubMenu31.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[15],"GameData/texture/SubMenu32.tga")) ||				// 调入  材质

		(!LoadTGA(&texture_TGA[20],"GameData/texture/GameOverEnter.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[21],"GameData/texture/SStime.tga"))		||				// 调入  材质
		(!LoadTGA(&texture_TGA[22],"GameData/texture/SSexcellent.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[23],"GameData/texture/SSscore.tga"))		||				// 调入  材质
		(!LoadTGA(&texture_TGA[24],"GameData/texture/SSnow.tga"))		||				// 调入  材质
		(!LoadTGA(&texture_TGA[25],"GameData/texture/SSaverage.tga"))	||				// 调入  材质
		(!LoadTGA(&texture_TGA[26],"GameData/texture/SShigh.tga"))		||				// 调入  材质
		(!LoadTGA(&texture_TGA[27],"GameData/texture/loss.tga"))		||				// 调入  材质
		(!LoadTGA(&texture_TGA[28],"GameData/texture/win.tga"))			||				// 调入  材质
		(!LoadTGA(&texture_TGA[29],"GameData/texture/draw.tga"))		||				// 调入  材质

		(!LoadTGA(&texture_TGA[39],"GameData/texture/excellent.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[40],"GameData/texture/plane1.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[41],"GameData/texture/plane2.tga")) ||				// 调入  材质
		//(!LoadTGA(&texture_TGA[48],"GameData/texture/tmpBK.tga")) ||				// 调入  材质
		(!LoadTGA(&texture_TGA[49],"GameData/texture/font.tga")))					// 调入  材质
	{
		exit(0);															// 若调入失败, 返回错误
	}

	BuildFont();

	return true;
}
// 载入TGA图片
bool CSCOpenGLmodel::LoadTGA( TextureImage *texture, char *filename )
{
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};					// (未)压缩的 TGA 头
	GLubyte		TGAcompare[12];												// 用来比较 TGA 头
	GLubyte		header[6];													// 首先 6 个有用的字节
	GLuint		bytesPerPixel;												// 每象素字节数在 TGA 文件使用
	GLuint		imageSize;													// 用来图片大小的存储
	GLuint		temp;														// 临时变量
	GLuint		type=GL_RGBA;												// 设置默认的 GL 模式 为 RBGA 

	FILE *file = fopen(filename, "rb");										// 打开 TGA 文件

	if(	file==NULL ||														// 文件是否已存在 ?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// 是否读出12个字节?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0 ||				// 文件头是不是我们想要的 ?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// 若正确则读下 6 个 Bytes
	{
		if (file == NULL)													// 文件是否已存在 ? 
			return FALSE;													// 返回错误
		else																// 否则
		{
			fclose(file);													// 若有任何错误, 关掉文件
			return FALSE;													// 返回错误
		}
	}

	texture->width  = header[1] * 256 + header[0];							// 定义 TGA 宽
	texture->height = header[3] * 256 + header[2];							// 定义 TGA 高

	if(	texture->width	<=0 ||												// 若 宽<=0
		texture->height	<=0 ||												// 若 高<=0
		(header[4]!=24 && header[4]!=32))									// 若 TGA 是 24 or 32 位?
	{
		fclose(file);														// 若有任何错误, 关掉文件
		return FALSE;														// 返回错误
	}

	texture->bpp	= header[4];											// 取 TGA 的位每象素 (24 或 32)
	bytesPerPixel	= texture->bpp/8;										// 除以 8 得到字节每象素
	imageSize		= texture->width*texture->height*bytesPerPixel;			// 计算 所需内存为 TGA 数据

	texture->imageData=(GLubyte *)malloc(imageSize);						// 分配 内存 为 TGA 数据

	if(	texture->imageData==NULL ||											// 这个内存是否存在?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)			// 图片大小与保留内存的大小想等 ?
	{
		if(texture->imageData!=NULL)										// 图片数据的调入
			free(texture->imageData);										// 若成功, 释放图象数据

		fclose(file);														// 关掉文件
		return FALSE;														// 返回错误
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)						// 在图象数据里循环
	{																		// 交换第1和第3 Bytes (’红’red 和 ’蓝’blue)
		temp=texture->imageData[i];											// 临时存储 图象的 ’i’
		texture->imageData[i] = texture->imageData[i + 2];					// 设 第1 Byte 得到变量 第3 Byte
		texture->imageData[i + 2] = temp;									// 设第3 Byte 得到变量 ’temp’ (第1 Byte 变量)
	}

	fclose (file);															// 关掉文件

	// 建立一个贴图材质从以上数据
	glGenTextures(1, &texture[0].texID);									// 生成 OpenGL 材质 ID

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);							// 绑定我们的材质
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		// 线过滤器
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// 线过滤器

	if (texture[0].bpp==24)													// 若 TGA 是24 位的
	{
		type=GL_RGB;														// 设 ’type’ 为 GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;															// 材质建立成功, 返回正确
}
// 建立字体
void CSCOpenGLmodel::BuildFont( GLvoid )
{
	base=glGenLists(95);													// 建立 95 显示列表
	glBindTexture(GL_TEXTURE_2D, texture_TGA[0].texID);						// 绑我们字体材质
	for (int loop=0; loop<95; loop++)										// 循环在 95 列表
	{
		float cx=float(loop%16)/16.0f;										// X 位置 在当前字母
		float cy=float(loop/16)/8.0f;										// Y 位置 在当前字母

		glNewList(base+loop,GL_COMPILE);									// 开始建立一个列表
		glBegin(GL_QUADS);													// 用四边形组成每个字母
		glTexCoord2f(cx,         1.0f-cy-0.120f); glVertex2i(0,0);			// 质地 / 点 座标 (底 左)
		glTexCoord2f(cx+0.0625f, 1.0f-cy-0.120f); glVertex2i(16,0);			// 质地 / 点 座标 (底 右)
		glTexCoord2f(cx+0.0625f, 1.0f-cy);	  glVertex2i(16,16);			// 质地 / 点 座标 (顶 右)
		glTexCoord2f(cx,         1.0f-cy);	  glVertex2i(0,16);				// 质地 / 点 座标 (顶 左)
		glEnd();															// 完成建立我们的 四边形 (字母)
		glTranslated(10,0,0);												// 移到字体的右边
		glEndList();														// 完成建军立这个显示列表
	}																		// 循环直到所有 256 完成建立
}
// 打印
void CSCOpenGLmodel::glPrint( GLint x, GLint y, const char *string, ... )
{
	char		text[256];													// 保存在我们的字符串
	va_list		ap;															// 到列表的指针

	if (string == NULL)														// 若文字为空
		return;																// 返回

	va_start(ap, string);													// 解析字符串
	vsprintf(text, string, ap);												// 转换字符串
	va_end(ap);																// 结果的字符串

	glBindTexture(GL_TEXTURE_2D, texture_TGA[0].texID);						// 选择我们字体材质
	glPushMatrix();															// 存观看模式矩阵
	glLoadIdentity();														// 设观看模式矩阵
	glTranslated(x,y,0);													// 文字输出位置 (0,0 - 底 左-Bottom Left)
	glListBase(base-32);													// 选择字体设置
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);						// 输出显示列表中的文字
	glPopMatrix();															// 取出以前的模式矩阵
}

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// 画出参数

void CSCOpenGLmodel::DrawLetter( char* string, float x, float y , float z )
{
	glPushMatrix();
	glRasterPos3f(x,y,z);

	char* p = NULL;
	for (p = string; *p; p++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);

	glPopMatrix();
}

void CSCOpenGLmodel::DrawLetter( int string, float x, float y , float z )
{
	char str[12];

	_itoa_s(string , str , 10);

	glPushMatrix();
	glRasterPos3f(x,y,z);

	char* p = NULL;
	for (p = str; *p; p++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);

	glPopMatrix();
}

void CSCOpenGLmodel::DrawLetter( float string, float x, float y , float z )
{
	char str[10];

	//sprintf(str, "%f" , string);
	sprintf_s(str, "%.2f" , string);

	glPushMatrix();
	glRasterPos3f(x,y,z);

	char* p = NULL;
	for (p = str; *p; p++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);

	glPopMatrix();
}

void CSCOpenGLmodel::DrawLetter( double string, float x, float y , float z )
{
	char str[20];

	//sprintf(str, "%f" , string);
	sprintf_s(str, "%f" , string);

	glPushMatrix();
	glRasterPos3f(x,y,z);

	char* p = NULL;
	for (p = str; *p; p++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);

	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// 绘制具体的图元

// 根据给出两点画出一条线，可以选择线宽度
void CSCOpenGLmodel::DrawLine( float x1, float y1, float z1,float x2, float y2, float z2, float r /*= 1*/ )
{
	glLineWidth(r);

	glBegin(GL_LINES);
	glVertex3f(x1,y1,z1);
	glVertex3f(x2,y2,z2);
	glEnd();

	glLineWidth(1.0);
}
// 画出一个渐变颜色的方盒子
void CSCOpenGLmodel::DrawColorCube( float x, float y, float z, DWORD inc)
{
	// EXAMPLE OPENGL CODE START /////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	static GLfloat xrot; // X Rotation 
	static GLfloat yrot; // Y Rotation
	static GLfloat zrot; // Z Rotation 

	glPushMatrix(); // Push Matrix Onto Stack (Copy The Current Matrix)
	//glLoadIdentity(); // Reset The Current Modelview Matrix
	glTranslatef(x,y,z); // Move Left 1.5 Units And Into The Screen 6.0

	glScalef(0.3,0.3,0.3);

	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(zrot,0.0f,0.0f,1.0f);

	glBegin(GL_QUADS);
	// Front Face
	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, 1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, 1.0f);
	glColor3f(0.0f,0.0f,1.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, -1.0f);
	glColor3f(0.0f,0.0f,1.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
	// Top Face
	glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
	glColor3f(0.0f,0.0f,1.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, 1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, -1.0f);
	// Bottom Face
	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
	glColor3f(0.0f,0.0f,1.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, 1.0f);
	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
	// Right face
	glColor3f(0.0f,0.0f,1.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, -1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, 1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, 1.0f);
	// Left Face
	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
	glColor3f(0.0f,0.0f,1.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, -1.0f);

	glEnd();
	glPopMatrix(); // Pop Matrix Off The Stack

	xrot+=0.3f*inc;
	yrot+=0.1f*inc;
	zrot+=0.2f*inc; // Decrease The Rotation Variable For The Quad
	glColor3f(1.0f,1.0f,1.0f);
	//////////////////////////////////////////////////////////////////////
	// EXAMPLE OPENGL CODE END ///////////////////////////////////////////
}
// 画出一个实心圆
void CSCOpenGLmodel::DrawSolidCircle( float x, float y, float z, float r /*= 1*/ )
{
	glDisable(GL_TEXTURE_2D);
	// 矩阵压栈
	glPushMatrix();
	// 定位
	glTranslatef(x,y,z);

	// 折线填充模式
	glBegin(GL_POLYGON);
	// 法线向Z轴正方向
	glNormal3f(0,0,1);
	for(int alpha=0; alpha <= 360 ;alpha+=10)
	{
		glVertex3f(r*sin(alpha*PI/180),r*cos(alpha*PI/180), r*z);
	}
	glEnd();

	//矩阵弹出堆栈
	glPopMatrix();
}


// 画出一个带纹理的盒子，定位点在中心
void CSCOpenGLmodel::TextureSquareTGA( double posX, double posY, double posZ,double length,double width,unsigned int i_texture /*= 0*/ )
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,  texture_TGA[i_texture].texID);

	glPushMatrix();

	glTranslatef(posX, posY, posZ);
	glScalef(length,width,1);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f,0.0f); glVertex3f(-0.5,-0.5,0.0f);	
	glTexCoord2f(1.0f,0.0f); glVertex3f( 0.5,-0.5,0.0f);	
	glTexCoord2f(1.0f,1.0f); glVertex3f( 0.5, 0.5,0.0f);	
	glTexCoord2f(0.0f,1.0f); glVertex3f(-0.5, 0.5,0.0f);	

	glEnd();											

	glPopMatrix();
}
// 一个四种颜色对称的方盒子
void CSCOpenGLmodel::DrawMenuCur( float x, float y, float z, DWORD inc )
{
	glDisable(GL_TEXTURE_2D);
	static GLfloat xrot; // X Rotation 
	static GLfloat yrot; // Y Rotation
	static GLfloat zrot; // Z Rotation 

	glPushMatrix(); // Push Matrix Onto Stack (Copy The Current Matrix)
	//glLoadIdentity(); // Reset The Current Modelview Matrix
	glTranslatef(x,y,z); // Move Left 1.5 Units And Into The Screen 6.0

	glScalef(0.25,0.25,0.25);

	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(zrot,0.0f,0.0f,1.0f);

	glBegin(GL_QUADS);
	// Front Face
	glColor3f(0.0f,0.0f,1.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, 1.0f);
	glColor3f(1.0f,0.0f,0.0f);glVertex3f( 1.0f, 1.0f, 1.0f);
	glColor3f(1.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
	glColor3f(0.0f,0.0f,1.0f);glVertex3f( 1.0f, 1.0f, -1.0f);
	glColor3f(1.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
	// Top Face
	glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
	glColor3f(1.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
	glColor3f(1.0f,0.0f,0.0f);glVertex3f( 1.0f, 1.0f, 1.0f);
	glColor3f(0.0f,0.0f,1.0f);glVertex3f( 1.0f, 1.0f, -1.0f);
	// Bottom Face
	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
	glColor3f(1.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, 1.0f);
	glColor3f(0.0f,0.0f,1.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
	// Right face
	glColor3f(1.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
	glColor3f(0.0f,0.0f,1.0f);glVertex3f( 1.0f, 1.0f, -1.0f);
	glColor3f(1.0f,0.0f,0.0f);glVertex3f( 1.0f, 1.0f, 1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, 1.0f);
	// Left Face
	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
	glColor3f(0.0f,0.0f,1.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
	glColor3f(1.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
	glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, -1.0f);

	glEnd();
	glPopMatrix(); // Pop Matrix Off The Stack

	xrot+=0.3f*inc;
	yrot+=0.1f*inc;
	zrot+=0.2f*inc; // Decrease The Rotation Variable For The Quad
	glColor3f(1.0f,1.0f,1.0f);
	
}

//////////////////////////////////////////////////////////////////////////
// 画出参考地面
void CSCOpenGLmodel::DrawGrid(GLfloat unit, int degree,DWORD inc)
{
	glDisable(GL_TEXTURE_2D);

	static GLfloat xrot2; // X Rotation 
	static GLfloat yrot2; // Y Rotation
	static GLfloat zrot2; // Z Rotation 

	//矩阵压栈
	glPushMatrix();

	glRotatef(xrot2,1.0f,0.0f,0.0f);
	glRotatef(yrot2,0.0f,1.0f,0.0f);
	glRotatef(zrot2,0.0f,0.0f,1.0f);

	glColor3f(0.5f,0.7f,1.0f);

	const int scale = 50;

	if (unit < 0.001)
	{
		unit = 1;
	}

	glLineWidth(1);
	glBegin(GL_LINES);

	for(int y = -scale;y < scale; y+=unit)
	{
		glVertex3i(-scale,y,0);
		glVertex3i(scale,y,0);
	}

	for(int x = -scale;x < scale; x+=unit)
	{
		glVertex3i(x,-scale,0);
		glVertex3i(x,scale,0);
	}

	glEnd();

	if (degree > 0)
	{
		for (int x = 0; x < scale+1; x+=degree)
		{
			glColor3f(1.0f, 1.0f, 0.0f);
			DrawLetter(x , 0 , 0 , x);
			DrawLetter(-x , 0 , 0 , -x);
		}

		for (int y = 0; y < scale+1; y+=degree)
		{
			glColor3f(1.0f, 1.0f, 0.0f);
			DrawLetter(0,y,0,y);
			DrawLetter(0,-y,0,-y);
		}
	}

	//矩阵弹出堆栈
	glPopMatrix();

	xrot2+=0.005f*inc;
	yrot2+=0.008f*inc;
	zrot2+=0.002f*inc; // Decrease The Rotation Variable For The Quad

	glColor3f(1.0f, 1.0f, 1.0f);

}

//////////////////////////////////////////////////////////////////////////