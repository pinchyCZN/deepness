// ImageLoader.cpp: implementation of the ImageLoader class.
//
//////////////////////////////////////////////////////////////////////
#include "ijl.h"
#include "jpeg.h"
#include "png.h"
#include "Stream.h"
#include "ImageLoader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*


	some day this will be a texturepool class ;)

  */

ImageLoader::ImageLoader()
{

}

ImageLoader::~ImageLoader()
{

}

void ImageLoader::error(String txt)
{
	ShowCursor (1);  // show cursor
    MessageBox(0, txt.c_String(), "Error", MB_OK|MB_ICONERROR);
    PostQuitMessage(0);
}

GLTexture * ImageLoader::loadJPG(String name, int blending)
{
	unsigned long *tex;
	int x,y;
	if (!DecodeJPGFileToGeneralBuffer(name.c_String(),&x,&y,(unsigned char **)&tex))
	{
		error(name+" error loading jpeg.");
		return NULL;
	}
	unsigned char *tchar=(unsigned char *)tex;

	for (int i=0;i<x*y;i++) // vaihdetaan punainen ja sininen keskenään. bgr -> rgb 
	{
		int r,b;
		r=tchar[i*3];  b=tchar[i*3+2];
		tchar[i*3  ]=b;  // pun
		tchar[i*3+2]=r;  // sin
	}

	GLTexture *tekstuuri=new GLTexture(tex,x,y,24,blending);
	return tekstuuri;
}

GLTexture * ImageLoader::loadPNG(String name, int blending)
{
	unsigned long *tex;
	int x=-1,y;
	loadpng32((char *)name.c_String(), x,y, (unsigned int*&)tex);
	if (x==-1)
	{
		error(name+" error loading png.");
		return NULL;
	}

	GLTexture *tekstuuri=new GLTexture(tex,x,y,32,blending);
	return tekstuuri;
}