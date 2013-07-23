#ifndef _XINDEXLISTENER_H
#define _XINDEXLISTENER_H

#include "XEventType.h"

class XIndexListener
{
public:
	virtual void selectIndex(X_Event *event,const ID ItemID,const int& index)=0;//选中某一项
};

#endif


