#include <iostream>

template <class T>
class SmartPointer {
    T* ref;
    size_t* ref_count;
public:
    SmartPointer() {
        ref_count = nullptr;
        ref = nullptr;
    }

    void Set(T* _object) {
        Release();
        ref = _object;
        ref_count = new size_t;
        *ref_count = 1;
    }

    T Get() {
        return *ref;
    }

    void Release() {
        if (!ref)
            return;

        (*ref_count)--;
        if (*ref_count == 0){
            delete ref_count;
            delete ref;
        }

        ref_count = nullptr;
        ref = nullptr;
    }

    ~SmartPointer() {
        if (ref)
            Release();
    }

    T &operator*() {
        return *ref;
    }

    T*operator ->(){
        return ref;
    }

    SmartPointer &operator=(const SmartPointer& other) {
        if (ref)
            Release();
        if (this != &other){
            Release();
            ref = other.ref;
            ref_count = other.ref_count;
        }

        return *this;
    }
};

using namespace std;

int main() {
    auto smartPtr1 = SmartPointer<int>();
    auto smartPtr2 = SmartPointer<int>();
    smartPtr1.Set(new int(45));
    smartPtr2.Set(new int(42));
    cout << "Pointer Values: " << smartPtr1.Get() << ' ' << smartPtr2.Get() << endl;
    smartPtr1 = smartPtr2;
    cout << "Pointer Values: " << smartPtr1.Get() << ' ' << smartPtr2.Get() << endl;
}
