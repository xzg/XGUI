#include "XDialog.h"


#define TITLECOLOR  0xffffffff

bool childEvent(XDialog *pDlg,X_Event *event);
bool popEvent(XDialog *pDlg,X_Event *event);
bool modalEvent(XDialog *pDlg,X_Event *event); 

XDialog::XDialog()
{
	setSize(200,150);
	setBackground(0xffffffff);
	
	m_TitleColor=0x8ba8ffff;

	m_MinSize.w=0;
	m_MinSize.h=0;
	
	m_BtnClose.setText("X");
	m_BtnClose.setBackground(0x00);
	
	pTitleLayout = new XHBoxLayout;
	
	pOldRoot=NULL;
	m_bIsPress=false;
	m_bDisplay=false;
	
}

XDialog::~XDialog()
{
	
}

void XDialog::create(int style)
{
	m_style=style;
	
	if(m_style&POP)
	{
		widget_rect.x=100;
		widget_rect.y=100;	
	}
	
}

void XDialog::setTitle(string text)
{
	m_StrTitleText = text;
}

void XDialog::showWindow(bool nShow)
{
	if(m_bDisplay)
	{
		return ;
	}
	
	onInit();
	
	m_bDisplay=true;
	visible = nShow;
	m_BtnClose.setVisible(nShow);
	
	if(nShow)
	{
		//恢复
		if(XAbstractWidget::focus_widget)
			XAbstractWidget::focus_widget->onPaint(X_BUTTONLEAVE);
		
		XAbstractWidget::addGlobalLow(this);
		onPosition(widget_rect.x,widget_rect.y);
		onSize(widget_rect.w,widget_rect.h);
		computeLayout();
		
		onPaint(X_PAINT);
	}else 
		exit();
}

int  XDialog::doModal()
{
	if(m_bDisplay)
	{
		return 0;
	}
	
	onInit();
	
	m_bDisplay=true;	
	//恢复
	if(XAbstractWidget::focus_widget)
		XAbstractWidget::focus_widget->onPaint(X_BUTTONLEAVE);
	XAbstractWidget::setFocusWidget(this);
	
	pOldRoot = (XWidget *)XAbstractWidget::root_widget;
	XAbstractWidget::setRoot(this);
	XAbstractWidget::addGlobalLow(this);
	
	onPosition(widget_rect.x,widget_rect.y);
	onSize(widget_rect.w,widget_rect.h);
	computeLayout();
	
	visible = true;
	m_BtnClose.setVisible(true);
	
	onPaint(X_PAINT);
	
	return 0;
}

bool XDialog::isModal()
{
	if(pOldRoot)
		return true;
	return false;
}

void XDialog::setMinSize(int w,int h)
{
	m_MinSize.w=w;
	m_MinSize.h=h;
}

int ii=0;
void XDialog::move(int x,int y)
{	
	XPoint moveStart={widget_rect.x,widget_rect.y};
	XRect  moveRect;
	
	widget_rect.x+=x;
	widget_rect.y+=y;
	
	if(widget_rect.x <0) widget_rect.x=0;
	if(widget_rect.y <0) widget_rect.y=0;
	
	
	onPosition(widget_rect.x,widget_rect.y);
	onSize(widget_rect.w,widget_rect.h);
	computeLayout();
	widget_state=X_ORIGINAL;
	
	if(widget_rect.x - moveStart.x <0)
	{
		moveRect.x = widget_rect.x;
		moveRect.w = widget_rect.w+moveStart.x-widget_rect.x;
	}else 
	{
		moveRect.x = moveStart.x;
		moveRect.w = widget_rect.w+widget_rect.x - moveStart.x;
	}
	
	if(widget_rect.y - moveStart.y <0)
	{
		moveRect.y = widget_rect.y;
		moveRect.h = widget_rect.h+moveStart.y-widget_rect.y;
	}else 
	{
		moveRect.y = moveStart.y;
		moveRect.h = widget_rect.h+widget_rect.y - moveStart.y;
	}
	
	{
		//printf("1\n");
		XRectF rectF={moveRect.x,moveRect.y,moveRect.x+moveRect.w,moveRect.y+moveRect.h};
		setClipRect(moveRect);
		
		if(pOldRoot)
			XAbstractWidget::updateGlobalLowDown(this,rectF);	
		else 
			XAbstractWidget::updateGlobalLowDown(this,rectF);	
		
		
		draw();
			
		//子组件
		if(layout)	
			layout->drawChild(rectF,this);
		
		XAbstractWidget::updateGlobalLowUp(this,rectF);	
		
		SDL_Flip(screen);
		cleanClipRect();
	}
}

