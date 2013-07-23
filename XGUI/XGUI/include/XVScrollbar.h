//水平滚动条

#ifndef _XVSEPARATOR_H
#define _XVSEPARATOR_H

#include "XAbstractWidegt.h"


class XListBox;

class XVScrollbar:public XAbstractWidget
{
public:
	XVScrollbar();
	void setBgColor(Uint32 color);
	void setWidth(int& w);
private:

	void setLayoutRect(XRect* rect){}
	void setLayoutSize(Uint16 w,Uint16 h){}
	
	void update(){}
	bool responseEvent(X_Event *event){ return false;}
	bool focusEvent(X_Event *event){return false;}
	
	XRect* getRect();
	//以下两个必须设置
	void setRect(XRect& rect); //不提供设置设置宽度
	void setParentLength(int& length);//依赖的组件的长度
	
	void setScrollbarBottom(); //将scrollbar设置到最下面
	void setScrollbarTop();    //将scrollbar设置到最上面
	
	int getScrollbarTopLength();
	int getScrollbarBottomLength();
	
	void computeScrollbar();//计算滑块的长度
	void move(int& length);//依赖的组件滚动的长度,而设置位置,设置的是整个组件区的Rect
	void paint();
	
	void drawChild(XRectF& pRect,XObject *pObject){};
	void drawParent(XRectF& pRect,XObject *pObject){};
	void localUpdate(XRectF& pRect,XObject *pObject);
private:
	Uint32 BgColor;//滑块背景色
	
	bool   run_state;
	Uint16 scrollbar_position;//滑块的位置
	int    parentMoveLength;  //依赖组件移动的距离
	int    parent_length;     //依赖组件的长度
	
	XRect  scrollbar_rect;
	
	float  scale; //滑块滑动一个像素点，依赖的组件实际滚动多长
	
	friend class XListBox;
};

#endif

