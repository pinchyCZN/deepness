// EfuShitti.cpp: implementation of the EfuShitti class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>

#include "VemputinMain.h"
#include "ModPlay.h"
#include "ImageLoader.h"
#include "EfuShitti.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define PI 3.1416

EfuShitti::EfuShitti(OpenglWindow *ogl)
{
	cuberotaZ=0;

	wnd=ogl;

//	texture[0] = ImageLoader::loadJPG("data\\pusscube.jpg",  3);
//	texture[0] = ImageLoader::loadJPG("data\\envi7.jpg", 3);
//	texture[1] = ImageLoader::loadJPG("data\\metalli2.jpg", 3);
	texture[0] = ImageLoader::loadJPG("data\\phong3.jpg", 3);
	texture[1] = ImageLoader::loadJPG("data\\metalli3.jpg", 3);

	texture[11] = ImageLoader::loadJPG("data\\valo2.jpg", 3);
	texture[12] = ImageLoader::loadJPG("data\\valo2.jpg", 3);

	texture[13] = ImageLoader::loadJPG("data\\noise.jpg", 3);
	texture[13]->filtering=false;
	texture[13]->params();

	texture[14] = ImageLoader::loadJPG("data\\mfx.jpg", 3);
	texture[14]->filtering=false;
	texture[14]->params();

	texture[15] = ImageLoader::loadJPG("data\\deepness.jpg", 3);
	texture[15]->filtering=false;
	texture[15]->params();

//	VITTU KYTÄT	
//	texture[0] = ImageLoader::loadJPG("data\\phong.jpg", 3);
//	texture[1] = ImageLoader::loadJPG("data\\metalli2.jpg", 3);

	texture[2]  = new GLTexture(512,256);
	texture[3]  = new GLTexture(256,256);
	texture[4]  = new GLTexture(128,128);
	texture[5]  = new GLTexture(64,64);
	texture[6]  = new GLTexture(32,32);
	texture[7]  = new GLTexture(16,16);
	texture[8]  = new GLTexture(8,8);
	texture[9]  = new GLTexture(4,4);
	texture[10] = new GLTexture(2,2);
	texture[16] = new GLTexture(512,256);
	texture[16]->setWrapping(false);

	for (int ii=2;ii<11;ii++) 
	{
		texture[ii]->setWrapping(false);
//		texture[ii]->filtering=false;
//		texture[ii]->params();
	}
}

EfuShitti::~EfuShitti()
{
//	delete fontarial;
//	delete impact_3d;
}

void EfuShitti::Ortho()
{
	glViewport(0,0,wnd->scrXres,wnd->scrYres);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, wnd->scrXres, wnd->scrYres, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void EfuShitti::d_fogi(GLfloat *color, float dist)
{
	glFogi(GL_FOG_MODE, GL_LINEAR);		// Fog Mode
	glFogfv(GL_FOG_COLOR, color);			// Set Fog Color
	glFogf(GL_FOG_DENSITY, 1.0f);				// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);			// Fog Hint Value
	glFogf(GL_FOG_START, 1.0f);				// Fog Start Depth
	glFogf(GL_FOG_END, dist);				// Fog End Depth
	glEnable(GL_FOG);					// Enables GL_FOG

}

void EfuShitti::Perspective(float fov, float nearr, float farr)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();						
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();						
	gluPerspective(fov, (float)wnd->scrXres/wnd->scrYres, nearr, farr);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();						
}

void EfuShitti::bg_pan_efu(int count, int texnum)
{
	Ortho();

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	GLfloat white[4] = {1.0,1.0,1.0,1.0};
//	GLfloat white[4] = {0.9,0.9,0.9,0.5};
	GLfloat black[4] = {1.0,1.0,1.0,1.0};

	wnd->glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	texture[texnum]->use();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	wnd->glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	texture[texnum]->use();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	wnd->glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	texture[texnum]->use();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	wnd->glActiveTextureARB(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	texture[texnum]->use();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

//	glBlendFunc(GL_SRC_COLOR,GL_DST_ALPHA);
	glBlendFunc(GL_ONE,GL_ONE);
	glEnable(GL_BLEND);
	glColor4f(0.03f,0.03f,0.03f,0.1f);
//	glColor4f(0,0,0,0);

	glPushMatrix();

	float xadd=-cos(count*0.00034);
	float yadd=sin(count*0.00024);


	glBegin(GL_QUADS);
	for(int i = 2; i < 18; i++) 
	{
		float f = count*0.0015f+i*0.002f;
		float k ;// = i*0.05f*(2.f+sinf(count*0.00245892f));
		k=(i-10.5f)*0.005f;
		float sfk = sinf(f)*k;
		float cfk = cosf(f)*k;

		i++;
		float f2 = count*0.0015f+i*0.002f;
		float k2 ;// = i*0.05f*(2.f+sinf(count*0.00245892f));
		k2=(i-10.5f)*0.005f;
		float sfk2 = sinf(f2)*k2;
		float cfk2 = cosf(f2)*k2;

		i++;
		float f3 = count*0.0015f+i*0.002f;
		float k3 ;// = i*0.05f*(2.f+sinf(count*0.00245892f));
		k3=(i-10.5f)*0.005f;
		float sfk3 = sinf(f3)*k3;
		float cfk3 = cosf(f3)*k3;

		i++;
		float f4 = count*0.0015f+i*0.002f;
		float k4 ;// = i*0.05f*(2.f+sinf(count*0.00245892f));
		k4=(i-10.5f)*0.005f;
		float sfk4 = sinf(f3)*k4;
		float cfk4 = cosf(f3)*k4;

//			float c=(1.f-(abs(i-20)/20.f))*0.01;

//		glColor3f(0.03f,0.03f,0.03f);
//		glColor3f(0.06f,0.06f,0.06f);
		
//		glTexCoord2f(sfk+xadd, cfk+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE0_ARB, sfk+xadd,cfk+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE1_ARB, sfk2+xadd,cfk2+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE2_ARB, sfk3+xadd,cfk3+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE3_ARB, sfk4+xadd,cfk4+yadd);
		glVertex2f(0, 0);
	
//		glTexCoord2f(1.f-cfk+xadd, sfk+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 2.f-cfk+xadd,sfk+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 2.f-cfk2+xadd,sfk2+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 2.f-cfk3+xadd,sfk3+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 2.f-cfk4+xadd,sfk4+yadd);
		glVertex2f(wnd->scrXres, 0);
		
//		glTexCoord2f(1.f-sfk+xadd, 0.75f-cfk+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 2.f-sfk+xadd,2.f-cfk+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 2.f-sfk2+xadd,2.f-cfk2+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 2.f-sfk3+xadd,2.f-cfk3+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 2.f-sfk4+xadd,2.f-cfk4+yadd);
		glVertex2f(wnd->scrXres, wnd->scrYres);
		
//		glTexCoord2f(cfk+xadd, 0.75f-sfk+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE0_ARB, cfk+xadd,2.f-sfk+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE1_ARB, cfk2+xadd,2.f-cfk2+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE2_ARB, cfk3+xadd,2.f-cfk3+yadd);
		wnd->glMultiTexCoord2fARB(GL_TEXTURE3_ARB, cfk4+xadd,2.f-cfk4+yadd);
		glVertex2f(0, wnd->scrYres);
	}
	glEnd();

	glPopMatrix();


	wnd->glActiveTextureARB(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_2D);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	wnd->glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	wnd->glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	wnd->glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void EfuShitti::toorus(int count)
{
	Perspective(90.0,0.1,1500);

	GLfloat jasso_ambient[4]  = {0.0, 0.0, 0.0, 0.0};
	GLfloat jasso_diffuse[4]  = {1.0, 1.0, 1.0, 1.0};
	GLfloat jasso_specular[4] = {1.0, 1.0, 1.0, 1.0};
	GLfloat jasso_position[4] = {0.0, 0.0, -600.0, 1.0};
	GLfloat jasso_emission[4] = {1.0, 1.0, 1.0, 1.0};
/*
	jasso_position[0]=sin(count*0.003)*300.0;
	jasso_position[1]=sin(count*0.004)*300.0;
	jasso_position[2]=sin(count*0.005)*300.0-200.0;
*/
/*
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT, jasso_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jasso_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,jasso_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,jasso_position);
//	glLightfv(GL_LIGHT0,GL_EMISSION,jasso_emission);
//	glLightf(GL_LIGHT0,GL_SHININESS,20.5);
*/
	glShadeModel(GL_SMOOTH);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	GLfloat white[4] = {1.0,1.0,1.0,1.0};
	GLfloat black[4] = {1.0,1.0,1.0,1.0};

	wnd->glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
//	texture[19]->use();
	texture[17]->use();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
//	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

	wnd->glActiveTextureARB(GL_TEXTURE1_ARB);
	
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	texture[20]->use();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glColor4f(1,1,1,1);

	glPushMatrix();

	glTranslatef(0,0,-600);

	glRotatef(count/10.0,1,2.35,2.75);

//	obut[0].drawList();
	obut[0].base2roto();
	obut[0].wobble4(count*10,120.0,10.0);
//	obut[0].calc_rnormals();
	obut[0].render();

	glPopMatrix();

	wnd->glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	wnd->glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void EfuShitti::tahti(int count, int texnum)
{
	Perspective(90.0,0.1,1500);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
//	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_TEST);
//	glDepthMask(GL_TRUE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	texture[texnum]->use();
	
//	glColor4f(0.1,0.1,0.1,1);
	glColor4f(1,1,1,1);

	glPushMatrix();

	glTranslatef(0,0,-600);
	glScalef(0.75,0.75,0.75);
	glRotatef(count/12.8,-1.32,-2.15,-1.58);

	obut[1].drawList();
	//obu.render();

	glPopMatrix();
}


void EfuShitti::kirjainNoice()
{
	Ortho();
	texture[1]->use();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glColor3f(1,1,1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_COLOR);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	float sizeMul=wnd->scrYres/480.0f;

	for (int x=0;x<=32;x++)
	{
//		if (abs(x-16)+(rand()%24)>22)
			for (int y=0;y<=24;y++)
			{
//				if (abs(y-12)+(rand()%18)>16)
				float dist=(y-12)*(y-12)+(x-16)*(x-16);
				if (dist+rand()%256>256)
				{
					glPushMatrix();
					glTranslatef(wnd->scrXres*x/32.f+rand()*20.0/65536.0,wnd->scrYres*y/24.f+rand()*20.0/65536.0,0);
					fontarial->printChar((rand()%96)+32,sizeMul*dist/2,sizeMul*dist/3,rand()%360,0);

					glPopMatrix();
				}
			}
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

void EfuShitti::fg_pan_ympyra(int count)
{
	Ortho();
	texture[1]->use();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

	static char *string = "Fatshit  Niggaz  Bitch    ";
	static char *string2 = "Blowjobbbb             ";
	if (count>100000) string=string2;
//	static char *string = "---------------------------------------------------------------";
	int len = strlen(string);
	float anglestep = 360.f/len;
	float base = count*0.01f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);

	for(int i = 0; i < len; i++) 
	{
		glPushMatrix();
		float angle=base+anglestep*i;
		float anglerad = angle*0.01745329252f;
		float dist = wnd->scrYres/2.5f;
		float dx = wnd->scrXres/2+dist*cosf(anglerad)*(0.9+0.15*cosf(anglerad*3+count*0.0015));
		float dy = wnd->scrYres/2+dist*sinf(anglerad)*(0.9+0.15*sinf(anglerad*3+count*0.0015));
		float xsize = sinf(count*0.001382f+i*0.2984f)*20.f+30.f;
		float cmul=sinf(anglerad*3+count*0.0017)*0.5+0.5;
		cmul=1;

		angle-=30.f*cosf(anglerad*3+count*0.0015+0.2);
		glTranslatef(dx, dy, 0.f);
		float sizeMul=wnd->scrYres/480.0f;
		for(int x = -1; x <= 1; x++) 
		{
			for(int y = -1; y <= 1; y++) 
			{
				float c = 0.7f*expf(-0.5f*(x*x+y*y)); // gaussian-like filtering
				c*=cmul*0.3;
				if (x+y>-2)
				{
					glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
					glColor4f(c,c,c,1);
				}else
				{
					glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_COLOR);
					glColor4f(c,c,c,1);
				}
				glRasterPos2f(static_cast<float>(x), static_cast<float>(y));
				glPushMatrix();
				glRotatef(rand()*50.0/65536.0-25.0,0,0,1);
//				fontarial->printChar(string[i], xsize*2+20, xsize*3-170, angle+90, 0);
				fontarial->printChar(string[i], 80*sizeMul, -80*sizeMul, angle+90, 0);
				glPopMatrix();
			}
		}
		glPopMatrix();
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

void EfuShitti::toorus2(int count, int tex, int env)
{
	Perspective(80.0,0.1,1500);

	GLfloat jasso_ambient[4]  = {0.0, 0.0, 0.0, 0.0};
	GLfloat jasso_diffuse[4]  = {1.0, 1.0, 1.0, 1.0};
	GLfloat jasso_specular[4] = {1.0, 1.0, 1.0, 1.0};
	GLfloat jasso_position[4] = {0.0, 0.0, -600.0, 1.0};
	GLfloat jasso_emission[4] = {1.0, 1.0, 1.0, 1.0};
/*
	jasso_position[0]=sin(count*0.003)*300.0;
	jasso_position[1]=sin(count*0.004)*300.0;
	jasso_position[2]=sin(count*0.005)*300.0-200.0;
*/

/*	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT, jasso_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jasso_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,jasso_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,jasso_position);
//	glLightfv(GL_LIGHT0,GL_EMISSION,jasso_emission);
//	glLightf(GL_LIGHT0,GL_SHININESS,20.5);
*/
	glShadeModel(GL_SMOOTH);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_COLOR,GL_SRC_COLOR);

	GLfloat white[4] = {1.0,1.0,1.0,1.0};
	GLfloat black[4] = {1.0,1.0,1.0,1.0};

	wnd->glActiveTextureARB(GL_TEXTURE0_ARB);
	texture[tex]->use();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
//	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);
//	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
//	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

	wnd->glActiveTextureARB(GL_TEXTURE1_ARB);
	
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_NV);
//	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_NV);

	texture[env]->use();
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_EXT);

	glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_RGB_EXT,GL_ADD_SIGNED_EXT);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_RGB_EXT,GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_RGB_EXT,GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE1_RGB_EXT,GL_PREVIOUS_EXT);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND1_RGB_EXT,GL_SRC_COLOR);

	// alpha
	glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_ALPHA_EXT,GL_REPLACE);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA_EXT,GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_ALPHA_EXT,GL_SRC_ALPHA);

