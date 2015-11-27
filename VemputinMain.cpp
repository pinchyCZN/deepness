// Main. window handling. startup. Markus Pasula

#include <windows.h>

#include "VemputinMain.h"
#include "Demo.h"
#include "OpenglWindow.h"

OpenglWindow *window;
Demo *demo;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return window->RealWndProc(hWnd,message,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	window = new OpenglWindow;
	demo = new Demo;

	window->create(hInstance, (WNDPROC)WndProc, 640, 480, 0);

	demo->init(window);
	demo->run();
	demo->deinit();

	window->close();

	delete window;

    PostQuitMessage(0);
//    ExitProcess(0);

	return 0;
}
