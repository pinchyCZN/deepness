#ifndef _DEMO_H_
#define _DEMO_H_


#include "OpenglWindow.h"
#include "EfuShitti.h"


class Demo  
{
public:
	int pehmeeaika(int aikanyt);
	void deinit();
	void run();
	void init(OpenglWindow *ogl);
	Demo();
	virtual ~Demo();

private:
	EfuShitti *efut;
	OpenglWindow *wnd;
	int *aikataulu;
	int frames;

};

#endif