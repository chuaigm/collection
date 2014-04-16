
//#include "stdafx.h"
#include "SCOpenGL.h"


CSCOpenGL::CSCOpenGL(void)
{
	// ��ʼ�����ڴ�С
	m_Width = 800;
	m_Height = 600;

	g_Width = 800;
	g_Height = 600;
	//// ��ʼ����ת�Ƕ�
	//rad_hor = 0;
	//rad_ver = 0;
	//// ��ʼ���۲�λ��9������
	//eye[0] =  0.0;
	//eye[1] =  0.0;
	//eye[2] =  14.0;

	//center[0] = 0.0;
	//center[1] = 0.0;
	//center[2] = 0.0;

	//up[0] = 0.0;
	//up[1] = 1.0;
	//up[2] = 0.0;
	// ��ʼ��ʱ���

	g_GameState = 1;

	NowTimeTicket = GetTickCount();
	LastTimeTicket = NowTimeTicket;

}

CSCOpenGL::~CSCOpenGL(void)
{
}


//////////////////////////////////////////////////////////////////////////
// ���߳�ģʽ������
void CSCOpenGL::MainScOpenGL()
{
	InitSCOpenGL();

	g_myModel.LoadTextureTGA();

	glutReshapeFunc(ReShape);

	glutDisplayFunc(RenderScene);

	//glutIdleFunc(RenderScene);
	
	glutMainLoop();
}

#ifdef USER_NUM2
// ���߳�ģʽ������(2),Ϊ�ڶ��ֻ��Ʒ���
void CSCOpenGL::YourScOpenGL()
{
	InitSCOpenGL();

	glutReshapeFunc(ReShape);

	glutDisplayFunc(RenderYourScene);

	glutIdleFunc(RenderYourScene);

	//glutKeyboardFunc(SCKeyBoard);

	glutMainLoop();
}
#endif

#ifdef USER_NUM3
// ���߳�ģʽ������(3)��Ϊ�����ֻ��Ʒ���
void CSCOpenGL::TheeScOpenGL()
{
	InitSCOpenGL();

	glutReshapeFunc(ReShape);

	glutDisplayFunc(RenderTheeScene);

	glutIdleFunc(RenderTheeScene);

	//glutKeyboardFunc(SCKeyBoard);

	glutMainLoop();
}
#endif

//// �߳�ģʽ���������̺߳���
//BOOL WINAPI CSCOpenGL::ThreadMainScOpenGL( void* )
//{
//	InitSCOpenGL();
//
//	glutReshapeFunc(ReShape);
//
//	glutDisplayFunc(RenderScene);
//
//	glutIdleFunc(RenderScene);
//
//	//glutKeyboardFunc(SCKeyBoard);
//
//	glutMainLoop();
//
//	return TRUE;
//}
//// �����̵߳ĺ���
//void CSCOpenGL::threadDraw()
//{
//	chBEGINTHREADEX( NULL, 0, ThreadMainScOpenGL, this, 0, NULL );
//}
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// OpenGL��ʼ������
void CSCOpenGL::InitSCOpenGL()
{
	glutInitDisplayMode(GLUT_DEPTH  | GLUT_DOUBLE | GLUT_RGBA);
	// ��ʼ�����ڴ�С
	glutInitWindowSize(m_Width,m_Height);

	// �õ���Ļ�ֱ���
	int   nFullWidth = GetSystemMetrics(SM_CXSCREEN); 
	int   nFullHeight = GetSystemMetrics(SM_CYSCREEN);
	// ��ʼ������λ��
	glutInitWindowPosition((nFullWidth-m_Width)/2,(nFullHeight-m_Height)/2);
	// ��������
	mainWindow = glutCreateWindow("Point-Point_World_1.0-------Created By Chuai!!!");
	// ��glut�Ĺ��ܣ���ֹ�ڴ�й¶���޷���������
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//�������䷽ʽ
	glShadeModel(GL_SMOOTH); //or GL_FLAT
	//���
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glAlphaFunc(GL_GREATER,0.1f);											// �� Alpha ����
	glEnable(GL_ALPHA_TEST);												// �� Alpha ����

	glEnable(GL_NORMALIZE);

	//��������εķ���
	//	glFrontFace(GL_CCW);
	//	glEnable(GL_CULL_FACE);
	//	glCullFace(GL_BACK);

	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	/////////////////////////////////////////////////////////������
	//glEnable(GL_SCISSOR_TEST);
	//	  glDisable(GL_DEPTH_TEST);
	// glEnable(GL_LINE_SMOOTH);
	// glEnable(GL_POLYGON_SMOOTH);
	//glPolygonOffset(0.01f,0.002f);
	::glPolygonMode(GL_FRONT,GL_FILL);

	float ratio;
	// ��ֹ�����
	if(m_Height == 0)
	{
		m_Height = 1;
	}
	if (m_Width == 0)
	{
		m_Width = 1;
	}
	ratio = 1.0f * m_Width / m_Height;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, m_Width, m_Height);

	// Set the clipping volume
	gluPerspective(45,ratio,0.1,1000);

	// setting the camera now
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();							// ����ģ�͹۲����
	// ��Ԥ��ֵ���������
	glClearColor(0.0f,0.0f,0.0f,0.0f);
}

