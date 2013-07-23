#include "XWidget.h"
#include "XPopMenu.h"

int XWidget::v_variable=0;
int XWidget::h_variable=0;
/*默认BOX与BOX边距*/
int XWidget::h_margin=6;
int XWidget::v_margin=6;


void*     XWidget::clone=NULL;

XWidget::XWidget()
{
	layout=NULL;
	parent=NULL;
	
	hVariable=false;
	vVariable=false;
	
	setFont(NULL);
}

XWidget::XWidget(int w,int h)
{
	widget_rect.x=0;
	widget_rect.y=0;
	
	widget_rect.w=w;
	widget_rect.h=h;
	
	background_color=0xffffffff;
	
	layout=NULL;
	parent=NULL;
	
	hVariable=false;
	vVariable=false;
	
	setFont(NULL);
}

XWidget::~XWidget()
{
	
}

void XWidget::setLayout(XLayout *layout)
{
	this->layout=layout;
	
	layout->setParent(this,WIDGET); //此组件是layout的父节点
}

const ID XWidget::getId()
{
	return (ID)this;
}

void XWidget::setLaction(int x,int y)
{
	//这里添加一个发送重新布局消息，但是必须在程序运行之后发送
	if(run_state && (x !=widget_rect.x || y != widget_rect.y))
	{
		X_Event event;
		event.type = X_RELAYOUT;
		XAbstractWidget::sendSystemMsg(&event);
		
		widget_rect.x=x;
		widget_rect.y=y;
		
		onPosition(x,y);//通知大小改变	
	}else
	{
		widget_rect.x=x;
		widget_rect.y=y;
	}
}
void XWidget::setSize(int w,int h)
{
	//这里添加一个发送重新布局消息，但是必须在程序运行之后发送
	if(run_state && (w !=widget_rect.w || h != widget_rect.h))
	{
		X_Event event;
		event.type = X_RELAYOUT;
		XAbstractWidget::sendSystemMsg(&event);
		
		this->widget_rect.w=w;
		this->widget_rect.h=h;
		
		onSize(w,h);//通知大小改变	
	}else
	{
		this->widget_rect.w=w;
		this->widget_rect.h=h;
	}
}

bool XWidget::isChild(XWidget *pObject)
{

	if(layout)
	{
		return layout->isChild(pObject);
	}
	return false;
}

Uint8 XWidget::getWidgetState()
{
	return widget_state;
}

/*返回不可变组件rect
* 如果是可变组件需覆盖重写此方法
*/
XRect XWidget::getMinRect()
{
	return widget_rect;
}
void XWidget::setRect(XRect* rect)
{
	//这里添加一个发送重新布局消息，但是必须在程序运行之后发送
	if(run_state && (rect->w !=widget_rect.w || rect->h != widget_rect.h))
	{
		X_Event event;
		event.type = X_RELAYOUT;
		XAbstractWidget::sendSystemMsg(&event);
		
		widget_rect=*rect;
		onSize(rect->w,rect->h);//通知大小改变
		onPosition(rect->x,rect->y);
		
		return ;
	}
	
	widget_rect=*rect;
}


XSurface* XWidget::getScreen()
{
	return screen;
}
/*是否可见*/
void XWidget::setVisible(bool visible)
{
	this->visible=visible;
}

/*是否可变*/
void XWidget::setVVariable(bool variable)
{
	this->vVariable=variable;
}

void XWidget::setHVariable(bool variable)
{
	this->hVariable=variable;
}


void XWidget::setFont(XFont *font)
{
	if(!share_font)
	{
		share_font = new XFont;
	}
	
	if(font)
		this->font=font;
	else 
		this->font = share_font;	
}

void XWidget::show()
{
	if(!this->screen){
		
		widget_rect.x=0;  //设置本组件大小
		widget_rect.y=0;
		widget_rect.w=629;
		widget_rect.h=477;
		
		preClipRect = widget_rect;
		setLayoutRect(&widget_rect);
		
		background_color=0xe7e0e7ff;
		screen=SDL_SetVideoMode(widget_rect.w, widget_rect.h, 0, SDL_ANYFORMAT); //主surface
		if(!screen)
		{
			X_Event event;
			event.type =X_QUIT;
			XAbstractWidget::sendSystemMsg(&event);
		}
		//将此作为根节点
		XAbstractWidget::setRoot(this);
		XAbstractWidget::setFirstRoot(this);	
	}
	else{
		int w,h;
		if(widget_rectF.x2 >= screen->w)
		{
			w= screen->w - widget_rectF.x1;
		}else 
			w= widget_rect.w+1;
			
		if(widget_rectF.y2 >= screen->h)
		{
			h= screen->h - widget_rectF.y1;
		}else 
			h= widget_rect.h+1;
		
		SDL_UpdateRect(screen,widget_rect.x,widget_rect.y,w,h);
	}		
}