//*********************保护*************************

bool XDialog::responseEvent(X_Event *event)
{	
	if(!visible)
	{
		return false;
	}
	
	if(event->type == X_MOUSEBUTTONDOWN || event->type == X_MOUSEBUTTONUP)
	{
		m_pointClick.x=event->button->x;
		m_pointClick.y=event->button->y;
		
		if(JUDGEMOUSE1(event,m_TitleRectF))
		{
			if((m_style&TITLE)){
				if(pTitleLayout->responseEvent(event))
				{
					return true;
				}
			}
			m_bIsPress = true;
		}else
		{
			m_bIsPress = false;
		}
	}else if((m_style&POP) && event->type == X_MOUSEMOTION  && isFocus() && m_bIsPress)
	{
		move(event->motion->x - m_pointClick.x,event->motion->y - m_pointClick.y);
		
		m_pointClick.x = event->motion->x;
		m_pointClick.y = event->motion->y;
	
		return true;
	}

	return  XWidget::responseEvent(event);
}

bool XDialog::focusEvent(X_Event *event)
{
	if((m_style&POP) && event->type == X_MOUSEMOTION && event->motion->state== SDL_PRESSED && m_bIsPress)
	{
		move(event->motion->x - m_pointClick.x,event->motion->y - m_pointClick.y);
		
		m_pointClick.x = event->motion->x;
		m_pointClick.y = event->motion->y;
		
		return true;
	}
	return false;
}

void XDialog::onInit()
{
	XWidget::onInit();
	
	pTitleLayout->addSpace();
	pTitleLayout->addWidget(&m_BtnClose);
	pTitleLayout->setParent(this,WIDGET);
	
	m_BtnClose.setMouseListener(this);
}

void XDialog::erase()
{
	setClipRect(widget_rect);
	if(m_style&POP)
	{
		if(pOldRoot)
		{	
			XAbstractWidget::updateGlobalLowDown(this,widget_rectF);
			XAbstractWidget::updateGlobalLowUp(this,widget_rectF);	
		}else 
		{
			XAbstractWidget::updateGlobalLowDown(this,widget_rectF);
			XAbstractWidget::updateGlobalLowUp(this,widget_rectF);	
		}
	}else if(m_style&CHILD)
	{
		parent->drawParent(widget_rectF,this);
		
	}
	cleanClipRect();	
}

void XDialog::exit()
{
	visible =false;
	m_BtnClose.setVisible(false);
	m_bDisplay =false;
	
	erase();
	XWidget::show();
	if(pOldRoot)
	{
		XAbstractWidget::setRoot(pOldRoot);
		XAbstractWidget::setFocusWidget(pOldRoot);
		XAbstractWidget::removeGlobalLow(this);
		pOldRoot=NULL;
	}else if(m_style&POP)
	{
		XAbstractWidget::setFocusWidget(XAbstractWidget::root_widget);
		XAbstractWidget::removeGlobalLow(this);
	}
}

void XDialog::onPaint(Uint16 msg)
{
	if(msg == X_PAINT)
	{
		draw();
		//子组件	
		if(layout)
			layout->drawChild(widget_rectF,this);
		
		if(m_style&POP)
		{
			setClipRect(widget_rect);
			
			XAbstractWidget::updateGlobalLowUp(this,widget_rectF);	
			show();
			
			cleanClipRect();
		}
			
	}else if(msg == X_UPDATE )
	{
		if(m_style&CHILD)
		{
			XWidget::paint();
		}
		else if(m_style&POP)
		{
			paint();
		}
		
	}else if(msg == X_BUTTONDOWN && !pOldRoot)
	{
		
	}
	
	if(msg ==X_BUTTONUP)
	{
		m_bIsPress = false;
	}
}

void XDialog::paint()
{
	setClipRect(widget_rect);
	
	if(pOldRoot)
		XAbstractWidget::updateGlobalLowDown(this,widget_rectF);	
	else 
		XAbstractWidget::updateGlobalLowDown(this,widget_rectF);	
		
	draw();	
	//子组件
	if(layout)	
		layout->drawChild(widget_rectF,this);
	
	XAbstractWidget::updateGlobalLowUp(this,widget_rectF);	
	
	SDL_Flip(screen);
	
	cleanClipRect();		
}

