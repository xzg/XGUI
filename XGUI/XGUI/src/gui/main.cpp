#include "XWidget.h"
#include "XApplication.h"
#include "XHBoxLayout.h"
#include "XVBoxLayout.h"
#include "XButton.h"
#include "XCheckBox.h"
#include "XMessageBox.h"
#include "XEditBox.h"
#include "XLabel.h"
#include "XListBox.h"
#include "XRadioButton.h"
#include "XToolTip.h"
#include "XPopMenu.h"
#include "XDialog.h"
#include "XUseful.h"


class Window:public XWidget
{
public:
	Window()
	{
		image=SDL_LoadBMP("F:\\VC++\\XGUI\\Debug\\imgs\\bg.bmp"); 
		if(!image)
		{
			LOG(SDL_GetError());
		}
	}
	~Window()
	{
		if(image)
			SDL_FreeSurface(image);
	}
	
	virtual bool onCloseWindowEvent()
	{return true;
		XMessageBox MesBox;
		if(MesBox.show("警告!","真的要退出吗?\n") == ID_CANCLE)
		{
			return false;
		}
				
		return true;
	}
	
	void onDrawBackground(XRectF& rectF)
	{
		SDL_BlitSurface(image,NULL,screen,NULL);
	}
	
	SDL_Surface *image;
};

class T:public XMouseListener,public XWidgetListener
{
public:
	T()
	{
		layout= new XHBoxLayout();
		layout01 = new XHBoxLayout();
		layout02 = new XVBoxLayout();
		main_layout = new XVBoxLayout();
		dlgLayout = new XVBoxLayout();
	}
	~T()
	{
		delete layout;
		delete main_layout;
		delete layout01;
		delete layout02;

		delete dlgLayout;
	}
	
	void init()
	{
		pop.addItem("复制");
		pop.addItem("剪切");
		pop.addItem("粘贴");

		listBox.addItem("Frees a cursor created with SDL_CreateCurs");
		listBox.addItem("2");
		listBox.addItem("3");
		listBox.addItem("5");
		listBox.addItem("6");
		listBox.addItem("7");
		listBox.addItem("8");
		listBox.addItem("9");
		listBox.addItem("10");
		listBox.addItem("11");
		listBox.addItem("12");
		listBox.addItem("13");
		listBox.addItem("15");
		
		edit.setText("edit...");
		dlg.setTitle("dialog1");
		dlg1.setTitle("dialog2");
		
		//button1.setBackground(0xff000077);
		//button3.setBackground(0xff000077);
		button2.setBackground(0xff000033);
		button2.setText("alpha ");
		button1.setText("OK");
		button3.setText("Cancel");
		button1.setSize(50,30);
		button3.setSize(50,30);
		button2.setSize(100,50);
		
		dlg_label.setText("test messagebox");
		
		dlg.create(POP|TITLE);
		dlg.setBackground(0xC7C0C7aa);
		
		dlg1.create(POP|TITLE);
		dlg1.setBackground(0x00ffff77);
		
		//对话框
		button1.setHVariable(true);
		button3.setHVariable(true);
		dlgLayout->addSpace();
		dlgLayout->addWidget(&dlg_label);
		layout->addWidget(&button1);
		layout->addWidget(&button3);
		dlgLayout->addSpace();
		dlgLayout->addLayout(layout);
		dlg.setLayout(dlgLayout);
		
		
		layout02->addWidget(&radioButton01);
		layout02->addWidget(&radioButton02);
		layout02->addWidget(&check01);
		
		layout01->addWidget(&label);
		//layout01->addWidget(&listBox);
		layout01->addLayout(layout02);
		
		main_layout->addWidget(&edit);
		//button2.setHVariable(true);
		//button2.setVVariable(true);
		main_layout->addWidget(&button2);
		main_layout->addLayout(layout01);
		
		widget.setLayout(main_layout);
		widget.show();
		
		button2.setMouseListener(this);
		button1.setMouseListener(this);
		check01.setMouseListener(this);
		
		tootip1.setBackground(0x0000ff77);
		tootip1.setFontColor(0x000000ff);
		tootip1.setPosion(130,250);
		tootip.setPosion(130,260);
		
		dlg1.setLaction(150,150);
	}
	
	void mousePressed(X_Event *event,const ID id)
	{	
		if(button1.getId() == id){
			tootip.showText("hello,world...");
			dlg1.showWindow(true);
		}else if(button2.getId() == id)
		{
			//tootip1.showText("hello,world...");
		}
	}  	
	
	void mouseReleased(X_Event *event,const ID id)
	{	
		if(check01.getId() == id)
		{
			//pop.setPosition(event->button->x,event->button->y-10);
			//pop.show(true);
			//tootip.showText("hello,world...");
		}else if(button2.getId() == id)
		{
			//dlg.doModal();
			dlg.showWindow(true);
		}else if(button1.getId() == id)
		{
			//dlg1.doModal();
			//dlg1.showWindow(true);
		}
		
	}
	
	void mouseEntered(X_Event *event,const ID id)
	{
		
	}
	void clicked(const ID id)
	{
		
	}
private:
	XHBoxLayout *layout;
	XHBoxLayout *layout01;
	XVBoxLayout *layout02;
	XVBoxLayout *main_layout;
	XVBoxLayout *dlgLayout;
	Window    widget;
	XButton   button1;
	XButton   button2;
	XButton   button3;
	XRadioButton radioButton01;
	XRadioButton radioButton02;
	XLabel    label;
	XLabel    dlg_label;
	XEditBox     edit;
	XCheckBox check01;
	XCheckBox check02;
	
	XToolTip  tootip;
	XToolTip    tootip1;
	XPopMenu  pop;
	XDialog   dlg;
	XDialog   dlg1;
	
	XListBox  listBox;

};


int main(/*int argc, char *argv[]*/)
{
	XApplication app;
	
	T c1;
	c1.init();
	
	return app.run();
}



