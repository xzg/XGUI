#ifndef _XEVENTTYPE_H
#define _XEVENTTYPE_H

#include "SDL/SDL.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
*下层向上层发消息
*鼠标事件类型
*双击，单击（左、右），移动
*滑动轮(前后)
*/

//1--100是子组件先响应，100之后是父组件先响应
typedef enum
{
	X_LOSE_FOCUS=1,
	X_KEYDOWN,
	X_KEYUP,
	X_MOUSEMOTION,	
	X_MOUSEBUTTONDOWN,
	X_MOUSEBUTTONUP,
	
	X_QUIT=12,        
	X_VIDEORESIZE=16, //窗口大小改变
	
	X_TSDOWN,   //触摸屏按下
	X_TSUP,     //触摸屏弹起
	X_TSMOVE,    //触摸屏移动
	
	X_RESUME,
//绘制本身和子组件
	X_RELAYOUT=100,        //重新布局,但是root组件会刷新，其它组件不必刷新
	X_PAINT,               //只绘制组件,但是root组件会刷新，其它组件不必刷新

//start	
//会污染父组件和子组件，所以需要重绘制被污染组件，最后刷新	
	X_HIDE,                //隐藏消息
	X_UPDATE,              //绘制组件并刷新
	X_BUTTONDOWN,
	X_BUTTONUP,
	X_BUTTONENTER,
	X_BUTTONLEAVE,
//end	
	
	X_WIDGETCLOSE//root组件退出，例如MessegeBox	
		
}XEventType;

typedef enum
{
	X_BUTTON_LEFT=1,
	X_BUTTON_MIDDLE,
	X_BUTTON_RIGHT,
	X_BUTTON_WHEELUP,
	X_BUTTON_WHEELDOWN
	
}X_MouseType;


//组件状态
typedef enum
{
	X_ACTIVE,
	X_ORIGINAL,   //原始	     
	X_PRESSED,    //按下
	X_RELEASED,
	X_CLICK,
	
}X_WIDGET_STATE;  


typedef enum
{
	ID_NONE,
	ID_OK,
	ID_CANCLE,
	ID_OKCANCLE
}BUTTON_TYPE;

typedef struct 
{
	Uint16 type;
	Uint16 mouse_type;
	SDL_MouseMotionEvent *motion;
	SDL_MouseButtonEvent *button;
	SDL_KeyboardEvent  *key;
	SDL_UserEvent      *user;
	
	
}X_Event;

#ifdef __cplusplus
}
#endif

#endif
