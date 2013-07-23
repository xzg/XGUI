#include "XFont.h"
#include "XWidget.h"

TTF_Font  * XFont::dafult_font=NULL;
int XFont::dafult_size =13;

XFont::XFont()
{
	font = NULL;
	font_size  = dafult_size;
	
	font_name ="/XGUI/fonts/simfang.ttf";
	init();
}

XFont::~XFont()
{
	/*如果额外的申请过内存，释放掉*/
	if(dafult_font != font)
		TTF_CloseFont(font);
}

bool XFont::init()
{

	if(!dafult_font)
	{
		dafult_font = TTF_OpenFont(font_name,font_size);
		if(!dafult_font)
		{	
			LOG(SDL_GetError());
			return false;
		}
	}
	
	font = dafult_font;
	
	return true;
}

void XFont::setFontColor(Uint32 color)
{
	this->font_color = color;
}

/*
*大小被重置，如果不是默认字体释放以前的
*/
void XFont::setSize(int size)
{
	font_size = size;
	/*如果设置字体大小和默认字体大小相等，那么使用默认字体库*/		
	if(dafult_size == size)
	{
		font = dafult_font;
		return ;
	}
	
	/*如果额外的申请过内存，释放掉*/
	if(dafult_font != font)
		TTF_CloseFont(font);
	
	font=TTF_OpenFont(font_name,font_size);
	if(!font)
	{	
		LOG(SDL_GetError());
		return ;
	}
}

/*
* 如果font为NULL，则证明没有初始化字库
* 那么按照默认大小计算
*/
void XFont::getFontSize(const char *text,int *w,int *h)
{
	if(this->font != NULL && strlen(text))
	{	
		TTF_SizeUTF8(this->font,text,w,h);
		return ;
	}
	
	*w =0;
	*h =0;
}

int XFont::getFontHeight()
{
	if(font)
	{
		return TTF_FontHeight(font);
	}
	return 0;
}
int  XFont::getFontWidth(const char *text)
{
	int w=0;
	int h=0;

	if(this->font != NULL && strlen(text))
	{	
		TTF_SizeUTF8(this->font,text,&w,&h);
		
	}
	
	return w;	
}

