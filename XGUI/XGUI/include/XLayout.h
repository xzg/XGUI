/*
*所有的布局管理器的基类
*布局管理器既充当容器又扮演布局
*/

#ifndef _XLAYOUT_H
#define _XLAYOUT_H

#include "XList.h"
#include "XUtils.h"
#include "XObject.h"
#include "XWidget.h"

typedef enum 
{
	LAYOUT,
	WIDGET,
	SPACE,
	NONE	
}LAYOUT_PARENT_STYLE;


typedef struct
{
	Uint8   type;	
	XObject *pObject;
		
}XLYContainer;


class XWidget;

class XLayout:public XObject
{
public:
	
	XLayout();
	~XLayout();
	
	void addWidget(XWidget  *widget);
	void addLayout(XLayout *layout);
	void addSpace();
	//插在那个第几个前面
	void insert(int index);
	
	void remove(XWidget  *widget);
	void remove(XLayout  *layout);
	void remove(int index);
	
	void removeAll(); //有问题，注意...
	/*
	*给布局中的组件或者布局分配位置的接口
	*必须继承实现这个借口
	*/
	virtual void computeLayout()=0;
	
	/*
	*返回水平可变组件最大与竖直可变组件最大数量   即使是窗口发生变化数目的最小高度和宽度是不会变化的
	*第一个函数参数得到BOX水平最小宽度
	*第二个函数参数得到BOX竖直最小高度
			W
		_________
		|
	H	|
		|
	*/
	virtual int getHVariableCount(int *w,bool cal=false)=0;
	virtual int getVVariableCount(int *h,bool cal=false)=0;
	
	/*分别是返回水平和竖直BOX的最大数量(包含子节点)*/
	virtual int getHCount()=0;
	virtual int getVCount()=0;
	
	void setHDock(bool dock){this->h_dock =dock;}
	void setVDock(bool dock){this->v_dock =dock;}
	/*
	void setHLeftDock(Uint16 dock);    //水平左边距
	void setHRightDock(Uint16 dock);   //水平右边距
	void setVTopDock(Uint16 dock);     //水平左边距
	void setVBottomDock(Uint16 dock);  //水平右边距
	void setMargin(Uint16 dock);       //组件间的边距
	*/
	void compute(int *h_margin,int *v_margin,int *h_variable,int *v_variable);
	void findOverridingWidget(XRect& pRect,XObject *pObject);
	void drawChild(XRectF& pRect,XObject *pObject);
	void drawParent(XRectF& pRect,XObject *pObject);
	
	/*
	*设置布局的位置
	*/
	void  setLaction(int x,int y);
	void  setSize(int w,int h);
	
	void setRect(XRect *rect);
	
	//返回布局容器现在的大小和位置
	XRect getRect();
	//返回布局容器最小大小和位置
	XRect getMinRect();	
	
	bool isChild(XObject *pObject);
	
	/*
	*分发事件
	*/
	bool responseEvent(X_Event *event);
	void update();
	void init();	
	void setParent(XObject* parent,Uint8 style); //设置布局的父节点
	//返回父节点类型，参数parent是父节点对象指针
	LAYOUT_PARENT_STYLE getParent(XObject* parent); 
	XWidget  *getWidgetParent(); // 得到Widget父组件


protected:
	
	int      v_v_num;    //竖直可变组件数量
	int      h_v_num;   //水平可变组件数量
	Uint16   min_h;
	Uint16   min_w;
	
	XRect  rect;
	
	/*
	*第一个是组件链表
	*第二个是布局链表
	*/
	
	XList<XWidget  *>   list_widget;
	XList<XLayout  *>   list_layout;
	
	//XList<Uint8>          list_int;
	XList<XLYContainer *>  list_int;
	
	bool                h_dock; //是否靠左
	bool                v_dock; //是否靠上
/*	
	Uint16          m_intHLeftDock;    //水平左边距
	Uint16          m_intHRightDock;   //水平右边距
	
	Uint16          m_intVTopDock;     //水平左边距
	Uint16          m_intVBottomDock;  //水平右边距

	Uint32          m_intMargin;       //组件间的边距
	*/
	Uint8           style;   //父节点类型，不这样做(声明两个指针XWidget*, XLayout *)的好处，保证了只有一个父节点
	XObject  		*parent; //父节点，可能是布局容器，也可能是XWidget
};

#endif


