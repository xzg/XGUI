#include "XMessageBox.h"
#include "XHBoxLayout.h"
#include "XUseful.h"


#define FONT_MAX_WIDTH      280
#define FONT_MAX_HEIGHT     200
#define FONT_SEPARATOR      2
#define FONT_MARGIN_V       10
#define FONT_MARGIN_H       10

XMessageBox::XMessageBox()
{
	setSize(120,90);	          //对话框最小大小
	setBackground(0xb9d2ffaa);
	
	title_color=0xffffffff;
	
	hlayout=NULL;
}

XMessageBox::~XMessageBox()
{
	if(hlayout)
		delete hlayout;
}

void XMessageBox::addButton(XButton *button)
{
	hlayout->addWidget(button);
}

/*显示用户所提供的string
* 步骤：
* 1.按钮ID＝0 2.消息框类型
* 3.将原来root替换成现在的对话框，并保存原来的root
* 4.布局 5.执行消息循环XApplication::run()
* 6.返回按钮ID
*/

ID XMessageBox::show(string title,string text,BUTTON_TYPE type)
{
	button_id =ID_NONE;
	
	if(!hlayout)
	{	
		hlayout = new XHBoxLayout;
		if(!hlayout)
		{	
			LOG("XMessageBox: don't new memory\n");	
		}
	}
	
	switch(type)
	{
		case ID_OK:
		{
			//...
		}	
		break;
		case ID_CANCLE:	
		{	
			
		}
		break;
		
		case ID_OKCANCLE:
		{
			ok.setText("OK");
			cancel.setText("cancel");
			
			hlayout->addWidget(&ok);
			hlayout->addWidget(&cancel);
			
			ok.setMouseListener(this);
			cancel.setMouseListener(this);
			
			this->setLayout(hlayout);
		}
	}

//布局
	saveData();
	
	init(text,title);
	
	paint(title,text);
	
	XApplication::PollMessage();
	
	resumeData();
	
	delete hlayout;
	hlayout=NULL;
	return button_id;
}

//关闭对话框
void XMessageBox::close()
{
	X_Event event;
	event.type=X_QUIT;
	XAbstractWidget::sendSystemMsg(&event);
	
	event.type=X_UPDATE;
	XAbstractWidget::sendSystemMsg(&event);
}

//*********************私有*************************
void XMessageBox::saveData()
{
/**************************************************/
/*             发送X_RESUME消息，以恢复现在组件         */
/**************************************************/
	X_Event event;
	event.type=X_RESUME;
	XAbstractWidget::sendSystemMsg(&event);
	XApplication::PollMessage();	
	
	old_root =XAbstractWidget::getRoot();
	old_focus=(XWidget  *)XAbstractWidget::focus_widget;
	
	XAbstractWidget::setRoot(this);
	XAbstractWidget::setFocusWidget(NULL);
	
}
void XMessageBox::resumeData()
{
	XAbstractWidget::setRoot(old_root);
	XAbstractWidget::setFocusWidget(old_focus);
	
}
bool XMessageBox::responseEvent(X_Event *event)
{
	/***************防止接收不该接收的消息*****************/
	if(widget_state==X_ORIGINAL)
		widget_state=X_ACTIVE;
	else
		hlayout->responseEvent(event);

	return false;
}

