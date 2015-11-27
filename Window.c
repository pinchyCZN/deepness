// window-management

BOOL                keys[256];          // Array Used For The Keyboard Routine
static  HGLRC       hRC;                // Permanent Rendering Context
static  HDC         hDC;                // Private GDI Device Context
int                 current_screenmode=0;
int					xres,yres;
float               FrontClip;

void changetowindow(HWND hWnd)
{
	if(ChangeDisplaySettings(NULL, 0)==DISP_CHANGE_BADMODE) error("can't go into window mode");
    ShowCursor(1);

	ReSizeGLScene(xres,yres);
	ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);
  SetFocus(hWnd);
  wglMakeCurrent(hDC,hRC);
}

void changetofullscreen(HWND hWnd, int x,int y)
{
	int i=0,i2=-1,i_bpp=-1;

	DEVMODE allScreenModes[5000]; // 5000 screenmodes should be enough for everyone ;)

    // search all available screenmodes and find (xres*yres) with highest bpp
	while(EnumDisplaySettings(NULL, i, &allScreenModes[i])!=0&&i<4999)
	{ 
    #pragma warning (disable: 4018)
		if (allScreenModes[i].dmPelsWidth==x&&allScreenModes[i].dmPelsHeight==y )
    #pragma warning (default: 4018)
		{
			if ((signed int)allScreenModes[i].dmBitsPerPel>i_bpp)
			{
				i2=i;
				i_bpp=allScreenModes[i].dmBitsPerPel;
			}
		}
		i++;
	}
	if (i2==-1) error("screenmode not found");
	if(ChangeDisplaySettings(&allScreenModes[i2], CDS_FULLSCREEN)==DISP_CHANGE_BADMODE) error("can't go into fullscreen mode");
    
	ShowCursor (0);  // hide cursor

	ReSizeGLScene(x,y);
	ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);
  SetFocus(hWnd);
  wglMakeCurrent(hDC,hRC);

}



GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
  if (Height==0) Height=1;

//        glViewport(0, 0, Width, Height);                        // Reset The Current Viewport And Perspective Transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                                                       // Reset The Projection Matrix

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,10000.0f);     // Calculate The Aspect Ratio Of The Window
//        gluPerspective(45.0f, 16.0/9.0, 1.0, 100.0);
//  glViewport(0, (Height-Width*9/16)/2, Width, Width*9/16);

  glViewport(0, 0, Width, Height);                        // Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  xres = Width; yres = Height;  
}


GLvoid InitGL(GLsizei Width, GLsizei Height)    // This Will Be Called Right After The GL Window Is Created
{
        glEnable(GL_TEXTURE_2D);                                        // Enable Texture Mapping
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                           // This Will Clear The Background Color To Black
        glClearDepth(1.0);                                              // Enables Clearing Of The Depth Buffer
        glDepthFunc(GL_LESS);                                           // The Type Of Depth Test To Do
        glEnable(GL_DEPTH_TEST);                                        // Enables Depth Testing
        glShadeModel(GL_SMOOTH);                                        // Enables Smooth Color Shading
        ReSizeGLScene(Width,Height); 
}

