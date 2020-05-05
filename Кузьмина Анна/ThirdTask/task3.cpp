#include "pch.h"
#include <stdio.h>
#include<iostream>
using namespace std;

template <class T>
class SmartPointer
{
private:
	T *pointer;
	size_t *ref_count;

public:
	SmartPointer()
	{
		pointer = NULL;
		ref_count = NULL;
	}

	~SmartPointer()
	{
		Release();
	}

	void Release()
	{
		if (pointer)
		{
			(*ref_count)--;
			if ((*ref_count) == 0)
			{
				delete pointer;
				delete ref_count;
			}
			ref_count = NULL;
			pointer = NULL;
		}

	}

	T Get()
	{
		return *pointer;
	}

	void Set(T *ptr)
	{
		pointer = ptr;
		ref_count = (size_t*)malloc(sizeof(size_t));
		*ref_count = 1;
	}

	SmartPointer& operator=(SmartPointer& other_pointer)
	{
		if (*ref_count > 0)
		{
			Release();
		}
		if (this != &other_pointer)
		{
			pointer = other_pointer.pointer;
			ref_count = other_pointer.ref_count;
			(*ref_count)++;
		}
		return *this;
	}

	T* operator->()
	{
		return Get();
	}

	T operator*()
	{
		return *this->pointer;
	}
};

int main() 
{
	SmartPointer<int> ptr_int1;
	SmartPointer<int> ptr_int2;
	ptr_int1.Set(new int(1));
	ptr_int2.Set(new int(2));
	cout << ptr_int1.Get() << " " << *ptr_int1 << " " << &ptr_int1 << endl;
	ptr_int1 = ptr_int2;
	cout << ptr_int1.Get() << " " << *ptr_int1 << " " << &ptr_int1 << endl;
	ptr_int1.Release();
	ptr_int2.Release();
}