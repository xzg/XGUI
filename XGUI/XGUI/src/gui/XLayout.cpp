/*
*所有的布局管理器的基类
*布局管理器既充当容器又扮演布局
*/

#include "XLayout.h"
#include "XAbstractWidegt.h"

XLayout::XLayout()
{
	v_v_num=-1;
	h_v_num=-1;
	min_h=0;
	min_h=0;
	
	//默认是不靠边
	h_dock=false;
	v_dock=false;
	
	parent = NULL;
	style = NONE;


/*	m_intHLeftDock = 0;
	m_intHRightDock = 0;
	m_intVTopDock =0;
	m_intVBottomDock = 0;*/
}

XLayout::~XLayout()
{
	list_int.begin();
	
	if( !list_int.isEmpty())
	{
		delete list_int.takeFirst();
		
		while(list_int.isNext())
		{
			delete list_int.next();
		}
	}
}
/*
*如果用布局容器添加组件或者布局管理器不会出现叠加现象
*所以就不需要去检查是否出现叠加
*/
void XLayout::addWidget(XWidget  *widget)
{
	XLYContainer *pContainer;
	
	list_widget.append(widget);
	
	widget->parent=this;  //此容器是widget的父节点
	
	pContainer = new XLYContainer;
	pContainer->type = WIDGET;
	pContainer->pObject = widget;
	
	list_int.append(pContainer);
}
//&&注意：如果将一个组件添加到多个布局容器，不检查是否在其它是否已加入组件
//        如果添加到其它组件容器，也不会出错

void XLayout::addLayout(XLayout *layout)
{	
	XLYContainer *pContainer;
	
	list_layout.append(layout);
	
	layout->setParent(this,LAYOUT); //此容器是layout的父节点
	
	pContainer = new XLYContainer;
	pContainer->type = LAYOUT;
	pContainer->pObject = layout;
	list_int.append(pContainer);
}

void XLayout::addSpace()
{
	XLYContainer *pContainer;
	
	pContainer = new XLYContainer;	
	pContainer->type = SPACE;
	pContainer->pObject = NULL;
	
	list_int.append(pContainer);
}

void XLayout::insert(int index)
{
	
}

void XLayout::remove(XWidget  *widget)
{
	
}

void XLayout::remove(XLayout  *layout)
{
	
}

void XLayout::remove(int index)
{
	
}

void XLayout::removeAll()
{
	
}

void XLayout::setSize(int w,int h)
{
	if(w < 0) w=0;
	if(h < 0) h=0;
		
	rect.w=w;
	rect.h=h;
}
/*
void XLayout::setHLeftDock(Uint16 dock)
{
	m_intHLeftDock = dock;
}
void XLayout::setHRightDock(Uint16 dock)
{
	m_intHRightDock = dock;
}
void XLayout::setVTopDock(Uint16 dock)
{
	m_intVTopDock = dock;
}
void XLayout::setVBottomDock(Uint16 dock)
{
	m_intVBottomDock = dock;
}
void XLayout::setMargin(Uint16 dock)
{
	m_intMargin = dock;
}
*/
void XLayout::setLaction(int x,int y)
{
	rect.x=x;
	rect.y=y;
}
#include <string.h>
void XLayout::setRect(XRect *rect)
{
	memcpy(&(this->rect),rect,sizeof(XRect));
}

XRect XLayout::getRect()
{
	return rect;
}
XRect XLayout::getMinRect()
{
	XRect swap_rect=rect;
	
	swap_rect.w = min_w;
	swap_rect.h = min_h;
	
	return swap_rect;
}
void XLayout::setParent(XObject* parent,Uint8 style)
{
	this->style = style;
	this->parent=parent;
	
}

LAYOUT_PARENT_STYLE XLayout::getParent(XObject *parent)
{
	parent=this->parent;
	return (LAYOUT_PARENT_STYLE)style;
}

XWidget* XLayout::getWidgetParent()
{
	if(style == WIDGET)
	{
		return (XWidget*)parent;
	}else if(style == LAYOUT)
	{
		return ((XLayout *)parent)->getWidgetParent();
	}
	
	return NULL;
}

