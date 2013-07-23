#ifndef _XEDITBOX_H
#define _XEDITBOX_H

#include "XWidget.h"
#include "XTextListener.h"

/***************说明*******************/
// 1.BCursor 是编辑框跳动的光标
// 2.注意在设置位置的时候必须先传递screen
// 3.在第一次run之前要先设置光标的位置
/*************************************/

class BCursor
{
public:
	BCursor();
	~BCursor();
	void run();
	void stop();
	void setLocation(Uint16 x,Uint16 y);
	Uint16 getX();
	void setHeight(Uint16 h);
	void add(XSurface *screen);
	void setCursorColor(Uint32 color);
	void paint();
	bool getState();
	void resume();
private:
	
	bool   state;
	bool   visible;
	Uint16 x,y;
	Uint16 height;
	Uint32 cursor_color;
	SDL_TimerID m_timerId;
	void       *clone_creen;
	XSurface 	*screen;
	
	XWidget *m_wParent;
};

class XEditBox:public XWidget
{
public:
	XEditBox();
	
	void setFontColor(Uint32 color);//设置字体颜色
	void setBoxColor(Uint32 color);//设置外边框颜色
	void setText(string text);    
	string& getText();
	void setEdit(bool eidt); //设置是否可编辑  还没做...
	void setPassword(bool password); //没做...
	//设置只能输入数字 ...
	//设置只能输入英文 ...
	//设置是否可编辑...	
private:
	
	void init();
	int computeCursor(const int& x);
	void deleteFont();
	void CursorRightMove(); //光标右移一个字符
	void CursorLeftMove();  //光标左移一个字符
	
	virtual void draw();
	virtual void onInit();
	virtual void onPaint(Uint16 msg);
	virtual void mouseLPressEvent(X_Event *event);
	virtual void mouseLReleaseEvent(X_Event *event);
	virtual void localUpdate(XRectF& pRect,XObject *pObject);
	virtual void keydownEvent(X_Event *event);
	
private:
	
	int    start_pointer;  //编辑框显示第一个文字的地址
	int    cursor_addr;    //光标所在位置
	int    cursor_postion; //光标与编辑左边的距离
	
	Uint32 font_color;   //字体颜色
	Uint32 box_color;    //外边框颜色
	string text;         //文本内容
	
	XPoint cursor_point; //闪动的光标现在所在位置
	XRect  text_rect;    //文本区大小和位置

	XTextListener *textListener; //文本类组件监听
	
	
	friend class BCursor;
};


extern BCursor static_cursor;

#endif

