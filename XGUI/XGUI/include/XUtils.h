/*
*对于SDL的封装
*/

#ifndef _XUTIL_H
#define _XUTIL_H

#include <SDL/SDL_timer.h>
#include <SDL/SDL.h>
#include <SDL/SDL_video.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

typedef unsigned int  ID;
typedef SDL_Rect XRect; 
typedef SDL_Color XColor;

typedef SDL_Surface XSurface;

typedef struct
{
	Uint16 w;
	Uint16 h;
}XSize;

typedef struct
{
	Uint16 x;
	Uint16 y;
}XPoint;

typedef struct
{
	Uint16 x1;
	Uint16 y1;
	Uint16 x2;
	Uint16 y2;
}XRectF;


typedef struct
{
	SDL_Surface *surface;
	XRect       *rect;
}XDC;


#endif
