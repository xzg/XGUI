#ifndef _XLABEL_H
#define _XLABEL_H

#include "XWidget.h"
#include "XEditBox.h"

class XLabel:public XWidget
{
public:
	XLabel();
	void setFontColor(Uint32 color);
	void setText(string text);	     //...
	void setSeparateW(Uint8 w);       //设置相邻两行之间的距离,不支持多行显示
	void setUnderLine(bool underLine); //是否显示下滑线
	
private: 
	
	virtual void draw();
	virtual void onInit();
	virtual void onPaint(Uint16 msg);
	virtual void localUpdate(XRectF& pRect,XObject *pObject);
private:

	Uint32 font_color;
	string text;
	XSurface  *screen;
	
	bool   is_underLine;//是否显示下滑线
};

#endif

