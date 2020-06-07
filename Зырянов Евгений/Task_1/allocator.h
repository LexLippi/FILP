#include "buffer.h"

template <class T>
struct Allocator {
    typedef T value_type;
    typedef T* pointer;

    BufferList* buffers;
    Allocator() {
        buffers = new BufferList();
    }

    pointer allocate(size_t n) {
        return reinterpret_cast<pointer>(buffers->allocate(n, sizeof(value_type)));
    }

    void deallocate(pointer p, size_t n) { }

    ~Allocator() {
        delete buffers;
    }
};