void XLayout::compute(int *h_margin,int *v_margin,int *h_variable,int *v_variable)
{
	int w=0;
	int h=0;
	int h_v=0; //水平可变数量
	int v_v=0; //竖直可变数量
	
	h_v=this->getHVariableCount(&w);
	v_v=this->getVVariableCount(&h);
	
	//总共水平组件数量
	int h_count=this->getHCount();
	int v_count=this->getVCount();
	
	if(h_v){
		*h_variable = (rect.w-w-XWidget::h_margin*(h_count+1) )/h_v;
	}
	if(v_v){
	
		*v_variable = (rect.h-h-XWidget::v_margin*(v_count+1) )/v_v;
	}
	
	if(!h_v){
		
		if(h_count){
			*h_margin= (rect.w-w)/(h_count+1);
		}
	}else
	{
		*h_margin=XWidget::h_margin;
	}
	
	if(!v_v)
	{
		if(v_count){
			*v_margin= (rect.h-h)/(v_count+1);
		}
	}else
	{
		*v_margin=XWidget::v_margin;
	}
}


/*查找布局中对应的组件或者布局容器*/
bool XLayout::responseEvent(X_Event *event)
{
	XWidget *widget;
	XLayout *layout;
	
	XPOSITION  pos = list_widget.getHeadPosition();
	while(!pos.isEmpty())
	{
		widget = list_widget.getNext(pos);
		if( ((XAbstractWidget* )widget)->responseEvent(event) )
		{
			return true;
		}
	}
		
	pos = list_layout.getHeadPosition();
	while(!pos.isEmpty())
	{
		layout = list_layout.getNext(pos);
		if( layout->responseEvent(event) )
		{
			return true;
		}
	}
	return false;
}


void XLayout::init()
{
	//计算边距


	XPOSITION  pos = list_widget.getHeadPosition();
	while(!pos.isEmpty())
	{
		( (XAbstractWidget* )list_widget.getNext(pos))->onInit() ;
	}
		
	pos = list_layout.getHeadPosition();
	while(!pos.isEmpty())
	{
		 list_layout.getNext(pos)->init() ;
	}	
}
void XLayout::update()
{
	list_widget.begin();
	list_layout.begin();
	
	if(!list_widget.isEmpty() )
	{
		X_Event event;
		event.type = X_PAINT;
		
		( (XAbstractWidget*)list_widget.takeFirst() )->responseEvent(&event);
		
		while(list_widget.isNext())
		{	
			((XAbstractWidget*)list_widget.next())->responseEvent(&event);
		}
		
	}
	
	if(!list_layout.isEmpty())
	{
		list_layout.takeFirst()->update();
		
		while(list_layout.isNext())
		{	
			list_layout.next()->update();
		}
	}
	
}

void XLayout::findOverridingWidget(XRect& pRect,XObject *pObject)
{
	XPOSITION  pos = list_widget.getHeadPosition();
	while(!pos.isEmpty())
	{
		( (XAbstractWidget*)list_widget.getNext(pos) )->findOverridingWidget(pRect,pObject);
	}
	
	pos = list_layout.getHeadPosition();
	while(!pos.isEmpty())
	{
		list_layout.getNext(pos)->findOverridingWidget(pRect,pObject);		
	}
}


void XLayout::drawParent(XRectF& pRect,XObject *pObject)
{
	if(style == LAYOUT)
	{
		((XLayout *)parent)->drawParent(pRect,pObject);
	}else if(style == WIDGET)
	{	
		((XWidget *)parent)->drawParent(pRect,pObject);
	}
}

//查找子节点的时候应当匹配是否覆盖...
void XLayout::drawChild(XRectF& pRect,XObject *pObject)
{
	XPOSITION  pos = list_widget.getHeadPosition();
	while(!pos.isEmpty())
	{
		
		( (XAbstractWidget*)list_widget.getNext(pos) )->drawChild(pRect,pObject);
	}
	
	pos = list_layout.getHeadPosition();
	while(!pos.isEmpty())
	{
		list_layout.getNext(pos)->drawChild(pRect,pObject);		
	}
	
}

bool XLayout::isChild(XObject *pObject)
{
	XPOSITION  pos = list_widget.getHeadPosition();
	while(!pos.isEmpty())
	{
		if(pObject == list_widget.getNext(pos) )
			return true;
	}
		
	pos = list_layout.getHeadPosition();
	while(!pos.isEmpty())
	{
		if(list_layout.getNext(pos)->isChild(pObject))
				return true;
	}
	
	return false;
}


