#include <iostream>
using namespace std;


template <class T>
class SmartPointer
{
private:
	T* ptr = nullptr;
	size_t* counter;

public:
	SmartPointer() :ptr(), counter()
	{}

	SmartPointer(T* value)
	{
		ptr = value;
		counter = new size_t(1);
	}

	SmartPointer(const SmartPointer<T>& oth)
	{
		counter = &++ * oth.counter;
		ptr = oth.ptr;
	}
	~SmartPointer()
	{
		Release();
	}

	void Set(T* value)
	{
		Release();
		ptr = value;
		counter = new size_t(1);
	}

	T* Get()
	{
		return counter ? ptr : nullptr;
	}

	void Release()
	{
		if (ptr == nullptr) {
			return;
		}
		if (counter)
		{
			if (-- * counter == 0)
			{
				delete counter;
				delete ptr;
			}
			counter = nullptr;
			ptr = nullptr;
		}
	}

	SmartPointer& operator=(const SmartPointer<T>& other)
	{
		ptr = other.ptr;
		counter = other.counter;
		if (counter)
		{
			(*counter)++;
		}
		return *this;
	}

	T* operator->() const
	{
		return Get();
	}

	T& operator*() const
	{
		return *Get();
	}
};

int main()
{
	SmartPointer<int> ptr(new int(1024));
	cout << ptr.Get() << endl;
	auto prt1 = ptr;
	cout << prt1.Get() << endl;
	SmartPointer<int> ptr2(ptr);
	cout << ptr2.Get() << endl;
	SmartPointer<int> prt3(new int(135));
	cout << prt3.Get() << endl;
	ptr.Release();
	cout << prt1.Get() << endl;
	cout << prt3.Get() << endl;
}