#include "XStringArray.h"
#include <string.h>

bool XStringArray::mark=false;

XStringArray::XStringArray()
{
	head=NULL;
	count=0;
	mark=false;
}
XStringArray::~XStringArray()
{
	string_data *p;
	
	if(mark==false){
		while(head)
		{
			p=head;
			head=head->next;
		
			delete p;
		}
		mark=true;
	}
}
void XStringArray::append(string& str)
{
	string_data *p;
			
	p=new string_data;
	p->data=str;
	
	if(head==NULL)
		head=p;
	else
		s->next=p;
		s=p;
		s->next=NULL;
	++count;
}
void XStringArray::remove(int index)
{
	int i;
	string_data *p;
	string_data *s;
	
	p=head;
	
	if(index<0) return ;
	
	if(index==0)
	{
		head=head->next;
		delete p;
		return ;	
	}
	
	for(i=0;i<index;++i)
	{
		s=p;
		p=p->next;
	}
	
	s->next=p->next;
	delete p;
}

void XStringArray::split(string& src,char c)
{
	int i=0;
	char *p=(char *)src.c_str();
	int pos=0;
	
	while(pos!=-1)
	{
		pos=src.find(c,pos);
		if(pos != -1)
		{	
			string x_str;
			x_str.append(p,i,pos-i);
			append(x_str);
			
			++pos;
			i=pos;
		}
	}
	
	if(i!=0){
		string x_str;
		x_str.append(p,i,src.length()-i);
		append(x_str);
	}
}
void XStringArray::split(string& src,int average)
{
	int i=0;
	char *p=(char *)src.c_str();
	
	for(;i<src.length(); i+=average)
	{
		string x_str;
		x_str.append(p,i,average);
		append(x_str);
	}	
}

XStringArray& XStringArray::operator=(XStringArray &src)
{
	memcpy(this,&src,sizeof(XStringArray));	
	
	return *this;
}

string& XStringArray::getAt(int index)
{
	int i;
	string_data *p;	
	
	p=head;
	for(i=0;i<index;++i)
		p=p->next;
	
	return p->data;
}
int XStringArray::getCount()
{
	return count;
}



