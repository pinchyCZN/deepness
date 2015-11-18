#include <windows.h>

HWND hmainwin=0;
LONG winstyle=0;
RECT winrect={0};

LRESULT CALLBACK dlgproc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	return 0;
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
												//0x86CF 0000
		hmainwin=CreateWindowEx(WS_EX_TOPMOST,name,name,WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_GROUP|WS_TABSTOP,
				0,0,400,400,0,0,hinstance,0);
		if(hmainwin){
			winstyle=GetWindowLong(hmainwin,GWL_STYLE);
			GetWindowRect(hmainwin,&winrect);
			result=TRUE;
		}
	}
	return result;

}
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{
	register_window(hInstance);

}