#include <windows.h>
#include <GL/gl.h>
#include "glext.h"

HWND hmainwin=0;
HDC hmaindc=0;
HGLRC hmainglrc=0;
LONG winstyle=0;
RECT winrect={0};

LRESULT CALLBACK dlgproc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	switch(msg){
	case WM_COMMAND:
		switch(LOWORD(wparam)){
		case 
		}
		break;
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}

int register_window(HINSTANCE hinstance)
{
	int result=FALSE;
	WNDCLASS wndclass;
	const char *name="fat niggaz bitch";
	memset(&wndclass,0,sizeof(wndclass));
	wndclass.lpfnWndProc=dlgproc;
	wndclass.hInstance=hinstance;
	wndclass.hCursor=LoadCursor(0,IDC_ARROW);
	wndclass.lpszClassName=name;
	if(RegisterClass(&wndclass)){
		int exstyle=0; //WS_EX_TOPMOST;
		//0x86CF 0000
		int style=WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_GROUP|WS_TABSTOP;
		hmainwin=CreateWindowEx(exstyle,name,name,style,
				0,0,640,480,0,0,hinstance,0);
		if(hmainwin){
			winstyle=GetWindowLong(hmainwin,GWL_STYLE);
			GetWindowRect(hmainwin,&winrect);
			result=TRUE;
		}
	}
	return result;
}
int setup_ogl(HWND hwnd)
{
	hmaindc=GetDC(hwnd);
	if(hmaindc!=0){
		PIXELFORMATDESCRIPTOR pfd;
		int pixelfmt;
		memset(&pfd,0,sizeof(pfd));
		pfd.nSize=sizeof(pfd);
		pfd.nVersion=1;
		pfd.dwFlags=PFD_DOUBLEBUFFER|PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL;
		pfd.iPixelType=0;
		pfd.cColorBits=32;
		pfd.cRedBits=32;
		pfd.cRedShift=32;
		pfd.cGreenBits=32;
		pfd.cGreenShift=32;
		pfd.cBlueBits=32;
		pfd.cBlueShift=32;
		pfd.cAlphaBits=32;
		pfd.cAlphaShift=32;
		pfd.cAccumBits=32;
		pfd.cAccumRedBits=32;
		pfd.cAccumGreenBits=32;
		pfd.cAccumBlueBits=32;
		pfd.cAccumAlphaBits=32;
		pfd.cDepthBits=32;
		pfd.cStencilBits=32;
		pfd.cAuxBuffers=32;
		pfd.iLayerType=0;
		pfd.bReserved=0;
		pfd.dwLayerMask=0;
		pfd.dwVisibleMask=0;
		pfd.dwDamageMask=0;
		pixelfmt=ChoosePixelFormat(hmaindc,&pfd);
		SetPixelFormat(hmaindc,pixelfmt,&pfd);
		hmainglrc=wglCreateContext(hmaindc);
		wglMakeCurrent(hmaindc,hmainglrc);
		ShowWindow(hmainwin,SW_SHOW);
		UpdateWindow(hmainwin);
		SetFocus(hmainwin);
		glEnable(GL_TEXTURE_2D);
		glClearColor(.2,.2,.2,0);
		glClearDepth(0);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		{
			RECT rect={0};
			GetClientRect(hwnd,&rect);
			glViewport(0,0,rect.right,rect.bottom);
		}
	}
}
int setup_display(HWND hwnd)
{
	RECT rect={0};
	GetWindowRect(hwnd,&rect);
}
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{
	if(register_window(hInstance)){
		setup_ogl(hmainwin);
		setup_display(hmainwin);
	}

}