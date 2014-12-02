// **************************************
// author: chuaiGM
// �����Լ���OpenGL-2D������
// �޸����������е�cs1��opengl��ʵ������
// 
// 2014-11-23 : ����2D���
// **************************************
#include "stdafx.h"

#include "myclock.h"
#include "OpenGLbase.h"
#include "Quoridor_openGL.h"
#include "glfont.h"
#include "inifileop.h"

//#include "filereport.h"

// ʱ��
MYCLOCK c1;
// OpenGl����
COpenGLbase * m_OpenGL;
// OpenGL�������(ȫ��)
CGLFont myfont;
// �豸�������
HDC		hDC;
// ���ڶ���
HWND	hWnd=NULL;
//���ڿ���
int WinWidth = WIN_WIDTH;
int WinHeight= WIN_HEIGHT;
// Ŀǰ��Ϸ����У���Ҫ����(ȫ��)
CQuoridor gm;

//����Ƿ���
int Lbutdown=0;
// ������ѭ��
void GameLoop()
{
	MSG msg; 
    BOOL fMessage;
	// �ú���Ϊһ����Ϣ����߳���Ϣ���У���������Ϣ��������ڣ�����ָ���Ľṹ
	// lpMsg ������Ϣ��Ϣ��MSG�ṹָ�롣
	// hWnd ����Ϣ�����Ĵ��ھ����
	// wMsgFilterMin ָ����������Ϣ��Χ��ĵ�һ����Ϣ��
	// wMsgFilterMax ָ����������Ϣ��Χ������һ����Ϣ��
	// wRemoveMsg ȷ����Ϣ��α��������˲�����ȡ����ֵ֮һ
	//			PM_NOREMOVE:��������Ϣ���Ӷ��������
	//			PM_REMOVE:��������Ϣ�Ӷ��������

	// PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
	//while(msg.message != WM_QUIT)

	for (;;)
	{
		// ��ȡ��Ϣ
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
// ������Ϣ��������
LRESULT WINAPI MsgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam )
{
	switch(message)
	{
		case WM_CREATE:
			//��ȡDC���Ӵ��ھ����ȡ�����������
			hDC = GetDC(hWnd);	
			//�������ظ�ʽ��ѡ��RC
			m_OpenGL->SetupPixelFormat(hDC);
			//��Ϸ���ݳ�ʼ��
			gm.init();
			return 0;
			break;

		case WM_CLOSE:
			m_OpenGL->CleanUp();
			PostQuitMessage(0);
			return 0;
			break;

		case WM_SIZE:
			// ��������ֱ��ʲ��������õķֱ��ʣ������˴���ı߿�
			WinHeight = HIWORD(lParam);
			WinWidth  = LOWORD(lParam);
			if (WinHeight==0)	
				WinHeight=100;
			// �����ӿں�ӳ�䷽ʽ
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
			case 0x70:	// F1 ��
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
		case WM_RBUTTONDOWN:
			//Rbutdown=1;
			break;

		case WM_MOUSEMOVE:
			//gm.mouseproc(lParam);
			m_OpenGL->mouseProc(lParam);
			break;
		
		case WM_LBUTTONUP:
			Lbutdown=0;
			gm.lbuttonproc(lParam);
			break;
		case WM_RBUTTONUP:
			//Lbutdown=0;
			gm.rbuttonproc(lParam);
			break;

		default:			
			break;
	}
	// �ú�������ȱʡ�Ĵ��ڹ�����ΪӦ�ó���û�д������κδ�����Ϣ�ṩȱʡ�Ĵ�����
	// �ú���ȷ��ÿһ����Ϣ�õ�����
	return (DefWindowProc(hWnd, message, wParam, lParam));
}

// ������main����������
int APIENTRY WinMain(HINSTANCE hInst,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	//--------------------------------------------------
	// �ļ����
	char errstr[]="ȱ�������ļ������޸���������Ϸ";
	if(!gm.haveDataFile())
	{
		MessageBox(NULL,errstr, "Quoridor_Game",MB_OK);
		return 0;
	}
	//--------------------------------------------------
	// ���������ļ�
	char tmpstr[64];
	if (CFG_OK != ConfigGetKeyValue("config.ini", "Window_Resolution", "width", tmpstr))
	{
		MessageBox(NULL, "�޷�������ȡ�����ļ�,�����̼����˳�", "Quoridor_Game",MB_OK);
		return 0;
	}
	WinWidth=atoi(tmpstr);
	ConfigGetKeyValue("config.ini", "Window_Resolution", "height", tmpstr);
	WinHeight=atoi(tmpstr);
	if (WinWidth<800 || WinHeight<600||WinWidth-WinHeight<100)
	{
		MessageBox(NULL, "���÷ֱ��ʹ�С������Ĭ�Ϸֱ��ʿ�ʼ��Ϸ", "Quoridor_Game",MB_OK);
		WinWidth = WIN_WIDTH;
		WinHeight= WIN_HEIGHT;
	}
	//--------------------------------------------------
	char cc[]="Quoridor";
	// ������
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), 
		CS_CLASSDC,								// ������ķ�񣬿�����or����������
		MsgProc,								// ���ڴ���������ָ��
		0L, 0L,									// ָ�������ڴ�����ṹ��ĸ����ֽ���
		GetModuleHandle(NULL),					// ��ģ���ʵ�����
		LoadIcon(hInst, (LPCTSTR)IDI_ICON1),	// ͼ��ľ��
		LoadCursor(NULL, IDC_ARROW),			// ���ľ�������
		NULL, 									// ������ˢ�ľ��
		NULL,									// ָ��˵���ָ��
        cc,										// ָ�������Ƶ�ָ��
		LoadIcon(hInst, (LPCTSTR)IDI_ICON3)		// �ʹ����������Сͼ�ꡣ�����ֵΪNULL��
												// ���hIcon�е�ͼ��ת���ɴ�С���ʵ�Сͼ��
	};
	// ע�����������
	RegisterClassEx( &wc );
	//--------------------------------------------------
	// ����OpenGL����
	m_OpenGL=new COpenGLbase();
	//--------------------------------------------------
	// ������
	//dwExStyle=WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;
	//DWORD dwStyle=WS_POPUP;			// �����嶼û�У�ֻ����������
	//DWORD dwStyle=WS_OVERLAPPEDWINDOW;
	DWORD dwStyle=WS_DLGFRAME;		// û�����Ͻ��������ܰ�ť��ֻ��һ������

	// �õ���ǰ����ֱ���
	int wid=GetSystemMetrics(SM_CXSCREEN);
	int hei=GetSystemMetrics(SM_CYSCREEN);
	// �������Ͻ�����
	int nX=(wid-WinWidth)/2;
	int nY=(hei-WinHeight)/2-10;
	// ���������ݴ��ھ��
	hWnd = CreateWindowEx(NULL,		// ���ڵ���չ���
		cc,							// ָ��ע��������ָ��
		"Quoridor_by_ChuaiGM",		// ָ�򴰿����Ƶ�ָ��
		dwStyle,					// ���ڷ��
		nX, nY,						// ���ڵ���ʼλ������
		WinWidth,					// ���ڵĿ���
		WinHeight,					// ���ڵĸ߶�
		NULL,						// �����ڵľ��
		NULL,						// �˵��ľ�������Ӵ��ڵı�ʶ��
		hInst,						// Ӧ�ó���ʵ�����
		NULL						// ָ�򴰿ڵĴ�������
		);
	//--------------------------------------------------
	// ��ʾ��ʽ
	//ShowWindow( hWnd, SW_SHOWDEFAULT );	//Ĭ�ϴ�С
	//ShowWindow( hWnd, SW_MAXIMIZE );	//�����ʾ
	ShowWindow( hWnd, SW_SHOWNORMAL );	// ����
	// ���´�����
	UpdateWindow( hWnd );	

	// init����������ʱ
	//c1.ReStart(TIME_PRE_ANI);

	//�Ƿ���ʾ���
	//ShowCursor(false);

	// ������ѭ��
	GameLoop();	
    
	return 0;
}