/*
	wnd->glActiveTextureARB(GL_TEXTURE2_ARB);
	
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	texture[env]->use();
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_EXT);

	glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_RGB_EXT,GL_ADD_SIGNED_EXT);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_RGB_EXT,GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_RGB_EXT,GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE1_RGB_EXT,GL_PREVIOUS_EXT);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND1_RGB_EXT,GL_SRC_COLOR);

	// alpha
	glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_ALPHA_EXT,GL_REPLACE);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA_EXT,GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_ALPHA_EXT,GL_SRC_ALPHA);
*/
	glColor4f(1,1,1,1);

//	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glRotatef(-30,1,0,0);
	glTranslatef(0,200,-300);
	glRotatef(180,0,0,1);
	glRotatef(count*0.02,0,1,0);
//	glRotatef(sin(count*0.000513)*90.0,1,0,0);
//	glRotatef(sin(count*0.000743)*90.0,0,1,0);
//	glRotatef(sin(count*0.000473)*90.0,0,0,1);
//	glTranslatef(cos(count*0.000314)*256,-sin(count*0.000415)*256,sin(count*0.000485)*256);
//	glTranslatef(0,0,-1000);
//	glTranslatef(0,0,-300);
//	glTranslatef(0,0,-100);

//	glMatrixMode(GL_MODELVIEW);

//	glPushMatrix();
	
//	glRotatef(count/40.0,1,2.35,2.75);

//	obut[2].drawList();



/*	obut[2].base2roto();
	obut[2].wobble2(count*3.0,30.0,100.0);
	obut[2].calc_rnormals();
*/
//	obut[2].base2roto();
//	obut[2].wobble3(count*4.0,210.0,80.0);
//	obut[2].calc_rnormals();
/*	obut[2].wobble1(count*3.1,150.0,60.0);
	obut[2].calc_rnormals();
	obut[2].wobble3(-count*5.3,110.0,30.0);
	obut[2].calc_rnormals();
*/	
//	obut[2].wobble4(-count*3.3,250.0,100.0);
//	obut[2].calc_rnormals();

//	obut[2].render();
	obut[2].drawList();

//	glScalef(0.5,0.5,0.5);
//	glEnable(GL_POINT_SMOOTH);
/*
static GLfloat constant[3] = { 1/5.0, 0.0, 0.0 };
static GLfloat linear[3] = { 0.0, 1/5.0, 0.0 };
static GLfloat quadratic[3] = { 0.0001, 0.0, 0.1/100.0 };
    wnd->glPointParameterfvEXT(GL_DISTANCE_ATTENUATION_EXT, quadratic);
    wnd->glPointParameterfEXT(GL_POINT_FADE_THRESHOLD_SIZE_EXT, 1.0);
	glPointSize (10000.0);
*/
//	glPointSize (20.0);
//	obut[2].renderPoints();

//	glRotatef(130,1,1,1);
	glScalef(0.3,0.3,0.3);
//	glScalef(0.2,0.2,0.2);
//	obut[2].flipNormals();
//	obut[2].render();
//	obut[2].flipNormals();

	glRotatef(cuberotaZ,0,0,1);
	glRotatef(cuberotaY,1,0,0);
	obut[3].drawList();

//	obut[2].render();

	glPopMatrix();

//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();

	wnd->glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	wnd->glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	wnd->glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}

void EfuShitti::drawAImage(int time, float stretch, int tn)
{
	Ortho();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	texture[tn]->use();

//	float xAdd=(640.0/480.0)*wnd->scrXres/8;
	float xAdd=(1.0-stretch)*wnd->scrXres/8;

	float scaleY=sin(time*0.001)*0.30+0.70;
	float scaleX=sin(time*0.001)*0.30+0.70;

	float xhalf=wnd->scrXres/2;
	float yhalf=wnd->scrYres/2;

	glTranslatef(xhalf,yhalf,0);

//	scaleX=1;
//	scaleY=1;

	//	glScalef(scaleX,scaleY,1);
//	glTranslatef((1-scaleX)*wnd->scrXres,(1-scaleY)*2*wnd->scrYres,0);
	glColor4f(1,1,1,scaleX);
//	glColor4f(1,1,1,1.5-scaleX);

//	scaleX*=0.75;
//	scaleY*=0.75;

	scaleX=1-scaleX;
	scaleY=1-scaleY;
	scaleX*=(wnd->scrXres/2-xAdd);
	scaleY*=wnd->scrYres;

//	glRotatef(time*0.01,0,0,1);


	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(xAdd+scaleX-xhalf, scaleY-yhalf);

	glTexCoord2f(0.99999,0);
	glVertex2f(xhalf-xAdd-scaleX, scaleY-yhalf);
	
	glTexCoord2f(0.99999,0.99999);
	glVertex2f(xhalf-xAdd-scaleX, yhalf);
	
	glTexCoord2f(0,0.99999);
	glVertex2f(xAdd+scaleX-xhalf, yhalf);

	glEnd();
}

void EfuShitti::drawAImage4(int time, float roto, float stretch, int tn)
{
	Ortho();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	texture[tn]->use();

//	float xAdd=(640.0/480.0)*wnd->scrXres/8;
	float xAdd=(1.0-stretch)*wnd->scrXres/8;

	float scaleY=sin(time*0.001)*0.30+0.70;
	float scaleX=sin(time*0.001)*0.30+0.70;

	float xhalf=wnd->scrXres/2;
	float yhalf=wnd->scrYres/2;

	glTranslatef(xhalf,yhalf,0);

//	scaleX=1;
//	scaleY=1;

	//	glScalef(scaleX,scaleY,1);
//	glTranslatef((1-scaleX)*wnd->scrXres,(1-scaleY)*2*wnd->scrYres,0);
	glColor4f(1,1,1,scaleX);
//	glColor4f(1,1,1,1.5-scaleX);

//	scaleX*=0.75;
//	scaleY*=0.75;

	scaleX=1-scaleX;
	scaleY=1-scaleY;
	scaleX*=(wnd->scrXres/2-xAdd);
	scaleY*=wnd->scrYres;

	glRotatef(roto,0,0,1);


	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(xAdd+scaleX-xhalf, scaleY-yhalf);

	glTexCoord2f(0.99999,0);
	glVertex2f(xhalf-xAdd-scaleX, scaleY-yhalf);
	
	glTexCoord2f(0.99999,0.99999);
	glVertex2f(xhalf-xAdd-scaleX, yhalf);
	
	glTexCoord2f(0,0.99999);
	glVertex2f(xAdd+scaleX-xhalf, yhalf);

	glEnd();
}

void EfuShitti::drawAImage2(int time, float fade, float stretch, int tn)
{
	Ortho();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	texture[tn]->use();

//	float xAdd=(640.0/480.0)*wnd->scrXres/8;
	float xAdd=(1.0-stretch)*wnd->scrXres/8;

	float xhalf=wnd->scrXres/2;
	float yhalf=wnd->scrYres/2;

	glTranslatef(xhalf,yhalf,0);

	glColor4f(1,1,1,fade);

//	float scaleX=0*(wnd->scrXres/2-xAdd);
//	float scaleY=0*wnd->scrYres;
	float scaleX=0;
	float scaleY=0;

//	glRotatef(time*0.01,0,0,1);


	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(xAdd+scaleX-xhalf, scaleY-yhalf);

	glTexCoord2f(0.99999,0);
	glVertex2f(xhalf-xAdd-scaleX, scaleY-yhalf);
	
	glTexCoord2f(0.99999,0.99999);
	glVertex2f(xhalf-xAdd-scaleX, yhalf);
	
	glTexCoord2f(0,0.99999);
	glVertex2f(xAdd+scaleX-xhalf, yhalf);

	glEnd();
}

void EfuShitti::drawAImage3(float x, float y, float fade, float stretch, int tn)
{
	Ortho();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	texture[tn]->use();

//	float xAdd=(640.0/480.0)*wnd->scrXres/8;
	float xAdd=(1.0-stretch)*wnd->scrXres/8;

	float xhalf=wnd->scrXres/2;
	float yhalf=wnd->scrYres/2;

	glTranslatef(xhalf+x*xhalf,yhalf+y*yhalf,0);

	glColor4f(1,1,1,fade);

//	float scaleX=0*(wnd->scrXres/2-xAdd);
//	float scaleY=0*wnd->scrYres;
	float scaleX=0;
	float scaleY=0;

//	glRotatef(time*0.01,0,0,1);


	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(xAdd+scaleX-xhalf, scaleY-yhalf);

	glTexCoord2f(0.99999,0);
	glVertex2f(xhalf-xAdd-scaleX, scaleY-yhalf);
	
	glTexCoord2f(0.99999,0.99999);
	glVertex2f(xhalf-xAdd-scaleX, yhalf);
	
	glTexCoord2f(0,0.99999);
	glVertex2f(xAdd+scaleX-xhalf, yhalf);

	glEnd();
}

void EfuShitti::drawAImage3b(float x, float y, float roto, float fade, float stretch, int tn)
{
	Ortho();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	texture[tn]->use();

//	float xAdd=(640.0/480.0)*wnd->scrXres/8;
	float xAdd=(1.0-stretch)*wnd->scrXres/8;

	float xhalf=wnd->scrXres/2;
	float yhalf=wnd->scrYres/2;

	glTranslatef(xhalf+x*xhalf,yhalf+y*yhalf,0);

	glColor4f(1,1,1,fade);

//	float scaleX=0*(wnd->scrXres/2-xAdd);
//	float scaleY=0*wnd->scrYres;
	float scaleX=0;
	float scaleY=0;

//	glRotatef(time*0.01,0,0,1);

	glRotatef(roto,0,0,1);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(xAdd+scaleX-xhalf, scaleY-yhalf);

	glTexCoord2f(0.99999,0);
	glVertex2f(xhalf-xAdd-scaleX, scaleY-yhalf);
	
	glTexCoord2f(0.99999,0.99999);
	glVertex2f(xhalf-xAdd-scaleX, yhalf);
	
	glTexCoord2f(0,0.99999);
	glVertex2f(xAdd+scaleX-xhalf, yhalf);

	glEnd();
}


void EfuShitti::kirjain_3d(int count, int texn)
{
	Perspective(100.0,0.01,500);
	glDisable(GL_TEXTURE_2D);
//	texture[texn]->use();

//	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);

//	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
//	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glColor3f(1,1,1);
	glColor3f(0,0,0);
	glDisable(GL_BLEND);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_COLOR,GL_SRC_COLOR);
//	glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_CULL_FACE);

	float yaddi=(count%1000)/1000.0;
	int rnum=count/1000-1;

	for (int z=0;z<2;z++)
	{
		if (z==1) 
		{
			glDisable(GL_BLEND);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1,0,0,0.5);
//			glColor4f(0,0,0,1);
			
		}
		else 
		{
			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//			glColor4f(0.75,0.75,0.75,1.0);
//			glColor4f(1,0,0,0.5);
			glColor4f(0,0,0,0.5);
		}

		for (int j=0;j<11;j++)
		{
			int linenum=rnum-j;
			if (linenum>=0)
			{
				glPushMatrix();
				glRotatef((j-(10+1)/2.0+yaddi)*12,1,0,0);

				float rotor=-(j-(10+1)/2.0+yaddi)*3-10.0;
				if (rotor>0) glRotatef(rotor*15,0,1,0.5);
				int ln=15;
				char *tx = (char *)teksti[linenum%TX_LEN];
				ln=strlen(tx);
				for (int i=0;i<ln;i++)
				{
					glPushMatrix();
			//		glRotatef(count*0.15-i*10,1,0,0);
			//		glRotatef(count*0.10-i*10,0,1,0);
			//		glRotatef(count*0.06-i*10,0,0,1);
					glRotatef(-(i-(ln-1)/2.0)*8,0,1,0);
					glTranslatef(0,0,-200);

//					glRotatef(sin((count*0.05+i*10+linenum*10)*0.05)*15,0,1,0);
//					glTranslatef(0,0,sin((count*0.05+i*10+linenum*10)*0.1)*30);
					
					glTranslatef(0,0,3.5);
					if (z==0) glScalef(2.5,2.5,1);
					 glTranslatef(0,0,sin((count*0.05+i*10+linenum*10)*0.1)*30);
					 glRotatef(rotor*25,0,1,1);
						//glRotatef(sin((count*0.05+i*10+linenum*10)*0.05)*25,0,1,0);
					float track=impact_3d->printChar(tx[i],50,50,0,0);
					glPopMatrix();
			//		glTranslatef(track*50,0,0);
				}
				glPopMatrix();
			}
		}
	}
		
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void EfuShitti::radialblur(int count, int texn)
{
	Ortho();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	texture[texn]->use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);

	glColor4f(0.5,0.5,0.5,1.0);

	glPushMatrix();

	glBegin(GL_QUADS);
	for(int i = 0; i < 20; i++) 
	{
		float ska=i*0.01;
		glPushMatrix();
		glRotatef(i*i*10,0,0,1);

		glTexCoord2f(-ska,1+ska);
		glVertex2f(0, 0);
	
		glTexCoord2f(1+ska,1+ska);
		glVertex2f(wnd->scrXres, 0);
		
		glTexCoord2f(1+ska,-ska);
		glVertex2f(wnd->scrXres, wnd->scrYres);
		
		glTexCoord2f(-ska,-ska);
		glVertex2f(0, wnd->scrYres);
		glPopMatrix();
	}
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void EfuShitti::kuupi(int count, int tex, int env)
{
	Perspective(80.0,0.1,1500);

	glShadeModel(GL_SMOOTH);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	GLfloat white[4] = {1.0,1.0,1.0,1.0};
	GLfloat black[4] = {1.0,1.0,1.0,1.0};

//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
//	glDisable(GL_DEPTH_TEST);

	texture[tex]->use();
	glColor4f(1,1,1,1);

	glPushMatrix();

//	glRotatef(-30,1,0,0);
	glTranslatef(0,0,-650);
	glRotatef(180,0,0,1);
	glRotatef(count*0.01,0,1,0);

	glScalef(1.5,1.5,1.5);

	obut[4].tx_scrollX=count*0.000095;
	obut[4].tx_scrollY=-count*0.000083;
	obut[4].render();

	glPopMatrix();

	wnd->glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	wnd->glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}

