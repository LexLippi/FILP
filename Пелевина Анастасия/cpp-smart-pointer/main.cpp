#include <iostream>

using namespace std;


template <typename T>
class SmartPointer {
private:
    T *obj_ptr = nullptr;
    int *ref_counter = nullptr;
public:
    ~SmartPointer() {
        if (obj_ptr != nullptr) {
            Release();
        }
    }

    T Get() {
        return *obj_ptr;
    }

    void Release() {
        if (obj_ptr == nullptr) {
            return;
        }

        (*ref_counter)--;

        if (*ref_counter == 0) {
            delete obj_ptr;
            obj_ptr = nullptr;
            ref_counter = nullptr;
        }
    }

    void Set(T *new_obj) {
        Release();

        obj_ptr = new_obj;
        ref_counter = new int(1);
    }

    SmartPointer& operator=(SmartPointer& other) {
        if (this == &other) {
            return *this;
        }

        if (obj_ptr) {
            Release();
        }

        obj_ptr = other.obj_ptr;
        ref_counter = other.ref_counter;

        if (ref_counter) {
            (*ref_counter)++;
        }

        return *this;
    }

    T* operator->() {
        return obj_ptr;
    }

    T& operator*() {
        return *obj_ptr;
    }
};


int main() {
    auto integerSmartPointer = SmartPointer<int>();
    integerSmartPointer.Set(new int(3));
    cout << integerSmartPointer.Get() << endl;
    integerSmartPointer.Release();
    integerSmartPointer.Set(new int(15));
    cout << integerSmartPointer.Get() << endl;
    integerSmartPointer.Release();
    integerSmartPointer.Set(new int(97));
    auto newIntegerSmartPointer = SmartPointer<int>();
    newIntegerSmartPointer = integerSmartPointer;
    cout << *newIntegerSmartPointer << endl;
    integerSmartPointer.Release();
    auto test3 = SmartPointer<int>();
    test3.Set(new int(7));
    cout << test3.Get() << endl;
    test3 = integerSmartPointer;
    cout << test3.Get() << endl;
    test3.Release();

    return 0;
}
