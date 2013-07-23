#ifndef _XFONT_H
#define _XFONT_H

#include "XObject.h"
#include "XUtils.h"
#include "XApplication.h"

typedef enum
{
	
	

}FONT_TYPE;


class XFont:public XObject
{
public:
	XFont();
	~XFont();
	void setSize(int size);
	void setFontType(FONT_TYPE type);
	void setFontColor(Uint32 color); //暂时没使用...
	
	void getFontSize(const char *text,int *w,int *h);
	int  getFontHeight();
	int  getFontWidth(const char *text);
	
/************************下面几个函数说明**********************/
// 1.第一个函数:按照(x,y)坐标为基准显示字符串
// 2.第二个函数:将text显示在outside中间,left如果-1正显示在中间，否则按给定的边距
// 3.第三个函数:同2,但是在text下面加下划线
// 4.第四个函数:将text靠着outside上边和左边显示
// 5.第五个函数:以text以最后一个字符为基准，按照Outside区域显示
// 6.第六个函数:以text以第一个字符为基准，按照outside区域显示
// 7.第七个函数:绘制text,显示出outside的上部的effside
// 8.第八个函数:绘制text,显示effSide部分
/**********************************************************/
	void displayFont(const char *text,int color,int x,int y);
	void displayFont(const char *text,int color,const XRect& outside,const int& left=-1);
	void displayFontUnder(const char *text,int color,const XRect& outside,bool left=false);
	void display(const char *text,int color,const XRect& outside); 
	void displayRight(const char *text,int color,const XRect& outside);
	void displayLeft(const char *text,int color,const XRect& outside);
	
	void displayTop(const char *text,int color,const XRect& outside,const XRect& effSide);
	void displaySection(const char *text,int color,const XRect& outside,const XRect& effSide,
				const int& left=-1);
	
	//可能会超过outside，可以通过setClipRect设置可写屏幕区域
	void fastDisplayFont(const char *text,int color,const XRect& outside,const int& left);
	
	/*如果不使用默认字体库，可以手动释放*/
	void freeFont();
private:
	bool init();	
private:

	/*将此处加一个默认ttf指针的原因是：申请一个ttf实体太耗费内存了
	  所以如果使用默认字库，可以共享一个ttf
	*/
	static TTF_Font  *dafult_font;
	static int       dafult_size;
	Uint32           font_color;
	
	Uint16     font_size;
	TTF_Font  *font;
	char      *font_name;
	
	friend class XApplication;
};


#endif
