
#include <windows.h>
#include <math.h>
#include <gl\gl.h>              // Header File For The OpenGL32 Library
#include <gl\glu.h>             // Header File For The GLu32 Library
#include <gl\glaux.h>			// Header File For The GLaux Library

#include "EfuShitti.h"
#include "VemputinMain.h"
#include "ModPlay.h"
#include "Streamplay.h"
#include "glfont.h"
#include "GLTexture.h"
#include "ImageLoader.h"
#include "DrObject.h"
#include "Demo.h"

#define PI 3.1415

Demo::Demo()
{
	aikataulu = new int[100];
}

Demo::~Demo()
{

	delete aikataulu;
}

void Demo::init(OpenglWindow *ogl)
{
	wnd=ogl;
	efut = new EfuShitti(wnd);
}

void Demo::deinit()
{
	delete efut;
}

int Demo::pehmeeaika(int aikanyt)
{
	int keskiaika=aikanyt;
	for (int ii=0;ii<10;ii++) keskiaika+=aikataulu[ii];
	int kivaaika=keskiaika/11+(aikanyt-aikataulu[0])/5;
//	frames++;
//	kivaaika=frames*(aikanyt-aikataulu[0])/11.0;
	for (int iii=0;iii<9;iii++)
	{
		aikataulu[iii]=aikataulu[iii+1];
	}
	aikataulu[9]=aikanyt;
	return kivaaika;
}


void Demo::run()
{
	unsigned int startCount = GetTickCount();

//	for (int j=0;j<10;j++) pehmeeaika(startCount);
	frames=0;

//	ModPlay modplay;
//	modplay.loadModule("fatshit.mo3");
//	modplay.play();	

	StreamPlay streamplay;
//	streamplay.loadStream("data\\milssi.mp3");

/*
	while(GetTickCount()<startCount+2000)
	{
		wnd->checkWindowMessages();
	};
*/
//	streamplay.play();
	startCount = GetTickCount()-400;

//	int kekkuli=startCount-GetTickCount();

	

	int pos=0;

	while (!wnd->keys[VK_ESCAPE]&&pos<(64*64*39-64*2)&&wnd->closeProgram==0)
	{
		if (wnd->scrReady)
		{
//			int count=pehmeeaika(GetTickCount()-startCount);
			int count=GetTickCount()-startCount;
			float a=(float)(sin((double)count*0.001)*0.25+0.25);
			a=0.25;


			glDepthMask(GL_TRUE);
//			glClearColor(0.0033,0.006,0.0085,0);
//			glClearColor(.05,.05,.05,0);
//			float vari=count/100000.0;
			float t=150.0*count/1000.0/60.0;
			float vari=1-fmod(t,1);
			float bri;
			float bra;
			int tt=(t-1)*4;
			vari=(vari*vari)*0.02;
			if(tt<448) {
				bri=1.5*(((t*4)/448)*((t*4)/448)*((t*4)/448)*((t*4)/448)*((t*4)/448)*((t*4)/448));
//				bra=3;
//				bri=1;
			} else if (tt>=448&&tt<800||tt>=864&&tt<1792||tt>=1920&&tt<3072){
				bri=1;
				bra=0.5;
				if(tt>=448&&tt<452||tt>=864&&tt<868||tt>=1920&&tt<1924) {
					vari=vari*12;
				} else {
					vari=vari*4;
				}
			} else if (tt>=3072) {
				bri=1-(t*4-3072)/128.0;
				if(bri<0) bri=0;
				bra=bri;
				vari=bri;
			} else {
				bri=1;
				bra=1;
				if(tt>=848&&tt<862) {
					vari=-(vari*4);
				} else if(tt>=1904&&tt<1920) {
					vari=((0.02-vari)*2);
				} else if (tt>=862&&tt<864||tt>=3072&&tt<3076) {
					vari=vari*16;
				} else {
					vari=0;
				}
				if(tt>=1792&&tt<1920) {
					bra=0;
				}
			}
			glClearColor((vari+0.05*bra)*bri,(vari*1.1+0.05*bra)*bri,(vari*1.5+0.05*bra)*bri,0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*			switch ((count/30000)%5)
			{
			case 0:
				efut->kuolonKorahdus(count,(count%2000)/2000.0,0);
				break;
			case 1:
				efut->kuolonKorahdus2(count,(count%2000)/2000.0,0);
//				efut->kuolonKorahdus2(count,(count%2000)/2000.0,efut->texture[0]->txt);
				break;
			case 2:
				efut->kuolonKorahdus3(count,(count%2000)/2000.0,0);
				break;
			case 3:
				efut->kuoloSpirals(count,(count%2000)/2000.0,0);
				break;
			case 4:
				efut->kuoloPlanes(count,(count%2000)/2000.0,0);
				break;
			}
*/
//			efut->kuolonKorahdus3(count,(count%2000)/2000.0,0);
//			efut->kuoloSpirals(count*4,(count*4%2000)/2000.0,0);
/*			glViewport(0, 0, 256,256);
//			efut->kuolonKorahdus2(count,(count%2000)/2000.0,0);
//			efut->kuoloPlanes(count,(count%2000)/2000.0,0);

			switch ((int)t%5)
			{
			case 0:
				efut->kuolonKorahdus(count*4,(count*4%2000)/2000.0,0);
				break;
			case 1:
				efut->kuolonKorahdus2(count*4,(count*4%2000)/2000.0,0);
//				efut->kuolonKorahdus2(count,(count%2000)/2000.0,efut->texture[0]->txt);
				break;
			case 2:
				efut->kuolonKorahdus3(count*4,(count*4%2000)/2000.0,0);
				break;
			case 3:
				efut->kuoloSpirals(count*4,(count*4%2000)/2000.0,0);
				break;
			case 4:
				efut->kuoloPlanes(count*4,(count*4%2000)/2000.0,0);
				break;
			}
*/
			efut->lukko(count);
//			efut->kuolonKorahdus2(count,(count%2000)/2000.0,0);
//			efut->kuolonKorahdus(count,(count%2000)/2000.0,0);
//			efut->kuoloPlanes(count,(count%2000)/2000.0,0);
			wnd->swapBuffers();
		}
		wnd->checkWindowMessages();
	}
//	modplay.stop();

//	streamplay.stop();
}
