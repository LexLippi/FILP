#include <iostream>
#include <fstream>
#include <map>

using namespace std;

size_t strlen(char* word) {
    auto b = word;
    while (*b != '\0')
        b++;

    return b - word;
}

class MemoryBlock {
public:
    size_t size;
    void* start;
    MemoryBlock* next;
    int count;
    size_t offset;

    MemoryBlock(size_t block_size, void* pointer) {
        this->size = block_size;
        this->next =  NULL;
        this->offset = 0;
        this->count = 0;
        this->start = reinterpret_cast<void*>(reinterpret_cast<char*>(pointer) + sizeof(MemoryBlock));
    }
};

template<class T>
class MyAlloc {
public:
    typedef size_t size_type;
    typedef T* pointer;
    typedef T value_type;

    size_t block_size = 1024 * 1024;
    MemoryBlock* head = NULL;
    MemoryBlock* current = NULL;

    ~MyAlloc() {
        if (head->count == 0) {
            MemoryBlock* block = head;
            while (block != NULL) {
                block = block->next;
            }
        }

        head->count--;
    }

    MyAlloc() {
        allocate(1024);
    }

    pointer allocate(size_type n)
    {
        if (head == NULL) {
            head = newBlock();
            current = head;
        }
        if (current->size - current->offset < n * sizeof(value_type)) {
            current->next = newBlock();
            current = current->next;
        }

        auto ptr = reinterpret_cast<pointer>(reinterpret_cast<char*>(current->start) + current->offset);
        current->offset += n * sizeof(T);

        return ptr;
    };

    void deallocate(pointer p, size_type n)
    {};

private:
    MemoryBlock* newBlock() {
        return new MemoryBlock(block_size, malloc(block_size + sizeof(MemoryBlock)));
    }
};

class MyCompare
{
public:
    bool operator()(char *a, char *b) const {
        while (isalpha(*a)) {
            char low_a = tolower(*a);
            char low_b = tolower(*b);
            if (low_a < low_b) {
                return true;
            }
            if (low_a > low_b) {
                return false;
            }

            a++;
            b++;
        }

        return isalpha(*b);
    }
};

int main() {
    const char* filename = "./book1.txt";

    ifstream file(filename, ifstream::ate | ifstream::binary);

    if (!file) {
        cout << "File wasn't opened!" << endl;
        return -1;
    }

    int file_size = file.tellg();

    char* text = new char[file_size + 1];

    file.seekg(0);
    file.read(text, file_size);
    file.close();

    const char* const_text = text;
    int text_size = strlen(text);
    map<string, size_t> counter;
    string word;

    int start_time = clock();

    for (int i = 0; i < text_size; i++) {
        if (!isalpha((int) const_text[i])) {
            if (word.length() == 0) {
                continue;
            }
            if (counter.count(word) == 0) {
                counter[word] = 1;
            } else {
                counter[word]++;
            }
            word = "";
        } else {
            word += text[i];
        }
    }

    int system_time = clock() - start_time;


    auto new_counter = map<char*, size_t, MyCompare, MyAlloc<char*> >();
    char* new_word;
    int start = 0;

    start_time = clock();

    for (int i = 0; i < text_size; i++) {
        if (!isalpha((int) const_text[i])) {
            if (i == start) {
                start++;
            }
            new_word = new char[i - start + 1];

            for (int j = 0; j < i - start; j++) {
                new_word[j] = const_text[start + j];
            }
            if (strlen(new_word) == 0) {
                continue;
            }
            start = i + 1;
            if (new_counter.count(const_cast<char*>(new_word)) == 0) {
                new_counter[new_word] = 1;
            } else {
                new_counter[new_word]++;
            }
        }
    }

    int my_time =  clock() - start_time;

    auto it = new_counter.begin();
    for (int i = 0; it != new_counter.end(); it++, i++) {
        cout << i << ") word " << it->first << ", count " << it->second << endl;
    }

    auto it2 = counter.begin();
    for (int i = 0; it2 != counter.end(); it2++, i++) {
        cout << i << ") word " << it2->first << ", count " << it2->second << endl;
    }

    cout << endl << "System allocator time: " << system_time << " ms" << endl;

    cout << "Custom allocator time: "  << my_time << " ms" << endl;

    return 0;
}