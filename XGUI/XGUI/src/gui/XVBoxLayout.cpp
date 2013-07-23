/*
*竖直布局管理器
*/

#include "XVBoxLayout.h"

XVBoxLayout::XVBoxLayout()
{
	
}

/*
*给容器里面的BOX按照循序分配位置
*算法
	 _______
	|	  |
	|_______|
	|	  |
	|_______|
	
*如果BOX可变，则增加相应的宽度和高度	
*/
void XVBoxLayout::computeLayout()
{
	int 	h=0;
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
	
	child_type=list_int.getNext(intPos)->type;
	if( child_type == WIDGET)
	{	
		swap_widget = list_widget.getNext(widPos);
		swap_rect=swap_widget->getMinRect();
		
		if(swap_widget->isHVariable())//水平可变
		{
			swap_rect.w =rect.w-2*h_margin;
		}
		
		//是否靠边
		if(h_dock == false)
			swap_rect.x=rect.x + (rect.w-swap_rect.w)/2;
		else
			swap_rect.x=rect.x;
				
		if(swap_widget->isVVariable())//竖直可变
		{	
			swap_rect.h +=v_variable;	
		}
		
		if(v_dock == false)
			swap_rect.y=rect.y+v_margin;
		else swap_rect.y=rect.y;
				
		swap_widget->setLayoutRect(&swap_rect);
		swap_widget->computeLayout();
		
		mark=true;
		
		/*这个计算是以窗口为起始点
		  h等于现在BOX底部Y坐标
		*/	
		h=swap_rect.y+swap_rect.h;
		
	}else if(child_type == LAYOUT)
	{
		swap_layout=list_layout.getNext(layPos);
		swap_rect=swap_layout->getMinRect();
		/*查看布局管理器中是否有可变BOX*/
		if(swap_layout->getHVariableCount(&un) !=0 || swap_layout->getVVariableCount(&un)!=0)
		{
			swap_rect.h+=v_variable * swap_layout->getVVariableCount(&un) ; 		
		}
		mark=false;
		
		/*将布局BOX加上边距*/
		swap_rect.h +=(swap_layout->getVCount()+1) *v_margin;
		swap_rect.w=rect.w;
		swap_rect.x=rect.x;
		swap_rect.y=rect.y;
			
		swap_layout->setRect(&swap_rect);
		
		//计算自节点
		swap_layout->computeLayout();
		
		/*这个计算是以窗口为起始点
	 	 h等于现在BOX底部Y坐标
		*/	
		h=swap_rect.y+swap_rect.h;
	}else
	{
		h = rect.y + v_variable;
	}
	
	while(!intPos.isEmpty())
	{	
		child_type = list_int.getNext(intPos)->type;
		if(child_type == WIDGET)
		{	
			swap_widget=list_widget.getNext(widPos);
			swap_rect=swap_widget->getMinRect();
			
			if(swap_widget->isHVariable())
			{
				swap_rect.w =rect.w-2*h_margin;
			}
			
			if(swap_widget->isVVariable())
				swap_rect.h+=v_variable;
			
			if(h_dock == false)
				swap_rect.x=rect.x + (rect.w-swap_rect.w)/2;
			else
				swap_rect.x=rect.x;
				
			swap_rect.y=h;
			
			if(mark == true) swap_rect.y+=v_margin;
			
			swap_widget->setLayoutRect(&swap_rect);
			swap_widget->computeLayout();
			mark=true;
			
			h=swap_rect.y+swap_rect.h;
			
		}else if(child_type == LAYOUT)
		{
			swap_layout=list_layout.getNext(layPos); 
			swap_rect=swap_layout->getMinRect();
			
			/*查看布局管理器中是否有可变BOX*/
			if(swap_layout->getHVariableCount(&un) !=0 || swap_layout->getVVariableCount(&un)!=0) //有
			{	
				swap_rect.h+=v_variable * swap_layout->getVVariableCount(&un);
			}
			
			swap_rect.x=rect.x;
			swap_rect.y= h;
			swap_rect.w =rect.w;
			
			/*如果上一个BOX是布局容器那么少算一个边距，如果上一个BOX是组件边距数目是（组件数目＋1）
			  如果是两个布局容器相邻，那么设置第二个布局容器第一个组件紧挨BOX顶部
			*/
			if(mark == false){
				swap_rect.h+=v_margin*(swap_layout->getVCount());
				swap_layout->setVDock(true);
			}else 
				swap_rect.h+=v_margin*(swap_layout->getVCount()+1);
			
			swap_layout->setRect(&swap_rect);
			
			//计算自节点
			swap_layout->computeLayout();
			
			mark=false;
			
			h=swap_rect.y+swap_rect.h;
	    }else
	    {
	    	 h+=v_variable;
		}
	}
}

