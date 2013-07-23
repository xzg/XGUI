#ifndef _XITEM_H
#define _XITEM_H

#include "XAbstractWidegt.h"

class XWidget;

typedef enum
{
	SECTION,
	ALL
}ITEM_TYPE;

class XItem:public XAbstractWidget
{
public:
	XItem(XWidget *parent=NULL);	
	void setText(string& text);
	string& getText();
	void setFontColor(Uint32 color);
	void setMarginLeft(const int& left);   //设置左间距
	void setMarginTB(const int& tb);       //设置上下间距
	void setFont(XFont *font);             //设置字体
	void setWidth(const int& w);           //设置宽度
	int  getItemMinWidth();
	const Uint16& getHeight();
	XPoint getPosition();
	
	void setIcon();                // 等待添加...
	XRect getEffective();          //得到有效区域
	void show();
private:
	
	bool focusEvent(X_Event *event){return false;}
	void setLocation(Uint16 x,Uint16 y);//设置位置
	
	//多行没做
	void curHeight();                   //计算H
	
	//不画出背景，也不刷新到屏幕
	void paint(Uint32 &divider_color,Uint32& bg);
	//字体和背景一起画出，但是不刷新到屏幕
	void paintAll(Uint32 &divider_color,Uint32& bg);
	//当绘制某一项时调用，此函数会刷新到屏幕，并通知覆盖组件重绘制
	void paintUpdate(Uint32 &divider_color,Uint32& bg);
	//当重画某一项是调用此函数，且某一项有可能不绘制全部
	SDL_Rect paint(XDC *pdc,Uint32 &divider_color,Uint32& bg);
	//基本不用...
	void paint(XDC *pdc,ITEM_TYPE type,Uint32 &,Uint32&);
	
	void draw(Uint32 &divider_color,Uint32& bg);
	
	virtual void onPaint(Uint16 msg);
	virtual void localUpdate(XRectF& pRect,XObject *pObject){};
	virtual void onSize(int w,int h);
	virtual void onPosition(int x,int y);
private:
		
	string text;
	Uint32 font_color;
	
	Uint8  margin_L;     //左间距 
	Uint8  margin_TB;    //上边间距和下边间距
	
	XFont *font;
	XWidget *parent;
	
	friend class XListBox;
	friend class XPopMenu;
};

#endif
