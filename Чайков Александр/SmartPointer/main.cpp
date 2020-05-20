#include<iostream>
#include <queue>
using namespace std;

class RCounter
{
private:
    size_t refs;

public:
    RCounter()
    {
        refs = 0;
    }
    size_t getCount()
    {
        return refs;
    }

    void add()
    {
        refs++;
    }

    void remove()
    {
        refs--;
    }
};

template<class T>
class SmartPointer
{
public:
    SmartPointer()
    {
        rCounter = new RCounter();
    }

    SmartPointer(T* object)
    {
        ptrT = object;
        rCounter = new RCounter();
        rCounter->add();
        cout << "creare smt ptr" << endl;
    }

    SmartPointer(const SmartPointer<T>& other)
    {
        ptrT = other.ptrT;
        rCounter = other.rCounter;
        rCounter->add();
        cout << "creare smt ptr by other" << endl;
    }

    ~SmartPointer()
    {
        remove();
    }

    T& operator*()
    {
        return *ptrT;
    }

    T* operator->()
    {
        return ptrT;
    }

    SmartPointer<T>& operator=(SmartPointer<T>& other)
 {
        cout << "=" << endl;
        if (this != &other)
        {
            this->remove();
            ptrT = other.ptrT;
            rCounter = other.rCounter;
            rCounter->add();
        }
        return *this;
    }

    void release()
    {
        remove();
        ptrT = nullptr;
        rCounter = new RCounter();
    }

    T get()
    {
        return *ptrT;
    }

    void set(T* new_ptr)
    {
        if (ptrT == nullptr)
            ptrT = new_ptr;
        else
            *ptrT = *new_ptr;
    }

private:
    T* ptrT;
    RCounter* rCounter;

    void remove()
    {
        rCounter->remove();
        if (rCounter->getCount() == 0)
        {
            delete ptrT;
            delete rCounter;
        }
    }
};


int main()
{
    int* int1 = new int(1);
    int* int2 = new int(2);
    SmartPointer<int> ptr1(int1);
    SmartPointer<int> ptr2 = ptr1;
    ptr2 = ptr1;
    ptr1.set(int2);
    ptr1.get();
    ptr1.release();
    *ptr1;
    return 0;
}