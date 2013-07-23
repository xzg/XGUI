#include "XAbstractWidegt.h"
#include"XDialog.h"

XSurface* XAbstractWidget::screen=NULL;
XFont*    XAbstractWidget::share_font =NULL;
XRect    XAbstractWidget::preClipRect;

XAbstractWidget  *XAbstractWidget::first_root=NULL;
XAbstractWidget  *XAbstractWidget::focus_widget=NULL;
XAbstractWidget  *XAbstractWidget::root_widget=NULL;
XList<XAbstractWidget* > XAbstractWidget::m_listHigh;
XList<XAbstractWidget* > XAbstractWidget::m_listLow;
XList<XAbstractWidget* > XAbstractWidget::m_listPollLow;

Uint32 NewTimerCallback(Uint32 interval, void *param)
{
	XAbstractWidget *pWidget=(XAbstractWidget *)param;
	
	pWidget->onTiemer();

	return interval;
}

XAbstractWidget::XAbstractWidget()
{
	visible =true;
	run_state = false;
	widget_state=X_ORIGINAL;
	
	key_listener = NULL;
	mouse_listener=NULL;
}

SDL_TimerID XAbstractWidget::setTimer(unsigned int interval)
{
	return SDL_AddTimer(interval,NewTimerCallback,this);
}
void XAbstractWidget::removeTimer(SDL_TimerID timerID)
{
	SDL_RemoveTimer(timerID);
}
void XAbstractWidget::onTiemer()
{
	
}

bool XAbstractWidget::match(XRectF& rectF)
{
	int wtw_w=(rectF.x2-rectF.x1)/2 + widget_rect.w/2;
	int wtw_h=(rectF.y2-rectF.y1)/2 + widget_rect.h/2;
	
	XPoint srcPoint={widget_rect.x+widget_rect.w/2,widget_rect.y+widget_rect.h/2};
	XPoint desPoint={rectF.x1+(rectF.x2-rectF.x1)/2,rectF.y1+(rectF.y2-rectF.y1)/2};
	
	int w=desPoint.x-srcPoint.x;
	int h=desPoint.y-srcPoint.y;
	
	if(w <0) w=-w;
	if(h <0) h=-h;
	
	//找到合适的组件
	if(w <=wtw_w && h <=wtw_h)
	{	
		return true;
	}
	
	return false;
}

bool XAbstractWidget::isFocus()
{
	if(XAbstractWidget::focus_widget == this)
	{
		return true;
	}
	
	return false;
}

XRect XAbstractWidget::getRect()
{
	return widget_rect;
}
XRectF XAbstractWidget::getRectF()
{
	return widget_rectF;
}

XSize XAbstractWidget::getWindowSize()
{
	XSize size={0};
	
	if(screen)
	{
		size.w= screen->w;
		size.h= screen->h;
	}
	
	return size;
}

void XAbstractWidget::setLayoutRect(XRect* rect)
{//在布局时所用
	
	widget_rect=*rect;
	
	onPosition(rect->x,rect->y);
	onSize(rect->w,rect->h);

}

