#pragma once

template <typename T, size_t size = 1024 * 1024 >
class Buffer
{
public:
	void* ptr;
	size_t taken;

	Buffer() {
		ptr = malloc(size);
		taken = 0;
	}

	~Buffer() {
		delete ptr;
	}

	T* allocate(size_t n) {
		if (taken + n > size) {
			return nullptr;
		}
		T* n_ptr = (T*)((T*)ptr + taken);
		taken += n;
		return n_ptr;
	}

	void deallocate(size_t n) {
		if (taken - n > 0) {
			taken -= n;
			return;
		}
		taken = 0;
	}
};

