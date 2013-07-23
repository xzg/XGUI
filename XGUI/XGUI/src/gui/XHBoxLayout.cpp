/*
*水平布局管理器
*/

#include "XHBoxLayout.h"

/*给容器里面的BOX按照循序分配位置
*算法
	________________
	|	  |	    |
	|_______|_______|

*/

void XHBoxLayout::computeLayout()
{
	int 	w=0;
	XRect   swap_rect;
	XLayout *swap_layout;
	XWidget *swap_widget;
	int 	un=0;
	bool    mark=true;
	
	Uint8  child_type;
	int    h_margin=0;
	int    v_margin=0;
	int    h_variable=0;
	int    v_variable=0;

	if(list_int.isEmpty())
		return ;
	compute(&h_margin,&v_margin,&h_variable,&v_variable);	
	
	
	XPOSITION intPos = list_int.getHeadPosition();
	XPOSITION widPos = list_widget.getHeadPosition();
	XPOSITION layPos = list_layout.getHeadPosition();

	child_type= list_int.getNext(intPos)->type;
	if(child_type == WIDGET)
	{	
		swap_widget = list_widget.getNext(widPos);
		
		swap_rect=swap_widget->getMinRect();
		if(swap_widget->isHVariable() ) //水平可变
		{
			swap_rect.w +=h_variable;
		}
		
		//是否靠边	
		if(h_dock == false)
				swap_rect.x=rect.x+h_margin;
		else swap_rect.x=rect.x;
		
		if(swap_widget->isVVariable() ) //竖直可变
		{	
			swap_rect.h =rect.h-2*v_margin;
			
			if(v_dock == false){
				swap_rect.y=rect.y+v_margin;
			}
			else swap_rect.y =rect.y;
		
		}else
		{
			if(v_dock == false)
				swap_rect.y=rect.y + (rect.h-swap_rect.h)/2;  //这里不会出现rect.h<swap_rect.h
			else
				swap_rect.y =rect.y;
		}
		swap_widget->setLayoutRect(&swap_rect);
		swap_widget->computeLayout();
		
		mark=true;
		
		w=swap_rect.x+swap_rect.w;
		
	}else if(child_type == LAYOUT)
	{
		swap_layout=list_layout.getNext(layPos);
		swap_rect=swap_layout->getMinRect();
		
		/*查看布局管理器中是否有可变BOX*/
		if(swap_layout->getHVariableCount(&un) !=0 || swap_layout->getVVariableCount(&un)!=0)
		{
			swap_rect.w+=h_variable * swap_layout->getHVariableCount(&un) ; 
			
		}
		mark=false;
		
		swap_rect.x=rect.x;
		swap_rect.y=rect.y;
		swap_rect.h=rect.h;	
		
		/*将布局BOX加上边距*/
		swap_rect.w +=(swap_layout->getHCount()+1) *h_margin;
		
		swap_layout->setRect(&swap_rect);
		//计算子节点
		swap_layout->computeLayout();
		
		/*这个计算是以窗口为起始点
	 	 h等于现在BOX底部Y坐标
		*/
		w=swap_rect.x+swap_rect.w;
	}else
	{
		w=rect.x+h_variable;
	}
	
	while(!intPos.isEmpty())
	{	
		child_type =list_int.getNext(intPos)->type;
		if(child_type == WIDGET)
		{	
			swap_widget=list_widget.getNext(widPos);
			swap_rect=swap_widget->getMinRect();
			
			if(swap_widget->isHVariable())  //水平可变
				swap_rect.w += h_variable;
			//x坐标
			swap_rect.x=w;
			
			if(swap_widget->isVVariable())//竖直可变
			{	
				swap_rect.h =rect.h-2*v_margin;
				
				if(v_dock == false)
					swap_rect.y=rect.y+v_margin;
				else 	swap_rect.y =rect.y;
				
			}else{
				if(v_dock == false)
					swap_rect.y=rect.y + (rect.h-swap_rect.h)/2;
				else swap_rect.y=rect.y;
			}
			
			if(mark == true) swap_rect.x+=h_margin;
			
			swap_widget->setLayoutRect(&swap_rect);
			swap_widget->computeLayout();
			mark=true;
			
			
			w=swap_rect.x+swap_rect.w;
		}else if(child_type == LAYOUT)
		{
			swap_layout=list_layout.getNext(layPos);
			swap_rect=swap_layout->getMinRect();
			
			/*查看布局管理器中是否有可变BOX*/
			if(swap_layout->getHVariableCount(&un) !=0 || swap_layout->getVVariableCount(&un)!=0) //有
			{	
				
				swap_rect.w+=h_variable * swap_layout->getHVariableCount(&un);
			}
			
			swap_rect.y=rect.y;
			swap_rect.x= w;
			swap_rect.h =rect.h;
			
			/*如果上一个BOX是布局容器那么少算一个边距，如果上一个BOX是组件边距数目是（组件数目＋1）
			  如果是两个布局容器相邻，那么设置第二个布局容器第一个组件紧挨BOX顶部
			*/
			if(mark == false){
				swap_rect.w+=h_margin*(swap_layout->getHCount());
				swap_layout->setHDock(true);
			}else 
				swap_rect.w+=h_margin*(swap_layout->getHCount()+1);
				
			swap_layout->setRect(&swap_rect);
			
			//计算子节点
			swap_layout->computeLayout();
			mark=false;
			w=swap_rect.x+swap_rect.w;
	    }else
	    {
	    	w += h_variable;
	    }
	}
}


