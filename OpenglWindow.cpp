#include "OpenglWindow.h"

#define IDM_TOGGLEFULLSCREEN            1000    // message-nr to be send

OpenglWindow::OpenglWindow()
{
	scrReady= FALSE;
	scrWindowed   = TRUE;

	TEXTURESIZE=0;          // telling the max. texture-resolution supported by the hardware (eg. for downscaling too large textures)
	MULTITEXTURE=0;         // flag indicating whether the hardware supports multitexturing
	closeProgram=0;

	for (int i=0;i<256;i++) keys[i]=FALSE;
}

OpenglWindow::~OpenglWindow()
{
	close();
}

LRESULT CALLBACK OpenglWindow::RealWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
        switch (message)  
        {
                case WM_CREATE:
                        break;
        
                case WM_COMMAND:
                    // Handle all menu and accelerator commands 
                    switch (LOWORD(wParam))
                    {
                        case IDM_TOGGLEFULLSCREEN:
                            if (scrReady)
                            {
                                scrReady= FALSE;
                                if (!reinitWindow(1))
                                {
//                                    error("cannot change displaymode");
                                    if (!reinitWindow(1))
                                    {
//                                        error("cannot restore displaymode");
                                        close();
										PostQuitMessage(0);
										ExitProcess(0);
                                    }
                                }
                                scrReady= TRUE;
                            }
                            return 0L;
                            break;
                    }
                    return FALSE;
                    break;

                case WM_DESTROY:
                case WM_CLOSE:
//                        scrReady= FALSE;
						closeProgram=1;
//						close();
//						PostQuitMessage(0);
//						ExitProcess(0);
                        break;

                case WM_KEYDOWN:
                        keys[wParam] = TRUE;
                        break;

                case WM_KEYUP:
                        keys[wParam] = FALSE;
                        break;

                case WM_SIZE:
                        if (scrReady)
                            resizeOpenGL(LOWORD(lParam),HIWORD(lParam));
                        break;

                case WM_SETCURSOR:
                        // Display the cursor in the window if windowed
                        if (!scrWindowed)
                        {
                            SetCursor(NULL);
                            return FALSE;
                        }
                        break;
        }
        
        return DefWindowProc(hWnd, message, wParam, lParam);
}

int OpenglWindow::create(HINSTANCE hInstance, WNDPROC wndProc, int xres, int yres, int fullscreen)
{
        scrReady= FALSE;

//		currentWindow=this;

        if (!startWindow(xres,yres,hInstance, wndProc)) return FALSE;

        initOpenGL(scrXres,scrYres);

		if (!reinitWindow(fullscreen))
        {
	    	// if windowed/fullscreen init failed, try the other mode:
            if (!reinitWindow(1))
            {
                // still fail
                return FALSE;
            }
        }


        // load keyboard accelerators
        ACCEL accels[1]={           // send 'idm_togglefullscreen' when alt & return is pressed
            FALT | FVIRTKEY,        // function-key
            VK_RETURN,              // key
            IDM_TOGGLEFULLSCREEN    // message to send
        };
        // load keyboard-messaging table
        hAccel= CreateAcceleratorTable(accels, 1);
        //if (hAccel==NULL) error("init for keyboard-shortcut failed");

        initExt();

        scrReady= TRUE;

        return TRUE;
}

int OpenglWindow::startWindow(int x, int y, HINSTANCE hInstance, WNDPROC wndProc)
{
	char windowTitle[] = "fatshit niggaz bitch";

    static WNDCLASS wc=
    {
        CS_OWNDC,
		wndProc,
        0,0,
        hInstance,
        NULL,
        LoadCursor(NULL, IDC_ARROW),
        NULL,
        NULL,
        windowTitle
    };

	// set window-class
    if(!RegisterClass(&wc)) return FALSE;

    // save original resolution
    oldXres= x;
    oldYres= y;

    // set actual resolution
    scrXres= x;
    scrYres= y;

    // create a window
    hWnd = CreateWindowEx(WS_EX_TOPMOST, windowTitle, windowTitle, WS_OVERLAPPEDWINDOW | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0,0, scrXres,scrYres, NULL, NULL, hInstance, NULL);
    if (hWnd==NULL) return FALSE;

    // save style and pos/size
    scrStyle= GetWindowLong(hWnd, GWL_STYLE);
    GetWindowRect(hWnd, &scrWindow);

    return TRUE;
}

