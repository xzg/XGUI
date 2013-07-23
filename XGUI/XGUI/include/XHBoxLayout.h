/*
*水平布局管理器
*/

#ifndef _XHBOXLAYOUT_H
#define _XHBOXLAYOUT_H


#include "XLayout.h"

class XHBoxLayout: public XLayout
{

private:
	void  computeLayout();
	int getHVariableCount(int *w,bool cal=false);
	int getVVariableCount(int *h,bool cal=false);
	
	int getHCount();
	int getVCount();	
};


#endif


