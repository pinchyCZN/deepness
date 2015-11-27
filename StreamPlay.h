// StreamPlay.h: interface for the StreamPlay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREAMPLAY_H__1E69E69E_4981_47DE_92D8_3511E7C3F1F4__INCLUDED_)
#define AFX_STREAMPLAY_H__1E69E69E_4981_47DE_92D8_3511E7C3F1F4__INCLUDED_

#include "bass.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class StreamPlay  
{
public:
	void error(char *text);
	HSTREAM stream;
	void stop();
	void play();
	void loadStream(char *name);
	StreamPlay();
	virtual ~StreamPlay();

};

#endif // !defined(AFX_STREAMPLAY_H__1E69E69E_4981_47DE_92D8_3511E7C3F1F4__INCLUDED_)
