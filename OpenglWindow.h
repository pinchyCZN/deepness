#ifndef _OPENGLWINDOW_H_
#define _OPENGLWINDOW_H_

#include "windows.h"
#include <gl\gl.h>              // Header File For The OpenGL32 Library
#include <gl\glu.h>             // Header File For The GLu32 Library
#include <gl\glaux.h>			// Header File For The GLaux Library
#include "glext.h"  			// Header File For The GL Extensions

#include "VemputinMain.h"

class OpenglWindow  
{
public:
	int closeProgram;
	bool POINTPARAMS;
	int keys[256];
	void swapBuffers();
	HWND hWnd;
	void checkWindowMessages();
	int create(HINSTANCE hInstance, WNDPROC wndProc, int xres, int yres, int fullscreen);
	void close();
	OpenglWindow();
	virtual ~OpenglWindow();
	LRESULT CALLBACK RealWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void error(char *txt);
	void resizeOpenGL(int Width, int Height);

	int TEXTURESIZE;          // telling the max. texture-resolution supported by the hardware (eg. for downscaling too large textures)
	int MULTITEXTURE;         // flag indicating whether the hardware supports multitexturing

	int scrXres;
	int scrYres;
	bool scrReady;

	// virtual function-pointer to opengl-extension (for multitexturing)
	PFNGLMULTITEXCOORD1DARBPROC		glMultiTexCoord1dARB;
	PFNGLMULTITEXCOORD1DVARBPROC	glMultiTexCoord1dvARB;
	PFNGLMULTITEXCOORD1FARBPROC		glMultiTexCoord1fARB;
	PFNGLMULTITEXCOORD1FVARBPROC	glMultiTexCoord1fvARB;
	PFNGLMULTITEXCOORD1IARBPROC		glMultiTexCoord1iARB;
	PFNGLMULTITEXCOORD1IVARBPROC	glMultiTexCoord1ivARB;	
	PFNGLMULTITEXCOORD1SARBPROC		glMultiTexCoord1sARB;
	PFNGLMULTITEXCOORD1SVARBPROC	glMultiTexCoord1svARB;
	PFNGLMULTITEXCOORD2DARBPROC		glMultiTexCoord2dARB;
	PFNGLMULTITEXCOORD2DVARBPROC	glMultiTexCoord2dvARB;	
	PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;
	PFNGLMULTITEXCOORD2FVARBPROC	glMultiTexCoord2fvARB;
	PFNGLMULTITEXCOORD2IARBPROC		glMultiTexCoord2iARB;
	PFNGLMULTITEXCOORD2IVARBPROC	glMultiTexCoord2ivARB;
	PFNGLMULTITEXCOORD2SARBPROC		glMultiTexCoord2sARB;
	PFNGLMULTITEXCOORD2SVARBPROC	glMultiTexCoord2svARB;
	PFNGLMULTITEXCOORD3DARBPROC		glMultiTexCoord3dARB;
	PFNGLMULTITEXCOORD3DVARBPROC	glMultiTexCoord3dvARB;
	PFNGLMULTITEXCOORD3FARBPROC		glMultiTexCoord3fARB;
	PFNGLMULTITEXCOORD3FVARBPROC	glMultiTexCoord3fvARB;
	PFNGLMULTITEXCOORD3IARBPROC		glMultiTexCoord3iARB;
	PFNGLMULTITEXCOORD3IVARBPROC	glMultiTexCoord3ivARB;
	PFNGLMULTITEXCOORD3SARBPROC		glMultiTexCoord3sARB;
	PFNGLMULTITEXCOORD3SVARBPROC	glMultiTexCoord3svARB;
	PFNGLMULTITEXCOORD4DARBPROC		glMultiTexCoord4dARB;
	PFNGLMULTITEXCOORD4DVARBPROC	glMultiTexCoord4dvARB;	
	PFNGLMULTITEXCOORD4FARBPROC		glMultiTexCoord4fARB;
	PFNGLMULTITEXCOORD4FVARBPROC	glMultiTexCoord4fvARB;
	PFNGLMULTITEXCOORD4IARBPROC		glMultiTexCoord4iARB;
	PFNGLMULTITEXCOORD4IVARBPROC	glMultiTexCoord4ivARB;
	PFNGLMULTITEXCOORD4SARBPROC		glMultiTexCoord4sdARB;
	PFNGLMULTITEXCOORD4SVARBPROC	glMultiTexCoord4svARB;
	PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
	PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB;
	PFNGLPOINTPARAMETERFEXTPROC		glPointParameterfEXT;
	PFNGLPOINTPARAMETERFVEXTPROC	glPointParameterfvEXT;

	HGLRC                   hRC;             // Permanent Rendering Context
	HDC                     hDC;             // Private GDI Device Context

private:
	HACCEL hAccel;
	int oldYres;
	int oldXres;
	unsigned int scrStyle;
	RECT scrWindow;
	int scrWindowed;
	int initExt();
	int CheckEXT(char* name);
	void initOpenGL(int Width, int Height);
	void deinitOpenGL();
	int searchMode(int x, int y, DEVMODE *bestMode);
	int reinitWindow(int swit);
	int startWindow(int x, int y, HINSTANCE hInstance, WNDPROC wndProc);
};

#endif