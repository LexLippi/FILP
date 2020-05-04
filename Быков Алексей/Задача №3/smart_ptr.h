#pragma once
template<class T>
class smart_ptr {
	T* ptr;
	unsigned int* counter;
public:
	smart_ptr() {
		ptr = nullptr;
		counter = nullptr;
	}

	smart_ptr(const smart_ptr& _second) {
		ptr = _second.ptr;
		counter = _second.counter;
		(*counter)++;
	}

	~smart_ptr() {
		Release();
	}

	smart_ptr& operator=(const smart_ptr& _second) {
		Release();
		ptr = _second.ptr;
		counter = _second.counter;
		(*counter)++;
		return *this;
	}

	T* operator->() {
		return ptr;
	}

	T& operator*() {
		return *ptr;
	}

	void Set(T* elem) {
		Release();
		ptr = elem;
		counter = new unsigned int(1);
	}

	T Get() {
		return *ptr;
	}

	void Release() {
		if (ptr != nullptr)
			--(*counter);
		if (counter != nullptr && *counter == 0) {
			delete ptr;
			delete counter;
			ptr = nullptr;
			counter = nullptr;
		}
	}
};