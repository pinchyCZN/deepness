// ModPlay.h: interface for the ModPlay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODPLAY_H__8EEFF624_9B5A_4CEC_9355_A3CB70B225DA__INCLUDED_)
#define AFX_MODPLAY_H__8EEFF624_9B5A_4CEC_9355_A3CB70B225DA__INCLUDED_

#include "bass.h"
#include "OpenglWindow.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ModPlay  
{
public:
	int getPos();
	void addEndSync(SYNCPROC *proc);
	void addFXSync(SYNCPROC *proc);
	void play();
	void stop();
	HMUSIC module;
	void error(char *text);
	void loadModule(char *name);
	ModPlay();
	virtual ~ModPlay();

};

#endif // !defined(AFX_MODPLAY_H__8EEFF624_9B5A_4CEC_9355_A3CB70B225DA__INCLUDED_)
