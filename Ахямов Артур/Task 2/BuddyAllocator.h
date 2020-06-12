#pragma once
#include <map>
#include <math.h>
extern const size_t MAX_BLOCK_SIZE;
extern const size_t MIN_BLOCK_SIZE;

enum class State
{
	Splitted, Free, Occupied
};

template<typename T>
class MemoryBlock
{
public:
	State State = State::Free;
	MemoryBlock<T>* Left = nullptr;
	MemoryBlock<T>* Right = nullptr;
	MemoryBlock<T>* Parent = nullptr;
	size_t size;
	void* buffer;

	MemoryBlock<T>(size_t size, void* buffer) :
		size(size), buffer(buffer)
	{}

	MemoryBlock<T>() = default;
};

template<typename T>
class BuddyAllocator
{
public:
	typedef T* pointer;
	typedef T value_type;

	MemoryBlock<T>* mainBlock;

	BuddyAllocator<T>()
	{
		mainBlock = new MemoryBlock<T>(MAX_BLOCK_SIZE, malloc(MAX_BLOCK_SIZE));
		pointersAndTheirBlocks = new std::map<void*, MemoryBlock<T>*>();
	}

	~BuddyAllocator<T>()
	{
		DeleteBlocksStartingWith(mainBlock);
		delete pointersAndTheirBlocks;
	}

	BuddyAllocator<T>(unsigned int degree)
	{
		if (pow(2, degree) > MAX_BLOCK_SIZE)
			throw new std::exception("Too much memory requested");
		if (pow(2, degree) < MIN_BLOCK_SIZE)
			throw new std::exception("Too litle memory requested");
		mainBlock = new MemoryBlock<T>(pow(2, degree), malloc(pow(2, degree)));
		pointersAndTheirBlocks = new std::map<void*, MemoryBlock<T>*>();
	}

	pointer allocate(size_t number)
	{
		MemoryBlock<T>* memorySearchResult = FindFreeMemoryBlock(mainBlock, number * sizeof(T));
		if (!memorySearchResult)
			return nullptr;
		memorySearchResult->State = State::Occupied;
		pointersAndTheirBlocks->emplace(memorySearchResult->buffer, memorySearchResult);
		return (T*)memorySearchResult->buffer;
	}

	void deallocate(T* p, size_t size = 1)
	{
		if ((void*)p == mainBlock->buffer && mainBlock->State == State::Occupied)
			mainBlock->State = State::Free;
		else if (pointersAndTheirBlocks->find((void*)p) != pointersAndTheirBlocks->end())
		{
			MemoryBlock<T>* block = pointersAndTheirBlocks->at((void*)p);
			block->State = State::Free;
			TryToMergeBlockStartingWith(block);
		}
	}

private:
	std::map<void*, MemoryBlock<T>*>* pointersAndTheirBlocks;

	void Split(MemoryBlock<T>* block)
	{
		block->Right = new MemoryBlock<T>(block->size / 2, block->buffer);
		block->Left = new MemoryBlock<T>(
			block->size / 2, (char*)block->buffer + block->size / 2);
		block->Right->Parent = block;
		block->Left->Parent = block;
		block->State = State::Splitted;
	}

	MemoryBlock<T>* FindFreeMemoryBlock(MemoryBlock<T>* block, size_t sizeInBytesRequired)
	{
		// —лишком большой запрос или маленький или места нет
		if (sizeInBytesRequired > block->size || sizeInBytesRequired < MIN_BLOCK_SIZE || block->State == State::Occupied)
			return nullptr;
		// «апрос > половиныЅлока и блокЌе«ан€т
		if (sizeInBytesRequired > block->size / 2 && block->State == State::Free) 
			return block;
		if (block->State == State::Free)
			Split(block);
		MemoryBlock<T>* rightSearchingResult = FindFreeMemoryBlock(block->Right, sizeInBytesRequired);
		MemoryBlock<T>* leftSearchingResult;
		if (!rightSearchingResult)
			leftSearchingResult = FindFreeMemoryBlock(block->Left, sizeInBytesRequired);
		else
			return rightSearchingResult;
		return leftSearchingResult;
	}

	void TryToMergeBlockStartingWith(MemoryBlock<T>* block)
	{
		MemoryBlock<T>* parent = block->Parent;
		if (!parent)
			return;
		if (parent->Left->State == State::Free && parent->Right->State == State::Free)
		{
			delete parent->Left, parent->Right;
			parent->Left = nullptr;
			parent->Right = nullptr;
			parent->State = State::Free;
			TryToMergeBlockStartingWith(parent);
		}
	}

	void DeleteBlocksStartingWith(MemoryBlock<T>* block)
	{
		if (block->Right != nullptr)
			DeleteBlocksStartingWith(block->Right);
		else if(block->Left != nullptr)
			DeleteBlocksStartingWith(block->Left);
		delete block;
		
	}
};