void XFont::freeFont()
{
	/*如果以后不再在使用此字库库可以释放掉
	* 例如组件的文字，有些不再发生变化释放掉
	*/
	
	if(font != dafult_font)
		TTF_CloseFont(font);
}
void XFont::displayFont(const char *text,int color,int x,int y)
{
	SDL_Surface  *surface;
	XRect         inside={x,y,0,0};
	int           w,h;
	SDL_Color     this_color;
	
	if(!font){
		 
		 if(init()==false){ 
		 
		 	LOG("font: font  is NULL!");
		 	return ;
		 }
	}
	
	this_color.unused = 0x000000ff & color;
	this_color.b = (0x0000ff00 & color) >> 8;
	this_color.g = (0x00ff0000 & color) >>16;
	this_color.r =(0xff000000 & color) >> 24;
	
	surface=TTF_RenderUTF8_Blended(font,text,this_color);
	
	TTF_SizeUTF8(font,text,&w,&h);
	
	inside.w=w;
	inside.h=h;	
	
	SDL_BlitSurface(surface, &inside, XWidget::screen,&inside);
	SDL_FreeSurface(surface);	
	
}	
/*
* 参数： rect 是字体外部的框架大小和位置
* 本函数将字体显示在rect中间
* 就一些判断注消，以免浪费时间
*/
void XFont::displayFont(const char *text,int color,const XRect& rect,const int& left)
{
	SDL_Surface  *surface;
	XRect         inside={0};
	XRect         outside={0};
	int           w,h;
	SDL_Color     this_color;

	if(!strlen(text)) return ; //if null return;
	/*
	if(!font ){
		 if(init()==false){ 
		 
		 	LOG("font: font  is NULL!");
		 	return ;
		 }
	}
	*/
	this_color.unused = 0x000000ff & color;
	this_color.b = (0x0000ff00 & color) >> 8;
	this_color.g = (0x00ff0000 & color) >>16;
	this_color.r =(0xff000000 & color) >> 24;
	
	surface=TTF_RenderUTF8_Blended(font,text,this_color);
	/*if(!surface)
	{
		LOG("font: 返回为NULL\n");
		return ;
	}*/
	TTF_SizeUTF8(font,text,&w,&h);
	
	if(w>rect.w) w=rect.w; //显示文字不能超过外BOX
	if(h>rect.h) h=rect.h;
	
	inside.w=w;
	inside.h=h;
	
	outside.w=w;
	outside.h=h;
	
	if(left==-1)
		outside.x = (rect.w - w)/2 + rect.x; 
		
	else  {
		outside.x = rect.x+left;
		inside.w = (rect.x+rect.w)-outside.x;
	}
	
	outside.y = (rect.h - h)/2 + rect.y;//这里rect.h不可能小于h，所以不必判断outside.y的范围了,同理rect.x
	
	SDL_BlitSurface(surface,&inside, XWidget::screen, &outside);
	
	SDL_FreeSurface(surface);	
}
void XFont::fastDisplayFont(const char *text,int color,const XRect& rect,const int& left)
{
	SDL_Surface  *surface;
	XRect         inside={0};
	XRect         outside={0};
	SDL_Color     this_color;

	if(!strlen(text)) return ; //if null return;
	
	this_color.b = (0x0000ff00 & color) >> 8;
	this_color.g = (0x00ff0000 & color) >>16;
	this_color.r =(0xff000000 & color) >> 24;
	
	surface=TTF_RenderUTF8_Blended(font,text,this_color);
	
	TTF_SizeUTF8(font,text,(int *)&inside.w,(int *)&inside.h);
	
	outside.w=rect.w;
	outside.h=rect.w;
	
	outside.x = rect.x+left;
	outside.y = (rect.h - outside.h)/2 + rect.y;
	
	SDL_BlitSurface(surface,&inside, XWidget::screen, &outside);
	
	SDL_FreeSurface(surface);

}
void XFont::displayFontUnder(const char *text,int color,const XRect& rect,bool left)
{
	SDL_Surface  *surface;
	XRect         inside={0};
	XRect         outside={0};
	int           w,h;
	SDL_Color     this_color;

	if(!strlen(text)) return ; //if null return;
	
	if(!font ){
		 if(init()==false){ 
		 
		 	LOG("font: font  is NULL!");
		 	return ;
		 }
	}
	
	this_color.unused = 0x000000ff & color;
	this_color.b = (0x0000ff00 & color) >> 8;
	this_color.g = (0x00ff0000 & color) >>16;
	this_color.r =(0xff000000 & color) >> 24;
	
	TTF_SetFontStyle(font,TTF_STYLE_UNDERLINE); //1.设置下划线
	surface=TTF_RenderUTF8_Blended(font,text,this_color);
	if(!surface)
	{
		LOG("font: 返回为NULL\n");
		return ;
	}
	TTF_SetFontStyle(font,TTF_STYLE_NORMAL);  //2.还原
	
	TTF_SizeUTF8(font,text,&w,&h);
	
	if(w>rect.w) w=rect.w; //显示文字不能超过外BOX
	if(h>rect.h) h=rect.h;
	
	inside.w=w;
	inside.h=h;
	
	outside.w=w;
	outside.h=h;
	
	if(left==false)
		outside.x = (rect.w - w)/2 + rect.x; 
	else  outside.x = rect.x;
	
	outside.y = (rect.h - h)/2 + rect.y;//这里rect.h不可能小于h，所以不必判断outside.y的范围了,同理rect.x
	
	SDL_BlitSurface(surface,&inside, XWidget::screen, &outside);
	
	SDL_FreeSurface(surface);
}

void XFont::display(const char *text,int color,const XRect& outside)
{
	SDL_Surface  *surface;
	SDL_Color     this_color;
	XRect         inside={0};
	
	
	if(!strlen(text)) return ; //if null return;
	
	if(!font ){
		 if(init()==false){ 
		 
		 	LOG("font: font  is NULL!");
		 	return ;
		 }
	}
	
	
	this_color.unused = 0x000000ff & color;
	this_color.b = (0x0000ff00 & color) >> 8;
	this_color.g = (0x00ff0000 & color) >>16;
	this_color.r =(0xff000000 & color) >> 24;
	
	surface=TTF_RenderUTF8_Blended(font,text,this_color);
	if(!surface)
	{
		LOG("font:返回为NULL\n");
		return ;
	}
	
	inside.w=outside.w;
	inside.h=outside.h;
	
	SDL_BlitSurface(surface,&inside, XWidget::screen, (XRect *)&outside);
	SDL_FreeSurface(surface);
}

void XFont::displayRight(const char *text,int color,const XRect& outside)
{
	int w,h;
	SDL_Surface  *surface;
	SDL_Color     this_color;
	XRect         inside;
	
	
	if(!strlen(text)) return ; //if null return;
	
	if(!font ){
		 if(init()==false){ 
		 
		 	LOG("font: font  is NULL!");
		 	return ;
		 }
	}
	
	this_color.unused = 0x000000ff & color;
	this_color.b = (0x0000ff00 & color) >> 8;
	this_color.g = (0x00ff0000 & color) >>16;
	this_color.r =(0xff000000 & color) >> 24;
	
	surface=TTF_RenderUTF8_Blended(font,text,this_color);
	if(!surface)
	{
		LOG("font:返回为NULL\n");
		return ;
	}
	
	TTF_SizeUTF8(font,text,&w,&h);
	
	inside.x=w-outside.w;
	inside.y=0;
	inside.w=outside.w;
	inside.h=outside.h;
	
	SDL_BlitSurface(surface,&inside, XWidget::screen, (XRect *)&outside);
	SDL_FreeSurface(surface);
}

