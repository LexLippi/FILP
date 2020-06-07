#include <iostream>
#include "block.h"

using namespace std;

class BuddyAllocator {
private:
    size_t size;
    Block* head;
public:
    BuddyAllocator(size_t size) {
        this->size = size;
        head = new Block(0, size);
    }

    int* allocate(size_t contentSize) {
        return head->fillBlock(contentSize);
    }

    void release(int* ptr) {
        head->tryReleaseData(ptr);
    }

    ~BuddyAllocator() {
        free(head);
    }
};