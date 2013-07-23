#ifndef  _XSTRINGARRAY_H
#define _XSTRINGARRAY_H

#include<iostream>
using namespace std;

typedef struct string_data
{
	string data;
	string_data *next;
		
}string_data;

class XStringArray
{
public:
	XStringArray();
	~XStringArray();
	string& getAt(int index);
	void split(string& src,char c);
	void split(string& src,int average);  //平均分 参数average是每个string切分长度
	void append(string& str);
	void remove(int index);
	int  getCount();
	
	XStringArray& operator=(XStringArray &src);
private:
	
	string_data *head;
	string_data *s;
	
	int   count;
	static bool mark;  //做标记
	
};


#endif