void XWidget::hide()
{
	
}

void XWidget::update()
{
	onPaint(X_UPDATE);
}

/**
*是否覆盖，如果覆盖那么重绘制
*/
void XWidget::findOverridingWidget(XRect& pRect,XObject *pObject)
{
	if(!visible)
	{
		return ;
	}
		
	XPoint centerPoint={widget_rect.w/2+widget_rect.x,widget_rect.h/2+widget_rect.y};
	XPoint srcPoint={pRect.w/2+pRect.x,pRect.h/2+pRect.y};
	
	int wtw_w=pRect.w/2 + widget_rect.w/2;
	int wtw_h=pRect.h/2 + widget_rect.h/2;
	
	int w=centerPoint.x-srcPoint.x;
	int h=centerPoint.y-srcPoint.y;
	
	if(w <0) w=-w;
	if(h <0) h=-h;
	
	
	//找到合适的组件
	if(w <=wtw_w && h <=wtw_h)
	{	
		XRectF rectF={pRect.x,pRect.y,pRect.x+pRect.w,pRect.y+pRect.h};
		localUpdate(rectF,pObject);
	
		if(layout)
		{	
			layout->findOverridingWidget(pRect,pObject);
		}
	}
}

void XWidget::setBackground(Uint32 color)
{
	background_color=color;
	
	//如果设置透明
	if( (color&0x000000ff)  != 0xff)
	{
		lucency =true;
	}else
	{
		lucency =false;
	} 
}
const Uint32& XWidget::getBackground()
{
	return background_color;
}
XLayout* XWidget::getParent()
{
	return parent;
}

XLayout* XWidget::getLayout()
{
	return layout;
}

void XWidget::onInit()
{
	run_state=true; //设置本组件运行状态
	if(layout)
		layout->init();
}

void XWidget::reLayout()
{
	computeLayout();
	onPaint(X_PAINT);
	showWidget();
}

/*
*布局递归算法
*私有成员
*/
void XWidget::computeLayout()
{
	/*数目*/
	int h_v=0;
	int v_v=0;
	int w=0;
	int h=0;
	
	h_margin=6;
	v_margin=6;
	
	if(layout)
	{
		h_v=layout->getHVariableCount(&w,true);
		v_v=layout->getVVariableCount(&h,true);
		layout->setRect(&widget_rect);
		layout->computeLayout();	
	}
}

/*
* 第一步应该查找子节点
* 如果没有查找到响应的字节点则此组件响应
*/
//绘制从父节点往子节点绘制
bool XWidget::responseEvent(X_Event *event)
{
	if(!visible)
	{
		return false;
	}

#ifdef USE_MOUSE	
	if(event->type >= X_MOUSEMOTION && event->type <= X_MOUSEBUTTONUP )
	{
		//如果鼠标或者点击在矩形内
		if(devEvent(event)){
			
			return true;
		}else
		{
			switch(event->type)
			{
				case SDL_MOUSEMOTION:
				{
					mouseLeaveEvent(event);
				}
				break;
			}
		}
	}
#endif
	else if(event->type >= X_PAINT)
	{
		onPaint(event->type);
	}
	
	return false;		
	// 处理本组件消息.................
}

//此组件作为焦点组件，处理消息事件..............s
bool XWidget::focusEvent(X_Event *event)
{
	return false;
}