// ����ߴ�ı亯��
void CSCOpenGL::ReShape(int width, int hight)
{
	float ratio;

	m_Height = hight;
	m_Width = width;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).

	if(m_Height == 0)
	{
		m_Height = 1;
	}
	if (m_Width == 0)
	{
		m_Width = 1;
	}

	ratio = 1.0f * m_Width / m_Height;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, m_Width, m_Height);

	// Set the clipping volume
	gluPerspective(45,ratio,0.1,1000);

	// setting the camera now
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();							// ����ģ�͹۲����

	g_Width = m_Width;
	g_Height = m_Height;

}
//// �����Ϣ
//void CSCOpenGL::SCMouseMove( int button, int state, int x, int y )
//{
//	/*if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
//	{
//
//	}*/
//
//}
//// ������Ϣ
//void CSCOpenGL::SCKeyBoard( unsigned char key, int x, int y )
//{
//	GetForwardVector(forwardVector[0],forwardVector[1],forwardVector[2]);
//	GetRightVector(rightVector[0],rightVector[1],rightVector[2]);
//
//	double d = GetCenterToEye();
//
//	switch (key) 
//	{
//	case 'w':
//		// ǰ
//		eye[0] += m_Step*forwardVector[0];
//		center[0] += m_Step*forwardVector[0];
//		eye[1] += m_Step*forwardVector[1];
//		center[1] += m_Step*forwardVector[1];
//		break;
//
//	case 's':
//		// ��
//		eye[0] -= m_Step*forwardVector[0];
//		center[0] -= m_Step*forwardVector[0];
//		eye[1] -= m_Step*forwardVector[1];
//		center[1] -= m_Step*forwardVector[1];
//		break;
//
//	case 'a':
//		// ��
//		eye[0] -= m_Step*rightVector[0];
//		center[0] -= m_Step*rightVector[0];
//		eye[1] -= m_Step*rightVector[1];
//		center[1] -= m_Step*rightVector[1];
//		break;
//
//	case 'd':
//		// ��
//		eye[0] += m_Step*rightVector[0];
//		center[0] += m_Step*rightVector[0];
//		eye[1] += m_Step*rightVector[1];
//		center[1] += m_Step*rightVector[1];
//		break;
//	case 'q':
//		// ��
//		eye[0] -= m_Step*up[0];
//		center[0] -= m_Step*up[0];
//		eye[1] -= m_Step*up[1];
//		center[1] -= m_Step*up[1];
//		eye[2] -= m_Step*up[2];
//		center[2] -= m_Step*up[2];
//		break;
//	case 'e':
//		// ��
//		eye[0] += m_Step*up[0];
//		center[0] += m_Step*up[0];
//		eye[1] += m_Step*up[1];
//		center[1] += m_Step*up[1];
//		eye[2] += m_Step*up[2];
//		center[2] += m_Step*up[2];
//		break;
//	case 'i':
//		rad_ver += r_Step*3.1415926535/180;
//		center[0]= eye[0] + d*cos(rad_ver)*cos(rad_hor);
//		center[1]= eye[1] + d*cos(rad_ver)*sin(rad_hor);
//		center[2]= eye[2] + d*sin(rad_ver);
//		break;
//	case 'k':
//		rad_ver -= r_Step*3.1415926535/180;
//		center[0]= eye[0] + d*cos(rad_ver)*cos(rad_hor);
//		center[1]= eye[1] + d*cos(rad_ver)*sin(rad_hor);
//		center[2]= eye[2] + d*sin(rad_ver);
//		break;
//	case 'j':
//		rad_hor += r_Step*3.1415926535/180;
//		center[0]= eye[0] + d*cos(rad_ver)*cos(rad_hor);
//		center[1]= eye[1] + d*cos(rad_ver)*sin(rad_hor);
//		center[2]= eye[2] + d*sin(rad_ver);
//		break;
//	case 'l':
//		rad_hor -= r_Step*3.1415926535/180;
//		center[0]= eye[0] + d*cos(rad_ver)*cos(rad_hor);
//		center[1]= eye[1] + d*cos(rad_ver)*sin(rad_hor);
//		center[2]= eye[2] + d*sin(rad_ver);
//		break;
//	}
//}
//// ����Ϣ��ķ�ʽ��Ӧ������Ϣ
//void CSCOpenGL::SCKeyBoardMarco(DWORD inc)
//{
//	GetForwardVector(forwardVector[0],forwardVector[1],forwardVector[2]);
//	GetRightVector(rightVector[0],rightVector[1],rightVector[2]);
//
//	double d = GetCenterToEye();
//	float Step = float(inc)*0.01;
//
//	// W ǰ
//	if (KEY_DOWN(87))
//	{
//		
//	}
//	// S ��
//	if (KEY_DOWN(83))
//	{
//		
//	}
//	// A ��
//	if (KEY_DOWN(65))
//	{
//		
//	}
//	// D ��
//	if (KEY_DOWN(68))
//	{
//		
//	}
//	// Q ��
//	if (KEY_DOWN(81))
//	{
//		eye[0] -= Step*up[0];
//		center[0] -= Step*up[0];
//		eye[1] -= Step*up[1];
//		center[1] -= Step*up[1];
//		eye[2] -= Step*up[2];
//		center[2] -= Step*up[2];
//	}
//	// E ��
//	if (KEY_DOWN(69))
//	{
//		eye[0] += Step*up[0];
//		center[0] += Step*up[0];
//		eye[1] += Step*up[1];
//		center[1] += Step*up[1];
//		eye[2] += Step*up[2];
//		center[2] += Step*up[2];
//	}
//	// I ����
//	if (KEY_DOWN(73))
//	{
//		
//	}
//	// K �¸�
//	if (KEY_DOWN(75))
//	{
//		
//	}
//	// J ��ת
//	if (KEY_DOWN(74))
//	{
//		
//	}
//	// L ��ת
//	if (KEY_DOWN(76))
//	{
//		
//	}
//
//	if (KEY_DOWN(82))
//	{
//		// ��ʼ����ת�Ƕ�
//		rad_hor = 0;
//		rad_ver = 0;
//		// ��ʼ���۲�λ��9������
//		eye[0] = -5.0;
//		eye[1] =  0.0;
//		eye[2] =  2.0;
//		center[0] = 0.0;
//		center[1] = 0.0;
//		center[2] = 2.0;
//		up[0] = 0.0;
//		up[1] = 0.0;
//		up[2] = 1.0;
//	}
//}

