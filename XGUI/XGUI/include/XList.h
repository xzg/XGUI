#ifndef _XLIST_H
#define _XLIST_H

#include<iostream>
using namespace std;


class XPOSITION;

template <class T> 
class Xdata
{
public:
	T  data;
	
	Xdata<T> *previous;
	Xdata<T> *next;
};

template <class T>
class XList
{
public:

	XList();
	~XList();
	/*
	*1 返回总长度
	*2 返回现在链表指针所在位置
	*/
	int  getCount();
	int  getCurrentIndex();
	
	XPOSITION getHeadPosition();
	XPOSITION getTailPosition();
	XPOSITION find(const T& value);
	T&        getNext(XPOSITION& rPosition);
	T&        getPrev(XPOSITION& rPosition);
	
	
	/*
	*第一个是检索元素的索引值
	*第二个是检索对象的索引值
	*return 非负代表检索到，否则没有
	*/
	int indexOf(const T& e);
	
	void setPosition(const T& value);
	//operation
	void append(const T& value);
	void remove(int index);
	void removeAll();
	
	/*
	*在索引值为before节点前面；
	*如果before==count那么插在最后
	*/
	void insert(int before,const T& value);
	
	T& getAt(int index);
	
	void begin();
	void final();
	/*
	*以下几个函数是对于检索的操作
	*/
	bool isEmpty();
	
	T& takeFirst();
	
	T& takeLast();
	
	T& current();
	
	bool isNext();
	T& next();
	
	bool isPrevious();
	T& previous();
	
private:
	Xdata<T> *head;
	Xdata<T> *last;
	
	Xdata<T> *current_data;

	int count;
	
};

class XPOSITION
{
public:
	bool isEmpty()
	{
		if(!pValue) return true;
		
		return false;
	}

	void *pValue;

};


#include "XList.h"

template <class T>
XList<T>::XList()
{
	head=NULL;
	last=NULL;
	
	count=0;
	current_data=NULL;
}

template <class T>
XList<T>::~XList()
{
	Xdata<T> *p;
	
	if(!head)
		return ;
	
	while(head)
	{
		p=head;
		head=head->next;
		delete p;
	}
}
template <class T>
void XList<T>::append(const T& value)
{
	Xdata<T> *p;
	
	p=new Xdata<T>;
	p->data=value;

	if(head==NULL){
		head=p;
		last=p;
		
		p->previous=NULL;
		p->next=NULL;
		current_data=head;
	}
	else{
		last->next=p;
		p->previous=last;
		last=last->next;
		last->next=NULL;
	}
		
	++count;
}

template <class T>
XPOSITION XList<T>::getHeadPosition()
{
	XPOSITION pos;
	pos.pValue = head;
	
	return pos;
}

template <class T>
XPOSITION XList<T>::getTailPosition()
{
	XPOSITION pos;
	pos.pValue = last;
	
	return pos;
}

template <class T>
XPOSITION XList<T>::find(const T& value)
{
	XPOSITION pos;
	Xdata<T> *swap;
	
	pos.pValue=NULL;
	swap=head;
	
	while(swap)
	{
		if(swap->data == value)
		{
			pos.pValue = swap;
			break;
		}
		swap = swap->next;
	}
	
	return pos;
}

template <class T>
T&  XList<T>::getNext(XPOSITION& rPosition)
{
	Xdata<T> *swap;
	
	swap = ((Xdata<T> *)rPosition.pValue);
	rPosition.pValue = ((Xdata<T> *)(rPosition.pValue))->next;
	return swap->data;
}

template <class T>
T&  XList<T>::getPrev(XPOSITION& rPosition)
{
	Xdata<T> *swap;
	
	swap = ((Xdata<T> *)rPosition.pValue);
	rPosition.pValue = ((Xdata<T> *)(rPosition.pValue))->previous;
	return swap->data;
}

template <class T>
int XList<T>::indexOf(const T& e)
{
	Xdata<T> *swap;
	int i=0;

	swap=head;

	while(swap)
	{
		if(e == (swap->data))
			return i;
		swap=swap->next;
		i++;
	}

	return -1;
}

