/*
*鼠标事件接口
*/

#ifndef _XMOUSELISTENER_H
#define _XMOUSELISTENER_H

#include "XEventType.h"

class XMouseListener
{
public:
	virtual void mousePressed(X_Event *event,const ID id)=0;
	virtual void mouseReleased(X_Event *event,const ID id)=0;
	virtual void mouseEntered(X_Event *event,const ID id)=0;
};



#endif


