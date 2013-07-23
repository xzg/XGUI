#include "XUseful.h"


/*将一个BOX填到另一个BOX中间*/
void BoxCenter(XRect *src,XRect *dst)
{
	int w,h;
	
	if(src->w < dst->w) dst->w=src->w;
	if(src->h < dst->h) dst->h=src->h;
	
	w=(src->w - dst->w)/2;
	h=(src->h - dst->h)/2;
	
	dst->x=src->x+w;
	dst->y=src->y+h;
}

typedef struct
{
	Uint8 a;
	Uint8 b;
	Uint8 c;
	Uint8 d;
}Color;

void ModifyAlpha1(Uint32* color,float percent)
{
	
	Uint8 *p =(Uint8 *)color;
	*p=(Uint8 )((float)((*color)&0x000000ff)*percent);
	
}

/*修改设置颜色的透明度,color必须是这样的0xffffffff*/
void ModifyAlpha(XColor *color,float percent)
{
	Color *this_color;
	
	this_color =(Color *)color;
	this_color->a = (Uint8 )(percent *0xff);
	
}
/*设置颜色的透明度，color必须是这样的0xffffff*/
void SetAlpha(XColor *color,float percent)
{
	int s=(int)(0xff * percent);
	int *p= (int *)color;
	
	*p= ((*p <<8)&0xffffff00 | s);
}

void separatorLine(SDL_Surface  *surface,Uint32 color,int x1,int x2,int y)
{
	Uint32 swap_color=color;
	char *p=(char *)&color;
	int i=x1;
	int j=x2;
	
	for(;i<x1+15;++i)
	{
		
		*p=(int )((float)(i-x1)/15 * 0xff);
		
		pixelColor(surface,i,y,color);
	}
	
	for(;j>x2-15;--j)
	{
		*p=(int )((float)(x2-j)/15 * 0xff);
		
		pixelColor(surface,j,y,color);
	}	
	
	hlineColor(surface,i,j,y,swap_color);
}

bool isAlpha(Uint32& color)
{
	Uint8 *p =(Uint8 *)color;
	
	if( (color&0x000000ff)  == 0xff) 
		return false;
	else 
		return true;
}


