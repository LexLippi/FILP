#include <iostream>
#include <map>
#include <fstream>
#include <cstring>
#include <Windows.h>
#include <time.h>
#include <vector>
using namespace std;

const int SIZE_BUF = 1024 * 1024;

template <class T>
struct Allocator
{
	typedef T value_type;
	vector<void*> buffers;
	size_t size = 0;
	T* ptr = NULL;

	Allocator& operator=(const Allocator& x) = default;

	T* allocate(size_t n) {
		if (size < n){
			ptr = static_cast<T*>(malloc(SIZE_BUF));
			buffers.push_back(ptr);
			size = SIZE_BUF;
		}
		size -= n * sizeof(T);
		return ptr++;
	}

	void deallocate(T* p, size_t n) {}
};

struct Comparator {
	bool operator()(char* a, char* b) const {
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


int main()
{
	HANDLE file = CreateFile(L"../book.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		cout << "file not found" << "\n";
	}
	LARGE_INTEGER file_size;
	GetFileSizeEx(file, &file_size);
	char* text = new char[file_size.QuadPart + 1];
	ReadFile(file, text, file_size.QuadPart, NULL, NULL);
	text[file_size.QuadPart] = '\0';
	CloseHandle(file);
	LONGLONG size_f = file_size.QuadPart;
	cout << "file size: " << size_f << "\n";

	clock_t start;
	start = clock();
	map<string, size_t> counter;

	bool isFirst = true;
	char* token = NULL;
	while (isFirst or token != NULL)
	{
		token = strtok(NULL, " ,.:;!?\r\n\t");
		counter[token]++;
		isFirst = false;
	}
	cout << "time simple allocator: " << clock() - start << "second" << "\n";
	vector<pair<string, size_t>> result;
	for (auto element : counter)
	{
		result.push_back(element);
	}

	int size = result.size();
	for (auto i = size - 1; i > size; i--)
	{
		cout << result[i].first << " -- " << result[i].second << "\n";
	}

	clock_t start;
	start = clock();
	vector<pair<char*, size_t>> result2;
	map<char*, size_t, Comparator, Allocator<pair<char*, size_t> >> counter2;

	bool isFirst = true;
	char* token = NULL;
	while (isFirst or token != NULL)
	{
		token = strtok(NULL, " ,.:;!?\r\n\t");
		counter2[token]++;
		isFirst = false;
	}
	cout << "time my allocator: " << clock() - start << "second" << "\n";

	for (auto element : counter2)
	{
		result2.push_back(element);
	}

	int size2 = result2.size();
	for (auto i = size2 - 1; i > size2; i--)
	{
		cout << result2[i].first << " -- " << result2[i].second << "\n";
	}
}
