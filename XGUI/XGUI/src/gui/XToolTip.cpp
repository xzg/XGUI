#include "XToolTip.h"
#include "XEventType.h"

#define BUTTON_H_MARGIN 3 //按钮区与外BOX水平距离
#define BUTTON_V_MARGIN 6 //按钮区与外BOX竖直距离

XToolTip::XToolTip()
{
	font=share_font;
	font_color=0xffffffff;
	
	bg_color=0x434343aa;
	
	timerID=NULL;
	
	setPosion(10,10);
}

XToolTip::~XToolTip()
{
	
}

void XToolTip::setPosion(int x,int y)
{
	widget_rect.x = x;
	widget_rect.y = y;
}

void setText(string text)
{
	
}

void XToolTip::showText(string text)
{
	int   w,h;
	XSize size=getWindowSize();
	
	if(timerID) return ;
	
	font->getFontSize((char *)text.c_str(),&w,&h);
	if(w<0 || h < 0)
	{
		w=0;
		h=0;
	}
	
	w +=2 * BUTTON_H_MARGIN + h;
	h +=2 * BUTTON_V_MARGIN;
	
	widget_rect.w = w;
	widget_rect.h = h;
	onSize(w,h);
	onPosition(widget_rect.x,widget_rect.y);
	
	show_text=text;
	paint();
	SDL_UpdateRect(screen,widget_rect.x,widget_rect.y,widget_rect.w+1,widget_rect.h+1);
	
	XAbstractWidget::addGlobalHigh(this);
	
	timerID=setTimer(3000);
}

void XToolTip::setFontColor(Uint32 color)
{
	font_color=color;
}

void XToolTip::setBackground(Uint32 color)
{
	bg_color=color;
}
//////////////////////////////////////////////////////////////
bool XToolTip::responseEvent(X_Event *event)
{
	if(event->type == X_RESUME)
	{
		onTiemer();
	}
	
	return false;
}
bool XToolTip::focusEvent(X_Event *event)
{
	return false;
}
void XToolTip::paint()
{
	roundedBoxColor(screen,widget_rectF.x1,widget_rectF.y1,widget_rectF.x2,widget_rectF.y2,3,bg_color);
	font->displayFont(show_text.c_str(),font_color,widget_rect);
}
/*
//特别说明：
//实行的是组件消失，重绘制所有组件，然后刷新局部
//应当实行重绘制覆盖的组件....
*/
void XToolTip::erase()
{
	setClipRect(widget_rect);
	
	XAbstractWidget::updateGlobalHighDown(this,widget_rectF);
	XAbstractWidget::updateGlobalHighUp(this,widget_rectF);
	
	//刷新屏幕
	SDL_UpdateRect(screen,widget_rect.x,widget_rect.y,widget_rect.w+1,widget_rect.h+1);	
	cleanClipRect();
}

void XToolTip::onTiemer()
{
	visible=0;
	removeTimer(timerID);
	timerID=NULL;
	erase();
	//printf("erase\n");
	XAbstractWidget::removeGlobalHigh(this);	
}

void XToolTip::onSize(int w,int h)
{
	XAbstractWidget::onSize(w,h);
	
	centerPoint.x= widget_rect.w/2+widget_rect.x;
	centerPoint.y= widget_rect.h/2+widget_rect.y;
	
}

void XToolTip::onPosition(int x,int y)
{
	XAbstractWidget::onPosition(x,y);
	
	centerPoint.x= widget_rect.w/2+widget_rect.x;
	centerPoint.y= widget_rect.h/2+widget_rect.y;
	
}

void XToolTip::localUpdate(XRectF& pRect,XObject *pObject)
{
	if( !match(pRect) || pObject == this) return ;
	
	paint();
	//printf("paint\n");
}



