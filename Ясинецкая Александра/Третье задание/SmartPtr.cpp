#include <iostream>

using namespace std;

template <class T>
class SmartPtr
{
	T* Ptr;
	size_t* Count;

public:

	void Attach(const SmartPtr& Other)
	{
		Count = Other.Count;
		(*Count)++;
		Ptr = Other.Ptr;
	}

	SmartPtr()
	{
		Ptr = 0;
		Count = 0;
	}

	SmartPtr(const SmartPtr& Other)
	{
		Attach(Other);
	}

	~SmartPtr()
	{
		Release();
	}

	T Get()
	{
		return *Ptr;
	}

	void Set(T* ptr) 
	{
		Ptr = ptr;
		Count = new size_t(1);
	}

	void Release()
	{
		if (Ptr)
		{
			(*Count)--;
			if ((*Count) == 0)
			{
				delete Ptr;
				delete Count;
			}
			Ptr = 0;
			Count = 0;
		}
	}

	SmartPtr& operator=(const SmartPtr& Other)
	{
		if (this != &Other)
		{		
			Release();
			Attach(Other);
		}
		return *this;
	}

	T& operator->()
	{
		return *Ptr;
	}

	T* operator* ()
	{
		return Ptr;
	}
};

int main(int argc, char *argv[]) {
	SmartPtr<int> ptr = SmartPtr<int>();
	ptr.Set(new int(7));
	cout << ptr.Get() << '\n';
	SmartPtr<int> ptr1 = ptr;
	ptr.Release();
	cout << ptr1.Get() << '\n';
	int* a = *ptr1;
	cout << *a << '\n';
	ptr1.Set(new int(11));
	cout << ptr1.Get() << '\n';
	cout << *ptr1 << '\n';
	ptr1.Release();
	cout << *ptr1 << '\n';
}