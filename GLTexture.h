
/*
  :: oidberg
  parts taken from: Pan/SpinningKids  
*/

#ifndef _GLTEXTURE_H_
#define _GLTEXTURE_H_

#ifdef _MSC_VER
#pragma once
#pragma pack(push, _PACK_GLTEXTURE_H_)
#endif //_MSC_VER

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

class GLTexture {
	unsigned long *mem;
	int bpp, logsize, xsize, ysize;
	bool init;
public:
	GLuint txt;
	void setWrapping(bool wrap);
	void getScreenCenter(int x, int y);
	void getScreen(int xres, int yres);
	void getScreen();
	bool filtering;
	void darken(float val);
	bool mipmap;
	GLTexture(unsigned long *buf, int xres, int yres, int bits, int blending);
	GLTexture(int xres, int yres);
	explicit GLTexture(int _logsize);

	~GLTexture() {
		delete[] mem;
		glDeleteTextures(1,&txt);
	}

	unsigned long *getImage() { return mem; }
	int getXsize() { return xsize; }
	int getYsize() { return ysize; }

	void update();
	void use();
	int getID(){ return txt; }
	void params();
private:
	bool wrapping;
	int glBpp;
};

#ifdef _MSC_VER
#pragma pack(pop, _PACK_GLTEXTURE_H_)
#endif //_MSC_VER

#endif //_GLTEXTURE_H_
