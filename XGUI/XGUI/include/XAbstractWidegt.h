/*
*所有组件类的抽象类
*/

#ifndef _XABSTRACTWIDGET_H
#define _XABSTRACTWIDGET_H

#include "XUtils.h"
#include "XObject.h"
#include "XEventType.h"
#include "XMouseListener.h"
#include "XUseful.h"
#include "XFont.h"
#include "XList.h"
#include "XKeyListener.h"

class XLayout;
class XWidget;

class XAbstractWidget:public XObject
{
public:
	XAbstractWidget();
	bool isFocus();
	
	/*得到组件现在大小*/
	XRect getRect();
	/* 得到组件坐标*/
	XRectF getRectF();
	
	//这个函数会刷新此组件，并且发送重绘消息,并且在运行之后调用
	void showWidget(); 
	void show(XRectF& pRect);
	void show(XRect rect);
	void showFillScreen();
	
	//设置可写屏区域
	void setClipRect(XRect rect);
	void cleanClipRect();
	XRect getClipRect();
	void setPreClipRect(); //设置上次设置clip的区域
	
	bool match(XRectF& rectF);
	
	static void setFirstRoot(XAbstractWidget *root);
	static void setRoot(XAbstractWidget  *root);
	static XWidget* getRoot();
	static void setFocusWidget(XAbstractWidget  *focus);
	//第二个函数可以发送系统消息,通过消息队列分发出去
	static void sendSystemMsg(X_Event *user);
	//添加最上层的组件，例如Tooltip
	static void addGlobalHigh(XAbstractWidget  *global);
	//添加下层组件，例如dialog
	static void addGlobalLow(XAbstractWidget  *global);
	static void removeGlobalHigh(XAbstractWidget  *global);
	static void removeGlobalLow(XAbstractWidget  *global);

	//全局组件事件轮询
	static bool globalPoll(X_Event *event);
	//刷新root上层所以组件
	static void updateAllGlobal(XAbstractWidget  *pObject);
	static void updateGlobalHighUp(XAbstractWidget  *global,XRectF& RectF);     //刷新最上层上面组件
	static void updateGlobalHighDown(XAbstractWidget  *global,XRectF& RectF);  //刷新最上层下面组件
	static void updateGlobalLowUp(XAbstractWidget  *global,XRectF& RectF);     //刷新下层的上面组件
	static void updateGlobalLowDown(XAbstractWidget  *global,XRectF& RectF);   //刷新下层的下面组件

#ifdef USE_MOUSE	
	void setMouseListener(XMouseListener *mouse); //添加鼠标事件监听
#endif	

#ifdef USE_KEYBOARD
	void setKeyListener(XKeyListener *key);
#endif
//可覆盖函数	
	/*
	*事件响应函数
	*所有的组件必须实现这个接口
	*返回值为bool，作用是一旦事件发生之后是否保留该对象
	*/
	virtual bool responseEvent(X_Event *event);
	//当此组件为焦点组件的时候，响应事件处理
	virtual bool focusEvent(X_Event *event)=0;
	virtual bool devEvent(X_Event *event);
	
	//不应该放在这里，应为有些组件没有加入到Layout的属性
	virtual void setLayoutRect(XRect* rect);
	virtual void setLayoutSize(Uint16 w,Uint16 h);
	
	/**
	*查找被组件覆盖的组件，并重绘制他们及重绘制组件的子节点
	*/	
	virtual void findOverridingWidget(XRect& pRect,XObject *pObject){}
	
	//重绘制上面组件
	virtual void drawChild(XRectF& pRect,XObject *pObject);
	virtual void drawParent(XRectF& pRect,XObject *pObject);
	virtual void localUpdate(XRectF& pRect,XObject *pObject)=0;
	
	virtual void mouseLPressEvent(X_Event *event){  onPaint(X_BUTTONDOWN); }
	virtual void mouseLReleaseEvent(X_Event *event){ onPaint(X_BUTTONUP); }
	virtual void mouseMPressEvent(X_Event *event){}
	virtual void mouseMReleaEvent(X_Event *event){}
	virtual void mouseRPressEvent(X_Event *event){}
	virtual void mouseRReleaseEvent(X_Event *event){}
	virtual void mouseWheelDown(X_Event *event){}
	virtual void mouseWheelUp(X_Event *event){}
	
	virtual void mouseEnterEvent(X_Event *event){}
	virtual void mouseLeaveEvent(X_Event *event){}
	
	virtual void keydownEvent(X_Event *event){}
	virtual void keyReleaseEvent(X_Event *event){}
	
	virtual void touchScreenPressEvent(X_Event *event){}
	virtual void touchScreenReleaseEvent(X_Event *event){}
	
	//离开焦点
	virtual void focusLeave();
	//进入焦点 
	virtual void focusEnter();
	virtual void loseFocus();
	
	//初始化调用的函数，此函数在布局以后被调用
	virtual void onInit();
	//erase
	virtual void onPaint(Uint16 msg);
	
	//当程序运行之后调用，先调用onSize,再调用的onPosition
	virtual void onSize(int w,int h)
	{
		widget_rectF.x2=widget_rect.x+w;
		widget_rectF.y2=widget_rect.y+h;
		
	}   
	virtual void onPosition(int x,int y)
	{	
		widget_rectF.x1=x;
		widget_rectF.y1=y;
	} 
	
	/*
	*定时器
	*/
	SDL_TimerID setTimer(unsigned int interval);
	void removeTimer(SDL_TimerID timerID);
	virtual void onTiemer();
	
	XSize getWindowSize();
	
protected:
	//组件的位置
	XRect  widget_rect;
	XRectF widget_rectF;
	//组件中心位置
	XPoint centerPoint;
	
	bool 	  visible;       //组件是否可见
	bool      lucency;       //是否透明
	bool      run_state;     
	Uint8     widget_state;  //例如组件状态，按下,弹起,悬停
	
	static XSurface 	*screen;  //SDL surface
	static XFont      *share_font; //所以组件默认字体
	static XRect      preClipRect;

	static XAbstractWidget  *root_widget;
	static XAbstractWidget  *focus_widget;
	static XAbstractWidget  *first_root;

	static XList<XAbstractWidget* > m_listHigh;
	static XList<XAbstractWidget* > m_listLow;
	static XList<XAbstractWidget* > m_listPollLow;

#ifdef USE_MOUSE		
	XMouseListener *mouse_listener; //鼠标事件
#endif

#ifdef USE_KEYBOARD
	XKeyListener  *key_listener;
#endif
};

#endif
