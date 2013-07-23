#include "XCheckBox.h"

#define BUTTON_H_MARGIN 5 //按钮区与外BOX水平距离
#define BUTTON_V_MARGIN 6 //按钮区与外BOX竖直距离

/****************说明************************/
// 1.checkbox可以不设置背景颜色
// 2.按钮区中对号用‘√’ 符号来代替，也可以用位图来做
/******************************************/


XCheckBox::XCheckBox()
{
	widget_listener=NULL;
	checked =true;
	box_color = 0x000000ff;
	setText("check");
	
	setBackground(0xffffff00);
}
void XCheckBox::setText(const string& text)
{
	int   w,h;
	this->text =text;
	
	if(font)
	{
		font->getFontSize((char *)text.c_str(),&w,&h);
	
		if(w<0 || h < 0)
		{
			w=0;
			h=0;
		}
		button_rect.w=h; //按钮区设置为和字体大高度相等的正方形
		button_rect.h=h;
		
		text_rect.w=w;  //文本区
		text_rect.h=h;
		
		w +=2 * BUTTON_H_MARGIN + h;
		h +=2 * BUTTON_V_MARGIN;
		
		setSize(w,h);
	}
}
void XCheckBox::setChecked(bool checked)
{
	this->checked = checked;
	
	if(run_state ==true)
		this->draw();
}
void XCheckBox::setColor(Uint32 color)
{
	this->box_color = color;
}
bool XCheckBox::getChecked()
{
	return this->checked;
}

void XCheckBox::addWidgetListener(XWidgetListener *widget)
{
	this->widget_listener =widget;
}

//**********************************

void XCheckBox::draw()
{
	onDrawBackground(widget_rectF);
	
	if(checked == true)
	{
		rectangleColor(screen,button_rect.x,button_rect.y,button_rect.x+button_rect.w,
				button_rect.y+button_rect.h,box_color);
		
		font->displayFont("√",box_color,button_rect);
		font->displayFont(text.c_str(),box_color,text_rect);
	}else{
		rectangleColor(screen,button_rect.x,button_rect.y,button_rect.x+button_rect.w,
				button_rect.y+button_rect.h,box_color);
		font->displayFont(text.c_str(),box_color,text_rect);
	}
}

void XCheckBox::onInit()
{
	XWidget::onInit();
}

void XCheckBox::onPaint(Uint16 msg)
{
	if(msg == X_PAINT)
	{
		draw();
		//子组件	
		if(layout)
			layout->drawChild(widget_rectF,this);
			
	}else if(msg == X_UPDATE )
	{
		paint();	
	}else if(msg == X_BUTTONUP)
	{
		if(checked)
			checked=false;
		else 
			checked=true;
			
		paint();
	}
}
void XCheckBox::paint()
{
	setClipRect(button_rect);
		
	//重绘制父组件
	if(lucency && parent)
	{
		parent->drawParent(widget_rectF,this);
	}
			
	//绘制本组件
	draw();
	//子组件
	if(layout)	
		layout->drawChild(widget_rectF,this);
	
	showWidget();	
	cleanClipRect();	
}


void XCheckBox::onSize(int w,int h)
{
	XWidget::onSize(w,h);
	
	button_rect.x = widget_rect.x+BUTTON_H_MARGIN;
	button_rect.y = widget_rect.y+BUTTON_V_MARGIN;
	
	text_rect.x = widget_rect.w+widget_rect.x -text_rect.w;
	text_rect.y = widget_rect.y+BUTTON_V_MARGIN;
}
void XCheckBox::onPosition(int x,int y)
{
	XWidget::onPosition(x,y);
	
	button_rect.x = widget_rect.x+BUTTON_H_MARGIN;
	button_rect.y = widget_rect.y+BUTTON_V_MARGIN;
	
	text_rect.x = widget_rect.w+widget_rect.x -text_rect.w;
	text_rect.y = widget_rect.y+BUTTON_V_MARGIN;
}
void XCheckBox::localUpdate(XRectF& pRect,XObject *pObject)
{
	if(pObject == this || !visible)
		return ;
	
	
	draw();
}
