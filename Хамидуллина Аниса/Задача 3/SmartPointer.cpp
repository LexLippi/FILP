#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <tchar.h>
#include <map>
#include <ctime>
#include <vector>
#include <cmath>

using namespace std;

template <class T>
class SmartPointer {
	T* pointer;
	size_t refCount;

	void Attach(const SmartPointer<T>& other) 
	{
		if (other.pointer) 
		{
			pointer = other.pointer;
			refCount = other.refCount;
			refCount++;
		}
	}

public:
	SmartPointer() 
	{
		pointer = NULL;
		refCount = NULL;
	}

	~SmartPointer() 
	{
		Release();
	}

	T* Get() 
	{
		return pointer;
	}

	void Set(T* ptr)
	{
		pointer = ptr;
		refCount = 1;
	}

	void Release() 
	{
		if (pointer) 
		{
			refCount--;
			if (refCount == 0) 
				delete pointer;
			pointer = NULL;
			refCount = NULL;
		}
	}

	SmartPointer(const SmartPointer<T>& other) 
	{
		Attach(other);
	}

	T* operator->() 
	{
		return pointer;
	}

	T& operator*() 
	{
		return *pointer;
	}

	SmartPointer& operator=(const SmartPointer& other) 
	{
		if (this == &other)
			return *this;
		Release();
		Attach(other);
		return *this;
	}
};



int main(int argc, char* argv[])
{
	auto smartPtr1 = SmartPointer<string>();
	smartPtr1.Set(new string("first pointer"));
	cout << *smartPtr1.Get() << endl;
	auto smartPtr2 = SmartPointer<string>();
	smartPtr2.Set(new string("second pointer"));
	smartPtr1 = smartPtr2;
	cout << *smartPtr1.Get() << endl;
	auto smartPtr3 = SmartPointer<string>(smartPtr1);
	cout << *smartPtr3.Get() << endl;
}