// automatically called window-check
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
        RECT    Screen;                                                 // Used Later On To Get The Size Of The Window
        GLuint  PixelFormat;
		int fActive;

        static  PIXELFORMATDESCRIPTOR pfd=
        {
                sizeof(PIXELFORMATDESCRIPTOR),          // Size Of This Pixel Format Descriptor
                1,                                                                      // Version Number (?)
                PFD_DRAW_TO_WINDOW |                            // Format Must Support Window
                PFD_SUPPORT_OPENGL |                            // Format Must Support OpenGL
                PFD_DOUBLEBUFFER,                                       // Must Support Double Buffering
                PFD_TYPE_RGBA,                                          // Request An RGBA Format
                16,                                                                     // Select A 16Bit Color Depth
                0, 0, 0, 0, 0, 0,                                       // Color Bits Ignored (?)
                0,                                                                      // No Alpha Buffer
                0,                                                                      // Shift Bit Ignored (?)
                0,                                                                      // No Accumulation Buffer
                0, 0, 0, 0,                                                     // Accumulation Bits Ignored (?)
                16,                                                                     // 16Bit Z-Buffer (Depth Buffer)  
                0,                                                                      // No Stencil Buffer
                0,                                                                      // No Auxiliary Buffer (?)
                PFD_MAIN_PLANE,                                         // Main Drawing Layer
                0,                                                                      // Reserved (?)
                0, 0, 0                                                         // Layer Masks Ignored (?)
        };

        switch (message)                                                // Tells Windows We Want To Check The Message
        {

          case WM_CREATE:
                        hDC = GetDC(hWnd);                              // Gets A Device Context For The Window
                        PixelFormat = ChoosePixelFormat(hDC, &pfd);             // Finds The Closest Match To The Pixel Format We Set Above

                        if (!PixelFormat)
                        {
                                MessageBox(0,"Can't Find A Suitable PixelFormat.","Error",MB_OK|MB_ICONERROR);
                                PostQuitMessage(0);                     // This Sends A 'Message' Telling The Program To Quit
                                break;                                          // Prevents The Rest Of The Code From Running
                        }

                        if(!SetPixelFormat(hDC,PixelFormat,&pfd))
                        {
                                MessageBox(0,"Can't Set The PixelFormat.","Error",MB_OK|MB_ICONERROR);
                                PostQuitMessage(0);
                                break;
                        }

                        hRC = wglCreateContext(hDC);
                        if(!hRC)
                        {
                                MessageBox(0,"Can't Create A GL Rendering Context.","Error",MB_OK|MB_ICONERROR);
                                PostQuitMessage(0);
                                break;
                        }

                        if(!wglMakeCurrent(hDC, hRC))
                        {
                                MessageBox(0,"Can't activate GLRC.","Error",MB_OK|MB_ICONERROR);
                                PostQuitMessage(0);
                                break;
                        }

                        GetClientRect(hWnd, &Screen);
                        InitGL(Screen.right, Screen.bottom);
                        break;

                case WM_DESTROY:
                case WM_CLOSE:
                        ChangeDisplaySettings(NULL, 0);

						BASS_Free();
//                        FSOUND_Close();
                       	ShowCursor (1);  // show cursor
					            	wglMakeCurrent(NULL,NULL);
                        wglDeleteContext(hRC);
                        ReleaseDC(hWnd,hDC);

                        PostQuitMessage(0);
                        exit(0);
                        break;

                case WM_KEYDOWN:
                        keys[wParam] = TRUE;
                        break;

                case WM_KEYUP:
                        keys[wParam] = FALSE;
                        break;

                case WM_SIZE:
                        ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
                        break;
				case WM_ACTIVATE:
						fActive = LOWORD(wParam); 
						if (fActive==WA_INACTIVE) SetForegroundWindow(hWnd);
						break;

/*                case WM_SYSKEYDOWN:

                        if (wParam==VK_RETURN && lParam&0x20000000)
                        {
							if (current_screenmode>0)
							{
        						changetowindow(hWnd);
								current_screenmode=0;
							}
							else{
								changetofullscreen(hWnd,xres,yres);
								current_screenmode=d_screenmode;
							}
                        }
                        break;
*/
                default:
                        return (DefWindowProc(hWnd, message, wParam, lParam));
        }
  return (0);
}

HWND initScreen(int x, int y, int i_fullscreen, HINSTANCE hInstance)
{
  HWND hWnd;           // Storage For Window Handle
  WNDCLASS wc;

  // define type of window

  wc.style                = CS_OWNDC;
  wc.lpfnWndProc          = (WNDPROC) WndProc;
  wc.cbClsExtra           = 0;
  wc.cbWndExtra           = 0;
  wc.hInstance            = hInstance;
  wc.hIcon                = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
  wc.hCursor              = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground        = NULL;
  wc.lpszMenuName         = NULL;
  wc.lpszClassName        = "OpenGL WinClass";

  xres=x;
  yres=y;
  FrontClip= 2.0;
        
	if(!RegisterClass(&wc)) return NULL;

	if (i_fullscreen==1) // fullscreenmode
	{
		hWnd = CreateWindowEx(WS_EX_TOPMOST,"OpenGL WinClass","Haujobb",WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,0,0, x,y, NULL, NULL, hInstance, NULL);
    if(!hWnd) return NULL;
		changetofullscreen(hWnd,xres,yres);
	}
	else // windowmode
	{
    hWnd = CreateWindowEx(WS_EX_TOPMOST,"OpenGL WinClass","Haujobb",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 0,0, x,y, NULL, NULL, hInstance, NULL);
		changetowindow(hWnd);
		if(!hWnd) return NULL;
	}

  return hWnd;
}


void checkWindowMessage()
{
  MSG msg;
  
  // Process All Messages
  while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
  {
    if (GetMessage(&msg, NULL, 0, 0))
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
  }
}


