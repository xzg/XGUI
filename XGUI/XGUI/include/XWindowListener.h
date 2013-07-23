/*
*窗口事件接口
*/

#ifndef _XWINDOWLISTENER_H
#define _XWINDOWLISTENER_H

#include "XObject.h"

class XWindowListener
{
public:
	virtual void windExit()=0;
	virtual void mouseMove()=0;
};


#endif
