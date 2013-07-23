/*
* 事件的轮询，分发
*/

#ifndef _XAPPLICATION_H
#define _XAPPLICATION_H

#include "XObject.h"


class XApplication
{
public:
	
	XApplication();
	~XApplication();
	int run();
	
	static int PollMessage();

};

#endif
