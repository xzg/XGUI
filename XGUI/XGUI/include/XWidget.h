/*
* 所有组件的父类
* 这个类抽象出所有组建共同的特性
* 不可将XWidget作为局部对象，那样会释放屏幕指针
*/

#ifndef _XWIDGET_H
#define _XWIDGET_H

#include "XAbstractWidegt.h"
#include "XLayout.h"
#include "XUseful.h"

class XLayout;
class XAbstractWidget;
class XVBoxLayout;
class XHBoxLayout;

class XWidget:public XAbstractWidget
{
public:
	XWidget(int w,int h);
	XWidget();
	~XWidget();
	
	/*
	*为布局管理器计算组件大小
	*可以告诉布局管理器不改变大小，只分配位置
	*/
	void setLayout(XLayout *layout);
	void setFont(XFont *font);
	
	/*
	*显示所用
	*/
	void show();
	void hide();
	
	void setVisible(bool visible);
	//返回组件ID
	const ID getId();
	/*
	*设置背景色
	*第二个函数是给组件设置背景图片
	*/
	void setBackground(Uint32 color);
	const Uint32& getBackground();
	
	//得到父节点
	XLayout*	getParent();
	XLayout*	getLayout();
	
	//重新布局
	void reLayout();
	void update();
	
	bool isChild(XWidget *pObject);
	
	/*
	*对于组件的操作
	*布局前: 调用可以设置大小，但是设置位置没有用
	*布局后: 可以设置位置和大小，但是不能超过布局
	*/
	void setSize(int w,int h);
	void setLaction(int x,int y);
	
	void setRect(XRect* rect);
	Uint8 getWidgetState();
		
public:	
	/*给布局提供一种计算组件大小的方法
	*返回值如果是true组件宽度不可变
	*返回值false组件宽度可变
	*/
	virtual bool isVVariable(){
		
		return vVariable;
	}
	virtual bool isHVariable()
	{
		return hVariable;
	}
	/*
	*布局算法
	*/
	virtual void computeLayout();
	virtual void setVVariable(bool variable); 
	virtual void setHVariable(bool variable); 
	
	/*分别是水平、竖直可变组件能得到的额外长度*/
	static int v_variable;
	static int h_variable;
	/*默认BOX与BOX边距*/
	static int h_margin;
	static int v_margin;
	
protected:
	
	virtual bool responseEvent(X_Event *event);
	//当此组件为焦点组件的时候，响应事件处理
	virtual bool focusEvent(X_Event *event);
	
	/*可覆盖函数*/
	virtual void reDrawRect(const XRect& rect);
	virtual void onSize(int w,int h);   //当程序运行之后调用
	virtual void onPosition(int x,int y);
	virtual bool onCloseWindowEvent();
	
	virtual void draw();
	virtual void paint();
	virtual void onPaint(Uint16 msg);
	virtual void onInit();
	virtual void onDrawBackground(XRectF& rectF);
	/**
	*查找被组件覆盖的组件，并重绘制他们及重绘制组件的子节点
	*/
	virtual void findOverridingWidget(XRect& pRect,XObject *pObject);
	virtual void drawParent(XRectF& pRect,XObject *pObject);
	virtual void drawChild(XRectF& pRect,XObject *pObject);
	virtual void localUpdate(XRectF& pRect,XObject *pObject);
	
	virtual bool devEvent(X_Event *event);
	
	XSurface* getScreen();
	XFont     *font;         //组件独有字体
	
	
	Uint32    background_color;    //背景颜色
	XLayout   *parent;             //父节点
	XLayout   *layout;  //组件中嵌入布局
	
/*
*实现抽象函数接口
*以及不对外公开的接口
*/
private:
		
	/*得到组件所需最小大小，用在运行后重新布局*/
	virtual XRect getMinRect(); 
	

private:

	bool              hVariable; //水平可变
	bool              vVariable;  //竖直可变
	                 
	static void*       clone;    //克隆某一区域，以便恢复
	
	friend class XFont;
	friend class XLayout;
	friend class XApplication;
	friend class XHBoxLayout;
	friend class XVBoxLayout;
};

#endif

