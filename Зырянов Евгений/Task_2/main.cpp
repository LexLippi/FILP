#include "allocator.h"

int main() {
    BuddyAllocator* allocator = new BuddyAllocator(1024);
    int* a = allocator->allocate(512);
    int* b = allocator->allocate(44);
    allocator->release(a);
    int* c = allocator->allocate(132);
    int* d = allocator->allocate(256);
    allocator->release(b);
    allocator->release(d);
    allocator->release(c);
}
