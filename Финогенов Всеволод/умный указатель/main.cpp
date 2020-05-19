

#include <iostream>
#include "SmartPointer.h"

using namespace std;

int main()
{
	cout << "smart pointer assign" << "\n";
	SmartPointer<int> smartPtr1 = new int(5);
	cout << "smart pointer get: " << smartPtr1.Get() << endl;
	cout << "smart pointer ref count: " << smartPtr1.GetRefCount() << endl;
	cout << "assign smart pointer 2" << endl;
	SmartPointer<int> smartPtr2 = smartPtr1;
	cout << "smart pointer 1 ref count: " << smartPtr1.GetRefCount() << endl;
	cout << "smart pointer 2 ref count: " << smartPtr2.GetRefCount() << endl;
	cout << "change smart pointer 1 value" << endl;
	*smartPtr1 = 3;
	cout << "smart pointer 1 get: " << smartPtr1.Get() << endl;
	cout << "smart pointer 2 get: " << smartPtr2.Get() << endl;
	cout << "release smart pointer 1" << endl;
	smartPtr1.Release();
	cout << "smart pointer 2 ref count: " << smartPtr2.GetRefCount() << endl;
	smartPtr2.Release();

    
}
