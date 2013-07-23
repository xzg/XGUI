//文本提示框

#ifndef _XTOOLTIP_H
#define _XTOOLTIP_H

#include "XAbstractWidegt.h"


/*
// tooltipbox 是最上层的组件
*/

class XToolTip:public XAbstractWidget
{
public:
	XToolTip();
	~XToolTip();
	void showText(string text);
	void setBackground(Uint32 color);
	void setFontColor(Uint32 color);
	void setPosion(int x,int y);
	void setText(string text);
protected:
	virtual void onTiemer();
	virtual void paint();
	virtual void erase();
	
	virtual void onSize(int w,int h);
	virtual void onPosition(int x,int y);
	virtual void localUpdate(XRectF& pRect,XObject *pObject);

private:
	void setLayoutRect(XRect* rect){}
	void setLayoutSize(Uint16 w,Uint16 h){}
	
	void update(){}
	bool responseEvent(X_Event *event);
	bool focusEvent(X_Event *event);
	
private:	
	string show_text;
	XFont   *font;
	Uint32 bg_color;
	Uint32 font_color;
	
	SDL_TimerID timerID;
};

#endif