void XAbstractWidget::setLayoutSize(Uint16 w,Uint16 h)
{
	widget_rect.w=w;
	widget_rect.h=h;
	
	onPosition(w,h);
}
/*
* 刷新，并发送消息
*/
void XAbstractWidget::showWidget()
{
	XAbstractWidget::updateAllGlobal(this);
	
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


void XAbstractWidget::show(XRectF& pRect)
{
	XAbstractWidget::updateAllGlobal(this);
	
	int w,h;
	if(pRect.x2 >= screen->w)
	{
		w= screen->w - pRect.x1;
	}else 
		w= widget_rect.w+1;
		
	if(widget_rectF.y2 >= screen->h)
	{
		h= screen->h - pRect.y1;
	}else 
		h= widget_rect.h+1;
	SDL_UpdateRect(screen,widget_rect.x,widget_rect.y,w,h);
}

void XAbstractWidget::show(XRect rect)
{
	XAbstractWidget::updateAllGlobal(this);
	SDL_UpdateRects(screen,1,&rect);
}

void XAbstractWidget::showFillScreen()
{
	SDL_Flip(screen);
}


void XAbstractWidget::setClipRect(XRect rect)
{
	int x2=rect.x+rect.w;
	int y2=rect.y+rect.h;
	
	if(x2 >= screen->w)
	{
		rect.w = screen->w -rect.x;
	}else 
		rect.w+=1;
		
	if(y2> screen->h)
	{
		rect.h = screen->h -rect.y;
	}else
		rect.h+=1;
	
	preClipRect = screen->clip_rect;
	SDL_SetClipRect(screen,&rect);	
}
void XAbstractWidget::cleanClipRect()
{
	SDL_SetClipRect(screen,NULL);
}

XRect XAbstractWidget::getClipRect()
{
	return screen->clip_rect;
}

void XAbstractWidget::setPreClipRect()
{
	SDL_SetClipRect(screen,&preClipRect);
}

void XAbstractWidget::onInit()
{
	
}

void XAbstractWidget::onPaint(Uint16 msg)
{
	
}

void XAbstractWidget::drawChild(XRectF& pRect,XObject *pObject)
{

}
void XAbstractWidget::drawParent(XRectF& pRect,XObject *pObject)
{
	
}
void XAbstractWidget::focusLeave()
{
	//onPaint(X_BUTTONLEAVE);
}

void XAbstractWidget::focusEnter()
{
	//onPaint(X_BUTTONENTER);
}

void XAbstractWidget::loseFocus()
{

}

bool XAbstractWidget::responseEvent(X_Event *event)
{
	if(!visible)
	{
		return false;
	}
	
#ifdef USE_MOUSE	
	if(event->type >= X_MOUSEMOTION && event->type <= X_MOUSEBUTTONUP )
	{
		if(devEvent(event)){
			
			return true;
		}
	}
#endif

#ifdef USE_KEYBOARD
	else if (event->type >= X_KEYDOWN && event->type <= X_KEYUP)
	{
		if(devEvent(event)){
			return true;
		}
	}
#endif
	return false;
}

//此组件作为焦点组件，处理消息事件..............s
bool XAbstractWidget::focusEvent(X_Event *event)
{
	return false;
}

bool XAbstractWidget::devEvent(X_Event *event)
{
	//如果是此组件
	if(JUDGEMOUSE(event,widget_rect))
	{
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
#ifdef USE_KEYBOARD
			case X_KEYDOWN:
				{
					if (key_listener)
					{
						key_listener->keyDown(event,(ID)this);
					}
					keydownEvent(event);
				}
				break;
			case X_KEYUP:
				{
					if (key_listener)
					{
						key_listener->keyUp(event,(ID)this);
					}
					keyReleaseEvent(event);
				}
				break;
#endif
		}
		
		return true;
	}
	
	return false;	
}

#ifdef USE_MOUSE	
void XAbstractWidget::setMouseListener(XMouseListener *mouse)
{
	this->mouse_listener = mouse;
	
	//可以做添加多个事件源，第一版本不支持算了******
}
#endif


void XAbstractWidget::setRoot(XAbstractWidget  *root)
{
	root_widget=root;

}

void XAbstractWidget::setFirstRoot(XAbstractWidget *root)
{
	first_root =root;
}

XWidget* XAbstractWidget::getRoot()
{
	return (XWidget* )root_widget;
}

void XAbstractWidget::setFocusWidget(XAbstractWidget  *focus)
{
	focus_widget = focus;
}

void XAbstractWidget::sendSystemMsg(X_Event *user)
{
	SDL_Event event;
	event.type = user->type;

	SDL_PushEvent(&event);
}

void XAbstractWidget::addGlobalHigh(XAbstractWidget  *global)
{
	m_listHigh.append(global);
}

void XAbstractWidget::addGlobalLow(XAbstractWidget  *global)
{
	m_listLow.append(global);
	m_listPollLow.append(global);
}

void XAbstractWidget::removeGlobalHigh(XAbstractWidget  *global)
{
	int i=0;

	int count = m_listHigh.getCount();
	XPOSITION  pos = m_listHigh.getHeadPosition(); 

	for(;i<count;i++)
	{
		if(m_listHigh.getNext(pos) == global)
		{
			m_listHigh.remove(i);
			break;
		}
	}
}

void XAbstractWidget::removeGlobalLow(XAbstractWidget  *global)
{
	int i=0;
	int count = m_listLow.getCount();
	XPOSITION  pos = m_listLow.getHeadPosition(); 

	for(;i<count;i++)
	{
		if(m_listLow.getNext(pos) == global)
		{
			m_listLow.remove(i);
			m_listPollLow.remove(i);
			break;
		}
	}
}

