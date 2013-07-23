#ifndef _XBUTTON_H
#define _XBUTTON_H

#include "XWidget.h"
#include "XFont.h"

#define BUTTON_H_MARGIN 8
#define BUTTON_V_MARGIN 6

class XButton:public XWidget
{
public:
	XButton();
	
	void setText(char *text);
	void setFontColor(int color);
	
private:
	
	virtual void draw();
	virtual void onInit();
	virtual void onPaint(Uint16 msg);
	virtual void localUpdate(XRectF& pRect,XObject *pObject);
	//离开焦点
	virtual void focusLeave();
	//进入焦点 
	virtual void focusEnter();
	
private:	
	char       *text;
	Uint32     font_color;
	
};


#endif

/**
* 遗漏：如果按钮是非透明的，重绘制背景不对（SDL_FillRect(screen,&rect,color);）
*/


