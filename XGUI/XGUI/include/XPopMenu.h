//弹出菜单,是一个动态组件

#ifndef __XPOPMENU_H
#define __XPOPMENU_H

#include "XArray.h"
#include "XAbstractWidegt.h"
#include "XItem.h"

class XPopMenu:public XAbstractWidget
{
public:
	XPopMenu();
	~XPopMenu();
	
	void addItem(string text);
	void addItem(XItem& item);
	//popmenu可以不用删除item
	void removeItem(int item);
	void setMarginLeft(const int& left);
	
	//设置item背景颜色
	void setItmeColor(Uint32 color);
	//设置分割线颜色
	void setDividerLine(Uint32 color);
	// 添加分割线，可以设置不同的分割线 ...没实现
	void setDividerLine(); 
	
	void setPosition(int x,int y);
	void setMaxSize(int w,int h);
	
	//false显示，true不显示
	void show(bool nShow);
protected:
	
	//设置背景
	virtual void onDrawBackground(XRectF& rectF);
	virtual void onSize(int w,int h);
	virtual void onPosition(int x,int y);
	virtual void localUpdate(XRectF& pRect,XObject *pObject);
	
	bool responseEvent(X_Event *event);
	bool focusEvent(X_Event *event); //当此组件作为焦点组件时响应事件
	
private:
	void update(){}
	void changSize();
	//绘制
	void paint();
	void erase();
	XItem * selectIndex(X_Event *event);
private:
	
	bool           run_state;
	int            m_indexTop; //显示的最上面的item
	
	XSize          m_MaxSize;

	XArray<XItem*> m_array; //item数组
	Uint16         m_left; //item左边边距
	Uint32         m_itemBGColor;
	Uint32         m_dividerColor;
	
	static XItem *pSelectItem;    //已经选中的Item
};

#endif
