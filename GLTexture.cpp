/*
  :: oidberg
  parts taken from: Pan/SpinningKids  
*/

#include "GLTexture.h"
#include <memory.h>

GLTexture::GLTexture(int _logsize) : logsize(_logsize), ysize(1<<_logsize), xsize(1<<_logsize), mem(new unsigned long[(1<<_logsize)*(1<<_logsize)]) {
	bpp=GL_RGBA;
	glBpp=GL_RGBA;
	glGenTextures(1, &txt);
//	memset(mem, 0, size*size*4*sizeof(float));
//	glBindTexture(GL_TEXTURE_2D, txt);
	mipmap=true;
	filtering=true;
	init = false;
	wrapping=true;
}

GLTexture::GLTexture(int xres, int yres)
{
	mem = new unsigned long[xres*yres];
	memset(mem,0,xres*yres*4);
	init=false;
	xsize=xres;
	ysize=yres;
	glGenTextures(1, &txt);
	bpp=GL_RGBA;
	glBpp=GL_RGB;
	mipmap=false;
	filtering=true;
	wrapping=true;
	update();
}

GLTexture::GLTexture(unsigned long *buf, int xres, int yres, int bits, int blending)
{
	switch(bits)
	{
	case 24:
		bpp=GL_RGB;
		glBpp=GL_RGB;
		break;
	case 32:
		bpp=GL_RGBA;
		glBpp=GL_RGBA;
		break;
	default:
		bpp=GL_RGB;
		glBpp=GL_RGB;
		break;
	}
	mem=buf;
	xsize=xres;
	ysize=yres;
	glGenTextures(1, &txt);
	init=false;
	if (blending>2) mipmap=true;
	else mipmap=false;
	if (blending<=1) filtering=false;
	else filtering=true;
	wrapping=true;
	update();
}

void GLTexture::update() {
  glBindTexture(GL_TEXTURE_2D, txt);
  if (!init) {
    glTexImage2D(GL_TEXTURE_2D, 0, glBpp, xsize, ysize, 0, bpp, GL_UNSIGNED_BYTE, mem);

	params();

    init = true;
  } else
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, xsize, ysize, bpp, GL_UNSIGNED_BYTE, mem);
}

void GLTexture::use() {
  if (!init)
    glDisable(GL_TEXTURE_2D);
  else {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txt);
  }
}


void GLTexture::darken(float val)
{
	unsigned char *b=(unsigned char *)mem;
	int bp=3;
	int mul=val*256.0;
	if (bpp==32) bp=4;
	for (int i=0;i<bp*xsize*ysize;i++)
	{
		*b=*b*mul>>8;
		b++;
	}
	update();
}

void GLTexture::params()
{
	use();
	if (wrapping)
	{
  		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}else
	{
  		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
  	if (filtering) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (mipmap) // mipmap
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 
		gluBuild2DMipmaps(GL_TEXTURE_2D,glBpp,xsize,ysize,bpp,GL_UNSIGNED_BYTE, mem);
	}
	else // no mipmap
	{
		if (filtering) glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
		else glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); 
	}
}

void GLTexture::getScreen()
{
	use();
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,xsize,ysize);
	if (mipmap)
	{
		mipmap=false;
		params();
	}
}

void GLTexture::getScreen(int xres, int yres)
{
	use();
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,xres,yres);
	if (mipmap)
	{
		mipmap=false;
		params();
	}
}

void GLTexture::getScreenCenter(int x, int y)
{
	use();
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,x-xsize/2,y-ysize/2,xsize,ysize);
	if (mipmap)
	{
		mipmap=false;
		params();
	}
}

void GLTexture::setWrapping(bool wrap)
{
	if (wrapping=wrap) return;
	wrapping=wrap;
	params();
}
