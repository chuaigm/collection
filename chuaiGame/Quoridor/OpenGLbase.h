// OpenGL.h: interface for the OpenGL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGL_H__17B7289C_7956_41C5_89B9_621E3C435389__INCLUDED_)
#define AFX_OPENGL_H__17B7289C_7956_41C5_89B9_621E3C435389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//font
#include "GLFont.h"

// OpenGL默认坐标系
//          Y ^
//            |
//            |
//            |--------> X
//           /
//          /
//       Z V

class COpenGLbase
{
public:
    COpenGLbase();
    virtual ~COpenGLbase();
    
public:
    //-----------------------------------------
    BOOL    SetupPixelFormat(HDC hDC);
    void    SetViewSize(int w,int h);
    void    init_3D();
    void    init_2D();
    void    Render();                       // 每次重绘的内容
    void    CleanUp();
    // 输出本OpenGL类一些调试测试参数
    void    text_Debug();
    // 处理鼠标
    void    mouseProc(long long lparam);
    // 加载图像素材的方法
    void    texture_select(UINT textur);        // 绑定贴图
    bool    LoadBMP_aux(char *filename, GLuint &texture);
    //-----------------------------------------
    HDC     hDC;        // 外部传入的上下位
    HGLRC   hRC;        // 生成的描述句柄
    // 在不重启程序的前提下，视口宽高应该是不变的
    int     RCwidth;    // 此OpenGL实例内部的视口宽度
    int     RCheight;   // 此OpenGL实例内部的视口高度
    //鼠标位置
    int     Xmouse;
    int     Ymouse;
    int     XmouseOld;
    int     YmouseOld;
};

#endif // !defined(AFX_OPENGL_H__17B7289C_7956_41C5_89B9_621E3C435389__INCLUDED_)