void CSCOpenGL::DetectKeyBoardState()
{
	// �����صļ��̴���
	//               �س�  ESC  ��   ��  �ո�
	char tKcode[] = { 13 , 27 , 40 , 38 , 32};
	// ȡ���鳤��
	short l = sizeof(tKcode);
	for (short i = 0; i < l; i++)
	{
		if( KEY_DOWN( tKcode[i] ) )
		{
			if( 0==KeyState[tKcode[i]] )	KeyState[tKcode[i]] = PRESSED;
		}
		else
		{
			if( KeyState[tKcode[i]] && (0==(PRESSED_UP&KeyState[tKcode[i]])) )
			{  KeyState[tKcode[i]] = PRESSED_UP;  }
			else	KeyState[tKcode[i]] = 0;
		}
	}
}
// ֡��ͬ��,������������
void CSCOpenGL::UpdateSCdata( DWORD millionSecond )
{
	DrawMainData(millionSecond);
	DrawAssitData(millionSecond);

	// ������Ϣ�귽ʽ����
	//SCKeyBoardMarco(millionSecond);
}
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ����ͼ����
void CSCOpenGL::RenderScene()
{
	//glutSetWindow(mainWindow);  

	// �����ɫ��������Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	// ���ص�λ�󣬳�ʼ���̶��ӽ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt( 0, 0, 19, 0, 0, 0, 0, 1, 0);

	// ������Ϣ�귽ʽ����
	//SCKeyBoardMarco(20);

	DetectKeyBoardState();

	//////////////////////////////////////////////////////////////////////////

	NowTimeTicket = GetTickCount();
	UpdateSCdata(NowTimeTicket - LastTimeTicket);
	LastTimeTicket = NowTimeTicket;

	//////////////////////////////////////////////////////////////////////////

	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
	
}

