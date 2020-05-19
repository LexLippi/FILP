#pragma once

template<typename T>
class SmartPointer
{
private:

	T* pointer;
	unsigned int* counter;

public:

	SmartPointer() {
		pointer = nullptr;
		counter = nullptr;
	}

	SmartPointer(T* ptr) {
		pointer = nullptr;
		counter = nullptr;
		Set(ptr);
	}

	SmartPointer(const SmartPointer& other) {
		pointer = other.pointer;
		counter = other.counter;
		(*counter)++;
	}

	~SmartPointer(){
		Release();
	}


	void Set(T* ptr) {
		Release();
		pointer = ptr;
		counter = new unsigned int(1);
	}

	void Set(const SmartPointer& other) {
		Release();
		pointer = other.pointer;
		counter = other.counter;
		(*counter)++;
	}

	void Release() {
		if (counter == nullptr)
			return;
		(*counter)--;
		if (*counter == 0) {
			delete pointer;
			delete counter;
			pointer = nullptr;
			counter = nullptr;
		}
	}

	T Get() {
		return *pointer;
	}

	SmartPointer& operator=(const SmartPointer other) {
		Set(other);
		return *this;
	}

	SmartPointer& operator=(T* pointer) {
		Set(pointer);
		return *this;
	}

	T& operator*() {
		return *pointer;
	}

	T* operator->() {
		return pointer;
	}

	
	unsigned int GetRefCount() {
		return *counter;
	}
};

