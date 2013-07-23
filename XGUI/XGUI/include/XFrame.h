/*
*øÚº‹¿‡
*/

#ifndef _XFRAME_H
#define _XFRAME_H

#include "XWidget.h"


class XFrame:public XWidget 
{
public:
	
	XFrame(int w,int h):XWidget(w,h){}
	
	void addMouseListener(XWidget *widget);
	void addWindowListener(XWidget *widget);
	
private:
	void update();	
};

#endif
