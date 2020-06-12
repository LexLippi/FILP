#include <iostream>
#include <unordered_map>
#include <map>
#include <Windows.h>
#include <string>
#include <ctime>



using namespace std;

unordered_map<int, int>* copiesStat = new unordered_map<int, int>();

template<class T>
class CustomAllocator
{
public:

	using value_type = T;
	using pointer = T*;

	std::vector<void*>* buffers;
	void* currentBuffer;
	int bufferSize;
	size_t bufferPosition;
	int id;

	CustomAllocator() noexcept
	{
		bufferSize = 1048576; //1024*1024
		buffers = new std::vector<void*>();
		allocateNewBuffer();
		id = 0;
	}

	~CustomAllocator()
	{
		// Чтобы избежать очистки буфера у копий аллокаторов
		auto currentIsCopy = copiesStat->find(id - 1) != copiesStat->end();
		if (!currentIsCopy || currentIsCopy && (*copiesStat)[id - 1] == 0)
			for (int i = 0; i < buffers->size(); ++i)
				delete buffers->at(i);
		else if (currentIsCopy)
			(*copiesStat)[id - 1]--;
		delete buffers;
	}

	template<typename T1>
	CustomAllocator(const CustomAllocator<T1>& other) throw()
	{
		buffers = new std::vector<void*>();
		bufferSize = other.bufferSize;
		for (int i = 0; i < other.buffers->size(); ++i)
		{
			auto buf = malloc(bufferSize);
			std::memcpy(buf, other.buffers->at(i), bufferSize);
			buffers->push_back(buf);
		}
		currentBuffer = buffers->back();
		bufferPosition = other.bufferPosition;
		id = other.id + 1;
		if (copiesStat->find(other.id) != copiesStat->end())
			(*copiesStat)[other.id]++;
		else
			(*copiesStat)[other.id] = 1;
	}

	pointer allocate(std::size_t size)
	{
		if (bufferPosition + size * sizeof(T) >= bufferSize)
			allocateNewBuffer();
		T* pointer = (T*)((char*)currentBuffer + bufferPosition);
		bufferPosition += size * sizeof(T);
		return pointer;
	}

	void deallocate(void* p, std::size_t size)
	{
		
	}

private:

	void allocateNewBuffer()
	{
		currentBuffer = malloc(bufferSize);
		buffers->push_back(currentBuffer);
		bufferPosition = 0;
	}
};


class WordSelectionResult
{
public:
	WordSelectionResult(unsigned int startIndex = -1, unsigned int endIndex = -1)
	{
		this->startIndex = startIndex;
		this->endIndex = endIndex;
	}
	unsigned int startIndex;
	unsigned int endIndex;

	void Reset()
	{
		startIndex = -1;
		endIndex = -1;
	}

	void Set(unsigned int startIndex, unsigned int endIndex)
	{
		this->startIndex = startIndex;
		this->endIndex = endIndex;
	}

	bool Success()
	{
		return startIndex < endIndex;
	}
};

struct MyEqualTo : public binary_function<char*, char*, bool>
{
    bool operator()(const char* first, const char* second) const
    {
        return _strcmpi(first, second) == 0;
    }
};


struct HashFunc {
    int operator()(char* str)const
    {
        int seed = 131;
        int hash = 0;
        for(int i = 0; str[i] != '\0'; ++i)
            hash = (hash * seed) + tolower(str[i]);
        return hash & (0x7FFFFFFF);
    }
};

#pragma region WinApi shell

HFILE GetFileHandle(char* fileName)
{
	OFSTRUCT buffer;
	HFILE handle = OpenFile(fileName, &buffer, OF_READ);
	if(handle == -1)
		throw new exception("Can't open the file");
	return handle;
}

unsigned int GetFileSize(HANDLE handle)
{
	LARGE_INTEGER sizeStruct;
	if (GetFileSizeEx(handle, &sizeStruct))
		return sizeStruct.QuadPart;
}

void ReadFile(HANDLE handle, char* buffer, unsigned int fileSize)
{
	LPDWORD bytesRead = NULL;
	if (!ReadFile((HANDLE)handle, (LPVOID)buffer, (DWORD)fileSize, bytesRead, NULL))
		throw new exception("Can't read file");
}

#pragma endregion


char* AskUserForFileName()
{
	char* fileName = new char[256];
	cout << "Enter file name: ";
	cin >> fileName;
	return fileName;
}

void PrintFileSize(unsigned int fileSize)
{
	cout << "File size: " << fileSize << "\n-----------------------------------------------------\n";
}

void SelectWord(
	const char* text, WordSelectionResult* wordSelectResult,
	unsigned int& textPosition, unsigned int textSize)
{
	unsigned int startIndex = textPosition;
	while (startIndex < textSize && !isalpha((unsigned char)text[startIndex]))
		++startIndex;
	unsigned int endIndex = startIndex;
	while (endIndex < textSize && (isalpha(text[endIndex]) || text[endIndex] == '-' || text[endIndex] == '\''))
		++endIndex;
	if (startIndex >= endIndex)
		wordSelectResult->Reset();
	else
		wordSelectResult->Set(startIndex, endIndex - 1);
	textPosition = endIndex;
}

