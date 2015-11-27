// StreamPlay.cpp: implementation of the StreamPlay class.
//
//////////////////////////////////////////////////////////////////////

#include "StreamPlay.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StreamPlay::StreamPlay()
{
	if (BASS_GetVersion()!=MAKELONG(1,6))				error("Bass versin 1.6 was not loaded");
	if (!BASS_Init(-1,44100,0,GetForegroundWindow())) 	error("Cannot initialize device");
}

StreamPlay::~StreamPlay()
{
	BASS_Free();
}

void StreamPlay::loadStream(char *name)
{
	if (!(stream=BASS_StreamCreateFile(FALSE, name,0,0,BASS_MP3_SETPOS))) error("Cannot load the music");
}

void StreamPlay::play()
{
	BASS_Start();
//	BASS_StreamPlay(stream,FALSE,BASS_SAMPLE_LOOP);
	BASS_StreamPlay(stream,FALSE,0);
}

void StreamPlay::stop()
{
	for (int a=100;a>=0;a-=5) {  // fadeout music
		BASS_SetVolume(a);
		Sleep(1);
	}
	BASS_Stop();
}

void StreamPlay::error(char *text)
{
	ShowCursor (1);  // show cursor
    MessageBox(0, text, "Error", MB_OK|MB_ICONERROR);
    PostQuitMessage(0);
}
