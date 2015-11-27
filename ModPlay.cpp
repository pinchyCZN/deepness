// ModPlay.cpp: implementation of the ModPlay class.
//
//////////////////////////////////////////////////////////////////////

#include "ModPlay.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ModPlay::ModPlay()
{
	if (BASS_GetVersion()!=MAKELONG(1,6))				error("Bass versin 1.6 was not loaded");
	if (!BASS_Init(-1,44100,0,GetForegroundWindow())) 	error("Can't initialize device");
}

ModPlay::~ModPlay()
{
	BASS_Free();
}

void ModPlay::loadModule(char *name)
{
	if (!(module=BASS_MusicLoad(FALSE,name,0,0,BASS_MUSIC_RAMP))) 
														error("Can't load the module");
	BASS_MusicSetPositionScaler(module,64);
}

void ModPlay::error(char *text)
{
	ShowCursor (1);  // show cursor
    MessageBox(0, text, "Error", MB_OK|MB_ICONERROR);
    PostQuitMessage(0);
}


void ModPlay::stop()
{
	for (int a=100;a>=0;a-=5) {  // fadeout music
		BASS_SetVolume(a);
		Sleep(1);
	}
	BASS_Stop();
}

void ModPlay::play()
{
	BASS_Start();
	BASS_MusicPlayEx(module,0,-1,1);
}

void ModPlay::addFXSync(SYNCPROC *proc)
{
	BASS_ChannelSetSync(module,BASS_SYNC_MUSICFX,1,proc,0);
}

void ModPlay::addEndSync(SYNCPROC *proc)
{
	BASS_ChannelSetSync(module,BASS_SYNC_END,1,proc,0);
}

int ModPlay::getPos()
{
	return BASS_ChannelGetPosition(module);
}