void EfuShitti::pallon_vemputus(int count, int tex, int env)
{
	Perspective(90.0,0.1,1500);
//	Perspective(90.0,0.1,2500);

	glShadeModel(GL_SMOOTH);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	GLfloat white[4] = {1.0,1.0,1.0,1.0};
	GLfloat black[4] = {1.0,1.0,1.0,1.0};

	wnd->glActiveTextureARB(GL_TEXTURE0_ARB);
	texture[tex]->use();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	wnd->glActiveTextureARB(GL_TEXTURE1_ARB);
	
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	texture[env]->use();
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_EXT);

	glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_RGB_EXT,GL_ADD_SIGNED_EXT);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_RGB_EXT,GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_RGB_EXT,GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE1_RGB_EXT,GL_PREVIOUS_EXT);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND1_RGB_EXT,GL_SRC_COLOR);

	// alpha
	glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_ALPHA_EXT,GL_MODULATE);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA_EXT,GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_ALPHA_EXT,GL_SRC_ALPHA);


/*	wnd->glActiveTextureARB(GL_TEXTURE2_ARB);
	
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	texture[env]->use();
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_EXT);

	glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_RGB_EXT,GL_ADD_SIGNED_EXT);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_RGB_EXT,GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_RGB_EXT,GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE1_RGB_EXT,GL_PREVIOUS_EXT);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND1_RGB_EXT,GL_SRC_COLOR);

	// alpha
	glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_ALPHA_EXT,GL_REPLACE);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA_EXT,GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_ALPHA_EXT,GL_SRC_ALPHA);
*/
	glColor4f(1,1,1,1);

	glPushMatrix();

	glTranslatef(0,0,-1000);
	glRotatef(count*0.017,1,0,0);
	glRotatef(count*0.023,0,1,0);
	glRotatef(count*0.029,0,0,1);

	obut[5].base2roto();
	obut[5].wobble3(count*4.0,210.0,80.0);
	obut[5].calc_rnormals();
	obut[5].wobble1(count*3.1,150.0,60.0);
	obut[5].calc_rnormals();
	obut[5].wobble3(-count*5.3,110.0,30.0);
	obut[5].calc_rnormals();

	glScalef(0.9,0.9,0.9);

	obut[5].toList();

	glPushMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	obut[5].flipNormals();
	glScalef(3.0,3.0,3.0);
	obut[5].render();
	obut[5].flipNormals();
	glPopMatrix();

//	obut[5].render();
	obut[5].drawList();

	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);
	glBlendFunc(GL_DST_COLOR,GL_ZERO);
//	glColor4f(0.5,0.5,0.5,0.5);

	glDisable(GL_DEPTH_TEST);

//	glPushMatrix();
	for (int xx=0;xx<7;xx++)
	{
		glScalef(1.03,1.03,1.03);
//		obut[5].render();
		obut[5].drawList();
	}
//	glPopMatrix();

//	glColor4f(0.5,0.5,0.5,0.5);
	glColor4f(1.0,1.0,1.0,0.5);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
//	obut[5].render();
	obut[5].drawList();



	glPopMatrix();
	wnd->glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	wnd->glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	wnd->glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, black);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}


void EfuShitti::part1(int count, int pos)
{
	bool homo=false;
	if (pos/64/16%2==0) homo=true;

	int x=wnd->scrXres;
	int y=wnd->scrYres;
	wnd->resizeOpenGL(512,256);
	toorus2(-count*2,10,4);
	if (homo)	drawAImage(count, 0.5, 9);
	else		drawAImage(count, 1, 11);
	texture[7]->getScreen();
	wnd->resizeOpenGL(x,y);

	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	toorus2(count,7,8);
//	drawAImage(count, 0.8, 12);
	if (pos/64/64<15)
	{
		if (!homo)	drawAImage(count*3, 0.5, 13);
		else drawAImage(count*3, 0.5, 14);
	}else potki(count,pos);


	texture[10]->getScreenCenter(wnd->scrXres/2,wnd->scrYres/2);

}

void EfuShitti::part2(int count)
{
	toorus(-count/4);
	kuupi(count,19,17);
	tahti(count/3,2);
	tahti(-count/2,2);
//	fg_pan_ympyra(count);
//	drawAImage(count,0.5,9);
	kirjainNoice();
}

void EfuShitti::part3(int count)
{
	part3(count,16);
}

void EfuShitti::part3b(int count)
{
	toorus2(count,16,4);
}
void EfuShitti::part3(int count, int texn)
{
//	toorus2(count,16,4);
	toorus2(count,texn,4);
//	toorus2(count,16,8);
//	toorus(count/4);
//	toorus(-count/3);
//	toorus(count/5);
//	drawAImage(count,0.8,12);
//	fg_pan_ympyra(count);
	kirjainNoice();
}

void EfuShitti::part4(int count)
{
/*	int x=wnd->scrXres;
	int y=wnd->scrYres;
	wnd->resizeOpenGL(512,256);

	kirjain_3d(count,8);

	texture[7]->getScreen();
	
	wnd->resizeOpenGL(x,y);

	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
*/	
//	bg_pan_efu(count,15);
//	radialblur(count,7);
//	drawAImage2(count,1,1.0,21+(count/2000%4));
	drawAImage2(0,1,1,37);
	kirjain_3d(count,8);
//	tahti(count,7);
	fontarial->printChar('x',1,1,0,1);
}

void EfuShitti::part5(int count)
{
	pallon_vemputus(count,25,20);
//	drawAImage(count,0.8,12);
}


void EfuShitti::part_alku(int count)
{
//	Ortho();
	Perspective(90,0.1,100.0);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);//
//	glBlendFunc(GL_SRC_COLOR, GL_ZERO);

	int x=wnd->scrXres;
	int y=wnd->scrYres;
	wnd->resizeOpenGL(512,256);


	glColor4f(1.0,1.0,1.0,1.0);

	texture[27]->use();
	rend_al_tex(0,0,-1,1,0,0);
	
	texture[26]->use();

	float heil=-cos(count*0.0014)*0.5;
//	heil=count/2000.0-2.5;
//	heil=-1;
	
//	glColor4f(1,1,1,count/200000.0-0.01);
//	glColor4f(1.0,1.0,1.0,0.1);
//	glColor4f(0.1,0.1,0.1,1.0);
//	glColor4f(1.0,1.0,1.0,1.0);

//	for (int xxx=0;xxx<40;xxx+=2)
//	{
	glPushMatrix();

		int xx=40;
		float skale=xx*heil+1;
		skale=heil-1+sin(xx+count*0.01)*0.1;
		skale=-1-xx*0.01;
//		rend_al_tex(xx*0.05,0,skale,-1,0.05,26);

		glRotatef(sin(count*0.0013)*14.0,0,0,xx);
		glScalef(0.5,0.3,1.0);
//		rend_al_tex(count/1000.0-3.0,xx*0.05*sin(count*0.004),skale,-1+xx*0.03,0.05,26);
		rend_al_tex(xx*0.01*cos(xx*0.1-count*0.0023),xx*0.02*sin(count*0.004),skale,-1,0.05,26);
//	}

	glPopMatrix();

	texture[27]->getScreen();
	
	wnd->resizeOpenGL(x,y);

	glColor4f(1.0,1.0,1.0,1.0);
	texture[27]->use();
	rend_al_tex(0,0,-1,1,0,0);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

//	rend_al_tex(0,0,sin(count*0.0013)*0.5+1.0,cos(count*0.0018)*0.5+1.0,1.0,26);
//	rend_al_tex(0,0,1,1,1,26);
}

void EfuShitti::rend_al_tex(float xcenter, float ycenter, float xscale, float yscale, float brightness, int texn)
{
	int halfx=wnd->scrXres/2;
	int halfy=wnd->scrYres/2;
	halfx=1.0;
	halfy=1.0;

//	glColor4f(brightness,brightness,brightness,1.0);

	glPushMatrix();

	glTranslatef(xcenter*halfx,ycenter*halfy,0);
	glScalef(xscale,yscale,1);
	glTranslatef(xcenter*halfx,ycenter*halfy,-1);
//	glScalef(0.5,0.3,1);
	
	glBegin(GL_QUADS);

	for(int i = 0; i < 20; i++) 
	{
		glTexCoord2f(1,0);
		glVertex3f(-halfx, -halfy,0);
	
		glTexCoord2f(0,0);
		glVertex3f(halfx, -halfy,0);
		
		glTexCoord2f(0,1);
		glVertex3f(halfx,halfy,0);
		
		glTexCoord2f(1,1);
		glVertex3f(-halfx,halfy,0);
	}
	glEnd();

	glPopMatrix();
}

void EfuShitti::rappaa(int count, int pos)
{
	int xkk=pos/64;
	float s=sin(xkk*4.6);
	float m1=sin(count*s*0.0092)*0.05+0.05;
	float mx=sin(count*0.0017)*0.05+0.05;
	float my=cos(-count*0.0013)*0.05+0.05;
	drawAImage3(mx,my,1,0.1,35);
	drawAImage3(mx,my-m1,1,0.1,36);
}

void EfuShitti::colorFade(float r, float g, float b, float a)
{
//	Ortho();
	Perspective(90,0.1,100.0);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

//	texture[texn]->use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(r,g,b,a*0.2);
	glColor4f(r,g,b,a);

	glBegin(GL_QUADS);
//	for(int i = 0; i < 20; i++) 
//	{
//		float ska=i*0.01;
/*
		glVertex2f(0, 0);
	
		glVertex2f(wnd->scrXres, 0);
		
		glVertex2f(wnd->scrXres, wnd->scrYres);
		
		glVertex2f(0, wnd->scrYres);
*/
		glVertex3f(-2, -1,-1);
	
		glVertex3f(2, -1,-1);
		
		glVertex3f(2, 1,-1);
		
		glVertex3f(-2, 1,-1);

//	}
	glEnd();
	glDisable(GL_BLEND);
}

void EfuShitti::rappaa2(int count, int pos)
{
	int xkk=pos/64;
	float s=sin(xkk*4.6);
	float m1=sin(count*s*0.0092)*0.05+0.05;
	float m2=cos(count*0.0022)*0.2+0.2;
	float mx=sin(count*0.0017)*0.05+0.05;
	float my=cos(-count*0.0013)*0.05+0.05;
	drawAImage3(mx,my,1,0.1,30);
	drawAImage3(mx-m1*0.7,my+m1,1,0.1,31);
	drawAImage3(mx,my+m2,1,0.1,32);

}

void EfuShitti::potki(int count, int pos)
{
	static int suunta[] = {-1,-1,1,-1,1,1,1,-1,1,-1,-1,1,-1,-1,-1,1,-1,1,1,1,1,-1,1,-1,-1,1,-1,1,-1,-1,-1,1,1};
	pos+=64*4;
	int xkk=pos/64;
	float s=sin(xkk*4.6);
	float mx=sin(count*0.0017)*0.05+0.05;
	float my=cos(-count*0.0013)*0.05+0.05;
	float siirtyma=(pos%(64*16))-(64*12);	
	siirtyma=siirtyma*siirtyma*0.00001;
//	siirtyma=0;
	float xs=siirtyma;
	float ys=siirtyma;
	xs*=suunta[(pos/64/16)%32];
	ys*=suunta[(pos/64/16+16)%32];
	float py=count*0.25*suunta[(pos/64/16+8)%32];
	int inum=38;
	if ((pos/64)%16>=12) inum=39;
	drawAImage3(mx,my,1,0.1,inum);
	float feid=1.0-((pos%(64*16))-(64*12))/256.0;
	if ((pos/64)%16>=12) drawAImage3b(mx+xs-0.5,my+ys+0.2,py, feid,0.1,40);
}

