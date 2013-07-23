//单选按钮

#ifndef _XRIDIOBUTTON_H
#define _XRIDIOBUTTON_H

#include "XWidget.h"

class XRadioButton:public XWidget
{
public:
	XRadioButton();
	~XRadioButton();
	
	bool isChecked();
	void setChecked(bool checked);
	void setText(const string& text);
	string& getText();
	
	void setMarginColor(Uint32& color);
	void setFontColor(Uint32& color);
	
private:

	virtual void draw();
	virtual void onInit();
	virtual void onPaint(Uint16 msg);
	virtual void onSize(int w,int h);
	virtual void onPosition(int x,int y);
	virtual void localUpdate(XRectF& pRect,XObject *pObject);
	//恢复
	virtual void resumeRidioButton();
	
	void  changSize();
private:
	string mStrText;
	XRect  button_rect; //按钮区
	XRect  text_rect;   //文本区
	
	Uint32 mFontColor;
	Uint32 m_BoderColor;
		
	static XRadioButton* pCurrentChecked;
};

#endif
