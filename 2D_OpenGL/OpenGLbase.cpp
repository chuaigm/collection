//
// OpenGL基础类，opengl对外接口与封装
//====================================================================
#include "stdafx.h"
#include "OpenGLbase.h"
#include "myclock.h"
#include "gameMain.h"

//extern HWND	hWnd;

extern MYCLOCK c1;
// 为了能让自定义的程序可以绘制，这里需要声明外部的绘制主体
extern gameMain gm;
// 定义在外部的
extern CGLFont myfont;
// 在外部定义的窗体的高度与宽度
//extern int WinWidth;
//extern int WinHeight;

//////////////////////////////////////////////////////////////////////
COpenGLbase::COpenGLbase()
{
	hDC = NULL;
	hRC = NULL;
	RCwidth = 0;
	RCheight = 0;

	Xmouse = 0;
	Ymouse = 0;
	XmouseOld = 0;
	YmouseOld = 0;
}

COpenGLbase::~COpenGLbase()
{
	CleanUp();
}

//设置像素格式，创建RC，选择RC
BOOL COpenGLbase::SetupPixelFormat(HDC hDC0)
{
	int nPixelFormat;
	// 获得设备上下文，从窗口句柄获取的
	hDC=hDC0;
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),	// 象素格式描述子结构的大小
		1,							// 是PIXELFORMATDESCRIPTOR结构的版本，一般设为1
	    PFD_DRAW_TO_WINDOW | 
	    PFD_SUPPORT_OPENGL | 
	    PFD_DOUBLEBUFFER,			// 是一组表明象素缓冲特性的标志位,如缓冲是否支持GDI或OpenGL等
	    PFD_TYPE_RGBA,				// 说明象素数据类型是RGBA还是颜色索引
	    24,							// 每个颜色缓冲区中颜色位平面的数目，对颜色索引方式是缓冲区大小
	    0,							// 每个RGBA颜色缓冲区中红色位平面的数目
		0,							// 每个RGBA颜色缓冲区中红色位平面的偏移数
		0,							// 每个RGBA颜色缓冲区中绿色位平面的数目
		0,							// 每个RGBA颜色缓冲区中绿色位平面的偏移数
		0,							// 每个RGBA颜色缓冲区中蓝色位平面的数目
		0,							// 每个RGBA颜色缓冲区中蓝色位平面的偏移数
	    0,							// 每个RGBA颜色缓冲区中alpha位平面的数目(保留的，现不支持)
	    0,							// 每个RGBA颜色缓冲区中alpha位平面的偏移数(保留的，现不支持)
	    0,							// 累加缓冲区中全部位平面的数目
		0,							// 累加缓冲区中红色位平面的数目
		0,							// 累加缓冲区中绿色位平面的数目
		0,							// 累加缓冲区中蓝色位平面的数目
		0,							// 累加缓冲区中alpha位平面的数目
	    16,							// Z(深度)缓冲区的深度
	    0, 							// 模板缓冲区的深度
	    0,							// 轴向缓冲区的数量(一般1.0版本不支持)
	    PFD_MAIN_PLANE,				// 被忽略，为了一致性而包含的
	    0, 							// 表层和底层平面的数量::位0-3表最多15层表层平面，位4-7表底层
	    0,							// 被忽略，为了一致性而包含的
		0,							// 是透明色彩的值(RGBA方式)或是一个底层平面的索引(Index)
		0 							// 被忽略，为了一致性而包含的
	}; 
	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		// MB_ICONEXCLAMATION : 对话框上有个感叹号
		MessageBox(NULL,"没找到合适的显示模式","Error",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (!SetPixelFormat(hDC,nPixelFormat,&pfd))
	{
		MessageBox(NULL,"不能设置像素格式","Error",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	// 创建一个新的OpenGL渲染描述表,此描述表必须适用于绘制到由hdc返回的设备.
	// 这个渲染描述表将有和设备上下文(dc)一样的像素格式
	hRC = wglCreateContext(hDC);
	// 使一个指定的OpenGL渲染上下文调用线程的当前呈现上下文
	wglMakeCurrent(hDC, hRC);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	//多边形填充方式
	glShadeModel(GL_SMOOTH); //or GL_FLAT

	//混合
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	//	  glEnable(GL_SCISSOR_TEST);
	//	  glDisable(GL_DEPTH_TEST);
	//glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	//glPolygonOffset(0.01f,0.002f);
	//glPolygonMode(GL_FRONT,GL_FILL);
	
	return TRUE;
}

//建立视口
//设置映射方式
void COpenGLbase::init_3D()
{
	glViewport(0,0,RCwidth,RCheight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective	
		( 54.0f,
		  (GLfloat)RCwidth/(GLfloat)RCheight,
		  0.1f,	
		  3000.0f
		);
	
	//指定模型视图堆栈
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();

	//启动颜色材质
	glEnable(GL_COLOR_MATERIAL);
	//初始化灯光
	//gm.light0();

//====================================================
}
// 
void COpenGLbase::init_2D()
{
	// glViewport(x, y, width, height)
	// x, y 以像素为单位，指定了视口的左下角
	// 表示这个视口矩形的宽度和高度，根据窗口的实时变化重绘窗口
	glViewport(0, 0, RCwidth, RCheight);

	// 设置当前矩阵模式，投影矩阵操作
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// 平行修剪空间
	//            _____.(right,top,far)
	//          /|_____|
	//         /_/___/ /
	//        .|/____|/
	// (left,bottom,near)  
	glOrtho (0.0, (GLdouble)RCwidth, 0.0, (GLdouble)RCheight, -1.0, 1.0);

	// 设置模型矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//启动颜色材质
	//glEnable(GL_COLOR_MATERIAL);
}

void COpenGLbase::SetViewSize(int w,int h)
{
	RCwidth=w;
	RCheight=h;
}

//渲染输出
void COpenGLbase::Render()
{
	// 默认背景色(值为0~1之间)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// 深度缓存区 [0, 1]，效果与glDepthFunc()函数有关
	glClearDepth(1.0f);
	// 清除颜色缓存与深度缓存
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	// 输出GL本类的一些参数
	text_OpenGL_log();
	// 游戏主体的绘制
	gm.show();

	glFlush();		
	SwapBuffers(hDC);
}

void COpenGLbase::CleanUp()
{ 
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
}

void COpenGLbase::text_OpenGL_log()
{
	//char str[128];
	//sprintf(str, "C %d/%d ",c1.iNumDest, c1.expire_time);
	//myfont.Print2D(0.1,0.1,str,FONT1,0.0f,0.0f,0.0f);
	//myfont.settext(WinWidth/2,WinHeight*7/8,"111111",FONT0,1.0,1.0,1.0);

	// Y /-------------\
	//   |600          |(800,600)
	//   |             |
	//   |0         800|
	//   \-------------/ X

	myfont.Print2D(50,2,"0,0",FONT0,1.0f,1.0f,1.0f);
	myfont.Print2D(100,2,"1,0",FONT1,1.0f,0.0f,0.0f);
	myfont.Print2D(200,2,"2,0",FONT2,1.0f,1.0f,0.0f);
	myfont.Print2D(300,2,"3,0",FONT3,1.0f,0.0f,1.0f);
	myfont.Print2D(400,2,"4,0",FONT4,0.0f,0.0f,1.0f);
	myfont.Print2D(500,2,"5,0",FONT5,0.0f,1.0f,0.0f);
	//myfont.Print2D(600,2,"6,0",FONT6,1.0f,1.0f,1.0f);
	
	// 显示鼠标坐标值
	char str[64];
	sprintf(str, "mx=%4d",Xmouse);
	myfont.Print2D(1,8,str,FONT0,1.0f,1.0f,1.0f);
	sprintf(str, "mx=%4d",Ymouse);
	myfont.Print2D(1,16,str,FONT0,1.0f,1.0f,1.0f);
}

void COpenGLbase::mouseProc(long long lparam)
{
	// 在win下转换进来的鼠标坐标系
	//
	//   /-------------\ X
	//   |(0,0)        | 800
	//   |             |
	//   | 600         |
	// Y \-------------/(800,600)  

	XmouseOld=Xmouse;
	YmouseOld=Ymouse;
	Xmouse=LOWORD(lparam);
	Ymouse=HIWORD(lparam);
}


