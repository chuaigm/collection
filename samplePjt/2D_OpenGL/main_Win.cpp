// **************************************
// author: chuaiGM
// 建立自己的OpenGL-2D程序框架
// 修改自网上流行的cs1的opengl的实例程序
// 
// 2014-11-23 : 整理2D框架
// **************************************
#include "stdafx.h"

#include "myclock.h"
#include "OpenGLbase.h"
#include "gameMain.h"
#include "glfont.h"
#include "inifileop.h"

//#include "filereport.h"

// 时钟
MYCLOCK c1;
// OpenGl对象
COpenGLbase * m_OpenGL;
// OpenGL字体对象(全局)
CGLFont myfont;
// 设备描述句柄
HDC		hDC;
// 窗口对象
HWND	hWnd=NULL;
//窗口宽高
int WinWidth = WIN_WIDTH;
int WinHeight= WIN_HEIGHT;
// 目前游戏框架中，主要代码(全局)
gameMain gm;

//左键是否按下
int Lbutdown=0;
// 程序主循环
void GameLoop()
{
	MSG msg; 
    BOOL fMessage;
	// 该函数为一个消息检查线程消息队列，并将该消息（如果存在）放于指定的结构
	// lpMsg 接收消息信息的MSG结构指针。
	// hWnd 其消息被检查的窗口句柄。
	// wMsgFilterMin 指定被检查的消息范围里的第一个消息。
	// wMsgFilterMax 指定被检查的消息范围里的最后一个消息。
	// wRemoveMsg 确定消息如何被处理。此参数可取下列值之一
	//			PM_NOREMOVE:处理后，消息不从队列里除掉
	//			PM_REMOVE:处理后，消息从队列里除掉

	// PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
	//while(msg.message != WM_QUIT)

	for (;;)
	{
		// 获取消息
		fMessage = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
		if (msg.message == WM_QUIT)
		{
			break;
		}

        if(fMessage)
		{
			TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
        else  
		{
			if(c1.clockcount())
			{
				gm.check();
				m_OpenGL->Render();
			}
			else
			{
				_sleep(0);
			}
		}
    }
}
// 窗口消息处理函数
LRESULT WINAPI MsgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam )
{
	switch(message)
	{
		case WM_CREATE:
			//读取DC，从窗口句柄获取窗体描述句柄
			hDC = GetDC(hWnd);	
			//设置像素格式，选择RC
			m_OpenGL->SetupPixelFormat(hDC);
			//游戏数据初始化
			gm.init();
			return 0;
			break;

		case WM_CLOSE:
			m_OpenGL->CleanUp();
			PostQuitMessage(0);
			return 0;
			break;

		case WM_SIZE:
			// 这里这个分辨率并不是配置的分辨率，算上了窗体的边框
			WinHeight = HIWORD(lParam);
			WinWidth  = LOWORD(lParam);
			if (WinHeight==0)	
				WinHeight=100;
			// 设置视口和映射方式
			m_OpenGL->SetViewSize(WinWidth,WinHeight);
			gm.initView();					
			return 0;
			break;

		case WM_DESTROY:
			delete m_OpenGL;
            PostQuitMessage(0);
            return 0;
			break;

        case WM_KEYUP:
            switch (wParam)
            {
			case 0x70:	// F1 键
				ShowWindow(hWnd, SW_MINIMIZE);	
				break;
			default:
				gm.keyupproc(wParam);
				break;
            } 
			break;

		case WM_LBUTTONDOWN:
			Lbutdown=1;
			break;

		case WM_MOUSEMOVE:
			//gm.mouseproc(lParam);
			m_OpenGL->mouseProc(lParam);
			break;
		
		case WM_LBUTTONUP:
			Lbutdown=0;
			gm.lbuttonproc(lParam);
			break;

		default:			
			break;
	}
	// 该函数调用缺省的窗口过程来为应用程序没有处理的任何窗口消息提供缺省的处理。
	// 该函数确保每一个消息得到处理
	return (DefWindowProc(hWnd, message, wParam, lParam));
}

