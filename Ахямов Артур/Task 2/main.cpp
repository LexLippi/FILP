#include <iostream>
#include "BuddyAllocator.h"
using namespace std;

const size_t MAX_BLOCK_SIZE = 4 * 1024 * 1024;
const size_t MIN_BLOCK_SIZE = 8;

struct A
{
	char z, u;
	double x;
	int y;
};


int main()
{
	cout << sizeof(A);
	auto allocator = new BuddyAllocator<A>(6);
	A* a = allocator->allocate(1); // not null
	A* b = allocator->allocate(2); // null
	return 0;
}