#ifdef USER_NUM2
// �ڶ����Ʒ���
void CSCOpenGL::RenderYourScene()
{
	//glutSetWindow(mainWindow);  

	// �����ɫ��������Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ��Ԥ��ֵ���������
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	// ���ص�λ�󣬳�ʼ���̶��ӽ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt( eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	// ������Ϣ�귽ʽ����
	SCKeyBoardMarco();

	//////////////////////////////////////////////////////////////////////////

	DrawTestCube(0,0,0);

	DrawYourData();

	DrawGrid(1,0);

	glColor3f(1,1,1);
	DrawLetter( 0, 0, 3, "Hello Everyone");

	//////////////////////////////////////////////////////////////////////////

	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
}
#endif

#ifdef USER_NUM3
// �������Ʒ���
void CSCOpenGL::RenderTheeScene()
{
	//glutSetWindow(mainWindow);  

	// �����ɫ��������Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ��Ԥ��ֵ���������
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	// ���ص�λ�󣬳�ʼ���̶��ӽ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt( eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	// ������Ϣ�귽ʽ����
	SCKeyBoardMarco();

	//////////////////////////////////////////////////////////////////////////

	DrawTestCube(0,0,0);

	DrawTheeData();

	DrawGrid(1,0);

	glColor3f(1,1,1);
	DrawLetter( 0, 0, 3, "Hello Everyone");

	//////////////////////////////////////////////////////////////////////////

	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
}
#endif
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// �����ַ�������

// �ַ�
void CSCOpenGL::DrawLetter( float x, float y, float z, char* string )
{
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glRasterPos3f(x,y,z);

	char* p = NULL;
	for (p = string; *p; p++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);

	glPopMatrix();
}
// ����
void CSCOpenGL::DrawLetter( float x, float y, float z, int string )
{
	glDisable(GL_TEXTURE_2D);
	char str[12];

	itoa(string , str , 10);

	glPushMatrix();
	glRasterPos3f(x,y,z);

	char* p = NULL;
	for (p = str; *p; p++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);

	glPopMatrix();
}
// ����
void CSCOpenGL::DrawLetter( float x, float y, float z, float string )
{
	glDisable(GL_TEXTURE_2D);
	char str[10];

	//sprintf(str, "%f" , string);
	sprintf_s(str, "%f" , string);

	glPushMatrix();
	glRasterPos3f(x,y,z);

	char* p = NULL;
	for (p = str; *p; p++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);

	glPopMatrix();
}
// ˫����
void CSCOpenGL::DrawLetter( float x, float y, float z, double string )
{
	glDisable(GL_TEXTURE_2D);
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





////////////////////////////////////////////////////////////////////////////
//// ����������ͼ��
//void CSCOpenGL::DrawTestCube( float x, float y, float z )
//{
//	// EXAMPLE OPENGL CODE START /////////////////////////////////////
//	//////////////////////////////////////////////////////////////////
//	static GLfloat xrot; // X Rotation 
//	static GLfloat yrot; // Y Rotation
//	static GLfloat zrot; // Z Rotation 
//
//	glPushMatrix(); // Push Matrix Onto Stack (Copy The Current Matrix)
//	//glLoadIdentity(); // Reset The Current Modelview Matrix
//	glTranslatef(x,y,z); // Move Left 1.5 Units And Into The Screen 6.0
//
//	glRotatef(xrot,1.0f,0.0f,0.0f);
//	glRotatef(yrot,0.0f,1.0f,0.0f);
//	glRotatef(zrot,0.0f,0.0f,1.0f);
//
//	glBegin(GL_QUADS);
//	// Front Face
//	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, 1.0f);
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, 1.0f);
//	glColor3f(0.0f,0.0f,1.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
//	// Back Face
//	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, -1.0f);
//	glColor3f(0.0f,0.0f,1.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
//	// Top Face
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
//	glColor3f(0.0f,0.0f,1.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, 1.0f);
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, -1.0f);
//	// Bottom Face
//	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
//	glColor3f(0.0f,0.0f,1.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, 1.0f);
//	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
//	// Right face
//	glColor3f(0.0f,0.0f,1.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, -1.0f);
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, 1.0f, 1.0f);
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f( 1.0f, -1.0f, 1.0f);
//	// Left Face
//	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
//	glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
//	glColor3f(0.0f,0.0f,1.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
//	glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
//
//	glEnd();
//	glPopMatrix(); // Pop Matrix Off The Stack
//
//	xrot+=0.03f;
//	yrot+=0.02f;
//	zrot+=0.04f; // Decrease The Rotation Variable For The Quad
//	glColor3f(1.0f,1.0f,1.0f);
//	//////////////////////////////////////////////////////////////////////
//	// EXAMPLE OPENGL CODE END ///////////////////////////////////////////
//}


