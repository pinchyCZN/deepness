// ImageLoader.h: interface for the ImageLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGELOADER_H__DB287B08_763D_4967_B0E9_E3032A275B9B__INCLUDED_)
#define AFX_IMAGELOADER_H__DB287B08_763D_4967_B0E9_E3032A275B9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GLTexture.h"
#include "String.h"

class ImageLoader  
{

public:
	static GLTexture * loadJPG(String name, int blending);
	static GLTexture * loadPNG(String name, int blending);
	ImageLoader();
	virtual ~ImageLoader();
private:
	static void error(String txt);
};

#endif // !defined(AFX_IMAGELOADER_H__DB287B08_763D_4967_B0E9_E3032A275B9B__INCLUDED_)
