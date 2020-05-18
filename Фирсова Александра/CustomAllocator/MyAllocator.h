#pragma once
#include "LinkedList.h"
#include "Buffer.h"

template<typename T>
struct MyAllocator
{
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = size_t;

	LinkedList<Buffer<T>>* linkedList;

	MyAllocator() {
		linkedList = new LinkedList<Buffer<T>>();
		linkedList->push(new Buffer<T>());
	}

	template<typename U>
	explicit MyAllocator(const MyAllocator<U>& other) noexcept {
		linkedList = new LinkedList<Buffer<T>>();
		linkedList->push(new Buffer<T>());
	}

	template<typename U>
	struct rebind {
		typedef MyAllocator<U> other;
	};

	~MyAllocator() {
	}

	pointer allocate(size_t n) {
		node<Buffer<T>>* current = linkedList->head;
		pointer n_pointer = linkedList->tail->data->allocate(n);
	
		if (n_pointer == nullptr) {
			Buffer<T>* n_buff = new Buffer<T>();
			linkedList->push(n_buff);
			n_pointer = n_buff->allocate(n);
		}

		return n_pointer;
	}

	void deallocate(pointer p, size_t n){ }
};