// 主函数main，程序的入口
int APIENTRY WinMain(HINSTANCE hInst,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	//--------------------------------------------------
	// 文件检查
	char errstr[]="缺少数据文件，请修复后重启游戏";
	if(!gm.haveDataFile())
	{
		MessageBox(NULL,errstr, "CGM_main",MB_OK);
		return 0;
	}
	//--------------------------------------------------
	// 加载配置文件
	char tmpstr[64];
	if (CFG_OK != ConfigGetKeyValue("config.ini", "Window_Resolution", "width", tmpstr))
	{
		MessageBox(NULL, "无法正常读取配置文件,主进程即将退出", "CGM_main",MB_OK);
		return 0;
	}
	WinWidth=atoi(tmpstr);
	ConfigGetKeyValue("config.ini", "Window_Resolution", "height", tmpstr);
	WinHeight=atoi(tmpstr);
	//--------------------------------------------------
	char cc[]="chuaiOpenGL";
	// 窗体类
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), 
		CS_CLASSDC,								// 窗体类的风格，可以用or操作符连接
		MsgProc,								// 窗口处理函数的指针
		0L, 0L,									// 指定紧跟在窗口类结构后的附加字节数
		GetModuleHandle(NULL),					// 本模块的实例句柄
		LoadIcon(hInst, (LPCTSTR)IDI_ICON1),	// 图标的句柄
		LoadCursor(NULL, IDC_ARROW),			// 光标的句柄，鼠标
		NULL, 									// 背景画刷的句柄
		NULL,									// 指向菜单的指针
        cc,										// 指向类名称的指针
		LoadIcon(hInst, (LPCTSTR)IDI_ICON3)		// 和窗口类关联的小图标。如果该值为NULL。
												// 则把hIcon中的图标转换成大小合适的小图标
	};
	// 注册这个窗口类
	RegisterClassEx( &wc );
	//--------------------------------------------------
	// 声明OpenGL对象
	m_OpenGL=new COpenGLbase();
	//--------------------------------------------------
	// 窗体风格
	//dwExStyle=WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;
	//DWORD dwStyle=WS_POPUP;			// 连窗体都没有，只有主体内容
	//DWORD dwStyle=WS_OVERLAPPEDWINDOW;
	DWORD dwStyle=WS_DLGFRAME;		// 没有右上角三个功能按钮，只有一个窗体

	// 得到当前桌面分辨率
	int wid=GetSystemMetrics(SM_CXSCREEN);
	int hei=GetSystemMetrics(SM_CYSCREEN);
	// 窗口左上角坐标
	int nX=(wid-WinWidth)/2;
	int nY=(hei-WinHeight)/2;	
	// 创建并传递窗口句柄
	hWnd = CreateWindowEx(NULL,		// 窗口的扩展风格
		cc,							// 指向注册类名的指针
		"chuai_OpenGL_Sample",		// 指向窗口名称的指针
		dwStyle,					// 窗口风格
		nX, nY,						// 窗口的起始位置坐标
		WinWidth,					// 窗口的宽度
		WinHeight,					// 窗口的高度
		NULL,						// 父窗口的句柄
		NULL,						// 菜单的句柄或是子窗口的标识符
		hInst,						// 应用程序实例句柄
		NULL						// 指向窗口的创建数据
		);
	//--------------------------------------------------
	// 显示方式
	//ShowWindow( hWnd, SW_SHOWDEFAULT );	//默认大小
	//ShowWindow( hWnd, SW_MAXIMIZE );	//最大化显示
	ShowWindow( hWnd, SW_SHOWNORMAL );	// 正常
	// 更新窗口域
	UpdateWindow( hWnd );	

	// init开屏动画计时
	//c1.ReStart(TIME_PRE_ANI);

	//是否显示鼠标
	//ShowCursor(false);
	// 程序主循环
	GameLoop();	
    
	return 0;
}



