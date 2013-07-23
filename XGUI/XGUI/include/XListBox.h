/*
*此组件是一个列表框
*/

#ifndef _XLISTBOX_H
#define _XLISTBOX_H

#include "XWidget.h"
#include "XItem.h"
#include "XIndexListener.h"
#include "XVScrollbar.h"

class XVScrollbar;

class XListBox:public XWidget
{
public:
	XListBox();
	~XListBox();

	void addItem(string text);
	void addItem(XItem& item);
	void removeItem(int item);
	void removeAll();
	
	void setItmeColor(Uint32 color);
	void setDividerLine(Uint32 color);
	void addDividerLine(); // 添加分割线，可以设置不同的分割线 ...没实现
	
	void addIndexlistener(XIndexListener *listener);
	void setLeftMargin();
	int&  CurrentIndex();//现在选中的索引
	
protected:
	
	XItem * selectIndex(X_Event *event);
	void setItemWidth(); //给所以的item设置宽度
	
	void resumeItem(); //恢复item
	void setResumeItem(XListBox *list,XItem *item);//设置需要恢复Item的List
	void paintSelectItem(XItem *item);
	
	//计算给每个Item分配位置
	void compute(int MoveLength);
	virtual void initDraw(); //初始化画出listbox
	virtual void draw();     //非出初始化画出
	virtual void onInit();
	virtual void onPaint(Uint16 msg);
	virtual void onSize(int w,int h);
	virtual void onPosition(int x,int y);
	virtual void changSize();//item的总长度变化，而不是ListBox大小变化
	virtual void localUpdate(XRectF& pRect,XObject *pObject);
	
	virtual void mouseWheelDown(X_Event *event);
	virtual void mouseWheelUp(X_Event *event);
	virtual void mouseLPressEvent(X_Event *event);
private:
	
	XVScrollbar vScrollbar;//竖直滚动条
	XIndexListener *indexListener;
	
	XList<XItem *> list_item;
	
	XItem *top_item;     //现在显示的最上面item
	XItem *bottom_item;  //现在显示的最后一个item
	
	int top_position;    //现在显示的最上面item的Y1坐标 
	int bottom_position; //现在显示的最下面item的Y2坐标
	
	Uint32 divider_color;
	Uint32 item_color;
	
	int index;              //选中Item的索引值
	
	static XListBox *pList; //需要恢复的List
	static XItem    *pItem;    //已经选中的Item
 
};

/*

*bug: 如果item的长度在运行后发生变化，那么滚动条就不对了
*/

#endif

