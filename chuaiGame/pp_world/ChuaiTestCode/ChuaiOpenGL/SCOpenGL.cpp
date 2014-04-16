
//#include "stdafx.h"
#include "SCOpenGL.h"


CSCOpenGL::CSCOpenGL(void)
{
	// 初始化窗口大小
	m_Width = 800;
	m_Height = 600;

	g_Width = 800;
	g_Height = 600;
	//// 初始化旋转角度
	//rad_hor = 0;
	//rad_ver = 0;
	//// 初始化观察位置9个变量
	//eye[0] =  0.0;
	//eye[1] =  0.0;
	//eye[2] =  14.0;

	//center[0] = 0.0;
	//center[1] = 0.0;
	//center[2] = 0.0;

	//up[0] = 0.0;
	//up[1] = 1.0;
	//up[2] = 0.0;
	// 初始化时间戳

	g_GameState = 1;

	NowTimeTicket = GetTickCount();
	LastTimeTicket = NowTimeTicket;

}

CSCOpenGL::~CSCOpenGL(void)
{
}


//////////////////////////////////////////////////////////////////////////
// 非线程模式主函数
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
// 非线程模式主函数(2),为第二种绘制方案
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
// 非线程模式主函数(3)，为第三种绘制方案
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

//// 线程模式主函数，线程函数
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
//// 启动线程的函数
//void CSCOpenGL::threadDraw()
//{
//	chBEGINTHREADEX( NULL, 0, ThreadMainScOpenGL, this, 0, NULL );
//}
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// OpenGL初始化函数
void CSCOpenGL::InitSCOpenGL()
{
	glutInitDisplayMode(GLUT_DEPTH  | GLUT_DOUBLE | GLUT_RGBA);
	// 初始化窗口大小
	glutInitWindowSize(m_Width,m_Height);

	// 得到屏幕分辨率
	int   nFullWidth = GetSystemMetrics(SM_CXSCREEN); 
	int   nFullHeight = GetSystemMetrics(SM_CYSCREEN);
	// 初始化窗口位置
	glutInitWindowPosition((nFullWidth-m_Width)/2,(nFullHeight-m_Height)/2);
	// 创建窗口
	mainWindow = glutCreateWindow("Point-Point_World_1.0-------Created By Chuai!!!");
	// 新glut的功能，防止内存泄露，无法结束程序
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//多边形填充方式
	glShadeModel(GL_SMOOTH); //or GL_FLAT
	//混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glAlphaFunc(GL_GREATER,0.1f);											// 设 Alpha 测试
	glEnable(GL_ALPHA_TEST);												// 打开 Alpha 测试

	glEnable(GL_NORMALIZE);

	//不画多边形的反面
	//	glFrontFace(GL_CCW);
	//	glEnable(GL_CULL_FACE);
	//	glCullFace(GL_BACK);

	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	/////////////////////////////////////////////////////////反走样
	//glEnable(GL_SCISSOR_TEST);
	//	  glDisable(GL_DEPTH_TEST);
	// glEnable(GL_LINE_SMOOTH);
	// glEnable(GL_POLYGON_SMOOTH);
	//glPolygonOffset(0.01f,0.002f);
	::glPolygonMode(GL_FRONT,GL_FILL);

	float ratio;
	// 防止被零除
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
	glLoadIdentity();							// 重置模型观察矩阵
	// 用预设值清楚缓冲区
	glClearColor(0.0f,0.0f,0.0f,0.0f);
}

