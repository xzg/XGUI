#include "XLabel.h"

#define LABEL_H_MARGIN 8 //设置text距离外边框水平距离
#define LABEL_V_MARGIN 6 //设置text距离外边框竖直距离

XLabel::XLabel()
{
	is_underLine=false;
	
	screen=NULL;
	font_color=0x000000ff;
	
	setBackground(0xffffff00);
	
	string str="hell,world";
	setText(str);
}
void XLabel::setFontColor(Uint32 color)
{
	font_color=color;
}
void XLabel::setText(string text)
{
	int   w,h;
	
	this->text=text;
	font->getFontSize(text.c_str(),&w,&h);
	
	if(w<=0 || h < 0)
	{
		w=LABEL_H_MARGIN;
		h=0;
	}
	
	w+=1;
	
	//如果父节点不是布局容器，那么在运行后text发生变化，不重新布局
	
	if(parent)
		setSize(w,h);
	else
		setLayoutSize(w,h);
}

void XLabel::setUnderLine(bool underLine)
{
	is_underLine=underLine;
}
/************************************************/
void XLabel::onInit()
{
	XWidget::onInit();
}
void XLabel::draw()
{
	onDrawBackground(widget_rectF);
		
	if(is_underLine)
		font->displayFontUnder(text.c_str(),font_color,widget_rect,true);
	else
		font->displayFont(text.c_str(),font_color,widget_rect,true);	
}

void XLabel::onPaint(Uint16 msg)
{
	if(msg == X_PAINT)
	{
		draw();
		
		//子组件	
		//if(layout)
		//	layout->drawChild(widget_rectF,this);
			
	}else if(msg == X_UPDATE)
	{
		setClipRect(widget_rect);
		
		//重绘制父组件
		if(lucency && parent)
		{
			parent->drawParent(widget_rectF,this);
		}
		
		draw();	
		//子组件
		//if(layout)	
		//	layout->drawChild(widget_rectF,this);
		
		showWidget();	
		cleanClipRect();		
	}
}

void XLabel::localUpdate(XRectF& pRect,XObject *pObject)
{
	if(pObject == this || !visible)
		return ;
		
	this->draw();
}