void XDialog::draw()
{
	onDrawBackground(widget_rectF);
	
	//标题栏
	if( (m_style&TITLE) == TITLE)
	{
		rectangleColor(screen,widget_rectF.x1,widget_rectF.y1,
				widget_rectF.x2,widget_rectF.y2,m_TitleColor);
				
		boxColor(screen,m_TitleRectF.x1,m_TitleRectF.y1,m_TitleRectF.x2,m_TitleRectF.y2,
				m_TitleColor);
		
		rectangleColor(screen,widget_rectF.x1+1,widget_rectF.y1+1,
				widget_rectF.x2-1,widget_rectF.y2-1,m_TitleColor);
				
		XRect  rect={m_TitleRectF.x1,m_TitleRectF.y1,m_TitleRectF.x2-m_TitleRectF.x1,
				m_TitleRectF.y2-m_TitleRectF.y1};
		font->displayFont(m_StrTitleText.c_str(),TITLECOLOR,rect);
		
		pTitleLayout->drawChild(widget_rectF,this);

	}
}


void XDialog::onSize(int w,int h)
{	
	XWidget::onSize(w,h);
	
	m_TitleRectF.x2=widget_rectF.x2;
	m_TitleRectF.y2=widget_rect.y+20;
	
	
	XAbstractWidget *p =(XAbstractWidget *) &m_BtnClose;
	
	m_BtnClose.setSize(25,18);
	m_BtnClose.setLaction(widget_rectF.x2-28,widget_rect.y+2);
	
	p->onPosition(widget_rectF.x2-28,widget_rect.y+2);
	p->onSize(25,18);
	
	XRect  rect={m_TitleRectF.x1,m_TitleRectF.y1,m_TitleRectF.x2-m_TitleRectF.x1,
				m_TitleRectF.y2-m_TitleRectF.y1};
	pTitleLayout->setRect(&rect);
		
}

void XDialog::onPosition(int x,int y)
{
	XWidget::onPosition(x,y);
	
	m_TitleRectF.x1=x;
	m_TitleRectF.y1=y;
	
}

void XDialog::computeLayout()
{
	/*数目*/
	int h_v=0;
	int v_v=0;
	int w=0;
	int h=0;
	
	h_margin=6;
	v_margin=6;
	
	XRect rect ={m_TitleRectF.x1,m_TitleRectF.y2,widget_rect.w,widget_rect.h-(m_TitleRectF.y2-m_TitleRectF.y1)};
	if(layout)
	{
		h_v=layout->getHVariableCount(&w,true);
		v_v=layout->getVVariableCount(&h,true);
		layout->setRect(&rect);
		layout->computeLayout();	
	}
}

void XDialog::findOverridingWidget(XRect& pRect,XObject *pObject)
{
	//当某个对话框的子组件发起污染时，不用重绘制这个对话框了
	if( (pObject!=this && layout && layout->isChild(pObject)) || pObject==this)
	{
		return ;
	}
	XWidget::findOverridingWidget(pRect,pObject);
}

void XDialog::localUpdate(XRectF& pRect,XObject *pObject)
{
	//如果是静态对话框，并且是对话框的子组件发起的污染
	if(m_style&POP && pOldRoot && isChild((XWidget *)pObject))
	{
		//如果有老roor，刷新老root
		XRect rect={pRect.x1,pRect.y1,pRect.x2,pRect.y2};
		((XAbstractWidget *)pOldRoot)->findOverridingWidget(rect,pObject);
	}else if(m_style&POP && isChild((XWidget *)pObject)) //如果是非模态对话框，并且是对话框的子组件发起的污染
	{
		XRect rect={pRect.x1,pRect.y1,pRect.x2,pRect.y2};
		XAbstractWidget::root_widget->findOverridingWidget(rect,pObject);
	}
	
	if((pObject == this || !visible))
		return ;
	draw();
}

void XDialog::mousePressed(X_Event *event,const ID id)
{
	
}
void XDialog::mouseReleased(X_Event *event,const ID id)
{
	exit();
}
void XDialog::mouseEntered(X_Event *event,const ID id)
{
	
}

