#include "iostream"
using namespace std;

template <typename T>
class SmartPointer {
private:
    T *ptr = nullptr;
    unsigned int *ref_count = nullptr;

public:
    T Get() {
        return *ptr;
    }

    void Set(T *ptr) {
        Release();
        this->ptr = ptr;
        ref_count = new unsigned int(1);
    }

    void Release() {
        if (!ref_count)
            return;
        (*ref_count)--;
        if (*ref_count == 0) {
            delete ptr;
            delete ref_count;
            ptr = nullptr;
            ref_count = nullptr;
        }
    }

    ~SmartPointer() {
        Release();
    }

    SmartPointer& operator=(SmartPointer& other) {
        Release();
        if (this != &other) {
            ptr = other.ptr;
            ref_count = other.ref_count;
            (*ref_count)++;
        }
        return *this;
    }

    T* operator->() {
        return ptr;
    }

    T& operator*() {
        return *ptr;
    }
};

int main() {
    auto ptr1 = SmartPointer<int>();
    auto ptr2 = SmartPointer<int>();
    ptr1.Set(new int(5));
    ptr2.Set(new int(7));
    cout << "Ptr1: " << ptr1.Get() << endl << "Ptr2: " << ptr2.Get() << endl;
    ptr1 = ptr2;
    cout << "Ptr1: " << ptr1.Get() << endl << "Ptr2: " << ptr2.Get() << endl;
}