template<typename keyValue, typename HashFuncType, typename EqualFuncType, typename AllocType>
unordered_map<keyValue, unsigned int, HashFuncType, EqualFuncType, AllocType>* CountStat(
	char* text, unsigned int textSize, 
	unordered_map<keyValue, unsigned int, HashFuncType, EqualFuncType, AllocType>* wordStat,
	void AddWord(char*, WordSelectionResult*, unordered_map<keyValue, unsigned int, HashFuncType, EqualFuncType, AllocType>*))
{
	unsigned int textPosition = 0;
	auto wordSelectResult = new WordSelectionResult();

	while (textPosition < textSize)
	{
		SelectWord(text, wordSelectResult, textPosition, textSize);
		if (wordSelectResult->Success())
		{
			text[wordSelectResult->endIndex + 1] = '\0';
			AddWord(text, wordSelectResult, wordStat);
		}
	}
	return wordStat;
}

void AddCharStringToWordStat(char* text, WordSelectionResult* wordSelectResult,
	unordered_map<char*, unsigned int, HashFunc, MyEqualTo, CustomAllocator<char>>* wordStat)
{
	if (wordStat->find(&text[wordSelectResult->startIndex]) == wordStat->end())
		(*wordStat)[&text[wordSelectResult->startIndex]] = 1;
	else
		++(*wordStat)[&text[wordSelectResult->startIndex]];
}

void AddStdStringToWordStat(char* text, WordSelectionResult* wordSelectResult,
	unordered_map<string, unsigned int, hash<string>, equal_to<string>, allocator<pair<string, unsigned int>>>* wordStat)
{
	string word = "";
	for (unsigned int i = wordSelectResult->startIndex; i <= wordSelectResult->endIndex; ++i)
		word += text[i];
	if (wordStat->find(word) == wordStat->end())
		(*wordStat)[word] = 1;
	else
		++(*wordStat)[word];
}

template<typename T>
vector<pair<T, unsigned int>>* UnorderedMapToSortedArray(
	unordered_map<T, unsigned int, HashFunc, MyEqualTo, CustomAllocator<char>>* u_map)
{
	int n_items = u_map->size();
	vector<pair<T, unsigned int>>* sortedStat = new vector<pair<T, unsigned int>>();
	pair<T, unsigned int> temp;
	int counter = 0;
	for (auto it = u_map->begin(); it != u_map->cend(); it++, ++counter)
		sortedStat->push_back(make_pair(it->first, it->second));
	//sort
	for (size_t i = 1; i < n_items; ++i)
	{
		for (size_t j = 0; j < n_items - i; j++)
		{
			if ((*sortedStat)[j].second < (*sortedStat)[j + 1].second)
			{
				temp = (*sortedStat)[j];
				(*sortedStat)[j].first = (*sortedStat)[j + 1].first;
				(*sortedStat)[j].second = (*sortedStat)[j + 1].second;
				(*sortedStat)[j + 1] = temp;
			}
		}
	}
	return sortedStat;
}

template<typename T>
void PrintResult(vector<pair<T, unsigned int>>* sortedStat)
{
	for (auto it = sortedStat->begin(); it != sortedStat->end(); ++it)
		cout << it->first << "   " << it->second << "\n";
}

int main()
{
	try
	{
		auto fileName = AskUserForFileName();
		HFILE handle = GetFileHandle(fileName);
		unsigned int fileSize = GetFileSize((HANDLE)handle);
		PrintFileSize(fileSize);


		auto text = new char[fileSize / sizeof(char) + 1];
		ReadFile((HANDLE)handle, text, fileSize);

		int startTimeCustomAllocator = clock();
		auto wordStat1 = CountStat(
			text, fileSize, new unordered_map<char*, unsigned int, HashFunc, MyEqualTo, CustomAllocator<char>>(),
			AddCharStringToWordStat);
		int algDurationUsingCustomAllocator = clock() - startTimeCustomAllocator;
		PrintResult(UnorderedMapToSortedArray(wordStat1));

		int startTimeStandartAllocator = clock();
		auto wordStat2 = CountStat(
			text, fileSize,
			new unordered_map<string, unsigned int, hash<string>, equal_to<string>, allocator<pair<string, unsigned int>>>(),
			AddStdStringToWordStat);
		int algDurationUsingStandartAllocator = clock() - startTimeStandartAllocator;

		cout << "\n\nCustom allocator = " << algDurationUsingCustomAllocator << " ms" <<
			"\nStandart allocator = " << algDurationUsingStandartAllocator << " ms" << "\n";

		delete wordStat1, wordStat2;
	}
	catch (...)
	{
		cout << "Something wrong";
	}
	return 0;
}
