#include <iostream>
using namespace std;

template <class T>
class SmartPointer {
private:
    T* ptr;
    size_t* ref_count;

public:
    SmartPointer() {
        ptr = NULL;
        ref_count = NULL;
    }

    ~SmartPointer() {
        Release();
    }

    void Release() {
        if (!ptr) {
            return;
        }

        (*ref_count)--;
        if (!ref_count) {
            delete ptr;
            delete ref_count;
        }

        ptr = NULL;
        ref_count = NULL;
    }

    void Set(T* other) {
        ptr = other;
        ref_count = (size_t*)malloc(sizeof(size_t));
        *ref_count = 1;
    }

    T Get() {
        return *ptr;
    }

    T* operator->() {
        return Get();
    }

    T& operator*() {
        return *ptr;
    }

    SmartPointer& operator=(SmartPointer& other) {
        if (*ref_count > 0) {
            Release();
        }
        if (this != &other) {
            ptr = other.ptr;
            ref_count = other.ref_count;
            (*ref_count)++;
        }
        return *this;
    }

    void Print() {
        cout << "object: " << Get() << "; count: " << *ref_count << "\n";
    }
};

int main() {
    auto p1 = SmartPointer<int>();
    p1.Set(new int(1));
    p1.Print();
    auto p2 = SmartPointer<int>();
    p2.Set(new int(2));
    p2.Print();
    p1 = p2;
    p1.Print();
    p2.Release();
    p1.Print();
    p1.Release();
}
