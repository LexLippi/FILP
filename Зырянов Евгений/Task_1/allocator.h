template <class T>
struct Allocator {
    typedef T value_type;
    typedef T* pointer;
    typedef size_t size_type;

private:
    void* ptr;
    size_t position;

public:
    Allocator() {
        ptr = malloc(1024 * 1024);
        position = 0;
    }

    pointer allocate(size_t size) {
        pointer result = ((pointer)ptr) + position;
        position += size;

        return result;
    }

    void deallocate(pointer p, size_t size) {
        position -= size;
    }

    ~Allocator() {
        free(ptr);
    }
};