void EfuShitti::tunneli(int count, int pos)
{
	glClearColor(0.5,0.5,0.5,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
//	colorFade(0.5,0.5,0.5,1);
	Perspective(90,0.1,1000.0);
	texture[40]->use();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};		// Fog Color
	d_fogi(fogColor, 1000.0);

	float yadd=(count%500)*100/500.0;
	int ykoh=count/500;

	glRotatef(sin(count*0.0001)*45.0,0,0,1);

	glBegin(GL_QUADS);
	for (int y=0;y<24;y++)
	{
		float yran=sin((y+ykoh)*0.4)*75.0+75.0;
		float xmul=sin(-(y+ykoh)*0.153)*25.0+25.0;
		for (int x=0;x<32;x++)
		{
			float xran=cos((x+(y+ykoh)*0.2)*2*16/PI)*xmul+50.0;
			float ran=xran+yran+10.0;
			glTexCoord2f(0,0);
			glVertex3f(cos((x+0)*16/PI)*ran,sin((x+0)*16/PI)*ran,yadd-(y+1)*100);
			glTexCoord2f(1,0);
			glVertex3f(cos((x+1)*16/PI)*ran,sin((x+1)*16/PI)*ran,yadd-(y+1)*100);
			glTexCoord2f(1,1);
			glVertex3f(cos((x+1)*16/PI)*ran,sin((x+1)*16/PI)*ran,yadd-(y+0)*100);
			glTexCoord2f(0,1);
			glVertex3f(cos((x+0)*16/PI)*ran,sin((x+0)*16/PI)*ran,yadd-(y+0)*100);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE);
	glDisable(GL_FOG);


}

typedef struct
{
	float x,y,z;
}d_vector;

typedef struct
{
	d_vector a;
	d_vector b;
	d_vector c;
	d_vector d;
	d_vector normal;
	unsigned char group;
//	void *next;
}d_quadi;

#define MAX_DQUAD 50000

d_quadi quadi_booli[MAX_DQUAD];
int qb_lastuse=0;
int maxz=0;
int qb_group=0;

	GLfloat jassoo_ambient[4]  = {0.75, 0.75, 0.75, 1.0};
	GLfloat jassoo_diffuse[4]  = {1.0, 1.0, 1.0, 1.0};
	GLfloat jassoo_diffuse_red[4]  = {1.0, 0.0, 0.0, 1.0};
	GLfloat jassoo_diffuse_yellow[4]  = {1.0, 1.0, 0.0, 1.0};
	GLfloat jassoo_specular[4] = {1.0, 1.0, 1.0, 1.0};
	GLfloat jassoo_position[4] = {-30.0, 90.0, 800.0, 1.0};
	GLfloat jassoo_emission[4] = {1.0, 1.0, 1.0, 1.0};


void d_bevel_move(d_vector *source, d_vector *center, d_vector *normal, d_vector *dest, float depth, float stretch)
{
	d_vector dir;
	dir.x=source->x-center->x;
	dir.y=source->y-center->y;
	dir.z=source->z-center->z;

	dest->x=source->x+dir.x*stretch+normal->x*depth;
	dest->y=source->y+dir.y*stretch+normal->y*depth;
	dest->z=source->z+dir.z*stretch+normal->z*depth;
}

void calc_dq_normal(d_quadi *quadi)
{
	float ax = (quadi->b.x - quadi->a.x);
	float ay = (quadi->b.y - quadi->a.y);
	float az = (quadi->b.z - quadi->a.z);
	float bx = (quadi->c.x - quadi->a.x);
	float by = (quadi->c.y - quadi->a.y);
	float bz = (quadi->c.z - quadi->a.z);

	float nx = ay * bz - az * by; 
	float ny = az * bx - ax * bz; 
	float nz = ax * by - ay * bx;

	float nl = sqrt(nx*nx + ny*ny + nz*nz);

	if(nl!=0) 
	{ 
		nl=1.0/nl;
		quadi->normal.x = nx*nl;
		quadi->normal.y = ny*nl;
		quadi->normal.z = nz*nl;     
	}
	else {
		quadi->normal.x = 0;
		quadi->normal.y = 0;
		quadi->normal.z = 1;     
	}
}

// bevels the quad. new four quads are d_quadi.next and .next.next. qb_lastuse increased and new ones taken out from quadi_booli
d_quadi *d_beveli(d_quadi *quadi, float depth, float stretch)
{
	int i;
	if (qb_lastuse+5>=10000) return NULL;

	d_quadi *temp = &quadi_booli[qb_lastuse+5];
	d_quadi *newquads = &(quadi_booli[qb_lastuse]);
	
/*	if (quadi->next==NULL)
	{
		quadi->next=&newquads[0];
	}else
	{
		quadi_booli[qb_lastuse-1].next=&newquads[0];
	}
	
	for (i=0;i<3;i++)
		newquads[i].next=&newquads[i+1];
	newquads[3].next=NULL;

*/	qb_lastuse+=4;

	d_vector center;
	center.x=(quadi->a.x+quadi->b.x+quadi->c.x+quadi->d.x)*0.25;
	center.y=(quadi->a.y+quadi->b.y+quadi->c.y+quadi->d.y)*0.25;
	center.z=(quadi->a.z+quadi->b.z+quadi->c.z+quadi->d.z)*0.25;

	memcpy(temp,quadi,sizeof(d_quadi));

	d_bevel_move(&temp->a,&center,&temp->normal,&quadi->a,depth,stretch);
	d_bevel_move(&temp->b,&center,&temp->normal,&quadi->b,depth,stretch);
	d_bevel_move(&temp->c,&center,&temp->normal,&quadi->c,depth,stretch);
	d_bevel_move(&temp->d,&center,&temp->normal,&quadi->d,depth,stretch);

	newquads[0].a=temp->a;
	newquads[0].b=temp->b;
	newquads[0].c=quadi->b;
	newquads[0].d=quadi->a;

	newquads[1].a=temp->b;
	newquads[1].b=temp->c;
	newquads[1].c=quadi->c;
	newquads[1].d=quadi->b;

	newquads[2].a=temp->c;
	newquads[2].b=temp->d;
	newquads[2].c=quadi->d;
	newquads[2].d=quadi->c;

	newquads[3].a=temp->d;
	newquads[3].b=temp->a;
	newquads[3].c=quadi->a;
	newquads[3].d=quadi->d;

	if (newquads[3].a.z>maxz) maxz=newquads[3].a.z;
	
	quadi->group=qb_group;

	for (i=0;i<4;i++) 
	{
		calc_dq_normal(&newquads[i]);
		newquads[i].group=qb_group;
	}
	return newquads;
}

#define NUMBEVS 150

float get_dist(d_vector *a, d_vector *b)
{
	float x,y,z;
	x=a->x-b->x;
	y=a->y-b->y;
	z=a->z-b->z;
	return sqrt(x*x+y*y+z*z);
}

void d_norr(d_vector *center, d_vector *vec, float dist)
{
	d_vector dir;
	dir.x=center->x-vec->x;
	dir.y=center->y-vec->y;
	dir.z=center->z-vec->z;

	float nl=sqrt(dir.x*dir.x+dir.y*dir.y+dir.z*dir.z);

	if(nl!=0) 
	{ 
		nl=1.0/nl;
		dir.x=dir.x*nl;
		dir.y=dir.y*nl;
		dir.z=dir.z*nl;
	}
	vec->x=center->x+dir.x*dist;
	vec->y=center->y+dir.y*dist;
	vec->z=center->z+dir.z*dist;

}

void nelioeks(d_quadi *quadi)
{
	d_vector center;
	center.x=(quadi->a.x+quadi->b.x+quadi->c.x+quadi->d.x)*0.25;
	center.y=(quadi->a.y+quadi->b.y+quadi->c.y+quadi->d.y)*0.25;
	center.z=(quadi->a.z+quadi->b.z+quadi->c.z+quadi->d.z)*0.25;
	float dist =
		(
		get_dist(&center, &quadi->a)+
		get_dist(&center, &quadi->b)+
		get_dist(&center, &quadi->c)+
		get_dist(&center, &quadi->d))*0.25;

	d_norr(&center, &quadi->a, dist);
	d_norr(&center, &quadi->b, dist);
	d_norr(&center, &quadi->c, dist);
	d_norr(&center, &quadi->d, dist);
}


void renderKuolo()
{
	d_quadi *quadi;
	glBegin(GL_QUADS);
//	while(quadi!=NULL)
	for (int i=0;i<qb_lastuse;i++)
	{
		quadi=&(quadi_booli[i]);
		glNormal3f(quadi->normal.x,quadi->normal.y,quadi->normal.z);
		glTexCoord2f(0,0);
		glVertex3f(quadi->a.x,quadi->a.y,quadi->a.z);
		glTexCoord2f(1,0);
		glVertex3f(quadi->b.x,quadi->b.y,quadi->b.z);
		glTexCoord2f(1,1);
		glVertex3f(quadi->c.x,quadi->c.y,quadi->c.z);
		glTexCoord2f(0,1);
		glVertex3f(quadi->d.x,quadi->d.y,quadi->d.z);

//		quadi=(d_quadi *)quadi->next;
	}
	glEnd();

}

void renderKuoloGroup(int group)
{
	d_quadi *quadi;
	glBegin(GL_QUADS);
//	while(quadi!=NULL)
	for (int i=0;i<qb_lastuse;i++)
	{
		quadi=&(quadi_booli[i]);
		if (quadi->group==group)
		{
			glNormal3f(quadi->normal.x,quadi->normal.y,quadi->normal.z);
			glTexCoord2f(0,0);
			glVertex3f(quadi->a.x,quadi->a.y,quadi->a.z);
			glTexCoord2f(1,0);
			glVertex3f(quadi->b.x,quadi->b.y,quadi->b.z);
			glTexCoord2f(1,1);
			glVertex3f(quadi->c.x,quadi->c.y,quadi->c.z);
			glTexCoord2f(0,1);
			glVertex3f(quadi->d.x,quadi->d.y,quadi->d.z);
		}
//		quadi=(d_quadi *)quadi->next;
	}
	glEnd();

}

void renderKuoloLines(int group)
{
	d_quadi *quadi;
	glDisable(GL_LIGHTING);
//	glDisable(GL_DEPTH_TEST);
//	glDepthMask(GL_FALSE);
	glColor3f(0,0,0);
//	glLineWidth(2);
	glBegin(GL_LINES);

	for (int i=0;i<qb_lastuse;i++)
	{
		quadi=&(quadi_booli[i]);
		if (group==-1||group==quadi->group)
		{
			glVertex3f(quadi->a.x,quadi->a.y,quadi->a.z);
			glVertex3f(quadi->b.x,quadi->b.y,quadi->b.z);

			glVertex3f(quadi->b.x,quadi->b.y,quadi->b.z);
			glVertex3f(quadi->c.x,quadi->c.y,quadi->c.z);

			glVertex3f(quadi->c.x,quadi->c.y,quadi->c.z);
			glVertex3f(quadi->d.x,quadi->d.y,quadi->d.z);

			glVertex3f(quadi->d.x,quadi->d.y,quadi->d.z);
			glVertex3f(quadi->a.x,quadi->a.y,quadi->a.z);
		}
	}
	glEnd();

}

int kekki_index=0;
int kekki_kekki=0;
int kekki_time=0;

void kekkinen(d_quadi *quadi,float n)
{
	if (n<=0) return;
	kekki_index++;

//	quadi->normal.x+=sin(n+quadi->a.x)*0.2;		
	float k=n;
	if (k>1) k=1;
	float dist=sin(kekki_index*1.76+kekki_kekki*2.25+kekki_time*0.001)*100+200;

	dist=dist/(kekki_index/2.0+0.25);

	d_beveli(quadi,k*dist,k*0.5);
//	nelioeks(quadi);

	dist=sin(kekki_index+kekki_kekki*2+kekki_time*0.001)*15+20;
//	dist=50;
	d_quadi *newq = d_beveli(quadi,k*dist,k*-0.33);

	int ka=kekki_kekki;
	for (int i=0;i<3;i++)
	{
		kekki_kekki=i;
//		if ((i*3+kekki_index)%10<4) 
			kekkinen(&newq[(i+ka+kekki_index)%4],n-1);
	}

//	kekkinen(&newq[kekki_index%4],n-1);

	kekki_index--;
}


void createKEK1(d_quadi *quadi, int count)
{
	for (int i=0;i<NUMBEVS;i++) 
	{
		qb_group=0;
		if (i>=NUMBEVS/2) quadi=&(quadi_booli[i*i*71016%(qb_lastuse)]);
		d_quadi *newq = d_beveli(quadi,200+sin(-count*0.0018+i*2.1)*50,sin(count*0.003+i*1.6)*0.25);
		if (newq!=NULL&&(i%2==0))
		{
//			float size=(((i%2)<<1)-0.5)*(150+sin(count*0.002-i)*50);
//			if (size<0) size=-50;
			for (int j=0;j<4;j++)
			{
				qb_group=0;
				float size=(100+sin(count*0.002-i+j*2.6)*100);
				if (j%2==1) size-=30;
				d_beveli(&newq[j],size,-0.5);
				if (size>0&&(j%2==0))
				{
					qb_group=1;
					d_quadi *newq2 = d_beveli(&newq[j],150,0);
					for (int k=0;k<4;k++)
					{
						d_beveli(&newq2[k],100,-0.25);
					}
					d_beveli(&newq[j],150,-0.75);
					if ((i/2+j)%3==0) quadi=&newq[(i/2%2)*2];
					else quadi= &(quadi_booli[0]);
				}
			}
		}		
	}

}

void createKEK2(d_quadi *quadi, float mu, int count)
{
	kekki_kekki=0;
	kekki_time=count;
	kekki_index=0;
	qb_group=0;
	d_beveli(quadi,-300,10);
	d_beveli(quadi,300,-0.9);
	d_beveli(quadi,50,-0.5);
	d_beveli(quadi,-300,-0.6);
	qb_group=1;
	d_beveli(quadi,250,-0.6);

	kekkinen(quadi,(-sin(count*0.0003)*2.5+2.5)*mu);
}

void EfuShitti::kuolonKorahdus(int count, float sync, GLuint texture)
{
	int i;

	d_quadi *quadi = &(quadi_booli[0]);
	qb_lastuse=1;
	maxz=0;
	
	quadi->a.x=-100;
	quadi->a.y=-100;
	quadi->a.z=0;
	quadi->b.x=100;
	quadi->b.y=-100;
	quadi->b.z=0;
	quadi->c.x=100;
	quadi->c.y=100;
	quadi->c.z=0;
	quadi->d.x=-100;
	quadi->d.y=100;
	quadi->d.z=0;
//	quadi->next=NULL;
	calc_dq_normal(quadi);

	float mu=count*0.00005-0.3;
	if (mu>1) mu=1;
	if (mu<0) mu=0;

	createKEK2(quadi,mu,count);


//	kekkinen(quadi,4);
//	for (int i=0;i<10;i++)
//	{
//		d_beveli(quadi,100,0);
//		d_beveli(quadi,30,-0.25);
//	}
	//	qb_lastuse=qb_lastuse;

//	d_beveli(quadi,100,sin(count*0.001+i)*sync);
//	d_beveli(quadi,100,0);
	Perspective(90,0.1,5000.0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glShadeModel(GL_FLAT);

	GLfloat jassoo_position_xx[4] = {-150.0, 300.0, 0.0, 1.0};

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT, jassoo_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,jassoo_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,jassoo_position_xx);

	GLfloat fogColor[4]= {1.0f, 0.0f, 0.0f, 1.0f};		// Fog Color
	d_fogi(fogColor, 5000.0);

//	glDisable(GL_LIGHTING);
//	glDisable(GL_LIGHT0);
//	glColor4f(0.5,0.5,0.5,1.0);
	glColor4f(1.0,1.0,1.0,1.0);

	qb_lastuse=qb_lastuse;

//	glRotatef(count*0.022,0,1,0);
//	glRotatef(count*0.016,1,0,0);
//	glRotatef(count*0.036,0,0,1);
//	glRotatef(sin(count*0.00056)*15+15,1,0,0);
//	glRotatef(sin(count*0.00056)*35+35,1,0,0);
//	glRotatef(sin(count*0.00006)*360,0,1,0);
	glRotatef(90,1,0,0);
	glRotatef(-90,0,0,1);
//	glRotatef(count*0.1,0,0,1);

//	glTranslatef(0,0,cos(count*0.00015)*2000);
	glTranslatef(700*mu,0,200*mu);

	mu=count*0.00005-0.3;
	mu*=3.3333;
	if (mu>1) mu=1;
	if (mu<0) 
	{
		glRotatef(90*-mu,1,0,0);
		glRotatef(-90*-mu,0,0,1);
		glRotatef(180*mu,0,1,0);
		glTranslatef(0,0,mu*200);
//		glRotatef(mu*360,1,0,0);
	}
	
	glRotatef(count*0.03,0,0,1);
	glRotatef(200,0,1,0);

	glTranslatef(0,0,-100);
//	glScalef(1.0/(count/10000.0+1),1.0/(count/10000.0+1),1.0/(count/10000.0+1));

//	glTranslatef(0,0,-maxz/2);

	glDisable(GL_TEXTURE_2D);
/*
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse);
	renderKuoloGroup(0);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse_red);
	renderKuoloGroup(1);
*/
	for (int yy=-2;yy<=2;yy++)
	{
		for (int xx=-2;xx<=2;xx++)
		{
			glPushMatrix();
			glTranslatef(xx*2200,yy*2200,0);
			glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse);
			renderKuoloGroup(0);
			if (abs(yy)<2&&abs(xx)<2)
			{
				glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse_yellow);
				renderKuoloGroup(1);
			}
			glPopMatrix();
		}
	}
	