void XFont::displayLeft(const char *text,int color,const XRect& outside)
{
	SDL_Surface  *surface;
	SDL_Color     this_color;
	XRect         inside;
	
	
	if(!strlen(text)) return ; //if null return;
	
	if(!font ){
		 if(init()==false){ 
		 
		 	LOG("font: font  is NULL!");
		 	return ;
		 }
	}
	
	this_color.unused = 0x000000ff & color;
	this_color.b = (0x0000ff00 & color) >> 8;
	this_color.g = (0x00ff0000 & color) >>16;
	this_color.r =(0xff000000 & color) >> 24;
	
	surface=TTF_RenderUTF8_Blended(font,text,this_color);
	if(!surface)
	{
		LOG("font:返回为NULL\n");
		return ;
	}
	
	inside.x=0;
	inside.y=0;
	inside.w=outside.w;
	inside.h=outside.h;
	
	SDL_BlitSurface(surface,&inside, XWidget::screen, (XRect *)&outside);
	SDL_FreeSurface(surface);
}
void XFont::displayTop(const char *text,int color,const XRect& rect,const XRect& effSide)
{
	SDL_Surface  *surface;
	XRect         inside={0};
	XRect         outside={0};
	int           w,h;
	SDL_Color     this_color;

	if(!strlen(text)) return ; //if null return;
	
	if(!font ){
		 if(init()==false){ 
		 
		 	LOG("font: font  is NULL!");
		 	return ;
		 }
	}

	this_color.unused = 0x000000ff & color;
	this_color.b = (0x0000ff00 & color) >> 8;
	this_color.g = (0x00ff0000 & color) >>16;
	this_color.r =(0xff000000 & color) >> 24;
	
	surface=TTF_RenderUTF8_Blended(font,text,this_color);
	if(!surface)
	{
		LOG("font: 返回为NULL\n");
		return ;
	}
	TTF_SizeUTF8(font,text,&w,&h);
	
	if(w>rect.w) w=rect.w; //显示文字不能超过外BOX
	if(h>rect.h) h=rect.h;
	
	inside.w=w;
	inside.h=h;
	
	outside.w=w;
	outside.h=h;
	
	
	outside.x = rect.x;
	outside.y = (rect.h - h)/2 + rect.y;//这里rect.h不可能小于h，所以不必判断outside.y的范围了,同理rect.x
	
	if(outside.y < effSide.y)
	{
		inside.y=effSide.y-outside.y;
		outside.y=effSide.y;
	}
	
	SDL_BlitSurface(surface,&inside,  XWidget::screen, &outside);
	
	SDL_FreeSurface(surface);	
}

void XFont::displaySection(const char *text,int color,const XRect& rect,const XRect& effSide,
					const int& left)
{
	SDL_Surface  *surface;
	XRect         inside={0};
	XRect         outside={0};
	int           w,h;
	SDL_Color     this_color;
	
	if(!strlen(text)) return ; //if null return;
	
	if(!font ){
		 if(init()==false){ 
		 
		 	LOG("font: font  is NULL!");
		 	return ;
		 }
	}

	this_color.unused = 0x000000ff & color;
	this_color.b = (0x0000ff00 & color) >> 8;
	this_color.g = (0x00ff0000 & color) >>16;
	this_color.r =(0xff000000 & color) >> 24;
	
	surface=TTF_RenderUTF8_Blended(font,text,this_color);
	if(!surface)
	{
		LOG("font: 返回为NULL\n");
		return ;
	}
	TTF_SizeUTF8(font,text,&w,&h);
	
	if(w>rect.w) w=rect.w; //显示文字不能超过外BOX
	if(h>rect.h) h=rect.h;
	
	inside.w=w;
	
	outside.w=w;
	outside.h=h;
	
	if(left==-1)
		outside.x = (rect.w - w)/2 + rect.x; 
	else  {
		outside.x = rect.x+left;
		inside.w = (rect.x+rect.w)-outside.x;
	}
	
	outside.y = (rect.h - h)/2 + rect.y;//这里rect.h不可能小于h，所以不必判断outside.y的范围了,同理rect.x
	
	
	if(effSide.y+effSide.h < outside.y) //有效BOX的Y2< 字体BOX的Y1
	{
		inside.h=0;
		
	}else{
		
		int y1= outside.y;           //字体BOX的Y1坐标
		int y2= outside.y+outside.h; //字体BOX的Y2坐标
		
		if(y1 < effSide.y)
		{
			y1=effSide.y;
		}
	
		if(y2 > effSide.y+effSide.h)
		{
			y2=effSide.y+effSide.h;
		
		}
		
		inside.y=y1-outside.y;
		inside.h=y2-y1;
	
		outside.y=y1;
		outside.h=inside.h;
	
		SDL_BlitSurface(surface,&inside, XWidget::screen, (SDL_Rect *)&outside);
	}
	
	SDL_FreeSurface(surface);	
}



