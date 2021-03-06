#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <tchar.h>
#include <map>
#include <ctime>
#include <vector>
#include <cmath>

using namespace std;

struct Block {
	int Level;
	bool Free;
	Block* Prev;
	Block* Next;
};

class BuddyAllocator {
public:
	Block** firstBlocks;
	char maxLevel;
	void* bufferPtr;

	BuddyAllocator(char dimension)
	{
		maxLevel = dimension - 1;
		auto commonSize = pow(2, dimension);
		bufferPtr = malloc(commonSize);
		firstBlocks = new Block*[dimension];
		for (auto level = 0; level <= maxLevel; level++) {
			firstBlocks[level] = nullptr;
		}
		auto firstBlock = reinterpret_cast<Block*> (bufferPtr);
		firstBlock->Level = maxLevel;
		firstBlock->Free = true;
		firstBlock->Prev = nullptr;
		firstBlock->Next = nullptr;
		firstBlocks[maxLevel] = firstBlock;
	};

	~BuddyAllocator() {
		free(bufferPtr);
	};

	void* allocate(size_t size) 
	{
		cout << "Allocate memory: " << size << "\n";
		for (auto level = 0; level <= maxLevel; level++) {
			if (firstBlocks[level] == nullptr)
				continue;
			if (pow(2, level + 1) - sizeof(Block) < size)
				continue;
			auto block = firstBlocks[level];
			firstBlocks[level] = block->Next;
			block->Next = nullptr;
			if (firstBlocks[level] != nullptr) 
				firstBlocks[level]->Prev = nullptr;
			split(block, size);
			block->Free = false;
			auto ptr = reinterpret_cast<void*>(reinterpret_cast<char*>(block) + sizeof(Block));
			cout << "Return pointer: " << ptr << "\n";
			return ptr;
		}
	};

	void split(Block* block, size_t neededSize) {
		cout << "Split block with level: " << block->Level << " Needed memory: " << neededSize << "\n";
		while (pow(2, block->Level) - sizeof(Block) >= neededSize) {
			block->Level--;
			cout << "Get block with level: " << block->Level << "\n";
			size_t size = pow(2, block->Level + 1);
			auto secondBlock = reinterpret_cast<Block*>(reinterpret_cast<char*>(block) + size);
			secondBlock->Free = true;
			secondBlock->Level = block->Level;
			if (firstBlocks[block->Level] != nullptr) {
				firstBlocks[block->Level]->Prev = secondBlock;
			}
			secondBlock->Next = firstBlocks[block->Level];
			secondBlock->Prev = nullptr;
			firstBlocks[block->Level] = secondBlock;
		}
	};

	void merge(Block* first, Block* second) {
		if (second->Prev == nullptr) {
			firstBlocks[second->Level] = second->Next;
		}
		else {
			second->Prev->Next = second->Next;
		}
		if (second->Next != nullptr) {
			second->Next->Prev = second->Prev;
		}
		cout << "Merge blocks with level: " << first->Level << "\n";
		first->Level++;
		second->Level++;
	};

	void deallocate(void* ptr) 
	{ 
		cout << "Deallocate memory with ptr: " << ptr << "\n";
		auto block = reinterpret_cast<Block*>(reinterpret_cast<char*>(ptr) - sizeof(Block));
		block->Free = true;
		while (block->Level != maxLevel) {
			size_t size = pow(2, block->Level + 1);
			int blockNumber = (reinterpret_cast<char*>(bufferPtr) - reinterpret_cast<char*>(block)) / size;
			Block* otherBlock;
			if (blockNumber % 2 == 0) {
				cout << "It is left block" << "\n";
				otherBlock = reinterpret_cast<Block*>(reinterpret_cast<char*>(block) + size);
			}
			else {
				cout << "It is right block" << "\n";
				otherBlock = reinterpret_cast<Block*>(reinterpret_cast<char*>(block) - size);
			}
			if (otherBlock->Level == block->Level && otherBlock->Free) {
				merge(block, otherBlock);
				if (blockNumber % 2 == 1) {
					block = otherBlock;
				}
			}
			else
				break;
		}
		cout << "Get block with level : " << block->Level << "\n";
		if (firstBlocks[block->Level] != nullptr) {
			firstBlocks[block->Level]->Prev = block;
		}
		block->Next = firstBlocks[block->Level];
		block->Prev = nullptr;
		firstBlocks[block->Level] = block;
	};
};


int main(int argc, char* argv[])
{
	cout << "Size of block: " << sizeof(Block) << "\n";
	auto allocator = BuddyAllocator(10);
	auto a = allocator.allocate(34);
	allocator.deallocate(a);
	auto b = allocator.allocate(100);
	auto c = allocator.allocate(20);
	allocator.deallocate(b);
	allocator.deallocate(c);
	cout << "Finish";
}