/*	glRotatef(180,1,0,0);
//	renderKuolo();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	renderKuoloLines(1);*/
}

int createTunnel(int count)
{
	float move=(count/10)%100;
	int yphase=count/1000;
	float xm=0.24;
	float ym=sin(count*0.0001)*1.5;
	float w=0;
	float jee=sin(count*0.0001)*0.5;
	jee=0.1;
	for (int y=0;y<10;y++)
	{
		float xadd=sin((y-yphase)*xm)*w;
		float yadd=sin(sin(y-yphase)*10*ym)*w;
		float xadd2=sin((y-yphase+1)*xm)*w;
		float yadd2=sin(sin(y-yphase+1)*10*ym)*w;
		for (int x=0;x<8;x++)
		{
			float x1=x-(-yphase+0+y)*jee;
			float x2=x-(-yphase+1+y)*jee;
			d_quadi *quadi = &(quadi_booli[x+y*8]);

			quadi->a.x=cos((x1+0)*PI/4)*100.0+xadd;
			quadi->a.y=sin((x1+0)*PI/4)*100.0+yadd;
			quadi->a.z=(y+0)*100+move;

			quadi->b.x=cos((x2+0)*PI/4)*100.0+xadd2;
			quadi->b.y=sin((x2+0)*PI/4)*100.0+yadd2;
			quadi->b.z=(y+1)*100+move;

			quadi->c.x=cos((x2+1)*PI/4)*100.0+xadd2;
			quadi->c.y=sin((x2+1)*PI/4)*100.0+yadd2;
			quadi->c.z=(y+1)*100+move;

			quadi->d.x=cos((x1+1)*PI/4)*100.0+xadd;
			quadi->d.y=sin((x1+1)*PI/4)*100.0+yadd;
			quadi->d.z=(y+0)*100+move;

			qb_lastuse++;

			calc_dq_normal(quadi);
		}
	}
	return yphase;
}

void EfuShitti::kuolonKorahdus2(int count, float sync, GLuint texture)
{
	int i;

	qb_lastuse=0;
	maxz=0;

	int yphase=createTunnel(count);


	qb_group=0;

	for (i=0;i<10*8;i++)
	{
		int phase=-yphase*8+i;
		float h1=sin(count*0.000-phase*0.063*5)*20.0;
		float d1=-0.40;
		qb_group=0;
		d_beveli(&quadi_booli[i],-10,-0.5);
		d_beveli(&quadi_booli[i],0,-0.25);
		d_beveli(&quadi_booli[i],h1/2,d1);
		if (i&1) 
		{
			d_beveli(&quadi_booli[i],h1,d1);
			float h2=-sin(-count*0.0013+phase*0.3);
			qb_group=1;
			d_beveli(&quadi_booli[i],h2*40.0,h2*1.0);
			if (h2>0)
			{
				d_quadi *quu = d_beveli(&quadi_booli[i],h2*h2*10,-0.3);
				for (int j=0;j<4;j++)
				{
					d_beveli(&quu[j],h2*-5,-0.7*h2);
					d_beveli(&quu[j],h2*30,0.25*h2);
					d_beveli(&quu[j],h2*5,-0.8*h2);
				}
				qb_group=2;
				d_beveli(&quadi_booli[i],h2*5,-0.5);
			}

		}else
		{
			d_beveli(&quadi_booli[i],-h1/2,d1);
		}
	}
	
	Perspective(90,0.1,1000.0);


	GLfloat fogColor[4]= {1.0f, 0.0f, 0.0f, 1.0f};		// Fog Color
	d_fogi(fogColor,1000.0);

//	glRotatef(90,1,0,0);
//	glRotatef(-90,0,0,1);

//	glTranslatef(700*mu,0,200*mu);

//	glRotatef(count*0.03,0,0,1);
//	glRotatef(200,0,1,0);
#define POT5(x) ((x)*(x)*(x)*(x)*(x))

	glRotatef(-15.0,0,1,0);
	glRotatef(-count*0.004,0,0,1);
	glRotatef(60*POT5(sin(count*0.0003)),1,0,0);

	glTranslatef(sin(count*0.00035)*50.0,-cos(-count*0.00043)*50.0,-900);
//	glTranslatef(0,0,-900);

	glDisable(GL_TEXTURE_2D);
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D,texture);

//	glLineWidth(1.0);
//	renderKuoloLines(-1);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glShadeModel(GL_FLAT);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT, jassoo_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,jassoo_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,jassoo_position);

	glColor4f(1.0,1.0,1.0,1.0);

	renderKuoloGroup(0);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse_yellow);
	renderKuoloGroup(1);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse_red);
	renderKuoloGroup(2);

	glDisable(GL_FOG);
}

void EfuShitti::kuolonKorahdus3(int count, float sync, GLuint texture)
{
	int i;

	d_quadi *quadi = &(quadi_booli[0]);
	qb_lastuse=1;
	maxz=0;
	
	quadi->a.x=-100;
	quadi->a.y=-100;
	quadi->a.z=0;
	quadi->b.x=100;
	quadi->b.y=-100;
	quadi->b.z=0;
	quadi->c.x=100;
	quadi->c.y=100;
	quadi->c.z=0;
	quadi->d.x=-100;
	quadi->d.y=100;
	quadi->d.z=0;
//	quadi->next=NULL;
	calc_dq_normal(quadi);

	createKEK1(quadi,count);


	Perspective(90,0.1,5000.0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glShadeModel(GL_FLAT);

	GLfloat jassoo_position_yy[4] = {-350.0, 700.0, -2000.0, 1.0};

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT, jassoo_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,jassoo_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,jassoo_position_yy);

	glColor4f(1.0,1.0,1.0,1.0);

	qb_lastuse=qb_lastuse;

//	glRotatef(count*0.022,0,1,0);
//	glRotatef(count*0.016,1,0,0);
//	glRotatef(count*0.036,0,0,1);
//	glRotatef(sin(count*0.00056)*15+15,1,0,0);
	glRotatef(sin(count*0.00056)*35+35,1,0,0);
	glRotatef(sin(count*0.00006)*360,0,1,0);
//	glRotatef(90,1,0,0);
	glRotatef(-90,0,0,1);
//	glRotatef(count*0.1,0,0,1);

//	glTranslatef(0,0,cos(count*0.00015)*2000);
//	glTranslatef(1000,0,0);

//	glScalef(1.0/(count/10000.0+1),1.0/(count/10000.0+1),1.0/(count/10000.0+1));

	glTranslatef(0,0,-maxz/2+sin(count*0.0001)*maxz/4);

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_CULL_FACE);

	renderKuolo();
//	glRotatef(180,1,0,0);
//	renderKuoloGroup(1);
//	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse_red);
//	glLineWidth(2);
//	renderKuoloLines(0);
//	glLineWidth(1);
//	renderKuoloLines(-1);

}

int createSpiral(int count)
{
	float move=(count/5)%100;
	int yphase=count/500;
	float xm=0.74;
	float w=200;
	for (int y=0;y<15;y++)
	{
		float xadd =cos((y-yphase  )*xm)*w;
		float yadd =sin((y-yphase  )*xm)*w;
		float xadd2=cos((y-yphase+1)*xm)*w;
		float yadd2=sin((y-yphase+1)*xm)*w;
		for (int x=0;x<8;x++)
		{
			d_quadi *quadi = &(quadi_booli[x+y*8]);

			quadi->a.x=cos((x+1)*PI/4)*100.0+xadd;
			quadi->a.y=sin((x+1)*PI/4)*100.0+yadd;
			quadi->a.z=(y+0)*100+move;

			quadi->b.x=cos((x+1)*PI/4)*100.0+xadd2;
			quadi->b.y=sin((x+1)*PI/4)*100.0+yadd2;
			quadi->b.z=(y+1)*100+move;

			quadi->c.x=cos((x+0)*PI/4)*100.0+xadd2;
			quadi->c.y=sin((x+0)*PI/4)*100.0+yadd2;
			quadi->c.z=(y+1)*100+move;

			quadi->d.x=cos((x+0)*PI/4)*100.0+xadd;
			quadi->d.y=sin((x+0)*PI/4)*100.0+yadd;
			quadi->d.z=(y+0)*100+move;

			qb_lastuse++;

			calc_dq_normal(quadi);
		}
	}
	return yphase;
}

void EfuShitti::kuoloSpirals(int count, float sync, GLuint texture)
{
	int i;

	qb_lastuse=0;
	maxz=0;

	int yphase=createSpiral(count);


	qb_group=0;

	for (i=0;i<15*8;i++)
	{
		int phase=-yphase*8+i;
		float h1=sin(count*0.002-phase*0.063*5)*50.0+40.0;
		float d1=-0.80;
		if (i%20==0&&count%17000>9000) h1*=-100,d1=5;
		qb_group=0;
		d_beveli(&quadi_booli[i],-10,-0.5);
		qb_group=1;
		d_beveli(&quadi_booli[i],h1/4,-0.5);
		qb_group=2;
		d_beveli(&quadi_booli[i],h1,d1);
	}
	
	Perspective(90,0.1,1300.0);


	GLfloat fogColor[4]= {1.0f, 0.0f, 0.0f, 1.0f};		// Fog Color
	d_fogi(fogColor,1300.0);

#define POT5(x) ((x)*(x)*(x)*(x)*(x))

	glRotatef(-15.0,0,1,0);
	glRotatef(-count*0.004,0,0,1);
	glRotatef(45*POT5(sin(count*0.0003)),1,0,0);

//	glTranslatef(sin(count*0.00035)*50.0,-cos(-count*0.00043)*50.0,-900);
	glTranslatef(0,0,-1300);

	glDisable(GL_TEXTURE_2D);
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D,texture);

//	glLineWidth(1.0);
//	renderKuoloLines(-1);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glShadeModel(GL_FLAT);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT, jassoo_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,jassoo_specular);
	GLfloat jassoo_position_yy[4] = {-50.0, 400.0, -500.0, 1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,jassoo_position_yy);

	glColor4f(1.0,1.0,1.0,1.0);

	for (int yy=-1;yy<=1;yy++)
	{
		for (int xx=-1;xx<=1;xx++)
		{
			glPushMatrix();
			float scale=1.0-abs(xx)*0.3-abs(yy)*0.3;
			glTranslatef(xx*800*scale,yy*800*scale,0);
			glScalef(scale,scale,1);
			glRotatef(xx*217+yy*153,0,0,1);
			glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse_red);
			if (count%10000>5000)
			{
				renderKuoloLines(0);
				glEnable(GL_LIGHTING);
			}else renderKuoloGroup(0);
			glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse);
			renderKuoloGroup(1);
			glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse_yellow);
			renderKuoloGroup(2);
			glPopMatrix();
		}
	}
	glDisable(GL_FOG);
}

int createPlane(int count)
{
	float move=(count/5)%100;
	int yphase=count/500;
	float xm=0.74;
	float w=200;
	for (int y=0;y<15;y++)
	{
		for (int x=0;x<15;x++)
		{
			d_quadi *quadi = &(quadi_booli[x+y*15]);

			quadi->a.x=(x-7)*100;
			quadi->a.y=0;
			quadi->a.z=(y+1)*100+move;

			quadi->b.x=(x-7)*100;
			quadi->b.y=0;
			quadi->b.z=(y+0)*100+move;

			quadi->c.x=(x-8)*100;
			quadi->c.y=0;
			quadi->c.z=(y+0)*100+move;

			quadi->d.x=(x-8)*100;
			quadi->d.y=0;
			quadi->d.z=(y+1)*100+move;

			qb_lastuse++;

			calc_dq_normal(quadi);
		}
	}
	return yphase;
}


void EfuShitti::kuoloPlanes(int count, float sync, GLuint texture)
{
	int i;

	qb_lastuse=0;
	maxz=0;

	int yphase=createPlane(count);


	qb_group=0;

	for (i=0;i<15*15;i++)
	{
		int phase=-yphase*15+i;
		float h1=sin(count*0.002-phase*0.063*5)*25.0+20.0;
		float d1=-0.80;
		qb_group=0;
		d_beveli(&quadi_booli[i],2,-0.75);
		qb_group=1;
		d_beveli(&quadi_booli[i],h1/4,-0.5);
		qb_group=2;
		d_beveli(&quadi_booli[i],h1,d1);
	}
	
	Perspective(90,0.1,1400.0);


	GLfloat fogColor[4]= {1.0f, 0.0f, 0.0f, 1.0f};		// Fog Color
	d_fogi(fogColor,1400.0);

#define POT5(x) ((x)*(x)*(x)*(x)*(x))

	glRotatef(-15.0,0,1,0);
	glRotatef(sin(-count*0.0004)*45.0,0,0,1);
	glRotatef(45*POT5(sin(count*0.0003)),1,0,0);

//	glTranslatef(sin(count*0.00035)*50.0,-cos(-count*0.00043)*50.0,-900);
	glTranslatef(0,-50,-1400);

	glDisable(GL_TEXTURE_2D);
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D,texture);