template <class T>
void XList<T>::setPosition(const T& value)
{
	Xdata<T> *swap;
	
	swap=head;
	do{
		current_data=swap;
		swap=swap->next;
	}while(swap->data != value);
	
}

/*
template <class T>
int XList<T>::indexOf(XObject *obj)
{
	Xdata<T> *swap;
	int i=0;

	swap=head;
	while(swap)
	{
		if(swap->data.equals(obj))
			return i;
			
		swap=swap->next;
		i++;
		
	}
	
	return -1;
}
*/

template <class T>
int XList<T>::getCount()
{
	return count;
}

template <class T>
void XList<T>::removeAll()
{
	Xdata<T> *p;
	
	
	while(head)
	{
		p=head;
		head=head->next;
		delete p;
	}
	head=NULL;
	last=NULL;
	
	count=0;
	current_data=NULL;
}

template <class T>
void XList<T>::remove(int index)
{
	Xdata<T> *swap;
	Xdata<T> *c;
	Xdata<T> *p;
	
	int i=0;
	
	if(index <0) return ;
	
	swap=head;
	while(swap)
	{
		/*
		*一旦删除的正是current_data就会出错
		*删除之后总长度减少
		*/
		
		if(index ==i){
			
			count--;
			
			/*
			*下面考虑current_data和currrent_index的移动情况
			*1.如果删除的位置大于current_index，没关系
			*2.如果删除的位置小于current_index，只需--current_index
			*3.如果删除的位置等于current_index,分两种情况一是在表头，只需current_data往后移动
			  另外如果不是在表头current_data往后移动
			*/
			if(current_data == swap)
			{
				current_data=current_data->next;
				
			}
			/*
			*如果删除表头只需head后移动
			*如果删除表尾只需last前移
			*如果删除中间抠掉一个
			*/
			if(index == 0)
			{	
				head=head->next;
				if(head)
					head->previous=NULL;
				
			}else if(index == count){
				
				last=last->previous;
				if(last)
					last->next=NULL;
				
			}else{
			
				c=swap->previous;
				p=swap->next;
				
				c->next=p;
				p->previous=c;			
			}
			
			delete swap;
			break;
		}
		swap=swap->next;
		++i;	
	}
}

template <class T>
void XList<T>::insert(int before,const T& value)
{
	Xdata<T> *p;
	Xdata<T> *c;
	Xdata<T> *swap;
	int i=0;
	
	swap=head;
				
	while(swap)
	{
		if(i == before ){
			
			p=new Xdata<T>;
			p->data=value;
			
			if(before == 0)
			{
				p->next=head;
				head=p;
				head->previous=NULL;
			}else
			{
				c=swap->previous;
				c->next=p;
				p->next=swap;
				swap->previous=p;
			}

			count++;
			break;
		}
		swap=swap->next;
		i++;	
	}
	
	if(before == count)
	{
		p=new Xdata<T>;
		p->data=value;
		
		last->next=p;
		p->previous=last;
		p->next=NULL;
	}
}
template <class T>
void XList<T>::begin()
{
	current_data=head;
}

template <class T>
void XList<T>::final()
{
	current_data =last;
}


/*
*以下几个函数是对于检索的操作
*/
template <class T>
T& XList<T>::takeFirst()
{
	return head->data;
}

template <class T>
T& XList<T>::takeLast()
{
	return last->data;
}

template <class T>
bool XList<T>::isEmpty()
{
	if(count==0)
		return true;
	
	return false;
}

template <class T>
bool XList<T>::isNext()
{
	if(current_data && current_data->next) 
		return true;
	
	return false;
}

template <class T>
T& XList<T>::next()
{
	current_data=current_data->next;
	
	return current_data->data;
}

static int i=0;

template <class T>
bool XList<T>::isPrevious()
{
	if(current_data && current_data->previous) 
		return true;
	
	return false;
}

template <class T>
T& XList<T>::previous()
{
	current_data=current_data->previous;
	
	return current_data->data;
}

template <class T>
T& XList<T>::current()
{
	return current_data->data; 
}

template <class T>
int  XList<T>::getCurrentIndex()
{
	return 0;
}


#endif
