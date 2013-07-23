#include "XButton.h"


#define  BORDER_COLOR 0x6c93ff77
#define  DOWN_COLOR 0x7bb7f3ff

XButton::XButton()
{
	font_color=0;
	
	setText("button");
	setBackground(0xbdd9f5ff);
}

void XButton::setText(char *text)
{
	int   w,h;

	this->text=text;
	font->getFontSize(text,&w,&h);
	
	if(w<0 || h < 0)
	{
		w=0;
		h=0;
	}
	w +=2 * BUTTON_H_MARGIN;
	h +=2 * BUTTON_V_MARGIN;
	
	setSize(w,h);
}

void XButton::setFontColor(int color)
{
	font_color = color;
}

/*按钮颜色提供自定义
* 但是现在当按下组件时颜色不提供自定义
* 如果能设置亮度就好了
*/
void XButton::draw()
{	
	if(widget_state == X_PRESSED)
	{
		XRect rect = getRect();
		roundedBoxColor(screen, rect.x, rect.y,widget_rectF.x2,widget_rectF.y2, 2,DOWN_COLOR);
		roundedRectangleColor(screen, rect.x, rect.y,widget_rectF.x2,widget_rectF.y2,3,BORDER_COLOR);
	
		if(font)
		{
			rect.x +=2;
			rect.y +=2;
		
			font->displayFont(text,font_color,rect);	
		}
	}else if(widget_state == X_RELEASED || widget_state == X_ORIGINAL)
	{
		roundedBoxColor(screen, widget_rect.x, widget_rect.y,
			 widget_rectF.x2,widget_rectF.y2, 2,background_color);
		roundedRectangleColor(screen, widget_rect.x, widget_rect.y, 
			widget_rectF.x2,widget_rectF.y2,3,BORDER_COLOR);
		if(font)
			font->displayFont(text,font_color,widget_rect);
		
	}
}

void XButton::localUpdate(XRectF& pRect,XObject *pObject)
{
	if(pObject == this || !visible)
	{	
		return ;
	}
	
	draw();
}
void XButton::onInit()
{
	XWidget::onInit();
}

void XButton::onPaint(Uint16 msg)
{
	if(msg == X_PAINT)
	{
		draw();
		//子组件	
		if(layout)
			layout->drawChild(widget_rectF,this);
			
	}else if(msg  == X_UPDATE)
	{
		paint();
			
	}else if(msg == X_BUTTONDOWN)
	{
		widget_state = X_PRESSED;
		paint();
		
	}else if(msg == X_BUTTONUP)
	{
		widget_state = X_RELEASED;
		paint();
	}else if(msg == X_BUTTONLEAVE)
	{
		widget_state = X_RELEASED;
		paint();
	}
}

void XButton::focusLeave()
{
	widget_state = X_RELEASED;
	paint();
}

void XButton::focusEnter()
{
	widget_state = X_PRESSED;
	paint();
}