void OpenglWindow::checkWindowMessages()
{
	MSG Message;
	while(PeekMessage(&Message, hWnd, 0U, 0U, PM_NOREMOVE))
	{
		GetMessage(&Message, NULL, 0U, 0U);
		TranslateAccelerator(hWnd, hAccel, &Message);
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

}

void OpenglWindow::close()
{
    scrReady= FALSE;
    DestroyAcceleratorTable(hAccel);
    deinitOpenGL();
    ChangeDisplaySettings(NULL, 0);
}

int OpenglWindow::reinitWindow(int swit)
{
//	int        sx,sy,style;
//    char       bla[256];
    static DEVMODE    mode;

    // save size of window    
    if (scrWindowed) GetWindowRect(hWnd, &scrWindow);

    // perform switch fullscreen<->windowed?
    if (swit) scrWindowed = !scrWindowed;

    // reset window-size and shape
    if (scrWindowed)
    {
        SetWindowLong(hWnd, GWL_STYLE, scrStyle);

        scrXres= (scrWindow.right - scrWindow.left);
        scrYres= (scrWindow.bottom - scrWindow.top);

        ChangeDisplaySettings(NULL, 0);

        SetWindowPos(hWnd, HWND_NOTOPMOST, scrWindow.left, scrWindow.top, scrXres, scrYres, SWP_SHOWWINDOW  | SWP_FRAMECHANGED);

//        ShowCursor (1);
    } else
    {
        // erase borders and menubars
        SetWindowLong(hWnd, GWL_STYLE, 0);

        // find bestfitting videomode
        if (!searchMode(oldXres, oldYres, &mode))
        {
            if (!searchMode(oldXres*2, oldYres*2, &mode))
            {
              return FALSE;
            }
        }

        // try to set videomode
        if (ChangeDisplaySettings(&mode, CDS_FULLSCREEN)==DISP_CHANGE_BADMODE) return FALSE;

        // set window-area and -position
        SetWindowPos(hWnd, HWND_TOPMOST, 0,0, scrXres, scrYres, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
    }

    resizeOpenGL(scrXres, scrYres);
		    
    return TRUE;
}

int OpenglWindow::searchMode(int x, int y, DEVMODE *bestMode)
{
    int     res=1, mode=0;
    DEVMODE aktMode;

    memset(bestMode, 0, sizeof(DEVMODE));

    while (res!=0)
    {
        res= EnumDisplaySettings(NULL, mode, &aktMode);

        if (res!=0)
        {
            if ((unsigned int)x==aktMode.dmPelsWidth && (unsigned int)y==aktMode.dmPelsHeight && aktMode.dmBitsPerPel > bestMode->dmBitsPerPel)
            {
                scrXres= x;
                scrYres= y;
                // reset to original resolution
                memcpy(bestMode, &aktMode, sizeof(aktMode));
            }
        }

        mode++;
    }

    if (bestMode->dmBitsPerPel < 15) return FALSE;

    return TRUE;
}

void OpenglWindow::deinitOpenGL()
{
//    ChangeDisplaySettings(NULL, 0);

    ShowCursor (1);  // show cursor
	wglMakeCurrent(NULL,NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd,hDC);
}

void OpenglWindow::initOpenGL(int Width, int Height)
{
    GLuint  PixelFormat;

    static  PIXELFORMATDESCRIPTOR pfd=
    {
        sizeof(PIXELFORMATDESCRIPTOR),          // Size Of This Pixel Format Descriptor
        1,                                      // Version Number (?)
        PFD_DRAW_TO_WINDOW |                    // Format Must Support Window
        PFD_SUPPORT_OPENGL |                    // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,                       // Must Support Double Buffering
        PFD_TYPE_RGBA,                          // Request An RGBA Format
/*        32,                                     // Select A 16Bit Color Depth
        0, 0, 0, 0, 0, 0,                       // Color Bits Ignored (?)
        0,                                      // No Alpha Buffer
        0,                                      // Shift Bit Ignored (?)
        0,                                      // No Accumulation Buffer
        0, 0, 0, 0,                             // Accumulation Bits Ignored (?)
        32,                                     // 16Bit Z-Buffer (Depth Buffer)  
        0,                                      // No Stencil Buffer
        0,                                      // No Auxiliary Buffer (?)
*/
        32,                                     // Select A 16Bit Color Depth
        32, 32, 32, 32, 32, 32,                       // Color Bits Ignored (?)
        32,                                      // No Alpha Buffer
        32,                                      // Shift Bit Ignored (?)
        32,                                      // No Accumulation Buffer
        32,32, 32, 32,                             // Accumulation Bits Ignored (?)
        32,                                     // 16Bit Z-Buffer (Depth Buffer)  
        32,                                      // No Stencil Buffer
        32,                                      // No Auxiliary Buffer (?)

        PFD_MAIN_PLANE,                         // Main Drawing Layer
        0,                                      // Reserved (?)
        0, 0, 0                                 // Layer Masks Ignored (?)
    };

    hDC = GetDC(hWnd);                              // Gets A Device Context For The Window
    if (hDC==NULL) error("cannot create devicecontext");

    PixelFormat = ChoosePixelFormat(hDC, &pfd);     // Finds The Closest Match To The Pixel Format We Set Above
    if (!PixelFormat) error("no suitable pixelformat");

    if(!SetPixelFormat(hDC,PixelFormat,&pfd)) error("cannot set pixelformat");
    
    hRC = wglCreateContext(hDC);
    if(!hRC) error("no rendering context");
    
    if(!wglMakeCurrent(hDC, hRC)) error("cannot activate glrc");

    // show window
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    SetFocus(hWnd);

    // enable first texture-pipeline
    glEnable(GL_TEXTURE_2D);                                        // Enable Texture Mapping
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);                           // This Will Clear The Background Color To Black
    glClearDepth(1.0);                                              // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);                                           // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);                                        // Enables Depth Testing
    glShadeModel(GL_SMOOTH);                                        // Enables Smooth Color Shading

    // reset view
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                                                       // Reset The Projection Matrix

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    resizeOpenGL(Width,Height); 
}

void OpenglWindow::resizeOpenGL(int Width, int Height)
{
    if (Height<1) Height=1;
    if (Width<1) Width=1;

    // set remdering windowsize
    glViewport(0, 0, Width, Height);                        // Reset The Current Viewport And Perspective Transformation

	scrXres=Width;
	scrYres=Height;
}

int OpenglWindow::initExt()
{
	// check whether multitexturing is supported
    MULTITEXTURE = CheckEXT("GL_ARB_multitexture") &&  CheckEXT("GL_EXT_texture_env_combine");
//	if (!MULTITEXTURE) error("no multitexturing supported");

//	POINTPARAMS = CheckEXT("GL_EXT_point_parameters");
//	if (!POINTPARAMS) error("point parameter extension not supported");

    // get max. texture-size
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &TEXTURESIZE);

	if (TEXTURESIZE<512) error("support for atleast 512*512 resolution textures required");

	// init ogl-extensions
    glMultiTexCoord1dARB	= (PFNGLMULTITEXCOORD1DARBPROC)		wglGetProcAddress("glMultiTexCoord1dEXT");
    glMultiTexCoord1dvARB	= (PFNGLMULTITEXCOORD1DVARBPROC)	wglGetProcAddress("glMultiTexCoord1dvARB");	
    glMultiTexCoord1fARB	= (PFNGLMULTITEXCOORD1FARBPROC)		wglGetProcAddress("glMultiTexCoord1fARB");
    glMultiTexCoord1fvARB	= (PFNGLMULTITEXCOORD1FVARBPROC)	wglGetProcAddress("glMultiTexCoord1fvARB");
    glMultiTexCoord1iARB	= (PFNGLMULTITEXCOORD1IARBPROC)		wglGetProcAddress("glMultiTexCoord1iARB");
    glMultiTexCoord1ivARB	= (PFNGLMULTITEXCOORD1IVARBPROC)	wglGetProcAddress("glMultiTexCoord1ivARB");
    glMultiTexCoord1sARB	= (PFNGLMULTITEXCOORD1SARBPROC)		wglGetProcAddress("glMultiTexCoord1sARB");
    glMultiTexCoord1svARB	= (PFNGLMULTITEXCOORD1SVARBPROC)	wglGetProcAddress("glMultiTexCoord1svARB");
    glMultiTexCoord2dARB	= (PFNGLMULTITEXCOORD2DARBPROC)		wglGetProcAddress("glMultiTexCoord2dARB");
    glMultiTexCoord2dvARB	= (PFNGLMULTITEXCOORD2DVARBPROC)	wglGetProcAddress("glMultiTexCoord2dvARB");	
    glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");
    glMultiTexCoord2fvARB	= (PFNGLMULTITEXCOORD2FVARBPROC)	wglGetProcAddress("glMultiTexCoord2fvARB");
    glMultiTexCoord2iARB	= (PFNGLMULTITEXCOORD2IARBPROC)		wglGetProcAddress("glMultiTexCoord2iARB");
    glMultiTexCoord2ivARB	= (PFNGLMULTITEXCOORD2IVARBPROC)	wglGetProcAddress("glMultiTexCoord2ivARB");
    glMultiTexCoord2sARB	= (PFNGLMULTITEXCOORD2SARBPROC)		wglGetProcAddress("glMultiTexCoord2sARB");
    glMultiTexCoord2svARB	= (PFNGLMULTITEXCOORD2SVARBPROC)	wglGetProcAddress("glMultiTexCoord2svARB");
    glMultiTexCoord3dARB	= (PFNGLMULTITEXCOORD3DARBPROC)		wglGetProcAddress("glMultiTexCoord3dARB");
    glMultiTexCoord3dvARB	= (PFNGLMULTITEXCOORD3DVARBPROC)	wglGetProcAddress("glMultiTexCoord3dvARB");
    glMultiTexCoord3fARB	= (PFNGLMULTITEXCOORD3FARBPROC)		wglGetProcAddress("glMultiTexCoord3fARB");
    glMultiTexCoord3fvARB	= (PFNGLMULTITEXCOORD3FVARBPROC)	wglGetProcAddress("glMultiTexCoord3fvARB");
    glMultiTexCoord3iARB	= (PFNGLMULTITEXCOORD3IARBPROC)		wglGetProcAddress("glMultiTexCoord3iARB");
    glMultiTexCoord3ivARB	= (PFNGLMULTITEXCOORD3IVARBPROC)	wglGetProcAddress("glMultiTexCoord3ivARB");
    glMultiTexCoord3sARB	= (PFNGLMULTITEXCOORD3SARBPROC)		wglGetProcAddress("glMultiTexCoord3sARB");
    glMultiTexCoord3svARB	= (PFNGLMULTITEXCOORD3SVARBPROC)	wglGetProcAddress("glMultiTexCoord3svARB");
    glMultiTexCoord4dARB	= (PFNGLMULTITEXCOORD4DARBPROC)		wglGetProcAddress("glMultiTexCoord4dARB");
    glMultiTexCoord4dvARB	= (PFNGLMULTITEXCOORD4DVARBPROC)	wglGetProcAddress("glMultiTexCoord4dvARB");	
    glMultiTexCoord4fARB	= (PFNGLMULTITEXCOORD4FARBPROC)		wglGetProcAddress("glMultiTexCoord4fARB");
    glMultiTexCoord4fvARB	= (PFNGLMULTITEXCOORD4FVARBPROC)	wglGetProcAddress("glMultiTexCoord4fvARB");
    glMultiTexCoord4iARB	= (PFNGLMULTITEXCOORD4IARBPROC)		wglGetProcAddress("glMultiTexCoord4iARB");
    glMultiTexCoord4ivARB	= (PFNGLMULTITEXCOORD4IVARBPROC)	wglGetProcAddress("glMultiTexCoord4ivARB");
    glMultiTexCoord4sdARB	= (PFNGLMULTITEXCOORD4SARBPROC)		wglGetProcAddress("glMultiTexCoord4sdARB");
    glMultiTexCoord4svARB	= (PFNGLMULTITEXCOORD4SVARBPROC)	wglGetProcAddress("glMultiTexCoord4svARB");
    glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
    glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)	wglGetProcAddress("glClientActiveTextureARB");