/*水平*/
int XHBoxLayout::getHVariableCount(int *w,bool cal)
{
	/*本BOX最小高度*/
	int t_w=0;
	XWidget  *swap_widget;

	/*如果计算过了，就不再计算了*/
	if(cal == false ){
		*w=min_w;
		return h_v_num;
	}
	h_v_num=0;
	
	XPOSITION widPos = list_widget.getHeadPosition();
	XPOSITION layPos = list_layout.getHeadPosition();
	XPOSITION intPos = list_int.getHeadPosition();

	if(list_int.getCount() ==0) return 0;
	
	while(!widPos.isEmpty())
	{
		swap_widget=list_widget.getNext(widPos);
		if(swap_widget->isHVariable())
			++h_v_num;
		t_w+= swap_widget->getMinRect().w;
	}

	while(!layPos.isEmpty())
	{
		h_v_num+=list_layout.getNext(layPos)->getHVariableCount(&t_w,cal);
	}
	
	while(!intPos.isEmpty())
	{
		if(list_int.getNext(intPos)->type == SPACE)
			++h_v_num;
	}
	
	rect.w=t_w;
	min_w = t_w;
	*w+=t_w;
	
	return h_v_num;	
}

/*
竖直
*/
int XHBoxLayout::getVVariableCount(int *h, bool cal)
{
	XWidget  *swap_widget;
	int      v_num=0;
	int      max_high=0;
	int      swap_h=0;
	int      s;
	XPOSITION widPos = list_widget.getHeadPosition();
	XPOSITION layPos = list_layout.getHeadPosition();

	if(cal == false ){
		 *h=min_h;
		 return v_v_num;
	}
	v_v_num=0;
	
	while(!widPos.isEmpty())
	{
		swap_widget =list_widget.getNext(widPos);
		if(swap_widget->isVVariable())
			v_v_num=1;
		if(max_high < swap_widget->getMinRect().h)
			max_high= swap_widget->getMinRect().h;
	}
	
	while(!layPos.isEmpty())
	{
		swap_h=0;
		s=list_layout.getNext(layPos)->getVVariableCount(&swap_h,cal);
		if(v_v_num < s)
			v_v_num=s;
		if(max_high < swap_h)
			max_high=swap_h;	
	}

	min_h= max_high;
	rect.h=max_high;
	*h+=max_high;
	
	return v_v_num; 	
}
int XHBoxLayout::getHCount()
{
	int num=0;
	XPOSITION widPos = list_widget.getHeadPosition();
	XPOSITION layPos = list_layout.getHeadPosition();

	if(!list_widget.isEmpty())
		num=list_widget.getCount();
	
	while(!layPos.isEmpty())
	{
		num+=list_layout.getNext(layPos)->getHCount();
	}

	return num;
}

int XHBoxLayout::getVCount()
{
	int num=0;
	int max_num=0;
	
	XPOSITION widPos = list_widget.getHeadPosition();
	XPOSITION layPos = list_layout.getHeadPosition();

	if(!list_widget.isEmpty()) max_num=1;
	
	while(!layPos.isEmpty())
	{
		num=list_layout.getNext(layPos)->getVCount();
		if(max_num < num) max_num=num;
	}

	return max_num;
}


