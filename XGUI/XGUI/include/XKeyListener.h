/*
*键盘事件接口
*/

#ifndef _XKEYLISTENER_H
#define _XKEYLISTENER_H

#include "XEventType.h"

class XKeyListener
{
public:
	virtual void keyDown(X_Event *event,const ID id);
	virtual void keyUp(X_Event *event,const ID id);
};



#endif


