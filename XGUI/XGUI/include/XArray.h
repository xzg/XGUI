#ifndef __XARRAY_H
#define __XARRAY_H

#include <iostream>
using namespace std;

template <class T>
class XArray
{
public:
	XArray();
	~XArray();	
	
public:

	void add(const T& element);
	T& getAt(int index);
	void insertAt(int index,const T& element);
	//ÐÞ¸ÄÔªËØ
	void setAt(int index,const T& element);
	void remove(int index);
	void removeAll();
	int getCount();
private:
	T *pArray;
	int count;
	int size;
}; 

template <class T>
XArray<T>::XArray()
{
	size=0;
	count=0;
	pArray=NULL;
}

template <class T>
XArray<T>::~XArray()
{
	if(pArray)
		free(pArray);
	size=0;
	count=-1;
	pArray=NULL;
}

template <class T>
void XArray<T>::add(const T& element)
{
	size+=sizeof(T);
	pArray= (T *)realloc(pArray,size);
	
	pArray[count]=element;
	++count;
}

template <class T>
T& XArray<T>::getAt(int index)
{
	if(index <count)
		return pArray[index];
		
	return pArray[-1];
}


template <class T>
void XArray<T>::setAt(int index,const T& element)
{
	if(index <count)
		pArray[index]=element;
}

template <class T>
int XArray<T>::getCount()
{
	return count;
}

template <class T>
void XArray<T>::remove(int index)
{
	if(index <count)
	{
		size-=sizeof(T);
		T *lP = (T *)malloc(size);
		
		memcpy(lP,pArray,index*sizeof(T));
		memcpy(lP+index,pArray+index+1,sizeof(T)*(count-index-1));
		free(pArray);
		pArray=lP;
		
		--count;
	}
	
}

template <class T>
void XArray<T>::removeAll()
{
	if(pArray)
		free(pArray);
	size=0;
	count=-1;
	pArray=NULL;
	
}

template <class T>
void XArray<T>::insertAt(int index,const T& element)
{
	if(index <count)
	{
		size+=sizeof(T);
		T *lP = (T *)malloc(size);
		
		memcpy(lP,pArray,index*sizeof(T));
		memcpy(lP+index+1,pArray+index,sizeof(T)*(count-index));
		free(pArray);
		pArray=lP;
		pArray[index]=element;
		++count;
	}
}

#endif
