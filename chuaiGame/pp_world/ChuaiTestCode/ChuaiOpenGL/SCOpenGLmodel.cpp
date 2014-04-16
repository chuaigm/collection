
#include "SCOpenGLmodel.h"

CSCOpenGLmodel::CSCOpenGLmodel(void)
{

}

CSCOpenGLmodel::~CSCOpenGLmodel(void)
{
	glDeleteLists(base,95);					// ɾ������95 ������ʾ�б�
}


//////////////////////////////////////////////////////////////////////////
// ������ز���
#ifdef TEXTURE_LOAD_BMP

// ��������BMPͼ�񣬵���ʱһ��Ҫ�������崴�����ʱ
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
// ����һ������
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
// ����һ��������ĺ��ӣ���λ��������
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
// ����TGA��ʽ������

// ��������
bool CSCOpenGLmodel::LoadTextureTGA()
{
	if ((!LoadTGA(&texture_TGA[0], "GameData/texture/font.tga"))  ||				// ���� ����
		(!LoadTGA(&texture_TGA[1], "GameData/texture/menu1.tga")) ||				// ���� ����
		(!LoadTGA(&texture_TGA[2], "GameData/texture/menu2.tga")) ||				// ���� ����
		(!LoadTGA(&texture_TGA[3], "GameData/texture/menu3.tga")) ||				// ���� ����
		(!LoadTGA(&texture_TGA[4], "GameData/texture/menu4.tga")) ||				// ���� ����
		(!LoadTGA(&texture_TGA[5], "GameData/texture/menu5.tga")) ||				// ���� ����

		(!LoadTGA(&texture_TGA[6], "GameData/texture/SubMenu11.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[7], "GameData/texture/SubMenu12.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[8], "GameData/texture/SubMenu13.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[9], "GameData/texture/SubMenu14.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[10],"GameData/texture/SubMenu15.tga")) ||				// ����  ����

		(!LoadTGA(&texture_TGA[11],"GameData/texture/SubMenu21.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[12],"GameData/texture/SubMenu22.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[13],"GameData/texture/SubMenu23.tga")) ||				// ����  ����

		(!LoadTGA(&texture_TGA[14],"GameData/texture/SubMenu31.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[15],"GameData/texture/SubMenu32.tga")) ||				// ����  ����

		(!LoadTGA(&texture_TGA[20],"GameData/texture/GameOverEnter.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[21],"GameData/texture/SStime.tga"))		||				// ����  ����
		(!LoadTGA(&texture_TGA[22],"GameData/texture/SSexcellent.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[23],"GameData/texture/SSscore.tga"))		||				// ����  ����
		(!LoadTGA(&texture_TGA[24],"GameData/texture/SSnow.tga"))		||				// ����  ����
		(!LoadTGA(&texture_TGA[25],"GameData/texture/SSaverage.tga"))	||				// ����  ����
		(!LoadTGA(&texture_TGA[26],"GameData/texture/SShigh.tga"))		||				// ����  ����
		(!LoadTGA(&texture_TGA[27],"GameData/texture/loss.tga"))		||				// ����  ����
		(!LoadTGA(&texture_TGA[28],"GameData/texture/win.tga"))			||				// ����  ����
		(!LoadTGA(&texture_TGA[29],"GameData/texture/draw.tga"))		||				// ����  ����

		(!LoadTGA(&texture_TGA[39],"GameData/texture/excellent.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[40],"GameData/texture/plane1.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[41],"GameData/texture/plane2.tga")) ||				// ����  ����
		//(!LoadTGA(&texture_TGA[48],"GameData/texture/tmpBK.tga")) ||				// ����  ����
		(!LoadTGA(&texture_TGA[49],"GameData/texture/font.tga")))					// ����  ����
	{
		exit(0);															// ������ʧ��, ���ش���
	}

	BuildFont();

	return true;
}
// ����TGAͼƬ
bool CSCOpenGLmodel::LoadTGA( TextureImage *texture, char *filename )
{
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};					// (δ)ѹ���� TGA ͷ
	GLubyte		TGAcompare[12];												// �����Ƚ� TGA ͷ
	GLubyte		header[6];													// ���� 6 �����õ��ֽ�
	GLuint		bytesPerPixel;												// ÿ�����ֽ����� TGA �ļ�ʹ��
	GLuint		imageSize;													// ����ͼƬ��С�Ĵ洢
	GLuint		temp;														// ��ʱ����
	GLuint		type=GL_RGBA;												// ����Ĭ�ϵ� GL ģʽ Ϊ RBGA 

	FILE *file = fopen(filename, "rb");										// �� TGA �ļ�

	if(	file==NULL ||														// �ļ��Ƿ��Ѵ��� ?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// �Ƿ����12���ֽ�?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0 ||				// �ļ�ͷ�ǲ���������Ҫ�� ?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// ����ȷ����� 6 �� Bytes
	{
		if (file == NULL)													// �ļ��Ƿ��Ѵ��� ? 
			return FALSE;													// ���ش���
		else																// ����
		{
			fclose(file);													// �����κδ���, �ص��ļ�
			return FALSE;													// ���ش���
		}
	}

	texture->width  = header[1] * 256 + header[0];							// ���� TGA ��
	texture->height = header[3] * 256 + header[2];							// ���� TGA ��

	if(	texture->width	<=0 ||												// �� ��<=0
		texture->height	<=0 ||												// �� ��<=0
		(header[4]!=24 && header[4]!=32))									// �� TGA �� 24 or 32 λ?
	{
		fclose(file);														// �����κδ���, �ص��ļ�
		return FALSE;														// ���ش���
	}

	texture->bpp	= header[4];											// ȡ TGA ��λÿ���� (24 �� 32)
	bytesPerPixel	= texture->bpp/8;										// ���� 8 �õ��ֽ�ÿ����
	imageSize		= texture->width*texture->height*bytesPerPixel;			// ���� �����ڴ�Ϊ TGA ����

	texture->imageData=(GLubyte *)malloc(imageSize);						// ���� �ڴ� Ϊ TGA ����

	if(	texture->imageData==NULL ||											// ����ڴ��Ƿ����?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)			// ͼƬ��С�뱣���ڴ�Ĵ�С��� ?
	{
		if(texture->imageData!=NULL)										// ͼƬ���ݵĵ���
			free(texture->imageData);										// ���ɹ�, �ͷ�ͼ������

		fclose(file);														// �ص��ļ�
		return FALSE;														// ���ش���
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)						// ��ͼ��������ѭ��
	{																		// ������1�͵�3 Bytes (���졯red �� ������blue)
		temp=texture->imageData[i];											// ��ʱ�洢 ͼ��� ��i��
		texture->imageData[i] = texture->imageData[i + 2];					// �� ��1 Byte �õ����� ��3 Byte
		texture->imageData[i + 2] = temp;									// ���3 Byte �õ����� ��temp�� (��1 Byte ����)
	}

	fclose (file);															// �ص��ļ�

	// ����һ����ͼ���ʴ���������
	glGenTextures(1, &texture[0].texID);									// ���� OpenGL ���� ID

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);							// �����ǵĲ���
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		// �߹�����
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// �߹�����

	if (texture[0].bpp==24)													// �� TGA ��24 λ��
	{
		type=GL_RGB;														// �� ��type�� Ϊ GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;															// ���ʽ����ɹ�, ������ȷ
}
// ��������
void CSCOpenGLmodel::BuildFont( GLvoid )
{
	base=glGenLists(95);													// ���� 95 ��ʾ�б�
	glBindTexture(GL_TEXTURE_2D, texture_TGA[0].texID);						// �������������
	for (int loop=0; loop<95; loop++)										// ѭ���� 95 �б�
	{
		float cx=float(loop%16)/16.0f;										// X λ�� �ڵ�ǰ��ĸ
		float cy=float(loop/16)/8.0f;										// Y λ�� �ڵ�ǰ��ĸ

		glNewList(base+loop,GL_COMPILE);									// ��ʼ����һ���б�
		glBegin(GL_QUADS);													// ���ı������ÿ����ĸ
		glTexCoord2f(cx,         1.0f-cy-0.120f); glVertex2i(0,0);			// �ʵ� / �� ���� (�� ��)
		glTexCoord2f(cx+0.0625f, 1.0f-cy-0.120f); glVertex2i(16,0);			// �ʵ� / �� ���� (�� ��)
		glTexCoord2f(cx+0.0625f, 1.0f-cy);	  glVertex2i(16,16);			// �ʵ� / �� ���� (�� ��)
		glTexCoord2f(cx,         1.0f-cy);	  glVertex2i(0,16);				// �ʵ� / �� ���� (�� ��)
		glEnd();															// ��ɽ������ǵ� �ı��� (��ĸ)
		glTranslated(10,0,0);												// �Ƶ�������ұ�
		glEndList();														// ��ɽ����������ʾ�б�
	}																		// ѭ��ֱ������ 256 ��ɽ���
}
// ��ӡ
void CSCOpenGLmodel::glPrint( GLint x, GLint y, const char *string, ... )
{
	char		text[256];													// ���������ǵ��ַ���
	va_list		ap;															// ���б��ָ��

	if (string == NULL)														// ������Ϊ��
		return;																// ����

	va_start(ap, string);													// �����ַ���
	vsprintf(text, string, ap);												// ת���ַ���
	va_end(ap);																// ������ַ���

	glBindTexture(GL_TEXTURE_2D, texture_TGA[0].texID);						// ѡ�������������
	glPushMatrix();															// ��ۿ�ģʽ����
	glLoadIdentity();														// ��ۿ�ģʽ����
	glTranslated(x,y,0);													// �������λ�� (0,0 - �� ��-Bottom Left)
	glListBase(base-32);													// ѡ����������
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);						// �����ʾ�б��е�����
	glPopMatrix();															// ȡ����ǰ��ģʽ����
}

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// ��������

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
// ���ƾ����ͼԪ

// ���ݸ������㻭��һ���ߣ�����ѡ���߿��
void CSCOpenGLmodel::DrawLine( float x1, float y1, float z1,float x2, float y2, float z2, float r /*= 1*/ )
{
	glLineWidth(r);

	glBegin(GL_LINES);
	glVertex3f(x1,y1,z1);
	glVertex3f(x2,y2,z2);
	glEnd();

	glLineWidth(1.0);
}
// ����һ��������ɫ�ķ�����
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
// ����һ��ʵ��Բ
void CSCOpenGLmodel::DrawSolidCircle( float x, float y, float z, float r /*= 1*/ )
{
	glDisable(GL_TEXTURE_2D);
	// ����ѹջ
	glPushMatrix();
	// ��λ
	glTranslatef(x,y,z);

	// �������ģʽ
	glBegin(GL_POLYGON);
	// ������Z��������
	glNormal3f(0,0,1);
	for(int alpha=0; alpha <= 360 ;alpha+=10)
	{
		glVertex3f(r*sin(alpha*PI/180),r*cos(alpha*PI/180), r*z);
	}
	glEnd();

	//���󵯳���ջ
	glPopMatrix();
}


// ����һ��������ĺ��ӣ���λ��������
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
// һ��������ɫ�ԳƵķ�����
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
// �����ο�����
void CSCOpenGLmodel::DrawGrid(GLfloat unit, int degree,DWORD inc)
{
	glDisable(GL_TEXTURE_2D);

	static GLfloat xrot2; // X Rotation 
	static GLfloat yrot2; // Y Rotation
	static GLfloat zrot2; // Z Rotation 

	//����ѹջ
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

	//���󵯳���ջ
	glPopMatrix();

	xrot2+=0.005f*inc;
	yrot2+=0.008f*inc;
	zrot2+=0.002f*inc; // Decrease The Rotation Variable For The Quad

	glColor3f(1.0f, 1.0f, 1.0f);

}

//////////////////////////////////////////////////////////////////////////