bool XWidget::devEvent(X_Event *event)
{
	//如果是此组件
	if(JUDGEMOUSE(event,widget_rect))
	{
		//先判断子组件是否符合
		if(layout && layout->responseEvent(event) )
		{
			return true;
		}

		switch(event->type)
		{
#ifdef USE_MOUSE
			case SDL_MOUSEMOTION:
				{
					mouseEnterEvent(event);
					
					if(isFocus() && event->motion->state == SDL_PRESSED)
					{
						focusEnter();	
					}else if(!isFocus() && event->motion->state == SDL_PRESSED)
					{
						XAbstractWidget::focus_widget->focusLeave();	
					}
				}
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				{
					switch(event->mouse_type)
					{
						case X_BUTTON_LEFT:
							{
								XAbstractWidget::setFocusWidget(this);
								mouseLPressEvent(event);
							}
							break;
							
						case X_BUTTON_RIGHT:
							{
								mouseRPressEvent(event);	
							}
							
						case X_BUTTON_WHEELUP:
							{
								mouseWheelUp(event);
							}
							break;
							
						case X_BUTTON_WHEELDOWN:
							{
								mouseWheelDown(event);
							}
							break;
							
						case X_BUTTON_MIDDLE:
							{
								mouseMPressEvent(event);	
							}
							break;
					}
						
					if(mouse_listener)
					{
						mouse_listener->mousePressed(event,(ID)this);
					}
				
				}
				break;
				
 			case SDL_MOUSEBUTTONUP:
				{	
					switch(event->mouse_type)
					{
						case X_BUTTON_LEFT:
							{
								//printf("this=%x,focus=%x\n",this,XWidgetProcess::focus_widget);
								if(isFocus())
								{
									mouseLReleaseEvent(event);
								}
							}
							break;
							
						case X_BUTTON_RIGHT:
							{
								mouseRReleaseEvent(event);	
							}
							break;
						case X_BUTTON_WHEELUP:
							{
								mouseWheelUp(event);
							}
							break;
							
						case X_BUTTON_WHEELDOWN:
							{
								mouseWheelDown(event);
							}
							break;
							
						case X_BUTTON_MIDDLE:
							{
								mouseMReleaEvent(event);	
							}
							break;
					}
				
					if(mouse_listener)
					{
						mouse_listener->mouseReleased(event,(ID)this);
					}
				}
				break;
#endif
		}
		return true;
	}
#ifdef USE_KEYBOARD
	switch(event->type)
	{
			case X_KEYDOWN:
				{
					keydownEvent(event);
				}
				break;
			case X_KEYUP:
				{
					keyReleaseEvent(event);
				}
				break;
	}
#endif
	return false;	
}

void XWidget::reDrawRect(const XRect& rect)
{
	if(!screen)
	{
		LOG("XWidget: screen is NULL");
		return ;
	}
	
	SDL_FillRect(screen,(XRect *)&rect,background_color);	
	SDL_UpdateRect(screen,rect.x,rect.y,rect.w,rect.h);
}

void XWidget::onSize(int w,int h)
{
	XAbstractWidget::onSize(w,h);
}
void XWidget::onPosition(int x,int y)
{
	XAbstractWidget::onPosition(x,y);
	XAbstractWidget::onSize(widget_rect.w,widget_rect.h);
}
//局部刷新
void XWidget::localUpdate(XRectF& pRect,XObject *pObject)
{
	if(pObject ==  this)
		return ;
		
	onDrawBackground(pRect);
}


void XWidget::drawChild(XRectF& pRect,XObject *pObject)
{
	localUpdate(pRect,pObject);
	if(layout)
	{
		layout->drawChild(pRect,pObject);
	}
}

void XWidget::drawParent(XRectF& pRect,XObject *pObject)
{
	if(parent && lucency)
	{
		parent->drawParent(pRect,pObject);
	}
	localUpdate(pRect,pObject);
}

void XWidget::onDrawBackground(XRectF& rectF)
{
	boxColor(screen,rectF.x1,rectF.y1,rectF.x2,rectF.y2,background_color);
}

bool XWidget::onCloseWindowEvent()
{
	return true;
}

void XWidget::onPaint(Uint16 msg)
{
	switch(msg)
	{
		case X_PAINT:
		case X_UPDATE:
			{
				onDrawBackground(widget_rectF);
				if(layout)
					layout->drawChild(widget_rectF,this);
				showWidget();
			}
			break;
		case X_RELAYOUT:
			{
				
			}
	}
}

void XWidget::draw()
{

}
void XWidget::paint()
{
	setClipRect(widget_rect);
	//重绘制父组件
	if(lucency && parent)
	{
		parent->drawParent(widget_rectF,this);
	}
	
	draw();
		
	//子组件
	if(layout)	
		layout->drawChild(widget_rectF,this);
	
	showWidget();	
	cleanClipRect();
}

