using namespace std;

template<class T>
class SmartPtr{
    unsigned int* ref_count;
    T* obj_ref;

    void Attach(SmartPtr& smartPtr){
        obj_ref = smartPtr.obj_ref;
        ref_count = smartPtr.ref_count;
        (*ref_count)++;
    }

public:
    SmartPtr() {
        ref_count = nullptr;
        obj_ref = nullptr;
    }

    ~SmartPtr() {
        if (obj_ref != nullptr)
            Release();
    }

    void Set(T* ref){
        obj_ref = ref;
        ref_count = new unsigned int[1];
        *ref_count = 1;
    }

    void Release() {
        if (ref_count == nullptr)
            return;
        if (--(*ref_count) == 0) {
            delete ref_count;
            delete obj_ref;
        }
        ref_count = nullptr;
        obj_ref = nullptr;
    }

    T Get() {
        return *obj_ref;
    }

    T *operator->() {
        return obj_ref;
    }

    T &operator*() {
        return *obj_ref;
    }

    SmartPtr &operator=(SmartPtr &otherPtr) {
        if (obj_ref != nullptr)
            Release();
        if (this != &otherPtr)
            Attach(otherPtr);
        return *this;
    }
};