//	glLineWidth(1.0);
//	renderKuoloLines(-1);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glShadeModel(GL_FLAT);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT, jassoo_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,jassoo_specular);
	GLfloat jassoo_position_yy[4] = {-250.0, 0.0, 1200.0, 1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,jassoo_position_yy);

	glColor4f(1.0,1.0,1.0,1.0);

	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse_red);
	renderKuoloGroup(0);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse);
	renderKuoloGroup(1);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse_yellow);
	renderKuoloGroup(2);

	glTranslatef(0,100,0);
	glRotatef(180,0,0,1);

	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse_red);
	renderKuoloGroup(0);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse);
	renderKuoloGroup(1);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse_yellow);
	renderKuoloGroup(2);
	glDisable(GL_FOG);
}

int lukko_init=-1;
#define LUKKO_SYLINTERIT 30

typedef struct
{
	float nopeus;
	float aikalisa;
}LUKKO_STR;

LUKKO_STR lukot[LUKKO_SYLINTERIT];

GLuint lukko_kuutio;
DrObject lootaobu;
DrObject tykkiobu;

void piirra_sylinteri(float kaannos,float z, float aika)
{
	float jm=z*7.321;
	float jp=z*23.964;
	float t=150.0*aika/1000.0/60.0;	
	float tx;	
	float tt=(t-1)*4;
	float xs,ys,zs;
//	int cylkoko=24;	
	int cylkoko=20;	


	t=100;
	tt=100;
	if(tt<128||tt>=3072) {
		tx=t/8;
	} else if (tt>=128&&tt<448||tt>2816&&tt<3068){
		tx=t/4;
	} else if (tt>=864&&tt<1792||tt>=1920&&tt<2364){
		tx=t;
	} else {
		tx=t/2;
	}

	if((int)(t*2)%16==0) {
		xs=1-t*0.002;
		ys=1-t*0.002;
		zs=1+10*((t*0.003)*(t*0.003));
	} else if((int)(t*2)%16==4) {
		xs=1-t*0.002;
		ys=1+10*((t*0.003)*(t*0.003));
		zs=1-t*0.002;
	} else if((int)(t*2)%16==8) {
		xs=1+10*((t*0.003)*(t*0.003));
		ys=1-t*0.002;
		zs=1-t*0.002;
	} else if((int)(t*2)%16==12) {
		xs=1+10*((t*0.0015)*(t*0.0015));
		ys=1+10*((t*0.0015)*(t*0.0015));
		zs=1-t*0.002;
	} else {
		xs=ys=zs=1;
	}

	t=(t+1)*0.25;

	for (int i=0;i<cylkoko;i++)
	{
		
		glPushMatrix();
		int zz=15-abs(z+15);
		float kokokoko=150+zz*zz*zz*zz*zz/1200.0;

		glRotatef(kaannos*90.0,0,0,1);
		glTranslatef(0,0,z*75);

		if((int)(tx)%4==0) {
			kokokoko=150+abs(tan(aika*0.0001+z*0.05)*1000.0);
		} else if((int)(tx)%4==1) {
			kokokoko=150+
				(
				abs(tan(aika*0.0001+z*0.05)*1000.0)+300*sin(z*2.1547+aika*0.00003262)
				)
				*(sin(aika*0.0002)*0.5+0.5)
				;
		} else if((int)(tx)%4==2) {
//			kokokoko=150+abs(tan(aika*0.0001+z*0.05)*1000.0);
		} else if((int)(tx)%4==3) {
			kokokoko=(1+kokokoko)+kokokoko;
		} else {
			kokokoko=150+abs(tan(aika*0.0001+z*0.05)*1000.0);
		}

		glTranslatef(cos((i)*PI/((float)(cylkoko)/2.0))*kokokoko,sin((i)*PI/((float)(cylkoko)/2.0))*kokokoko,0);
		glRotatef(i*360/(float)(cylkoko),0,0,1);

		if((int)(z)%3==0) {
			glRotatef((int)(z)*22.5,0,0,1);
		} else {
			glRotatef((int)(z)*90,0,0,-1);
		}
		if((i+(int)(z))%3==0) {
			glRotatef(-kaannos*541,sin(i*3.21),cos(i*2.121),sin(-i*3.2421));
		} else {
			glRotatef(-kaannos*241,cos(-i*2.21),sin(i*4.121),cos(i*5.2421));
		}

		if((i+(int)(z))%5==0) {
			glTranslatef(0,0,-1);
			glScalef(1,1,4);
			glTranslatef(0,0,1);
		}
		if(i%17==0) {
			glScalef(1.1+0.1*sin(i*3.321-jp),1.2+0.4*sin(i*3.321-jp),1.1+0.1*sin(i*6.321-jp));
		}
		if(i%7==0) {
			glScalef(1.4+0.4*sin(i*3.321+z),0.8+0.2*sin(i*9.321+z),1.4+0.4*sin(i*2.321+z));
		} else {
			glScalef(1.1+0.1*cos(i*4.321+z),1.1+0.1*cos(i*5.843+z),1.1+0.1*cos(i*2.386+z));
		}
		glScalef(1.4+0.2*sin(i*7.321+jm),1.4+0.2*sin(i*7.321+jm),1.4+0.2*sin(i*7.321+jm));


		glScalef(0.04,0.04,0.04);
		glScalef(xs,ys,zs);
		lootaobu.drawList();
//		glCallList(lukko_kuutio);

		glPopMatrix();
	}

}

void init_lukko()
{
		for (int i=0;i<LUKKO_SYLINTERIT;i++)
		{
			lukot[i].nopeus=(rand()%10000)/5000.0-1.0;
			lukot[i].aikalisa=(rand()%10000)/1000.0;
		}
		lootaobu.load("data\\loota2.ob3");
//		lootaobu.load("data\\loota3.ob3");

		lootaobu.toList_cubeNormals(-0.5);

		tykkiobu.load("data\\tykki.ob3");
//		lootaobu.load("data\\sm_cube.ob3");
//		lootaobu.calc_normals();
//		lootaobu.calc_rnormals();
//		lootaobu.toList();

		lukko_kuutio = glGenLists(1);
		glNewList(lukko_kuutio, GL_COMPILE);

		glBegin(GL_QUADS);

		float x1=-12;
		float x2= 12;
		float y1=-12;
		float y2= 12;
		float z1=-12;
		float z2= 12;

		glNormal3f(0,0,1);
		glVertex3f(x1,y1,z1);
		glVertex3f(x1,y2,z1);
		glVertex3f(x2,y2,z1);
		glVertex3f(x2,y1,z1);

		glNormal3f(0,0,-1);
		glVertex3f(x2,y1,z2);
		glVertex3f(x2,y2,z2);
		glVertex3f(x1,y2,z2);
		glVertex3f(x1,y1,z2);

		glNormal3f(1,0,0);
		glVertex3f(x1,y1,z1);
		glVertex3f(x1,y1,z2);
		glVertex3f(x1,y2,z2);
		glVertex3f(x1,y2,z1);

		glNormal3f(-1,0,0);
		glVertex3f(x2,y1,z1);
		glVertex3f(x2,y2,z1);
		glVertex3f(x2,y2,z2);
		glVertex3f(x2,y1,z2);

		glNormal3f(0,1,0);
		glVertex3f(x1,y1,z1);
		glVertex3f(x2,y1,z1);
		glVertex3f(x2,y1,z2);
		glVertex3f(x1,y1,z2);

		glNormal3f(0,-1,0);
		glVertex3f(x2,y2,z1);
		glVertex3f(x1,y2,z1);
		glVertex3f(x1,y2,z2);
		glVertex3f(x2,y2,z2);

		glEnd();

		glEndList();
}

void EfuShitti::set_lukko_gl(int count)
{

	float t=150.0*count/1000.0/60.0;
	float angle;
	int tt=(t-1)*4;
	if(tt<448) {
		angle=10+40*(1-(1-((t*4)/448.0))*(1-((t*4)/448.0)));
	} else {
		angle=50.0;
	}
	Perspective(angle,10,30000.0);

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
//	glDisable(GL_DEPTH_TEST);
//	glDepthMask(GL_FALSE);
	glEnable(GL_CULL_FACE);
//	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glShadeModel(GL_SMOOTH);


/*
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT, jassoo_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, jassoo_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,jassoo_specular);
	GLfloat jassoo_position_yy[4] = {-10000.0, 45774.0, -10000.0, 1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,jassoo_position_yy);
*/

	glDisable(GL_LIGHTING);
//	glEnable(GL_TEXTURE_2D);

	GLfloat white[4] = {1.0,1.0,1.0,1.0};

	wnd->glActiveTextureARB(GL_TEXTURE0_ARB);
	texture[1]->use();

	glEnable(GL_TEXTURE_GEN_S); 
	glEnable(GL_TEXTURE_GEN_T); 
	glEnable(GL_TEXTURE_GEN_R); 
	glEnable(GL_TEXTURE_GEN_Q); 

	GLfloat objPlaneS[] = {1.0, 0.0, 0.0, 0.0}; 
	GLfloat objPlaneT[] = {0.0, 1.0, 0.0, 0.0}; 
	GLfloat objPlaneR[] = {0.0, 0.0, 1.0, 0.0}; 
	GLfloat objPlaneQ[] = {0.0, 0.0, 0.0, 1.0}; 
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); 
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); 
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); 
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); 
	glTexGenfv(GL_S, GL_OBJECT_PLANE, objPlaneS); 
	glTexGenfv(GL_T, GL_OBJECT_PLANE, objPlaneT); 
	glTexGenfv(GL_R, GL_OBJECT_PLANE, objPlaneR); 
	glTexGenfv(GL_Q, GL_OBJECT_PLANE, objPlaneQ); 

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);


	glMatrixMode(GL_TEXTURE); 
	glLoadIdentity(); 
	glTranslatef(0.5,0.5,0); 
	//glScalef(0.5,0.5,1); 
	glScalef(0.05,0.05,1); 
	glMultMatrixd( projMatrix ); 
	glMultMatrixd( modelMatrix ); 

	glMatrixMode(GL_MODELVIEW);

	wnd->glActiveTextureARB(GL_TEXTURE1_ARB);
	texture[0]->use();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_EXT);

	glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_RGB_EXT,GL_ADD_SIGNED_EXT);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_RGB_EXT,GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_RGB_EXT,GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE1_RGB_EXT,GL_PREVIOUS_EXT);
	glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND1_RGB_EXT,GL_SRC_COLOR);


}

void EfuShitti::unset_lukko_gl()
{
	wnd->glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);

	wnd->glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_GEN_S); 
	glDisable(GL_TEXTURE_GEN_T); 
	glDisable(GL_TEXTURE_GEN_R); 
	glDisable(GL_TEXTURE_GEN_Q); 
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glMatrixMode(GL_TEXTURE); 
	glLoadIdentity(); 

	glMatrixMode(GL_MODELVIEW);

}

void drawOrtho(int xres, int yres, GLTexture *texture)
{
	glViewport(0,0,xres,yres);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, xres, yres, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glShadeModel(GL_FLAT);

	texture->use();

//	glTranslatef(((rand()%100)/100.0-0.5)/xres,0,0);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(0, 0);

	glTexCoord2f(1,1);
	glVertex2f(xres, 0);
	
	glTexCoord2f(1,0);
	glVertex2f(xres, yres);
	
	glTexCoord2f(0,0);
	glVertex2f(0, yres);
	glEnd();
}

void drawOrthoAlpha(float alpha, int xres, int yres, GLTexture *texture)
{
	glViewport(0,0,xres,yres);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, xres, yres, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(1,1,1,alpha);

	texture->use();


	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(0, 0);

	glTexCoord2f(1,1);
	glVertex2f(xres, 0);
	
	glTexCoord2f(1,0);
	glVertex2f(xres, yres);
	
	glTexCoord2f(0,0);
	glVertex2f(0, yres);
	

	glEnd();

}

void drawOrthoAlpha_zoom(float alpha, int x1, int y1, int x2, int y2, int xres, int yres, GLTexture *texture)
{
	glViewport(0,0,xres,yres);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, xres, yres, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(1,1,1,alpha);

	texture->use();


	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(x1, y1);

	glTexCoord2f(1,1);
	glVertex2f(x2, y1);
	
	glTexCoord2f(1,0);
	glVertex2f(x2, y2);
	
	glTexCoord2f(0,0);
	glVertex2f(x1, y2);
	

	glEnd();

}


void drawOrthoAlpha_fluid(float alpha, float vaihe, int xres, int yres, GLTexture *texture)
{
	glViewport(0,0,xres,yres);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, xres, yres, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(1,1,1,alpha);

	texture->use();

/*
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(0, 0);

	glTexCoord2f(1,1);
	glVertex2f(xres, 0);
	
	glTexCoord2f(1,0);
	glVertex2f(xres, yres);
	
	glTexCoord2f(0,0);
	glVertex2f(0, yres);
*/	

	glBegin(GL_QUADS);
	for (int yy=0;yy<32;yy++)
	{
		float yp1=sin((31-yy)*1.7+vaihe)*0.02;
		float yp2=sin((32-yy)*1.7+vaihe)*0.02;
		float ya1=(yy+0)*yres/32.0;
		float ya2=(yy+1)*yres/32.0;
		for (int xx=0;xx<32;xx++)
		{
			float xp1=sin((xx+0)*1.1+vaihe)*0.02;
			float xp2=sin((xx+1)*1.1+vaihe)*0.02;
			float yy1=(31-yy)/32.0+xp1;
			float yy2=(32-yy)/32.0+xp1;
			float yy3=(31-yy)/32.0+xp2;
			float yy4=(32-yy)/32.0+xp2;
			float xx1=(xx+0)/32.0+yp1;
			float xx2=(xx+1)/32.0+yp1;
			float xx3=(xx+0)/32.0+yp2;
			float xx4=(xx+1)/32.0+yp2;
			float xa1=(xx+0)*xres/32.0;
			float xa2=(xx+1)*xres/32.0;
			glTexCoord2f(xx3, yy2);
			glVertex2f(xa1, ya1);

			glTexCoord2f(xx4,yy4);
			glVertex2f(xa2, ya1);

			glTexCoord2f(xx2,yy3);
			glVertex2f(xa2, ya2);

			glTexCoord2f(xx1,yy1);
			glVertex2f(xa1, ya2);
		}
	}

	glEnd();

}