/*水平*/
int XVBoxLayout::getHVariableCount(int *w,bool cal)
{
	XWidget  *swap_widget;
	
	int      max_weight=0;
	int      swap_w=0;
	int      s;

	/*这个作用很大，去掉需谨慎！！*/
	if(cal == false ){
	   *w= min_w;
	   return h_v_num;
	}
	h_v_num=0;
	
	XPOSITION widPos = list_widget.getHeadPosition();
	XPOSITION layPos = list_layout.getHeadPosition();

	while(!widPos.isEmpty())
	{
		swap_widget = list_widget.getNext(widPos);

		if (swap_widget->isHVariable())
		{
			h_v_num=1;
		}
		
		if(max_weight < swap_widget->getMinRect().w)
			max_weight= swap_widget->getMinRect().w;
	}
	
	while(!layPos.isEmpty())
	{
		swap_w=0;
		s=list_layout.getNext(layPos)->getHVariableCount(&swap_w,cal);
		if(h_v_num < s)
			h_v_num=s;
		if(max_weight < swap_w)
			max_weight=swap_w;	
	}
	rect.w=max_weight;
	min_w=max_weight;
	
	*w+=max_weight;
	
	return h_v_num; 	
}

/*垂直*/
/*算法：如果第一个是组件的话那么留空隙，否则不留
	相邻BOX如果有布局容器有不留空隙
*/
int XVBoxLayout::getVVariableCount(int *h,bool cal)
{
	/*本BOX最小高度*/
	int t_h=0;
	XWidget  *swap_widget;
	
	/*这个作用很大，去掉需谨慎！！*/
	if(cal == false ) {
		*h=min_h;
		return v_v_num;
	}
	v_v_num=0;
	
	XPOSITION widPos = list_widget.getHeadPosition();
	XPOSITION layPos = list_layout.getHeadPosition();
	XPOSITION intPos = list_int.getHeadPosition();

	if(list_int.getCount() ==0) return 0;
	
	while(!widPos.isEmpty())
	{
		swap_widget=list_widget.getNext(widPos);
		if(swap_widget->isVVariable())
			++v_v_num;
		t_h+= swap_widget->getMinRect().h;
	}

	while(!layPos.isEmpty())
	{
		v_v_num+=list_layout.getNext(layPos)->getVVariableCount(&t_h,cal);
	}
	
	while(!intPos.isEmpty())
	{
		if(list_int.getNext(intPos)->type == SPACE)
			++v_v_num;
	}

	rect.h=t_h;
	min_h=t_h;
	
	*h+=t_h;
	
	return v_v_num;	
}

int XVBoxLayout::getHCount()
{
	int num=0;
	int max_num=0;
	
	XPOSITION widPos = list_widget.getHeadPosition();
	XPOSITION layPos = list_layout.getHeadPosition();

	if(!list_widget.isEmpty()) max_num=1;
	
	while(!layPos.isEmpty())
	{
		num=list_layout.getNext(layPos)->getHCount();
		if(max_num < num) max_num=num;
	}
	
	return max_num;
}

int XVBoxLayout::getVCount()
{
	int num=0;
	list_layout.begin();
	list_widget.begin();
	
	XPOSITION widPos = list_widget.getHeadPosition();
	XPOSITION layPos = list_layout.getHeadPosition();


	if(!list_widget.isEmpty())
		num=list_widget.getCount(); 

	while (!layPos.isEmpty())
	{
		num+=list_layout.getNext(layPos)->getVCount();
	}
	
	return num;
}