/*初始化消息框大小
* 如果遇到
*/
void XMessageBox::init(string& text,string& title)
{
	int i=0;
	int w=0;
	int h=0;
	int layout_h=0;
	int layout_w=0;
	int widget_count_h=0;
	text_rect.h=0;
	
	XSize   size = this->getWindowSize();
	
	{
		widget_count_h=hlayout->getHVariableCount(&layout_w,true);
		hlayout->getVVariableCount(&layout_h,true);
		
		button_rect.h=layout_h;           //按钮区高度
		
		/*
		*如果组件的宽度小于按钮区宽度,则让组件宽度和布局容器宽度相等
		*/
		if(widget_rect.w < layout_w+XWidget::h_margin*(widget_count_h+1) ) 
			widget_rect.w = layout_w+XWidget::h_margin*(widget_count_h+1); 
	}
	
	arry.split(text,'\n');
	for(;i<arry.getCount(); ++i)
	{
		font->getFontSize((char *)arry.getAt(i).c_str(),&w,&h);
		
		if(widget_rect.w < w) {
		   widget_rect.w=w;
		}
		text_rect.h+=h+FONT_SEPARATOR;
	}
	
	if(arry.getCount()==0)  //如果是一行文本
	{
		font->getFontSize((char *)text.c_str(),&w,&h);
		
		if(widget_rect.w < w) {
		   widget_rect.w=w;
		}
		text_rect.h+=h+FONT_SEPARATOR;
	}
	
	text_rect.w=widget_rect.w;
	//组件宽度
	if(widget_rect.w > FONT_MAX_WIDTH- 2*FONT_MARGIN_H)                      
		widget_rect.w= FONT_MAX_WIDTH+ 2*FONT_MARGIN_H;
	else
	{
		widget_rect.w+= 2*FONT_MARGIN_H;
	}
	widget_rect.x = (size.w - widget_rect.w)/2;            //组件的X坐标
	
	title_rect.h=font->getFontHeight()+XWidget::v_margin;  //标题栏高度
	
	if(text_rect.h > FONT_MAX_HEIGHT-title_rect.h-button_rect.h-2*FONT_MARGIN_H) //文本的宽度不能超过最大宽度
		text_rect.h =FONT_MAX_HEIGHT-title_rect.h-button_rect.h-2*FONT_MARGIN_H;
	
	
	widget_rect.y = (size.h - title_rect.h - text_rect.h - 2*FONT_MARGIN_H  - button_rect.h)/2; //组件的Y坐标
	widget_rect.h = button_rect.h+text_rect.h+title_rect.h+2*FONT_MARGIN_H;
	
	
	//标题区
	title_rect.x=widget_rect.x;
	title_rect.w=widget_rect.w;
	title_rect.y=widget_rect.y;
	//文本区
	text_rect.x=widget_rect.x+FONT_MARGIN_V;
	text_rect.y=widget_rect.y+title_rect.h+FONT_MARGIN_V;
	
	
	//按钮区
	button_rect.y=(widget_rect.y+widget_rect.h) -(layout_h+XWidget::v_margin);
	button_rect.x=widget_rect.x;
	button_rect.w=widget_rect.w;
	
	
	//给布局容器分配位置
	hlayout->setRect(&button_rect);
	//开始布局
	hlayout->computeLayout();
}
bool XMessageBox::onCloseWindowEvent()
{
	
	return false;
}	

/*过滤"\n"
*"\n"表示换行
*/
/*   _______
*   |_______|    标题区 21
*   |       |   文本区  36
*   |_______|
*   |_______|   按钮区  33
*/

void XMessageBox::paint(string& title,string& text)
{
	int i;
	int x,y;   //这个是组件的坐标,因为多处用到，牺牲一点空间换时间
	Uint32      color = getBackground();
	XSurface 	*screen=this->getScreen();
	
	XSize screen_size=getWindowSize();
	//boxColor(screen,0,0,screen_size.w,screen_size.h,0x8c8c8c77); //背景变成灰色，以便分辨出消息框 
	
	//标题区
	XRect font_rect; //内
	x=  widget_rect.w+widget_rect.x;
	y=  widget_rect.y+widget_rect.h;
	
	//主框架
	roundedBoxColor(screen,widget_rect.x, widget_rect.y, x,
					widget_rect.y+title_rect.h,3,title_color); //标题栏
	roundedBoxColor(screen,widget_rect.x, widget_rect.y+title_rect.h, x, y,3,color); //文本区
	roundedRectangleColor(screen,widget_rect.x, widget_rect.y, x, y,3,0xd1d1d1ff); //外框架
	
	//线条
	hlineColor(screen,widget_rect.x+3,x-3,y,0xffffffff); //
	
	//文本区
	if(font){
		font->displayFont((char *)title.c_str(),0,title_rect); //标题
		//文本
		
		if(arry.getCount() == 0)
		{
			font->displayFont((char *)text.c_str(),0,text_rect);
		}else{
			font_rect.x=widget_rect.x;
			font_rect.y=text_rect.y;
			font_rect.w=widget_rect.w; 
			font_rect.h=font->getFontHeight()+FONT_SEPARATOR;
		}
		for(i=0;i<arry.getCount(); ++i){
			
			font->displayFont((char *)arry.getAt(i).c_str(),0,font_rect);
			
			font_rect.y+=font->getFontHeight()+FONT_SEPARATOR;
		}
	}

	//按钮区
	hlayout->update();

	SDL_Flip(screen);
	//SDL_UpdateRect(screen,widget_rect.x,widget_rect.y,widget_rect.w+1,widget_rect.h+1);
	run_state=true; //设置本组件运行状态
}

void XMessageBox::mousePressed(X_Event *event,const ID id)
{
	//************
}

/*当消息框退出的时候，退出本循环，并重画*/
void XMessageBox::mouseReleased(X_Event *event,const ID id)
{
	if(ok.equals((XObject *)id))
	{
		button_id = ID_OK;
	}else if(cancel.equals((XObject *)id))
	{
		button_id = ID_CANCLE;
	}
	
	event->type=X_WIDGETCLOSE;
	XAbstractWidget::sendSystemMsg(event);
	
	event->type=X_UPDATE;
	XAbstractWidget::sendSystemMsg(event);
	
}
void XMessageBox::mouseEntered(X_Event *event,const ID id)
{
	
	//*************
}

//*********************私有*************************

