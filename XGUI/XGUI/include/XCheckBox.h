#ifndef _XCHECKBOX_H
#define _XCHECKBOX_H

#include "XWidget.h"
#include "XWidgetListener.h"


class XCheckBox:public XWidget
{
public:
	XCheckBox();
	
	void setColor(Uint32 color); //边框颜色和字体颜色
	void setText(const string& text);
	string& getText();
	void setChecked(bool checked);
	bool getChecked();
	
	void addWidgetListener(XWidgetListener *widget);
private:
	virtual void draw();
	virtual void onInit();
	virtual void paint();
	virtual void onPaint(Uint16 msg);
	
	virtual void localUpdate(XRectF& pRect,XObject *pObject);
	virtual void onSize(int w,int h);   //当程序运行之后调用
	virtual void onPosition(int x,int y);
private:
	XRect  button_rect; //按钮区
	XRect  text_rect;   //文本区
	
	XWidgetListener *widget_listener; //组件监听器
		
	string text;
	Uint32 box_color;
	bool   checked;
};

#endif
