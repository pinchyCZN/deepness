// EfuShitti.h: interface for the EfuShitti class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFUSHITTI_H__FD93797F_CC5F_4106_9C46_C9C80E679B27__INCLUDED_)
#define AFX_EFUSHITTI_H__FD93797F_CC5F_4106_9C46_C9C80E679B27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OpenglWindow.h"
#include "DrObject.h"
#include "GLFont.h"
#include "GLTexture.h"

const int TX_LEN =40;

class EfuShitti  
{
public:
	void ganzeffu(int count);
	void gantzJuttu(int sides, double rotate, double kirkkaus, double width, double z1, double z2, double texU1, double texU2, double texV1, double texV2, GLTexture *texture);
	void draw_lukko(int count);
	void set_lukko_gl(int count);
	void unset_lukko_gl();
	void lukko(int count);
	void kuoloPlanes(int count, float sync, GLuint texture);
	void d_fogi(GLfloat *color, float dist);

	void kuoloSpirals(int count, float sync, GLuint texture);
	void kuolonKorahdus3(int count, float sync, GLuint texture);
	void kuolonKorahdus2(int count, float sync, GLuint texture);
	void kuolonKorahdus(int count, float sync, GLuint texture);
	void tunneli(int count, int pos);
	void potki(int count, int pos);
	void rappaa2(int count, int pos);
	void colorFade(float r, float g, float b, float a);
	float cuberotaY;
	float cuberotaZ;
	void rappaa(int count, int pos);
	void rend_al_tex(float xcenter, float ycenter, float xscale, float yscale, float brightness, int texn);
	void part_alku(int count);
	void pallon_vemputus(int count, int tex, int env);
	void part5(int count);
	void kuupi(int count, int tex, int env);
	void radialblur(int count, int texn);
	void kirjain_3d(int count, int texn);
	void part3b(int count);
	void part3(int count);
	void part3(int count, int texn);
	void part4(int count);
	void part2(int count);
	void part1(int count, int pos);
	void drawAImage(int time, float stretch, int tn);
	void drawAImage2(int time, float fade, float stretch, int tn);
	void drawAImage3(float x, float y, float fade, float stretch, int tn);
	void drawAImage3b(float x, float y, float roto, float fade, float stretch, int tn);
	void drawAImage4(int time, float roto, float stretch, int tn);
	void toorus2(int count, int tex, int env);
	void fg_pan_ympyra(int count);
	void kirjainNoice();
	void toorus(int count);
	void tahti(int count, int texnum);
	void bg_pan_efu(int count, int texnum);
	void Perspective(float fov, float near, float far);
	void Ortho();
	explicit EfuShitti(OpenglWindow *ogl);
	virtual ~EfuShitti();
	GLTexture *texture[64];

private:
	OpenglWindow *wnd;
	DrObject obut[32];
	GLFont *fontarial;
	GLFont *impact_3d;
	char teksti[TX_LEN][15];

};

#endif // !defined(AFX_EFUSHITTI_H__FD93797F_CC5F_4106_9C46_C9C80E679B27__INCLUDED_)
