#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <time.h>   

using namespace std;

size_t SIZE;

class Item
{
public:
	const char* word;
	mutable int count;
	Item(const char* word)
	{
		this->word = word;
		this->count = 1;
	}
};

struct Comparator
{
	bool operator()(const Item left, const char *right)
	{
		return strcmp(left.word, right) < 0;
	}
	bool operator()(const char *left, const Item right)
	{
		return strcmp(left, right.word) < 0;
	}



	bool operator()(const Item left, const Item right)
	{
		return strcmp(left.word, right.word) < 0;
	}
};

struct ComparatorForCount
{
	bool operator()(const int left, const Item right)
	{
		return left < right.count;
	}

	bool operator()(const Item left, const int right)
	{
		return left.count < right;
	}

	bool operator()(const Item left, const Item right)
	{
		return left.count < right.count;
	}
};



char* getText(char* fileName)
{
	std::ifstream in(fileName);
	char symbol;
	size_t fileSize = getFileSize(fileName);
	SIZE = fileSize;
	std::cout << fileSize << std::endl;
	char* buffer = new char[fileSize + 1];
	int i = 0;
	while (in.get(symbol))
	{
		buffer[i] = (char)tolower((int)symbol);
		i++;
	}
	buffer[i] = 0;
	in.close();
	return buffer;
}


std::set<Item, Comparator> parse(char* buffer)
{

	char *word;
	size_t length = 0;
	size_t startInd = 0;
	size_t finishInd = 0;


	std::set<Item, Comparator> Dict;
	for (size_t i = 0; i < SIZE; i++)
	{
		if (!isalpha(buffer[i]) and buffer[i] != '-')
		{
			if (isalpha(buffer[i - 1]))
			{

				word = (char*)malloc(length);
				for (size_t j = 0; j < length; j++)
				{
					word[j] = buffer[startInd + j];
				}
				auto item = Dict.find(word);
				if (item != Dict.end())
				{
					item->count = item->count + 1;
				}
				else
				{
					Item item = Item(word);
					Dict.insert(item);
				}
			}
			startInd = i + 1;
			finishInd = i + 1;
			length = 0;
		}
		else
		{
			finishInd++;
			length++;
		}
	}
	word = (char*)malloc(length);
	for (int i = 0; i < length; i++)
	{
		word[i] = buffer[startInd + i];
	}
	auto item = Dict.find(word);
	if (item != Dict.end())
		item->count = item->count + 1;

	else
	{
		Item item = Item(word);
		Dict.insert(item);
	}
	return Dict;
}

size_t getFileSize(char* fileName)
{
	std::ifstream in(fileName);
	in.seekg(0, std::ios_base::end);
	return in.tellg();
}

void printSlice(size_t start, size_t finish, char* buffer)
{
	for (int j = start; j < finish; j++)
	{
		char k = *(buffer + j);
		std::cout << k;
	}
}

std::set<Item, ComparatorForCount> sorting(std::set<Item, Comparator> map)
{
	std::set<Item, ComparatorForCount> map2;
	for (Item item : map)
	{
		map2.insert(item);
	}
	return map2;
}

std::map<std::string, size_t> handleStd(char* buffer)
{
	size_t startInd = 0;
	size_t finishInd = 0;
	size_t length = 0;
	std::string word;
	std::map<std::string, size_t> Dict = std::map<std::string, size_t>();

	for (int i = 0; i < SIZE; i++)
	{
		if (!isalpha(buffer[i]) and buffer[i] != '-')
		{
			if (isalpha(buffer[i - 1])) {
				for (int k = 0; k < length; k++)
				{
					word += buffer[startInd + k];
				}
				auto item = Dict.find(word);
				if (item != Dict.end())

					item->second = item->second + 1;

				else

					Dict.insert(std::pair<std::string, size_t>(word, 1));
			}
			startInd = i + 1;
			finishInd = i + 1;
			length = 0;
		}
		else
		{
			word = "";
			finishInd++;
			length++;
		}
	}
	cout << word;
	for (int i = 0; i < length; i++)
	{
		word += buffer[startInd + i];
	}
	auto item = Dict.find(word);
	if (item != Dict.end())

		item->second = item->second + 1;

	else

		Dict.insert(std::pair<std::string, size_t>(word, 1));

	return Dict;
}



int main()
{
	char* fileName = "text.txt";
	char* content = getText(fileName);
	size_t fileSize = getFileSize(fileName);
	cout << "Filename = " << fileName << ", size of file = " << fileSize;

	clock_t start = clock();
	const std::set<Item, Comparator> dictionary = parse(content);
	clock_t finish = clock();

	for (Item i : dictionary)
	{
		cout << i.word << " - " << i.count;
	}

	clock_t start2 = clock();
	const std::map<std::string, size_t> dict2 = handleStd(content);
	clock_t finish2 = clock();
	for (auto p : dict2)
	{
		cout << p.first << " - " << p.second;
	}

	double t1 = ((double)(finish - start) * 1000) / CLOCKS_PER_SEC;
	double t2 = ((double)(finish2 - start2) * 1000) / CLOCKS_PER_SEC;
	cout << t1 << " user allocator ";
	cout << t2 << " std::map";
	std::set<Item, ComparatorForCount> map = sorting(dictionary);
	for (Item item : map)
	{
		cout << item.word << " - " << item.count;
	}
}
