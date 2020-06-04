#include "pch.h"
#include <cstdint>
#include <iostream>

using namespace std;

class BuddyAllocator {
public:
	static const uint8_t maxLevel = 10;
	static const uint8_t minLevel = 4;
	static const int size = 1 << maxLevel;
	uintptr_t memoryBase;
	typedef void * ptr_t;

	typedef struct Container {
		uint8_t memory[size];
		ptr_t freePtrs[maxLevel + 2];
	};

	typedef struct Info {
		uint8_t level;
		Info(uint8_t level) : level(level) {}
	};

	BuddyAllocator() {
		container = new Container();
		container->freePtrs[maxLevel] = container->memory;
		memoryBase = (uintptr_t)container->memory;
	}

	~BuddyAllocator() {
		free(container);
	}

	ptr_t alloc(int bytes) {
		cout << "Allocate " << bytes << " bytes" << endl;
		int i = 0;
		while (blockSize(i) < bytes + sizeof(Info))
			i++;
		if (i < minLevel)
			i = minLevel;
		int order = i;
		int level = findFreeLevel(i);
		ptr_t block = container->freePtrs[level];
		container->freePtrs[level] = nullptr;
		cout << "Block " << block << endl;
		while (level-- > order) {
			ptr_t buddy = getBuddy(block, level);
			container->freePtrs[level] = buddy;
			cout << "Level " << level << " buddy:" << buddy << endl;
		}
		auto info = Info(order);
		setInfo(block, info);
		return block;
	}

	void free(ptr_t block) {
		cout << "Delete " << block << endl;
		Info info = getInfo(block);
		for (int i = info.level; ; i++) {
			ptr_t buddy = getBuddy(block, i);
			ptr_t *list = toFreeList(buddy, i);
			if (list == nullptr) {
				cout << "Container for " << block << " is occupied" << endl;
				*((ptr_t *)block) = container->freePtrs[i];
				container->freePtrs[i] = block;
				return;
			}
			else {
				cout << "Container found " << block << " and " << buddy << " merged" << endl;
				if (block >= buddy)
					block = buddy;
				*list = *((ptr_t *)*list);
			}
		}
	}

	void toString() {
		cout << "Space  " << getFreeSpace() << "/" << size << endl;
	}

private:
	Container *container = nullptr;
	ptr_t getBuddy(ptr_t block, int i) {
		uintptr_t offset = (uintptr_t)block - memoryBase;
		uintptr_t buddy = offset ^ blockSize(i);
		return (ptr_t)(buddy + memoryBase);
	}

	uintptr_t blockSize(int i) {
		return 1 << (i);
	}

	void setInfo(ptr_t &block, Info info) {
		*(((Info *)block) - sizeof(Info)) = info;
	}

	Info getInfo(ptr_t &block) {
		return *(((Info *)block) - sizeof(Info));
	}

	int getFreeSpace() {
		int bytes = 0;
		for (int i = 0; i <= maxLevel; i++) {
			int count = 0;
			ptr_t *p = &(container->freePtrs[i]);
			while (*p != NULL) {
				count++;
				p = (ptr_t *)*p;
			}
			bytes += count * blockSize(i);
		}
		return bytes;
	}

	ptr_t *toFreeList(ptr_t buddy, int level) {
		ptr_t *list = &(container->freePtrs[level]);
		while (true) {
			if (*list == buddy)
				return list;
			if (*list == nullptr)
				return nullptr;
			list = (ptr_t *)*list;
		}
	}

	int findFreeLevel(int min) {
		for (int i = min; i <= maxLevel; i++) {
			if (container->freePtrs[i])
				return i;
		}
		return NULL;
	}
};

int main() {
	auto *buddy = new BuddyAllocator();
	buddy->toString();
	void *p1, *p2, *p3;
	p1 = buddy->alloc(1);
	p2 = buddy->alloc(1);
	p3 = buddy->alloc(1);
	buddy->free(p1);
	buddy->free(p3);
	buddy->toString();
	buddy->free(p2);
	buddy->toString();
	p1 = buddy->alloc(255);
	p2 = buddy->alloc(255);
	p3 = buddy->alloc(255);
	buddy->free(p3);
	buddy->free(p1);
	buddy->toString();
	buddy->free(p2);
	buddy->toString();
	return 0;
}