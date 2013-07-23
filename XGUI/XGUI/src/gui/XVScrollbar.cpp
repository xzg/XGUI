#include "XVScrollbar.h"


XVScrollbar::XVScrollbar()
{
	BgColor=0x7895ff77;
	parent_length=0;
	
	scrollbar_position=0;
	parentMoveLength=0;
	
	widget_rect.w =3;
	scrollbar_rect.w=1;
	
	run_state=false;
}

void XVScrollbar::setBgColor(Uint32 color)
{
	BgColor=color;
}

void XVScrollbar::setWidth(int& w)
{
	widget_rect.w=w;
	
	scrollbar_rect.x=scrollbar_rect.x+(w-scrollbar_rect.w);
	scrollbar_rect.w=w;
}
////////////////////////////////////////私有/////////////////////////////////////////

void XVScrollbar::setRect(XRect& rect)
{
	
	widget_rect.x=rect.x;
	widget_rect.y=rect.y;
	widget_rect.h=rect.h;
	
	scrollbar_rect.x=rect.x+rect.w-3;
	scrollbar_rect.y=widget_rect.y;

	if(run_state)
		computeScrollbar();// 计算滑块的长度
}

void XVScrollbar::setParentLength(int& length)
{
	parent_length=length;
	
	if(run_state)
		computeScrollbar();// 计算滑块的长度
}

XRect* XVScrollbar::getRect()
{
	return &widget_rect;
}
//计算滑块的长度
//计算滑块每个像素代表依赖组件的滑动多久
void XVScrollbar::computeScrollbar()
{
	int h=widget_rect.h;
	float rate;
	
	if(parent_length <h) parent_length=h;
		
	rate=(float)h/(float)parent_length;
	scrollbar_rect.h=(int)((float)h*rate);
	
	if(scrollbar_rect.h <3) scrollbar_rect.h=3; //滑块的长度不能太小
	
	scale= (float)(parent_length-widget_rect.h)/(float)(widget_rect.h-scrollbar_rect.h);
}

void XVScrollbar::paint()
{
	boxColor(screen,scrollbar_rect.x,scrollbar_rect.y+scrollbar_position,
			scrollbar_rect.x+scrollbar_rect.w,
			scrollbar_rect.y+scrollbar_rect.h+scrollbar_position,
			BgColor);
	
	run_state=true;
}

void XVScrollbar::move(int& length)
{
	parentMoveLength+=length;
	
	if(parentMoveLength <0) parentMoveLength=0;
	scrollbar_position=(int)((float)parentMoveLength/scale);
	
	paint();
}

int XVScrollbar::getScrollbarTopLength()
{
	return scrollbar_position;
}
int XVScrollbar::getScrollbarBottomLength()
{
	return (widget_rect.h+widget_rect.y) - (scrollbar_rect.y+scrollbar_rect.h+scrollbar_position);
}

void XVScrollbar::setScrollbarBottom()
{
	parentMoveLength=parent_length-widget_rect.h;
	scrollbar_position=widget_rect.h-scrollbar_rect.h;
	
	paint();
}
void XVScrollbar::setScrollbarTop()
{
	parentMoveLength=0;
	scrollbar_position=0;
	paint();
}
void XVScrollbar::localUpdate(XRectF& pRect,XObject *pObject)
{
	if(pObject == this )
		return ;
	boxColor(screen,scrollbar_rect.x,scrollbar_rect.y+scrollbar_position,
			scrollbar_rect.x+scrollbar_rect.w,
			scrollbar_rect.y+scrollbar_rect.h+scrollbar_position,
			BgColor);
}
