#include "stdafx.h"
#include <iostream>

template <typename T>
class SmartPointer {
	T *m_obj;
	unsigned int *counter;
public:
	SmartPointer() {
		m_obj = nullptr;
		counter = nullptr;
	}

	~SmartPointer() {
		Release();
	}

	void Release() {
		if (m_obj) {
			--(*counter);
		}

		if (counter && *counter == 0) {
			delete m_obj;
			delete counter;
			m_obj = nullptr;
			counter = nullptr;
		}
	}

	void Set(T* obj) {
		Release();
		m_obj = obj;
		counter = new unsigned int(0);
		(*counter)++;

	}

	T Get() { return *m_obj; }

	SmartPointer& operator=(SmartPointer& other) {
		Release();
		m_obj = other.m_obj;
		counter = other.counter;
		(*counter)++;
		return *this;
	}

	T* operator-> () {
		return m_obj;
	}

	T& operator* () {
		return *m_obj;
	}
};


int main() {
	SmartPointer<int> ptr1;
	ptr1.Set(new int(1));
	std::cout << ptr1.Get() << " " << *ptr1 << " " << &ptr1 << std::endl;

	SmartPointer<int>ptr2;
	ptr2.Set(new int(5));

	SmartPointer<int>ptr3;
	ptr3.Set(new int(65));

	ptr1 = ptr2;
	std::cout << ptr1.Get() << " " << *ptr1 << " " << &ptr1 << std::endl;

	ptr1 = ptr3;
	std::cout << ptr1.Get() << " " << *ptr1 << " " << &ptr1 << std::endl;

	ptr1.Release();
	ptr2.Release();
	ptr2.Release();

	system("PAUSE");
	return 0;
}