//	glPointParameterfEXT	= (PFNGLPOINTPARAMETERFEXTPROC)		wglGetProcAddress("glPointParameterfEXT");
//	glPointParameterfvEXT	= (PFNGLPOINTPARAMETERFVEXTPROC)	wglGetProcAddress("glPointParameterfvEXT");

	return TRUE;

}

int OpenglWindow::CheckEXT(char *name)
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	// extension names should not have spaces
	where = (GLubyte *) strchr(name, ' ');
	if (where || *name == '\0')	return FALSE;
	
	extensions = glGetString(GL_EXTENSIONS);
	
	start = extensions;
	
	for (;;)
	{
		where = (GLubyte *) strstr((const char *)start, name);
		if (!where) break;
		terminator = where + strlen(name);
		if (where == start || *(where - 1) == ' ')
		{
			if (*terminator == ' ' || *terminator == '\0')	return TRUE;
		}
		start = terminator;
	}
	
	return FALSE;
}

void OpenglWindow::error(char *txt)
{
	ShowCursor (1);  // show cursor
    MessageBox(0, txt, "Error", MB_OK|MB_ICONERROR);
    PostQuitMessage(0);
}

void OpenglWindow::swapBuffers()
{
	glFinish();
	glFlush();
	SwapBuffers(hDC);
}
