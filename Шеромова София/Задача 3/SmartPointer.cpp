#include <iostream>

using namespace std;

template<typename T>
class SmartPointer {
    T *value = nullptr;
    size_t *count = nullptr;

public:
    explicit SmartPointer(T *value) : value(value), count(new size_t(1)) {
    }

    SmartPointer() = default;

    ~SmartPointer<T>() {
        Release();
    }

    size_t GetCount() {
        return *count;
    }

    T *Get() {
        return value;
    }

    void Set(T *new_value) {
        Release();
        value = new_value;
        count = new size_t(1);
    }

    void Release() {
        if (value == nullptr) {
            return;
        }

        (*count)--;
        if (*count == 0) {
            delete value;
            delete count;
        }

        value = nullptr;
        count = nullptr;
    }

    SmartPointer &operator=(const SmartPointer<T> &other) {
        if (this == &other) {
            return *this;
        }

        Release();
        value = other.value;
        count = other.count;
        (*count)++;

        return *this;
    }

    T *operator->() {
        return value;
    }

    T &operator*() {
        return *value;
    }
};

int main() {
    cout << "Initialization ptr1 with a value." << endl;
    SmartPointer<int> ptr1(new int(100));
    cout << "ptr1 value: " << *ptr1 << endl;
    cout << "ptr1 count: " << ptr1.GetCount() << endl;
    cout << "Set new value to ptr1." << endl;
    ptr1.Set(new int(200));
    cout << "ptr1 new value: " << *ptr1 << endl;
    cout << "ptr1 count: " << ptr1.GetCount() << endl << endl;

    cout << "Initialization ptr2 without a value." << endl;
    SmartPointer<int> ptr2;
    cout << "Assignment ptr1 to ptr2." << endl;
    ptr2 = ptr1;
    cout << "ptr2 value: " << *ptr2 << endl;
    cout << "ptr2 count: " << ptr2.GetCount() << endl;
    cout << "ptr1 count: " << ptr1.GetCount() << endl << endl;

    cout << "Release ptr1." << endl;
    ptr1.Release();
    cout << "ptr2 value: " << *ptr2 << endl;
    cout << "ptr2 count: " << ptr2.GetCount() << endl;
    cout << "Release ptr2." << endl;
    ptr2.Release();
}