//////////////////////////////////////////////////////////////////////////
// �����ο�����
void CSCOpenGL::DrawGrid(GLfloat unit, int degree)
{
	glDisable(GL_TEXTURE_2D);

	//����ѹջ
	glPushMatrix();

	glColor3f(0.5f,0.7f,1.0f);

	const int scale = 50;

	if (unit < 0.001)
	{
		unit = 1;
	}

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

	glColor3f(1.0f, 1.0f, 1.0f);

}

//////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////
//// ��ȡ���ֲ���
//
//// �õ��ӵ�λ��
//void CSCOpenGL::GetEyePos( float & x, float & y, float & z )
//{
//	x = eye[0];
//	y = eye[1];
//	z = eye[2];
//}
//// �õ�����λ��
//void CSCOpenGL::GetCenterPos( float & x, float & y, float & z )
//{
//	x = center[0];
//	y = center[1];
//	z = center[2];
//}
//// �õ�������
//void CSCOpenGL::GetUpVector( float & x, float & y, float & z )
//{
//	double d = sqrt(up[0]*up[0]+up[1]*up[1]+up[2]*up[2]);
//	x = up[0]/d;
//	y = up[1]/d;
//	z = up[2]/d;
//}
//// �õ�ǰ������
//void CSCOpenGL::GetForwardVector( float & x, float & y, float & z )
//{
//	float d = GetCenterToEye();
//	x = (center[0]-eye[0])/d;
//	y = (center[1]-eye[1])/d;
//	z = (center[2]-eye[2])/d;
//}
//// �õ�������
//void CSCOpenGL::GetRightVector( float &x, float &y, float &z )
//{
//	float fx,fy,fz,ux,uy,uz;
//	GetForwardVector(fx,fy,fz);
//	GetUpVector(ux,uy,uz);
//	x = fy*uz-fz*uy; // ��ǰ�Ҳ������
//	y = fz*ux-fx*uz;
//	z = fx*uy-fy*ux;
//}
//// �õ��������ӵ�ľ���
//float CSCOpenGL::GetCenterToEye( void )
//{
//	float d = sqrt((center[0]-eye[0])*(center[0]-eye[0])+(center[1]-eye[1])*(center[1]-eye[1])+(center[2]-eye[2])*(center[2]-eye[2]));
//	return d;
//}
//// �õ����������ӵ�ľ���
//float CSCOpenGL::GetPointToEye( float x, float y, float z )
//{
//	float d = sqrt((eye[0] - x)*(eye[0] - x)+(eye[1] - y)*(eye[1] - y)+(eye[2] - z)*(eye[2] - z));
//	return d;
//}

//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
// �����Զ�������
void CSCOpenGL::DrawMainData(DWORD inc)
{
	switch(g_GameState)
	{
	case 0: // ����
		break;

	case 1: // �����˵�
		myGameMenu.DrawMenuPPW(inc);
		break;

	case 2: // ��Ϸ����
		myGameCore.Gaming(inc);
		break;

	case 3: // �ɾ�
		myGameMenu.DrawAchievement(inc);
		break;

	case 4: // ����
		myGameMenu.DrawAbout(inc);
		break;

	//	break;
	}
}
void CSCOpenGL::DrawAssitData(DWORD inc)
{

	//// ������ͼ������ͼ�Ĳ������������
	//glEnable(GL_TEXTURE_2D);

	//glMatrixMode(GL_PROJECTION);	

	//glPushMatrix();								
	//glLoadIdentity();						
	//glOrtho(0,800,0,600,-1,1);				// ����Ϊ��ͶӰ
	//glMatrixMode(GL_MODELVIEW);						

	//glColor3f(1,1,1);
	//g_myModel.glPrint(400, 300,"asdfasdf");

	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);

	/*glDisable(GL_TEXTURE_2D);
	DrawGrid(1,1);*/

	/*glColor3f(1,1,1);
	DrawLetter( -6.5, -6.5, 0, g_MenuStateMain);
	DrawLetter( -5.5, -6.5, 0, g_MenuStateSub);*/
}
#ifdef USER_NUM2
void CSCOpenGL::DrawYourData()
{
	DrawLetter(0,0,2,"This is Your data!!!");
}
#endif

#ifdef USER_NUM3
void CSCOpenGL::DrawTheeData()
{
	DrawLetter(0,0,2,"This is Thee data!!!");
}
#endif
