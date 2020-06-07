#include <iostream>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <chrono>
#include "allocator.h"

using namespace std;

struct cstrless {
    bool operator() (const char* a, const char* b) const
    {
        return strcmp(a, b) < 0;
    }
};

char* getFileContents(const char *filename)
{
    auto *fp = fopen(filename, "rb");
    if (!fp)
        throw(exception());
    char* contents;
    fseek(fp, 0, SEEK_END);
    auto size = ftell(fp);
    contents = new char[size + 1];
    rewind(fp);
    fread(contents, 1, size, fp);
    fclose(fp);
    contents[size] = '\0';
    return contents;
}

long getFileSize(const char* filename)
{
    struct stat stat_buffer{};
    auto rc = stat(filename, &stat_buffer);
    return rc == 0 ? stat_buffer.st_size : -1;
}

vector<char*> splitString(char* string, char* delimiters)
{
    auto result = vector<char*>();
    char* part;
    part = strtok(string, delimiters);
    while (part != nullptr)
    {
        result.push_back(part);
        part = strtok(nullptr, delimiters);
    }
    return result;
}

map<char*, size_t, cstrless, Allocator<pair<const char*, size_t>>> getStatsCustom(const vector<char*>& words)
{
    auto frequency = map<char*, size_t, cstrless, Allocator<pair<const char*, size_t>>>();
    for (auto const& word: words)
    {
        if (!frequency.contains(word))
            frequency[word] = 0;
        frequency[word]++;
    }
    return frequency;
}

set<pair<size_t, char*>, greater<>> getSortedStatsCustom(const map<char*, size_t, cstrless, Allocator<pair<const char*, size_t>
>>& original)
{
    auto result = set<pair<size_t, char*>, greater<>>();
    for (auto const& [word, frequency]: original)
        result.emplace(frequency, word);
    return result;
}

set<pair<size_t, char*>, greater<>> getStatsWithCustomAllocator(char* text)
{
    auto words = splitString(text, " ,':-—.\";()!?_[]#\r\n");
    auto stats = getStatsCustom(words);
    return getSortedStatsCustom(stats);
}

map<string, size_t> getStats(const vector<char*>& words)
{
    auto frequency = map<string, size_t>();
    for (auto const& word: words)
    {
        if (!frequency.contains(word))
            frequency[word] = 0;
        frequency[word]++;
    }
    return frequency;
}

set<pair<size_t, string>, greater<>> getSortedStats(const map<string, size_t>& original)
{
    auto result = set<pair<size_t, string>, greater<>>();
    for (auto const& [word, frequency]: original)
        result.emplace(frequency, word);
    return result;
}

set<pair<size_t, string>, greater<>> getStatsWithStandardAllocator(char* text)
{
    auto words = splitString(text, " ,':-—.\";()!?_[]#\r\n");
    auto stats = getStats(words);
    return getSortedStats(stats);
}