void XAbstractWidget::updateAllGlobal(XAbstractWidget  *pObject)
{
	XRect   l_Rect=((XWidget *)pObject)->getRect();
	XRectF   l_RectF=((XWidget *)pObject)->getRectF();
	XAbstractWidget* pWidget;
	int i=0;
	int count = m_listLow.getCount();
	XPOSITION  pos = m_listLow.getHeadPosition(); 

	for(;i<count;i++)
	{
		pWidget=m_listLow.getNext(pos);
		pWidget->findOverridingWidget(l_Rect,pObject);
	}

	count = m_listHigh.getCount();
	pos = m_listHigh.getHeadPosition(); 

	for(i=0;i<count;i++)
	{
		m_listHigh.getNext(pos)->localUpdate(l_RectF,pObject);;
	}
}

//上上
void XAbstractWidget::updateGlobalHighUp(XAbstractWidget  *global,XRectF& RectF)
{
	XPOSITION  pos = m_listHigh.find(global); 

	if(!pos.isEmpty())
		m_listHigh.getNext(pos);
	while(!pos.isEmpty())
	{
		m_listHigh.getNext(pos)->localUpdate(RectF,global);
	}
}

//上下
void XAbstractWidget::updateGlobalHighDown(XAbstractWidget  *global,XRectF& RectF)
{
	int i;
	int count  = m_listLow.getCount();
	XPOSITION  pos=m_listLow.getHeadPosition(); 
	XAbstractWidget* pWidget;

	XRect rect={RectF.x1, RectF.y1, RectF.x2-RectF.x1, RectF.y2-RectF.y1};

	//根组件
	first_root->findOverridingWidget(rect,global);

	for(i=0;i<count;i++)
	{
		m_listLow.getNext(pos)->findOverridingWidget(rect,global);
	}

	pos = m_listHigh.getHeadPosition(); 
	while(!pos.isEmpty())
	{
		pWidget=m_listHigh.getNext(pos);
		if(pWidget==global) 
			return ;
		pWidget->localUpdate(RectF,global);

	}
}

//下上
void XAbstractWidget::updateGlobalLowUp(XAbstractWidget  *global,XRectF& RectF)
{
	XRect   l_Rect={RectF.x1,RectF.y1,RectF.x2-RectF.x1,RectF.y2-RectF.y1};

	XPOSITION  pos = m_listLow.find(global); 
	if(!pos.isEmpty())
		m_listLow.getNext(pos);
	while(!pos.isEmpty())
	{
		m_listLow.getNext(pos)->findOverridingWidget(l_Rect,global);
	}

	pos = m_listHigh.getHeadPosition();
	while(!pos.isEmpty())
	{
		m_listHigh.getNext(pos)->localUpdate(RectF,global);
	}
}
//下下
void XAbstractWidget::updateGlobalLowDown(XAbstractWidget  *global,XRectF& RectF)
{	
	XAbstractWidget* pWidget;	
	XRect rect={RectF.x1, RectF.y1, RectF.x2-RectF.x1, RectF.y2-RectF.y1};

	//根组件
	first_root->findOverridingWidget(rect,global);

	//同类
	XPOSITION  pos= m_listLow.getHeadPosition(); 
	while(!pos.isEmpty())
	{
		pWidget=m_listLow.getNext(pos);
		if(pWidget==global) 
			return ;
		pWidget->findOverridingWidget(rect,global);
	}
}

bool XAbstractWidget::globalPoll(X_Event *event)
{
	XAbstractWidget* pWidget;

	if (focus_widget)
	{
		if (event->type>= X_KEYDOWN && event->type <=X_KEYUP)
		{
			focus_widget->devEvent(event);
		}		
	}

	m_listPollLow.final();
	if(!m_listPollLow.isEmpty())
	{	
		pWidget=m_listPollLow.takeLast();
		if(pWidget->responseEvent(event))
		{
			return true;
		}
		if(((XDialog *)pWidget)->isModal())
		{
			return false;
		}
		while(m_listPollLow.isPrevious())
		{
			pWidget=m_listPollLow.previous();

			if(pWidget->responseEvent(event))
			{
				return true;
			}
			if(((XDialog *)pWidget)->isModal())
			{
				return false;
			}
		}
	}	
	return false;
}

