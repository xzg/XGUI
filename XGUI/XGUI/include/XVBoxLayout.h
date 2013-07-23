/*
*竖直布局管理器
*/

#ifndef _XVBOXLAYOUT_H
#define _XVBOXLAYOUT_H


#include "XLayout.h"

class XVBoxLayout:public XLayout
{

public:
	XVBoxLayout();
	
private:
	void  computeLayout();
	int getHVariableCount(int *w,bool cal=false);
	int getVVariableCount(int *h,bool cal=false);
	
	int getHCount();
	int getVCount();
	
};

#endif