// 窗体尺寸改变函数
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
	glLoadIdentity();							// 重置模型观察矩阵

	g_Width = m_Width;
	g_Height = m_Height;

}
//// 鼠标消息
//void CSCOpenGL::SCMouseMove( int button, int state, int x, int y )
//{
//	/*if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
//	{
//
//	}*/
//
//}
//// 键盘消息
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
//		// 前
//		eye[0] += m_Step*forwardVector[0];
//		center[0] += m_Step*forwardVector[0];
//		eye[1] += m_Step*forwardVector[1];
//		center[1] += m_Step*forwardVector[1];
//		break;
//
//	case 's':
//		// 后
//		eye[0] -= m_Step*forwardVector[0];
//		center[0] -= m_Step*forwardVector[0];
//		eye[1] -= m_Step*forwardVector[1];
//		center[1] -= m_Step*forwardVector[1];
//		break;
//
//	case 'a':
//		// 左
//		eye[0] -= m_Step*rightVector[0];
//		center[0] -= m_Step*rightVector[0];
//		eye[1] -= m_Step*rightVector[1];
//		center[1] -= m_Step*rightVector[1];
//		break;
//
//	case 'd':
//		// 右
//		eye[0] += m_Step*rightVector[0];
//		center[0] += m_Step*rightVector[0];
//		eye[1] += m_Step*rightVector[1];
//		center[1] += m_Step*rightVector[1];
//		break;
//	case 'q':
//		// 下
//		eye[0] -= m_Step*up[0];
//		center[0] -= m_Step*up[0];
//		eye[1] -= m_Step*up[1];
//		center[1] -= m_Step*up[1];
//		eye[2] -= m_Step*up[2];
//		center[2] -= m_Step*up[2];
//		break;
//	case 'e':
//		// 上
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
//// 用消息宏的方式相应键盘消息
//void CSCOpenGL::SCKeyBoardMarco(DWORD inc)
//{
//	GetForwardVector(forwardVector[0],forwardVector[1],forwardVector[2]);
//	GetRightVector(rightVector[0],rightVector[1],rightVector[2]);
//
//	double d = GetCenterToEye();
//	float Step = float(inc)*0.01;
//
//	// W 前
//	if (KEY_DOWN(87))
//	{
//		
//	}
//	// S 后
//	if (KEY_DOWN(83))
//	{
//		
//	}
//	// A 左
//	if (KEY_DOWN(65))
//	{
//		
//	}
//	// D 右
//	if (KEY_DOWN(68))
//	{
//		
//	}
//	// Q 下
//	if (KEY_DOWN(81))
//	{
//		eye[0] -= Step*up[0];
//		center[0] -= Step*up[0];
//		eye[1] -= Step*up[1];
//		center[1] -= Step*up[1];
//		eye[2] -= Step*up[2];
//		center[2] -= Step*up[2];
//	}
//	// E 上
//	if (KEY_DOWN(69))
//	{
//		eye[0] += Step*up[0];
//		center[0] += Step*up[0];
//		eye[1] += Step*up[1];
//		center[1] += Step*up[1];
//		eye[2] += Step*up[2];
//		center[2] += Step*up[2];
//	}
//	// I 上仰
//	if (KEY_DOWN(73))
//	{
//		
//	}
//	// K 下俯
//	if (KEY_DOWN(75))
//	{
//		
//	}
//	// J 左转
//	if (KEY_DOWN(74))
//	{
//		
//	}
//	// L 右转
//	if (KEY_DOWN(76))
//	{
//		
//	}
//
//	if (KEY_DOWN(82))
//	{
//		// 初始化旋转角度
//		rad_hor = 0;
//		rad_ver = 0;
//		// 初始化观察位置9个变量
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
	// 所需监控的键盘代码
	//               回车  ESC  下   上  空格
	char tKcode[] = { 13 , 27 , 40 , 38 , 32};
	// 取数组长度
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
// 帧率同步,加入增量控制
void CSCOpenGL::UpdateSCdata( DWORD millionSecond )
{
	DrawMainData(millionSecond);
	DrawAssitData(millionSecond);

	// 键盘消息宏方式控制
	//SCKeyBoardMarco(millionSecond);
}
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// 主绘图函数
void CSCOpenGL::RenderScene()
{
	//glutSetWindow(mainWindow);  

	// 清楚颜色缓存与深度缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	// 加载单位阵，初始化固定视角
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt( 0, 0, 19, 0, 0, 0, 0, 1, 0);

	// 键盘消息宏方式控制
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
// 第二绘制方案
void CSCOpenGL::RenderYourScene()
{
	//glutSetWindow(mainWindow);  

	// 清楚颜色缓存与深度缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 用预设值清楚缓冲区
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	// 加载单位阵，初始化固定视角
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt( eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	// 键盘消息宏方式控制
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
// 第三绘制方案
void CSCOpenGL::RenderTheeScene()
{
	//glutSetWindow(mainWindow);  

	// 清楚颜色缓存与深度缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 用预设值清楚缓冲区
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	// 加载单位阵，初始化固定视角
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt( eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	// 键盘消息宏方式控制
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
// 画出字符与数字

// 字符
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
// 整形
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
// 浮点
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
// 双精度
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
//// 画出测试用图形
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
// 画出参考地面
void CSCOpenGL::DrawGrid(GLfloat unit, int degree)
{
	glDisable(GL_TEXTURE_2D);

	//矩阵压栈
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

	//矩阵弹出堆栈
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);

}

//////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////
//// 获取各种参数
//
//// 得到视点位置
//void CSCOpenGL::GetEyePos( float & x, float & y, float & z )
//{
//	x = eye[0];
//	y = eye[1];
//	z = eye[2];
//}
//// 得到焦点位置
//void CSCOpenGL::GetCenterPos( float & x, float & y, float & z )
//{
//	x = center[0];
//	y = center[1];
//	z = center[2];
//}
//// 得到上向量
//void CSCOpenGL::GetUpVector( float & x, float & y, float & z )
//{
//	double d = sqrt(up[0]*up[0]+up[1]*up[1]+up[2]*up[2]);
//	x = up[0]/d;
//	y = up[1]/d;
//	z = up[2]/d;
//}
//// 得到前向向量
//void CSCOpenGL::GetForwardVector( float & x, float & y, float & z )
//{
//	float d = GetCenterToEye();
//	x = (center[0]-eye[0])/d;
//	y = (center[1]-eye[1])/d;
//	z = (center[2]-eye[2])/d;
//}
//// 得到右向量
//void CSCOpenGL::GetRightVector( float &x, float &y, float &z )
//{
//	float fx,fy,fz,ux,uy,uz;
//	GetForwardVector(fx,fy,fz);
//	GetUpVector(ux,uy,uz);
//	x = fy*uz-fz*uy; // 当前右侧的向量
//	y = fz*ux-fx*uz;
//	z = fx*uy-fy*ux;
//}
//// 得到焦点与视点的距离
//float CSCOpenGL::GetCenterToEye( void )
//{
//	float d = sqrt((center[0]-eye[0])*(center[0]-eye[0])+(center[1]-eye[1])*(center[1]-eye[1])+(center[2]-eye[2])*(center[2]-eye[2]));
//	return d;
//}
//// 得到给定点与视点的距离
//float CSCOpenGL::GetPointToEye( float x, float y, float z )
//{
//	float d = sqrt((eye[0] - x)*(eye[0] - x)+(eye[1] - y)*(eye[1] - y)+(eye[2] - z)*(eye[2] - z));
//	return d;
//}

//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
// 画出自定义数据
void CSCOpenGL::DrawMainData(DWORD inc)
{
	switch(g_GameState)
	{
	case 0: // 标题
		break;

	case 1: // 画出菜单
		myGameMenu.DrawMenuPPW(inc);
		break;

	case 2: // 游戏核心
		myGameCore.Gaming(inc);
		break;

	case 3: // 成就
		myGameMenu.DrawAchievement(inc);
		break;

	case 4: // 关于
		myGameMenu.DrawAbout(inc);
		break;

	//	break;
	}
}
void CSCOpenGL::DrawAssitData(DWORD inc)
{

	//// 开启贴图，有贴图的部分在这里绘制
	//glEnable(GL_TEXTURE_2D);

	//glMatrixMode(GL_PROJECTION);	

	//glPushMatrix();								
	//glLoadIdentity();						
	//glOrtho(0,800,0,600,-1,1);				// 设置为正投影
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
