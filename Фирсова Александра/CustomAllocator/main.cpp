#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <map>
#include <list>
#include <ctime>
#include <vector>
#include <algorithm>
#include "MyAllocator.h"

using namespace std;


list<char*> FormWords(char* text) {
    static const char* separators = " ,.?!:;\n\t\"()-";
    char* nextWord = nullptr;
    list<char*> words;

    char* word = strtok_s(text, separators, &nextWord);

    while (word != NULL) {
        words.push_back(word);
        word = strtok_s(NULL, separators, &nextWord);
    }

    return words;
}


struct strComp {
    bool operator()(char* a, char* b) const {
        return strcmp(a, b) < 0;
    }
};

template <class T>
bool cmp(const pair<T, size_t>& a, const pair<T, size_t>& b)
{
    return a.second < b.second;
}


long GetFileSize(const char* filename) {
    HANDLE hFile;

    hFile = CreateFile((LPCWSTR)filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        if (hFile == INVALID_HANDLE_VALUE) {
            return NULL;
        }
        return NULL;
    }

    LARGE_INTEGER fileSize;
    GetFileSizeEx(hFile, &fileSize);

    CloseHandle(hFile);

    cout << "file size: " << fileSize.LowPart << "\n";

    return fileSize.LowPart;
}

char* ReadText(const char* filename, long fileSize) {
    HANDLE hFile;
    DWORD bytesRead;

    hFile = CreateFile((LPCWSTR)filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        return NULL;
    }


    auto Text = new char[fileSize + 1];
    ReadFile(hFile, Text, fileSize, &bytesRead, NULL);
    CloseHandle(hFile);

    Text[fileSize] = '\0';
    return Text;
}

void countWithMyAllocator(const list<char*>& text, bool verbose=true) {
    static const char* separators = " ,.?!:;\n\t\"()-";

    int start = clock();
    map<char*, size_t, strComp, MyAllocator<char*>> counter;

    for (auto word : text) {
        counter[word]++;
    }
    int res = clock() - start;
    cout << "custom allocator time: " << res << "\n";

    if (!verbose) return;

    vector<pair<char*, size_t>> result;
    for (auto element : counter)
    {
        result.push_back(element);
    }
    sort(result.begin(), result.end(), cmp<char*>);

    int size = result.size();
    for (auto i = size - 1; i > 0; i--)
    {
        cout << result[i].first << ": " << result[i].second << "\n";
    }
}

void countWithSystemAllocator(const list<char*>& text, bool verbose = false) {
    static const char* separators = " ,.?!:;\n\t\"()-";

    int start = clock();
    map<string, size_t> counter;

    for (auto word : text) {
        counter[word]++;
    }
    int res = clock() - start;
    cout << "default allocator time: " << res << "\n";

    if (!verbose) return;

    vector<pair<string, size_t>> result;
        for (auto element : counter)
        {
            result.push_back(element);
        }
        sort(result.begin(), result.end(), cmp<string>);

        int size = result.size();
        for (auto i = size - 1; i > 0; i--)
        {
            cout << result[i].first << ": " << result[i].second << "\n";
        }
}


int main()
{
    const char* file = "text.txt";
    long fileSize = GetFileSize(file);
    char* text = ReadText(file, fileSize);
    auto words = FormWords(text);

    countWithSystemAllocator(words);
    countWithMyAllocator(words);
    system("PAUSE");
}