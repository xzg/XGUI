#include "XPopMenu.h"
#include "XWidget.h"

XItem *XPopMenu::pSelectItem=NULL;

XPopMenu::XPopMenu()
{
	m_left=10;
	m_itemBGColor=0xffffffff;
	m_dividerColor=0x000000ff;
	
	m_indexTop=0;	
	
	m_MaxSize.w=100;
	m_MaxSize.h=80;
	
	widget_rect.w=30;
	
	run_state=false;
	
	setPosition(0,0);
}

XPopMenu::~XPopMenu()
{
	int i;
	
	for(i=0;i<m_array.getCount();++i)
	{
		free(m_array.getAt(i));
	}
}

void XPopMenu::addItem(string text)
{
	XItem *pItem = new XItem;
	
	pItem->setText(text);
	m_array.add(pItem);
	
	if(run_state == true)
	{
		pItem->setWidth(widget_rect.w);
		changSize();
	}
}

void XPopMenu::addItem(XItem& item)
{
	XItem *pItem = new XItem;
	memcpy(pItem,&item,sizeof(XItem));
	
	m_array.add(pItem);
	
	if(run_state == true)
	{
		pItem->setWidth(widget_rect.w);
		changSize();
	}
}


void XPopMenu::removeItem(int item)
{
	
}

void XPopMenu::setMarginLeft(const int& left)
{
	
}
	
void XPopMenu::setItmeColor(Uint32 color)
{
	m_itemBGColor=color;
}

void XPopMenu::setDividerLine(Uint32 color)
{
	m_dividerColor=color;
}

void XPopMenu::setPosition(int x,int y)
{
	widget_rect.x=x;
	widget_rect.y=y;
	
	onPosition(x,y);
	changSize();
}

void XPopMenu::setMaxSize(int w,int h)
{
	widget_rect.w=w;
	widget_rect.h=h;
	
	widget_rectF.x2=widget_rect.x+widget_rect.w;
	widget_rectF.y2=widget_rect.y+widget_rect.h;
	
	onSize(w,h);
}

void XPopMenu::show(bool nShow)
{
	if(nShow)
	{
		XAbstractWidget::focus_widget->onPaint(X_BUTTONLEAVE);
		paint();
		XAbstractWidget::setFocusWidget(this);
		run_state=true;
	}else
	{
		erase();
	}
}

///////////////////////////////////////////////////////////
void XPopMenu::onSize(int w,int h)
{
	
}

void XPopMenu::onPosition(int x,int y)
{
	widget_rectF.x1=x;
	widget_rectF.y1=y;
	
	changSize();
}

bool XPopMenu::responseEvent(X_Event *event)
{
	return false;
}

bool XPopMenu::focusEvent(X_Event *event)
{
	
	switch(event->type)
	{
		case X_MOUSEBUTTONDOWN:
			{
				JUDGE_MOUSE_BEGIN
				{
					pSelectItem=selectIndex(event);
					return true;
				}
				JUDGE_MOUSE_END
				{
					erase();
					return false;
				}
			}
		break;
		
		case X_MOUSEBUTTONUP:
			{
				JUDGE_MOUSE_BEGIN
				{
					erase();
					return true;
				}
				JUDGE_MOUSE_END
			}
		break;
		
		case X_MOUSEMOTION:
			{
				JUDGE_MOUSE_BEGIN
				{
					XItem* lItem=selectIndex(event);
					//如果上次的等于本次的
					//if(lItem == pSelectItem)
					//{
					//	return false;
					//}
					//恢复
					if(pSelectItem)
					{
						pSelectItem->paintUpdate(m_dividerColor,m_itemBGColor);
					}
					
					//重绘制,一般情况下lItem不会为NULL
					pSelectItem=lItem;
					Uint32 color=0xb2b3ffff;
					pSelectItem->paintUpdate(m_dividerColor,color);
					SDL_Flip(screen);
					return true;
				}
				JUDGE_MOUSE_END
				{
					//恢复
					if(pSelectItem)
					{
						pSelectItem->paintUpdate(m_dividerColor,m_itemBGColor);
					}
					pSelectItem=NULL;
				}
				
			}
		break;
		
		case X_RESUME:
			erase();
		break;
	}
	return false;
}

