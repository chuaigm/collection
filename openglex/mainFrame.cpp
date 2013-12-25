//
// 2013-12-6
// 修改自网上流行的cs1的opengl的实例程序
//

#include "stdafx.h"

#include "myclock.h"
#include "OpenGLbase.h"
#include "gamemap.h"
#include "glfont.h"

#include "filereport.h"

//时钟
MYCLOCK c1;

COpenGLbase * m_OpenGL;
CGLFont myfont;

HDC		hDC;
HWND	hWnd=NULL;
//窗口宽高
int WinWidth = WIN_WIDTH;
int WinHeight= WIN_HEIGHT;

GAMEMAP gm;

//左键是否按下
int Lbutdown=0;

void GameLoop()
{
	MSG msg; 
    BOOL fMessage;

    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
    while(msg.message != WM_QUIT)
    {   
		fMessage = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

        if(fMessage)
		{
			TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
        else  
		{
			if(c1.clockcount())
			{
				m_OpenGL->Render();				
				gm.check();
			}
			else
			{
				_sleep(0);
			}
		}
    }
}

LRESULT WINAPI MsgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam )
{
	switch(message)
	{
		case WM_CREATE:	
			//读取DC
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
			WinHeight = HIWORD(lParam);
			WinWidth  = LOWORD(lParam);
			if (WinHeight==0)	
				WinHeight=1;
			//设置视口和映射方式
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
			case 0x70:	// F1
				ShowWindow(hWnd, SW_MINIMIZE);	
				break;		

			default:
				gm.keyupproc(wParam);
				break;
            } 
			//注意break
			break;

		case WM_LBUTTONDOWN: 
			Lbutdown=1;
			break;

		case WM_MOUSEMOVE:
			gm.mouseproc(lParam);
			break;
		
		case WM_LBUTTONUP:
			Lbutdown=0;
			gm.lbuttonproc(lParam);
			break;

		default:			
			break;
	}

	return (DefWindowProc(hWnd, message, wParam, lParam));
}

// 主函数main
int APIENTRY WinMain(HINSTANCE hInst,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	//--------------------------------------------------
	//begin 文件检查
	char errstr[]="缺少数据文件，请修复后重启游戏";
	if(!gm.haveDataFile())
	{
		MessageBox(NULL,errstr, "CS",MB_OK);
		return 0;
	}
	//end 文件检查

	//--------------------------------------------------
	char cc[]="chuaiOpenGL";
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), 
		CS_CLASSDC,
		MsgProc,
		0L, 0L, 
		GetModuleHandle(NULL), 
		LoadIcon(hInst, (LPCTSTR)IDI_ICON1),
		LoadCursor(NULL, IDC_ARROW),
		NULL, NULL,
        cc, 
		LoadIcon(hInst, (LPCTSTR)IDI_ICON3)
	};
	RegisterClassEx( &wc );
	//--------------------------------------------------
	m_OpenGL=new COpenGLbase();

	//--------------------------------------------------
	// 窗体风格
	//dwExStyle=WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;
	//dwStyle=WS_POPUP;
	//DWORD dwStyle=WS_OVERLAPPEDWINDOW;
	DWORD dwStyle=WS_DLGFRAME;

	// 得到当前桌面分辨率
	int wid=GetSystemMetrics(SM_CXSCREEN);
	int hei=GetSystemMetrics(SM_CYSCREEN);
	// 窗口左上角坐标
	int nX=(wid-WinWidth)/2;
	int nY=(hei-WinHeight)/2;	

	hWnd = CreateWindowEx(NULL,
		cc,
		"chuai_OpenGL_Sample",
		dwStyle,
		nX, nY,
		WinWidth, 
		WinHeight,
		NULL,NULL,
		hInst,NULL);

// 显示方式
	//ShowWindow( hWnd, SW_SHOWDEFAULT );	//默认大小
	//ShowWindow( hWnd, SW_MAXIMIZE );	//最大化显示
	ShowWindow( hWnd, SW_SHOWNORMAL );	// 正常
	UpdateWindow( hWnd );	

	// init开屏动画计时
	c1.ReStart(TIME_PRE_ANI);

	GameLoop();	
    
	return 0;
}