void drawOrthoAlpha_niko(float alpha, int xres, int yres, GLTexture *texture, float sizemul)
{
	glViewport(0,0,xres,yres);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, xres, yres, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(1,1,1,alpha);

	texture->use();

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(-sizemul, -sizemul);

	glTexCoord2f(1,1);
	glVertex2f(xres+sizemul, -sizemul);
	
	glTexCoord2f(1,0);
	glVertex2f(xres+sizemul, yres+sizemul);
	
	glTexCoord2f(0,0);
	glVertex2f(0-sizemul, yres+sizemul);
	glEnd();

}


void EfuShitti::draw_lukko(int count)
{
	float t=150.0*count/1000.0/60.0;
	float tp=t;
	int tt=(t-1)*4;
	float __t;

	__t=t;
	t=150;
	tt=66100;

	set_lukko_gl(count);

	if(tt==440||tt==443||tt==446||		/*eka filli*/
	   tt==636||tt==637||tt==642||		/*arvailua tokan fillin koostumuksesta*/
	   tt==862||						/*ekan suvannon loppuisku*/
	   tt==1404||tt==1406||				/*joku tuplabasarifilli*/
	   tt==2364||tt==2367||tt==2370||	/*arvailua tän fillin koostumuksesta*/
	   tt==2808||tt==2811||tt==2814		/*vika filli, kait*/
	   ) {
		t=t+48;
	} else {
		t=t;
	}


	if(tt>=800&&tt<864/*||tt>=1792&&tt<1920*/) {
	} else {
		glRotatef(85,0,1,0);
		glTranslatef(2750,0,0);
	}

//	if(tt<800||tt>=864&&tt<1792||tt>=1920&&tt<3072) {
//	if(tt<800||tt>=864&&tt<1792||tt>=1920&&tt<3072) {
		if(tt>=128) {
			for(int j=0;j<8;j++) {
				if((int)((t+3)/4)%8==j) {
					if(j%2==0) {
						glRotatef(45.0+22.5*j,j%2,(int)(j*0.5)%3,(int)(j*0.25)%4+0.32);
					} else {
						glRotatef(-(45.0+22.5*j),j%2,(int)(j*0.5)%3,(int)(j*0.25)%4+0.32);
					}
				}
			}
		}
		if(tt<448) {
			tp=t*(((t*4)/448)*((t*4)/448));
		} else {
			tp=t;
		}
		if((int)((t)/2)%4==0) {
			glRotatef(tp*22.5,0,0,-1);
		} else if((int)((t)/2)%4==2) {
			glRotatef(tp*45,0,0,1);
		} else {
			glRotatef(tp*11.25,1,0.5,0);
		}
//	}

	glPushMatrix();

	glRotatef(count/50.0,0,5,1);
	glTranslatef(0,0,1000);

	float aikaa=sin((t*PI)/32.0)*6+6;

	for (int i=2;i<LUKKO_SYLINTERIT-2;i++)
	{
		float aika=aikaa-lukot[i].aikalisa;
		if (aika>0) aika=0;

		if(tt<1792||tt>=1920) {
			if(tt<128) {
				piirra_sylinteri(0.1*aika*lukot[i].nopeus,-i,count);
			} else if(tt>=128&&tt<448) {
				piirra_sylinteri(0.1*aika*lukot[i].nopeus+0.9*((t*4-128)/(448-128))*aika*lukot[i].nopeus,-i,count);
			} else {
				piirra_sylinteri(aika*lukot[i].nopeus,-i,count);
			}
		}
	}

		glPushMatrix();
			if(tt>128) {
				glTranslatef(0,0,0);
			} else {
				glTranslatef(0,0,2000-2000*((t*4)/128)*((t*4)/128));
			}
			glRotatef(90,1,0,0);
			glScalef(0.2,0.2,0.2);
			tykkiobu.drawList();
		glPopMatrix();
		glPushMatrix();
			if(tt>128) {
				glTranslatef(0,0,-2000);
			} else {
				glTranslatef(0,0,-4000+2000*((t*4)/128)*((t*4)/128));
			}
			glRotatef(-90,1,0,0);
			glScalef(0.2,0.2,0.2);
			tykkiobu.drawList();
		glPopMatrix();
	unset_lukko_gl();

	if(tt>1792&&tt<2812) {
		float briwi1,briwi2,pos1,pos2,scale;
		float bri;
		if((int)(t)%8==5||(int)(t)%8==6) {
			if((int)(t*2)%16==10) {
				scale=0.025;
			} else if((int)(t*2)%16==11) {
				scale=1;
			} else if((int)(t*2)%16==12) {
				scale=2;
			} else if((int)(t*2)%16==13) {
				scale=4;
			}
			for(int j=0;j<3; j++) {
				for(int i=0;i<20;i++) {
					briwi1=0.5+0.5*sin((t+i*0.066)*PI)*cos((t+i*0.05)*PI*0.5);
					bri=briwi1*(1.0/scale)*4;
					pos1=200*i+j*100;
					gantzJuttu(12,(i*45+j*11.25)/180.0,1-bri,scale*(100+j*30*sin((t*0.1625+i*j*0.25)*PI*2)+i*100*sin((t*0.0625+i*0.125)*PI*2)*briwi1),0.-(2400+pos1),-(2200.0+pos1),0.0,4.0,0.0,i*0.0625+1+j*0.125,texture[12]);
					gantzJuttu(12,(i*45+j*11.25)/180.0,1-bri,scale*(100+j*30*sin((t*0.1625+i*j*0.25)*PI*2)+i*100*sin((t*0.0625+i*0.125)*PI*2)*briwi1),0.-(-400-pos1),-(-200.0-pos1),0.0,4.0,0.0,i*0.0625+1+j*0.125,texture[12]);
				}
			}
		}
	}


	if(tt>864&&/*tt<1792||tt>1920&&*/tt<2816) {
		ganzeffu(count);
	} else if(tt>=768&&tt<864||tt>2816) {
		t=t+1.5;
		float briwi1,briwi2,pos1,pos2,scale;
		float bri;
		if(tt>=768&&tt<800) {
			bri=1.0*(t*4-768)/(800-768);
			bri=bri*bri;
			bri=bri*0.25;
		} else {
			bri=1;
		}
		t=__t;
		if((int)(t)%4==0) {
			if((int)(t*2)%8==0) {
				scale=0.025;
			} else {
				scale=1;
			}
			for(int i=0;i<20;i++) {
				briwi1=0.5+0.5*sin((t+i*0.66)*PI)*cos((t+i*0.5)*PI*0.5)*scale;
				pos1=25*sin((t+i*0.75)*PI*0.125);
				gantzJuttu(7+(int)(2*cos(t*PI*0.25)*sin(t*PI*0.125)),(i*45)/180.0,1-briwi1,(1.0+100*i+100*cos((t*0.125+i*0.125)*PI)*sin((t*0.0625+i*0.125)*PI*2)*briwi1)*scale,-1100.0+pos1+99*sin(t*PI*16)*briwi1*(10*(0.5*0.5*sin(t*PI*0.25))),-900.0+pos1-99*sin(t*PI*16)*briwi1*(10*(0.5*0.5*sin(t*PI*0.25))),0.0,4.0,0.0,i*0.0625+1,texture[11]);
			}
		} else if((int)(t)%4==1) {
			briwi1=0.5+0.5*sin(t*PI)*cos(t*PI*0.5);
			briwi2=0.5+0.5*cos(t*PI*2)*cos(t*PI*0.125);
			pos1=500*sin(t*PI*0.125);
			pos2=750*cos(t*PI*0.125)*cos(t*PI*0.125);
			gantzJuttu(10+(int)(5*cos(t*PI*0.25)),0,1-briwi1,700.0+150*cos(t*PI)*sin(t*PI*2),-1100.0+pos1+99*sin(t*PI*16)*briwi1*10,-900.0+pos1-99*sin(t*PI*16)*briwi1*10,0.0,1.0,0.0+t/50.0,3.0+t/50.0,texture[11]);
			gantzJuttu(12+(int)(9*sin(t*PI*0.5)),0,1-briwi2,1500.0-450*sin(t*PI*2)*sin(t*PI),-1100.0+pos2+99*cos(t*PI*4)*briwi2*10,-900.0+pos2-99*cos(t*PI*4)*briwi2*10,0.0,1.0,0.0-t/50.0,1.0-t/50.0,texture[11]);
		} else if((int)(t)%4==2) {
			for(int i=0;i<20;i++) {
				briwi1=0.5+0.5*sin((t+i*0.66)*PI)*cos((t+i*0.5)*PI*0.5);
				pos1=2500*sin((t+i*0.75)*PI*0.125);
				gantzJuttu(7+(int)(4*cos(t*PI*0.25)*sin(t*PI*0.5)),0,1-briwi1,150.0+100*cos(t*PI)*sin(t*PI*2)*briwi1,-1100.0+pos1+99*sin(t*PI*16)*briwi1*(10*10*(0.5*0.5*sin(t*PI*0.25))),-900.0+pos1-99*sin(t*PI*16)*briwi1*(10*10*(0.5*0.5*sin(t*PI*0.25))),0.0,4.0,0.0,i+1,texture[11]);
			}
		}
	} else {
		t=t;
	}

//	gantzJuttu(-4,0,1,2000.0,-20000.0,19000.0,0.0,2.0,0.0+t/4.0,4.0+t/4.0,texture[13]);
	if(tt>=644&&tt<1792||tt>=2372&&tt<3072) {
		for(i=0;i<4;i++) {
			gantzJuttu(-(3+i),0,1,3000.0+100*i,-16000.0+i*1000,15000.0+i*1000,0.0,2.0,0.0+t/4.0,4.0+t/4.0,texture[13]);
		}
	}
	if(tt>=1164&&tt<2372||tt>=2562&&tt<3072) {
		float bri;
		if(tt<1408) {
			bri=0.03*(((t*4-1164)/244.0)*((t*4-1164)/244.0));
		} else {
			bri=0.075;
		}
		for(i=0;i<3;i++) {
			gantzJuttu((3+i),0,bri,3000.0+100*i,-16000.0+i*1000,15000.0+i*1000,0.0,12.0+i*sin(t*PI*2*0.0125),0.0-t/2.0,4.0-t/2.0,texture[11]);
		}
	}
	

	glPopMatrix();

}


void EfuShitti::gantzJuttu(int sides, double rotate, double kirkkaus, double width, double z1, double z2, double texU1, double texU2, double texV1, double texV2, GLTexture *texture)
{
	texture->use();
	glEnable(GL_DEPTH_TEST);
//	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	if (sides>0)
	{
		glShadeModel(GL_SMOOTH);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glColor4f(1,1,1,kirkkaus);
	}else
	{
		sides=-sides;
		glShadeModel(GL_SMOOTH);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);

//		glColor4f(1,1,1,kirkkaus);
		glColor4f(1,1,1,1);
	}


	glBegin(GL_QUADS);

	for (int i=0;i<sides;i++)
	{
		double x1,x2;
		double y1,y2;

		double uu1=(texU1*(sides-i)+texU2*i)/sides;
		double uu2=(texU1*(sides-(i+1))+texU2*(i+1))/sides;

		x1=cos(( i   *(PI*2))/sides+rotate)*width;
		y1=sin(( i   *(PI*2))/sides+rotate)*width;
		x2=cos(((i+1)*(PI*2))/sides+rotate)*width;
		y2=sin(((i+1)*(PI*2))/sides+rotate)*width;

		glTexCoord2f(uu1,texV1);
		glVertex3f(x1,y1,z1);
		glTexCoord2f(uu2,texV1);
		glVertex3f(x2,y2,z1);
		glTexCoord2f(uu2,texV2);
		glVertex3f(x2,y2,z2);
		glTexCoord2f(uu1,texV2);
		glVertex3f(x1,y1,z2);
	}

	glEnd();
	glDisable(GL_BLEND);

}


void invert(float alpha, int xres, int yres)
{
	glViewport(0,0,xres,yres);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, xres, yres, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glShadeModel(GL_FLAT);
	glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR);
	glColor4f(0.1,0.2,0.1,0);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glVertex2f(0, 0);

	glVertex2f(xres, 0);
	
	glVertex2f(xres, yres);
	
	glVertex2f(0, yres);
	

	glEnd();

}

int daohadsfga=0;

