using namespace std;

struct Buffer {
    size_t size = 1024 * 1024;
    size_t ptr;
    char *buffer = nullptr;
    Buffer *next;

    Buffer() {
        buffer = new char[size];
        ptr = 0;
        next = nullptr;
    }

    ~Buffer() {
        if (buffer != nullptr)
            delete[] buffer;
    }
};

struct BufferList {
    Buffer* current;
    Buffer* head;

    BufferList() {
        current = new Buffer();
        head = current;
    }
    ~BufferList() {
        while (head != NULL) {
            Buffer* oldBuffer = head;
            head = head->next;
            delete oldBuffer;
        }
    }

    void* allocate(size_t number, size_t size) {
        if (current->ptr + size * number > current->size) {
            addNew();
        }
        void* newPtr = current->buffer + current->ptr;
        current->ptr += size * number;
        return newPtr;
    }

private:
    void addNew() {
        Buffer *newBuffer = new Buffer();
        current->next = newBuffer;
        current = newBuffer;
    }
};