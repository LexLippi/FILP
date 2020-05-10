#include <math.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


struct CItem {
	char Level;
	bool Free;
	union {
		struct {
			CItem* Next;
			CItem* Prev;
		};
		char Data[1];
	};
};


struct buddyAlloc {
	char* memory;
	CItem* freeBlocks[24];

	buddyAlloc() {
		memory = (char*)malloc(pow(2, 24));
		CItem* block = reinterpret_cast<CItem*>(memory);
		block->Level = 23;
		freeBlocks[23] = block;
	}
	~buddyAlloc() { 
		free(memory);
	}

	void* allocate(size_t n) {
		for (int i = 3; i < 24; i++) {
			if (pow(2, i) - 8 < n)
				continue;
			if (freeBlocks[i] == freeBlocks[0])
				continue;
			CItem* free = freeBlocks[i];
			if(free->Next != freeBlocks[0])
				free->Next->Prev = freeBlocks[0];
			freeBlocks[i] = free->Next;
			free->Free = false;
			while (pow(2, i - 1) - 8 > n) {
				size_t delta = pow(2, i - 1);
				CItem* item = reinterpret_cast<CItem*>(reinterpret_cast<char*>(free) + delta);
				if (freeBlocks[i -1] != freeBlocks[0])
					freeBlocks[i - 1]->Prev = item;
				item->Next = freeBlocks[i - 1];
				item->Level = i - 1;
				freeBlocks[i - 1] = item;
				free->Level--;
				i--;
			}
			return reinterpret_cast<void*>(reinterpret_cast<char*>(free) + 8);
		}
	}

	void deallocate(void* p) {
		CItem* item = reinterpret_cast<CItem*>(reinterpret_cast<char*>(p) - 8);
		while (item->Level < 23) {
			int size = pow(2, item->Level);
			int number = (reinterpret_cast<char*>(item) - memory) / size;
			if(number % 2 == 0){
				CItem* neighbour = reinterpret_cast<CItem*>(reinterpret_cast<char*>(item) + size);
				if (neighbour->Free &&  neighbour->Level == item->Level) {
					if (neighbour->Prev != freeBlocks[0])
						neighbour->Prev->Next = neighbour->Next;
					else
						freeBlocks[neighbour->Level] = neighbour->Next;
					if (neighbour->Next != freeBlocks[0])
						neighbour->Next->Prev = neighbour->Prev;
					item->Level++;
				}
				else
					break;
			}
			else {
				CItem* neighbour = reinterpret_cast<CItem*>(reinterpret_cast<char*>(item) - size);
				if (neighbour->Free &&  neighbour->Level == item->Level) {
					if (neighbour->Prev != freeBlocks[0])
						neighbour->Prev->Next = neighbour->Next;
					else
						freeBlocks[neighbour->Level] = neighbour->Next;
					if (neighbour->Next != freeBlocks[0])
						neighbour->Next->Prev = neighbour->Prev;
					neighbour->Level++;
					item = neighbour;
				}
				else
					break;
			}
		}
		if (freeBlocks[item->Level] != freeBlocks[0]) {
			freeBlocks[item->Level]->Prev = item;
		}
		item->Free = true;
		item->Next = freeBlocks[item->Level];
		freeBlocks[item->Level] = item;
	}

	string dump() {
		string result = "";
		for (int i = 23; i > 0; i--)
		{
			cout << "Level " << i;
			if (freeBlocks[i] != freeBlocks[0]) {
				auto free = freeBlocks[i];
				cout << " " << free;
				while (free->Next != freeBlocks[0]) {
					cout << " " << free;
					free = free->Next;
				}
			}
			cout << "\n";
		}
		cout << "\n";
		return result;
	}
};


int main(int argc, char *argv[]) {
	buddyAlloc* allocator = new buddyAlloc();
	auto a = allocator->allocate(28);
	auto b = allocator->allocate(56);
	auto c = allocator->allocate(15);
	allocator->dump();
	allocator->deallocate(b);
	allocator->deallocate(c);
	allocator->dump();
	auto d = allocator->allocate(15);
	allocator->deallocate(a);
	allocator->dump();
	allocator->deallocate(d);
	allocator->dump();
}