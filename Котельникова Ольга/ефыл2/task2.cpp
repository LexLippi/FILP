#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

struct Block 
{
	Block* Previous;
	Block* Next;
	int Level;
	bool Free;
	
};

class UserBuddyAllocator 
{
public:
	Block** startBlock;
	char highLevel;
	void* bufferPtr;

	~UserBuddyAllocator()
	{
		free(bufferPtr);
	};
	
	UserBuddyAllocator(char levels)
	{
		startBlock = new Block*[levels];
		highLevel = levels - 1;
		size_t size = pow(2, levels);
		bufferPtr = malloc(size);
		
		for (size_t i = 0; i <= highLevel; i++) {
			startBlock[i] = nullptr;
		}

		auto headBlock = reinterpret_cast<Block*> (bufferPtr);
		headBlock->Level = highLevel;
		headBlock->Free = true;
		headBlock->Previous = nullptr;
		headBlock->Next = nullptr;
		startBlock[highLevel] = headBlock;

	};




	void* allocate(size_t size)
	{
		cout << "Get memory: " << size << "\n";
		for (size_t i = 0; i <= highLevel; i++) {
			if (startBlock[i] == nullptr) || (pow(2, i + 1) - sizeof(Block) < size)
				continue;

			auto block = startBlock[i];
			startBlock[i] = block->Next;
			block->Next = nullptr;
			
			if (startBlock[i] != nullptr)
				startBlock[i]->Previous = nullptr;
			split(block, size);
			
			block->Free = false;
			auto ptr = reinterpret_cast<void*>(sizeof(Block)+reinterpret_cast<char*>(block));
			cout << "Pointer: " << ptr << "\n";
			return ptr;
		}
	};

	void mergeBlocks(Block* current, Block* next) {
		if (next->Previous == nullptr)
			startBlock[next->Level] = next->Next;

		else
			next->Previous->Next = next->Next;

		if (next->Next != nullptr)
			next->Next->Previous = next->Previous;

		cout << "Merge blocks with level: " << current->Level << "\n";

		current->Level++;
		next->Level++;
	};

	void split(Block* block, size_t newSize) 
	{
		cout << "Split block with level: " << block->Level << " Memory: " << newSize << "\n";
		
		while (pow(2, block->Level) - sizeof(Block) >= newSize) 
		{
			block->Level--;
			cout << "Block from level: " << block->Level << "\n";
			size_t size = pow(2, block->Level + 1);
			
			auto nextBlock = reinterpret_cast<Block*>(reinterpret_cast<char*>(block) + size);
			nextBlock->Free = true;
			nextBlock->Level = block->Level;
			
			if (startBlock[block->Level] != nullptr) 
				startBlock[block->Level]->Previous = nextBlock;
			
			nextBlock->Next = startBlock[block->Level];
			nextBlock->Previous = nullptr;
			startBlock[block->Level] = nextBlock;
		}
	};

	void freeMemory(void* ptr)
	{
		cout << "Free memory, ptr = " << ptr << "\n";
		auto block = reinterpret_cast<Block*>(reinterpret_cast<char*>(ptr) - sizeof(Block));
		block->Free = true;
		
		while (block->Level != highLevel) 
		{
			size_t size = pow(2, block->Level + 1);
			size_t blockNumber = (reinterpret_cast<char*>(bufferPtr) - reinterpret_cast<char*>(block)) / size;
			Block* otherBlock;
			if (blockNumber % 2 == 0) 
			{
				cout << "Left block" << "\n";
				otherBlock = reinterpret_cast<Block*>(reinterpret_cast<char*>(block) + size);
			}
			else 
			{
				cout << "Right block" << "\n";
				otherBlock = reinterpret_cast<Block*>(reinterpret_cast<char*>(block) - size);
			}
			if (otherBlock->Level == block->Level && otherBlock->Free) 
			{
				merge(block, otherBlock);
				if (blockNumber % 2 == 1) 
					block = otherBlock;
				
			}
			else
				break;
		}
		cout << "Get block, level : " << block->Level << "\n";
		if (startBlock[block->Level] != nullptr) 
			startBlock[block->Level]->Previous = block;
		
		
		block->Next = startBlock[block->Level];
		block->Previous = nullptr;
		startBlock[block->Level] = block;
	};
};

int main()
{
	
	auto allocator = UserBuddyAllocator(20);
	auto x = allocator.allocate(1);
	auto y = allocator.allocate(5);
	auto z = allocator.allocate(10);
	cout << "Start \n" << "Size of block: " << sizeof(Block) << "\n";
	allocator.freeMemory(x);
	allocator.freeMemory(y);
	allocator.freeMemory(z);
	cout << "Done";
}