void XPopMenu::localUpdate(XRectF& pRect,XObject *pObject)
{
	if(pObject == this)
	{
		return ;
	}
	
	int i;
	int count=m_array.getCount();
	for(i=m_indexTop;i<count;++i)
	{
		(m_array.getAt(i))->paintAll(m_dividerColor,m_itemBGColor);
	}
	rectangleColor(screen,widget_rect.x,widget_rect.y,widget_rectF.x2-1,widget_rectF.y2-1,0x7991c9ff);
}

void XPopMenu::onDrawBackground(XRectF& rectF)
{
	boxColor(screen,widget_rectF.x1,widget_rectF.y1,widget_rectF.x2,widget_rectF.y2,0xffffffff);
}

///////////////////////////////////////////////////////////
void XPopMenu::paint()
{
	int i;
	int count=m_array.getCount();
	
	int item_y=widget_rect.y;
	
	for(i=m_indexTop;i<count;++i)
	{
		(m_array.getAt(i))->setLocation(widget_rect.x,item_y);
		item_y+=m_array.getAt(i)->getHeight();
	}
	
	onDrawBackground(widget_rectF);
	
	for(i=m_indexTop;i<count;++i)
	{
		(m_array.getAt(i))->paint(m_dividerColor,m_itemBGColor);
	}
	
	rectangleColor(screen,widget_rect.x,widget_rect.y,widget_rectF.x2-1,widget_rectF.y2-1,0x7991c9ff);
	
	showWidget();
}

void XPopMenu::erase()
{
	
	setClipRect(widget_rect);
	XAbstractWidget::root_widget->findOverridingWidget(widget_rect,this);	
	SDL_UpdateRect(screen,widget_rect.x,widget_rect.y,widget_rect.w+1,widget_rect.h+1);
	XAbstractWidget::setFocusWidget(XAbstractWidget::root_widget);
	cleanClipRect();
}

XItem* XPopMenu::selectIndex(X_Event *event)
{
	int i=m_indexTop;
	int count=m_array.getCount();
	int y=widget_rect.y;
	
	
	for(;i<count;++i)
	{
		y+=m_array.getAt(i)->getHeight();
		if(y >= event->motion->y)
		{
			return m_array.getAt(i);
		}
		
	}
	
	return NULL;
}

void XPopMenu::changSize()
{
	int i=0;
	int m=0;
	widget_rect.h=0;
	XSize lSize=getWindowSize();
	
	//计算Rect宽和高
	for(;i<m_array.getCount();++i)
	{
		widget_rect.h+=m_array.getAt(i)->getHeight();
		m=m_array.getAt(i)->getItemMinWidth()+2*m_left;
		if(widget_rect.w < m)
			widget_rect.w=m;	
	}
	widget_rect.h+=1;
	
	//判断是否符合屏幕
	if(widget_rect.h+widget_rect.y > lSize.h)
	{
		widget_rect.y=lSize.h-widget_rect.h-2;
		widget_rectF.y1=widget_rect.y;
	}
	if(widget_rect.w+widget_rect.x > lSize.w)
	{
		widget_rect.x=lSize.w-widget_rect.w-2;
		widget_rectF.x1=widget_rect.x;
	}
	
	//设置item的宽
	for(i=0;i<m_array.getCount();++i)
	{
		m_array.getAt(i)->setWidth(widget_rect.w);
	}
	
	widget_rectF.x2=widget_rect.x+widget_rect.w;
	widget_rectF.y2=widget_rect.y+widget_rect.h;
	
}



