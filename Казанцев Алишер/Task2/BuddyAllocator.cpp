#include "stdafx.h"
#include <iostream>

using namespace std;

enum Status {
	Free,
	Splitted,
	Occuped
};

class BuddyBlock {
	static const size_t min_size = 32;

public:
	BuddyBlock(int block_start_position, size_t size_) {
		position = block_start_position;
		size = size_;
		status = Free;
	}

	~BuddyBlock() {
		free(top_child);
		free(bottom_child);
		free(ptr);
	}

	int* fill_block(size_t content_size) {
		if (status == Occuped) {
			return nullptr;
		}

		if (status == Free) {
			if (size <= min_size && content_size <= size) {
				allocate(content_size);
				return ptr;
			}

			if (content_size <= size / 2) {
				split_block();
				return top_child->fill_block(content_size);
			}
			else {
				allocate(content_size);
				return ptr;
			}
		}

		if (status == Splitted) {
			int* res = top_child->fill_block(content_size);
			if (res == nullptr) {
				return bottom_child->fill_block(content_size);
			}
			return res;
		}
	}

	bool try_release_data(int* data_ptr) {
		if (status == Free)
			return false;

		if (status == Occuped) {
			if (ptr == data_ptr) {
				free(ptr);
				status = Free;

				cout << "Data was released from block [" << position << ":" << position + size - 1 << "]" << endl;

				return true;
			}
			return false;
		}

		if (status == Splitted) {
			if (top_child->try_release_data(data_ptr) || bottom_child->try_release_data(data_ptr)) {
				try_merge();
				return true;
			}
		}

		return false;
	}

	Status get_status() {
		return status;
	}

private:
	BuddyBlock* top_child;
	BuddyBlock* bottom_child;
	size_t size;
	Status status;
	int position;
	int* ptr;

	void split_block() {
		status = Splitted;

		top_child = new BuddyBlock(position, size / 2);
		bottom_child = new BuddyBlock(position + size / 2, size / 2);
	}

	void try_merge() {
		if (bottom_child->get_status() == Free && top_child->get_status() == Free)
		{
			int top_pos = top_child->get_position();
			int bot_pos = bottom_child->get_position();

			cout << "Blocks [" << top_pos << ":" << top_pos + size / 2 - 1 << "] and [" <<
				bot_pos << ":" << bot_pos + size / 2 - 1 << "] was merged." << endl;

			free(top_child);
			free(bottom_child);
			status = Free;
		}
	}

	int get_position() {
		return position;
	}

	void allocate(size_t content_size) {
		status = Occuped;
		ptr = (int*)malloc(content_size);

		cout << content_size << " bytes added to block [" << position << ":" << position + size - 1 << "]" << endl;
	}
};

class BuddyAllocator {
public:
	BuddyAllocator(size_t size_) {
		size = size_;
		head = new BuddyBlock(0, size);
	}

	int* allocate(size_t content_size) {
		return head->fill_block(content_size);
	}

	void release(int* ptr) {
		head->try_release_data(ptr);
	}

	~BuddyAllocator() {
		free(head);
	}
private:
	size_t size;
	BuddyBlock* head;
};



int main(int argc, const char * argv[]) {
	BuddyAllocator* allocator = new BuddyAllocator(256);
	int* a = allocator->allocate(128);
	int* b = allocator->allocate(11);
	allocator->release(a);
	int* c = allocator->allocate(33);
	int* d = allocator->allocate(64);
	allocator->release(b);
	allocator->release(d);
	allocator->release(c);

	system("PAUSE");
	return 0;
}