void EfuShitti::lukko(int count)
{
	float t=150.0*count/1000.0/60.0;
	int tt=(t-1)*4;

	if (lukko_init==-1)
	{
		lukko_init=0;

		init_lukko();
	}
/*
	glViewport(0, 0, 512,256);
	draw_lukko(count);

	texture[2]->getScreen();
	glDepthFunc(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i=3;i<=10;i++)
	{
		drawOrtho(pow(2,8-(i-3)),pow(2,8-(i-3)),texture[2]);
		texture[i]->getScreen();
	}
*/

	glViewport(0, 0, 512,256);
	draw_lukko(count);

//	return;

	glDepthFunc(GL_FALSE);
//	drawOrthoAlpha(sin(count*0.005)*0.5+0.5,256+16,256+12,texture[3]);
//	drawOrthoAlpha_zoom(0.3, -16, -16, 256+16, 256+16, 256, 256, texture[3]);

	float koppa=count/(5.10*60*10000);

	if(tt>=1792&&tt<1920) {
		koppa=koppa*2;
	} else {
		koppa=koppa;
		if (koppa>0.2) koppa=0.2;
	}
	if (tt<446) koppa=0.2-tt/4460.0;

	glDepthFunc(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	texture[2]->getScreen();

	glViewport(0, 0, 256,256);
	drawOrtho(256,256,texture[2]);
	drawOrthoAlpha_zoom(koppa, 0, 0, 256, 256, 256, 256, texture[16]);
	texture[3]->getScreen();
	glDepthFunc(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	for (int i=4;i<=9;i++)
	{
		drawOrtho(pow(2,8-(i-3)),pow(2,8-(i-3)),texture[3]);
		texture[i]->getScreen();
//	return;
	}

	glDepthFunc(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,wnd->scrXres,wnd->scrYres);
//	glDepthMask(GL_TRUE);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	float t=150.0*count/1000.0/60.0;
//	int tt=(t-1)*4;
	float kirkkk=t*4/448.0;
	if (kirkkk>1) {		
		kirkkk=1;
/*		if(tt>=3072) {
			kirkkk=1-(t*4-3072)/64.0;
			if(kirkkk<0) kirkkk=0;
		}*/
	}

//	kirkkk=1;

//	drawOrtho(wnd->scrXres,wnd->scrYres,texture[2]);

	if (tt<448)
	{
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		float pu=11-t*4.0*6/448.0;
		if (pu>9) pu=9;
		if (pu<4) pu=4;
		int pu1=pu;
		float fa=fmod(pu,1.0);
		int k1=pu1-2;
		int k2=pu1-3;

		for (int c=-1;c<2;c++)
		{
			for (int b=-1;b<2;b++)
			{
				texture[pu1]->filtering=false;
				texture[pu1]->params();
				texture[pu1-1]->filtering=false;
				texture[pu1-1]->params();
				drawOrthoAlpha_zoom((    fa)/8.0,b*k1,c*k1,wnd->scrXres+b*k1,wnd->scrYres+c*k1,wnd->scrXres,wnd->scrYres,texture[pu1]);
				drawOrthoAlpha_zoom((1.0-fa)/8.0,b*k2,c*k2,wnd->scrXres+b*k2,wnd->scrYres+c*k2,wnd->scrXres,wnd->scrYres,texture[pu1-1]);
				texture[pu1]->filtering=true;
				texture[pu1]->params();
				texture[pu1-1]->filtering=true;
				texture[pu1-1]->params();
				drawOrthoAlpha_zoom((    fa)/8.0,b*k1,c*k1,wnd->scrXres+b*k1,wnd->scrYres+c*k1,wnd->scrXres,wnd->scrYres,texture[pu1]);
				drawOrthoAlpha_zoom((1.0-fa)/8.0,b*k2,c*k2,wnd->scrXres+b*k2,wnd->scrYres+c*k2,wnd->scrXres,wnd->scrYres,texture[pu1-1]);
			}
		}
	}else
	{
//		draw_lukko(count);
		drawOrtho(wnd->scrXres,wnd->scrYres,texture[2]);
		glDisable(GL_DEPTH_TEST);
		if(0)
		for (i=3;i<=8;i++) {
			texture[i]->filtering=true;
			texture[i]->params();

			drawOrthoAlpha(0.25*kirkkk,wnd->scrXres,wnd->scrYres,texture[i]);
			drawOrthoAlpha_fluid(0.25*kirkkk,i*1025.5+count/400.0,wnd->scrXres,wnd->scrYres,texture[i]);
			drawOrthoAlpha_fluid(0.15*kirkkk,i*1025.5+552-count/373.4,wnd->scrXres,wnd->scrYres,texture[i]);

			texture[i]->filtering=false;
			texture[i]->params();
			drawOrthoAlpha_fluid(0.1*kirkkk,i*1025.5+1325+count/712.7,wnd->scrXres,wnd->scrYres,texture[i]);
		}
	}


	texture[16]->getScreenCenter(wnd->scrXres/2, wnd->scrYres/2);
//	texture[3]->getScreen();

//	for (i=0;i<20;i++)
//	{
//		drawOrthoAlpha(0.25,wnd->scrXres+i*i,wnd->scrYres+i*i,texture[3]);
//	}


//	drawOrthoAlpha_niko(0.5,wnd->scrXres,wnd->scrYres,texture[2],200000);
//	for (int z=0;z<300;z++)
//	drawOrthoAlpha_niko(1.0,wnd->scrXres,wnd->scrYres,texture[2],cos(count*0.05+0*1.124)*1000+1000);

	if (tt<220)
	{
		colorFade(0,0,0,1.0-tt/220.0);
	}


//	if (tt>=3072) drawOrthoAlpha_zoom(1, wnd->scrXres/2-128, wnd->scrYres/2+128, wnd->scrXres/2+128-1, wnd->scrYres/2-128+1, wnd->scrXres, wnd->scrYres, texture[14]);
//	if (tt<220) drawOrthoAlpha_zoom(tt/55.0, wnd->scrXres/2-256, wnd->scrYres/2+32+10, wnd->scrXres/2+256-1, wnd->scrYres/2-32+1+10, wnd->scrXres, wnd->scrYres, texture[15]);
//	else if (tt<440) drawOrthoAlpha_zoom(1-(tt-220)/220.0, wnd->scrXres/2-256, wnd->scrYres/2+32+10, wnd->scrXres/2+256-1, wnd->scrYres/2-32+1+10, wnd->scrXres, wnd->scrYres, texture[15]);
//	if (tt==440||tt==441||tt==442) drawOrthoAlpha_zoom(1, wnd->scrXres/2-256*2, wnd->scrYres/2+32*2, wnd->scrXres/2+256*2-1, wnd->scrYres/2-32*2+1, wnd->scrXres, wnd->scrYres, texture[15]);
//	if (tt==443||tt==444||tt==445) drawOrthoAlpha_zoom(1, wnd->scrXres/2-256*4, wnd->scrYres/2+32*4, wnd->scrXres/2+256*4-1, wnd->scrYres/2-32*4+1, wnd->scrXres, wnd->scrYres, texture[15]);
//	if (tt==446||tt==447) drawOrthoAlpha_zoom(1, wnd->scrXres/2-256*8, wnd->scrYres/2+32*8, wnd->scrXres/2+256*8-1, wnd->scrYres/2-32*8+1, wnd->scrXres, wnd->scrYres, texture[15]);
	
}

void EfuShitti::ganzeffu(int count)
{
	float briwi1,briwi2;
	float pos1,pos2;
	float scale;
	float t=150.0*count/1000.0/60.0;
	int i;
	int tt=(t-1)*4;
	t=t+1.5;

	if(tt>=1856&&tt<1888) {
		if((int)(t)%4==0) {
			briwi1=0.5+0.5*sin(t*PI)*cos(t*PI*0.5);
			briwi2=0.5+0.5*cos(t*PI*2)*cos(t*PI*0.125);
			pos1=500*sin(t*PI*0.125);
			pos2=750*cos(t*PI*0.125)*cos(t*PI*0.125);
			gantzJuttu(10+(int)(5*cos(t*PI*0.25)),0,1-briwi1,700.0+150*cos(t*PI)*sin(t*PI*2),-1100.0+pos1+99*sin(t*PI*16)*briwi1*10,-900.0+pos1-99*sin(t*PI*16)*briwi1*10,0.0,1.0,0.0+t/50.0,3.0+t/50.0,texture[11]);
			gantzJuttu(12+(int)(9*sin(t*PI*0.5)),0,1-briwi2,1500.0-450*sin(t*PI*2)*sin(t*PI),-1100.0+pos2+99*cos(t*PI*4)*briwi2*10,-900.0+pos2-99*cos(t*PI*4)*briwi2*10,0.0,1.0,0.0-t/50.0,1.0-t/50.0,texture[11]);
		} else if((int)(t)%4==1) {
			briwi1=0.5+0.5*sin(t*PI)*cos(t*PI*0.5);
			briwi2=0.5+0.5*cos(t*PI*2)*cos(t*PI*0.125);
			pos1=500*sin(t*PI*0.125);
			pos2=750*cos(t*PI*0.125)*cos(t*PI*0.125);
			gantzJuttu(10+(int)(5*cos(t*PI*0.25)),0,1-briwi1,700.0+150*cos(t*PI)*sin(t*PI*2),-1100.0+pos1+99*sin(t*PI*16)*briwi1*10,-900.0+pos1-99*sin(t*PI*16)*briwi1*10,0.0,1.0,0.0+t/50.0,3.0+t/50.0,texture[11]);
			gantzJuttu(12+(int)(9*sin(t*PI*0.5)),0,1-briwi2,1500.0-450*sin(t*PI*2)*sin(t*PI),-1100.0+pos2+99*cos(t*PI*4)*briwi2*10,-900.0+pos2-99*cos(t*PI*4)*briwi2*10,0.0,1.0,0.0-t/50.0,1.0-t/50.0,texture[11]);
		} else if((int)(t)%4==2) {
			for(int i=0;i<20;i++) {
				briwi1=0.5+0.5*sin((t+i*0.66)*PI)*cos((t+i*0.5)*PI*0.5);
				pos1=2500*sin((t+i*0.75)*PI*0.125);
				gantzJuttu(7+(int)(4*cos(t*PI*0.25)*sin(t*PI*0.5)),0,1-briwi1,150.0+100*cos(t*PI)*sin(t*PI*2)*briwi1,-1100.0+pos1+99*sin(t*PI*16)*briwi1*(10*10*(0.5*0.5*sin(t*PI*0.25))),-900.0+pos1-99*sin(t*PI*16)*briwi1*(10*10*(0.5*0.5*sin(t*PI*0.25))),0.0,4.0,0.0,i+1,texture[11]);
			}
		} else {
/*			for(i=-20;i<20;i++) {
				briwi1=0.5+0.5*sin((t+i*0.66)*PI)*cos((t+i*0.5)*PI*0.125);
				pos1=1000*sin((t+i*0.75)*PI*0.0625);
				gantzJuttu(7+(int)(4*cos(t*PI*0.0625)*sin(t*PI*0.125)),0,1-briwi1,150.0+1000*(0.5*0.5*sin(((float)(i)/10)*PI)),-1100.0+pos1+99*sin(t*PI*0.5)*briwi1*(10+10*(0.5*0.5*sin(t*PI*0.0625))),-900.0+pos1-99*sin(t*PI*0.5)*briwi1*(10+10*(0.5*0.5*sin(t*PI*0.0625))),0.0,4.0,0.0,i*0.125+1,texture[11]);
			}*/
		}
	} else if(tt>=1888&&tt<1920) {
		if((int)(t*4)%4!=3) {
			briwi1=0.5+0.5*sin(t*PI)*cos(t*PI*0.5);
			briwi2=0.5+0.5*cos(t*PI*2)*cos(t*PI*0.125);
			pos1=500*sin(t*PI*0.125);
			pos2=750*cos(t*PI*0.125)*cos(t*PI*0.125);
			gantzJuttu(10+(int)(5*cos(t*PI*0.25)),0,1-briwi1,700.0+150*cos(t*PI)*sin(t*PI*2),-1100.0+pos1+99*sin(t*PI*16)*briwi1*10,-900.0+pos1-99*sin(t*PI*16)*briwi1*10,0.0,1.0,0.0+t/50.0,3.0+t/50.0,texture[11]);
			gantzJuttu(12+(int)(9*sin(t*PI*0.5)),0,1-briwi2,1500.0-450*sin(t*PI*2)*sin(t*PI),-1100.0+pos2+99*cos(t*PI*4)*briwi2*10,-900.0+pos2-99*cos(t*PI*4)*briwi2*10,0.0,1.0,0.0-t/50.0,1.0-t/50.0,texture[11]);
		}
	} else {
		if((int)(t)%4==0) {
//			if((int)(t+0.375)%4==0) {
				if((int)(t*2)%8==0) {
					scale=0.05;
				} else {
					scale=1;
				}
				for(int i=0;i<20;i++) {
					briwi1=0.5+0.5*sin((t+i*0.66)*PI)*cos((t+i*0.5)*PI*0.5)*scale;
					pos1=25*sin((t+i*0.75)*PI*0.125);
					gantzJuttu(7+(int)(2*cos(t*PI*0.25)*sin(t*PI*0.125)),(i*45)/180.0,1-briwi1,(1.0+100*i+100*cos((t*0.125+i*0.125)*PI)*sin((t*0.0625+i*0.125)*PI*2)*briwi1)*scale,-1100.0+pos1+99*sin(t*PI*16)*briwi1*(10*(0.5*0.5*sin(t*PI*0.25))),-900.0+pos1-99*sin(t*PI*16)*briwi1*(10*(0.5*0.5*sin(t*PI*0.25))),0.0,4.0,0.0,i*0.0625+1,texture[11]);
				}
//			}
		} else if((int)(t)%4==1) {
			briwi1=0.5+0.5*sin(t*PI)*cos(t*PI*0.5);
			briwi2=0.5+0.5*cos(t*PI*2)*cos(t*PI*0.125);
			pos1=500*sin(t*PI*0.125);
			pos2=750*cos(t*PI*0.125)*cos(t*PI*0.125);
			gantzJuttu(10+(int)(5*cos(t*PI*0.25)),0,1-briwi1,700.0+150*cos(t*PI)*sin(t*PI*2),-1100.0+pos1+99*sin(t*PI*16)*briwi1*10,-900.0+pos1-99*sin(t*PI*16)*briwi1*10,0.0,1.0,0.0+t/50.0,3.0+t/50.0,texture[11]);
			gantzJuttu(12+(int)(9*sin(t*PI*0.5)),0,1-briwi2,1500.0-450*sin(t*PI*2)*sin(t*PI),-1100.0+pos2+99*cos(t*PI*4)*briwi2*10,-900.0+pos2-99*cos(t*PI*4)*briwi2*10,0.0,1.0,0.0-t/50.0,1.0-t/50.0,texture[11]);
		} else if((int)(t)%4==2) {
			for(int i=0;i<20;i++) {
				briwi1=0.5+0.5*sin((t+i*0.66)*PI)*cos((t+i*0.5)*PI*0.5);
				pos1=2500*sin((t+i*0.75)*PI*0.125);
				gantzJuttu(7+(int)(4*cos(t*PI*0.25)*sin(t*PI*0.5)),0,1-briwi1,150.0+100*cos(t*PI)*sin(t*PI*2)*briwi1,-1100.0+pos1+99*sin(t*PI*16)*briwi1*(10*10*(0.5*0.5*sin(t*PI*0.25))),-900.0+pos1-99*sin(t*PI*16)*briwi1*(10*10*(0.5*0.5*sin(t*PI*0.25))),0.0,4.0,0.0,i+1,texture[11]);
			}
		} else {
			if(tt>=1406) {
				for(i=-20;i<20;i++) {
					briwi1=0.5+0.5*sin((t+i*0.66)*PI)*cos((t+i*0.5)*PI*0.125);
					pos1=1000*sin((t+i*0.75)*PI*0.0625);
					gantzJuttu(7+(int)(4*cos(t*PI*0.0625)*sin(t*PI*0.125)),0,1-briwi1,150.0+1000*(0.5*0.5*sin(((float)(i)/10)*PI)),-1100.0+pos1+99*sin(t*PI*0.5)*briwi1*(10+10*(0.5*0.5*sin(t*PI*0.0625))),-900.0+pos1-99*sin(t*PI*0.5)*briwi1*(10+10*(0.5*0.5*sin(t*PI*0.0625))),0.0,4.0,0.0,i*0.125+1,texture[11]);
				}
			}
		}
	}

}
