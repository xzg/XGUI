#ifndef _XMESSAGEBOX_H
#define _XMESSAGEBOX_H

#include "XEventType.h"
#include "XWidget.h"
#include "XFont.h"
#include "XMouseListener.h"
#include "XButton.h"
#include "XStringArray.h"



class XMessageBox: public XWidget,public XMouseListener
{
public:
	XMessageBox();
	~XMessageBox();
	
	void setTitleAreaColor();
	void addButton(XButton *button);
	ID show(string title,string text,BUTTON_TYPE type=ID_OKCANCLE);
	void close();
private:
	bool responseEvent(X_Event *event);
	void paint(string& title,string& text);
	
	void init(string& text,string& title);
	
	void mousePressed(X_Event *event,const ID id);
	void mouseReleased(X_Event *event,const ID id);
	void mouseEntered(X_Event *event,const ID id);
	
	void saveData();
	void resumeData();
	
	virtual bool onCloseWindowEvent();
private:
	
	XStringArray  arry;
	
	unsigned  int button_id; //点中按钮ID
	XButton  ok;
	XButton  cancel;
	XLayout  *hlayout;    //按钮布局
	
	XRect    title_rect;   //标题区
	XRect    text_rect;   //文本区
	XRect    button_rect; //按钮区
	
	Uint32   title_color;
	
	XWidget  *old_root;   
	XWidget   *old_focus;
	XWidget   *old_hover;
};

#endif

