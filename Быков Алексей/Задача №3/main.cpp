#ifdef _DEBUG
	#include <crtdbg.h>
	#define _CRTDBG_MAP_ALLOC
#endif

#include <iostream>
#include <string>
#include "smart_ptr.h"

int main() {
#ifdef _DEBUG
	_CrtMemState _ms;
	_CrtMemCheckpoint(&_ms);
#endif

	std::cout << "Testing equality Get option and operator*" << std::endl;
	smart_ptr<int> ismart_ptr;
	ismart_ptr.Set(new int(rand()));
	std::cout << "Get: " << ismart_ptr.Get() << std::endl;
	std::cout << "Operator* : " << *ismart_ptr << std::endl;

	std::cout << "Testing Release option" << std::endl;
	ismart_ptr.Set(new int(rand()));
	std::cout << "New int: " << ismart_ptr.Get() << std::endl;
	ismart_ptr.Release();

	std::cout << "Testing multiple links" << std::endl;
	ismart_ptr.Set(new int(rand()));
	std::cout << "New int: " << ismart_ptr.Get() << std::endl;
	smart_ptr<int> ismart_ptr_sec = ismart_ptr;
	std::cout << "New ptr: " << ismart_ptr_sec.Get() << std::endl;
	ismart_ptr_sec = ismart_ptr;
	ismart_ptr.Release();
	std::cout << "New ptr after Release: " << ismart_ptr_sec.Get() << std::endl;
	
	std::cout << "Testing operator->" << std::endl;
	smart_ptr<std::string> ssmart_ptr;
	ssmart_ptr.Set(new std::string("abracadabra"));
	std::cout << "Size of string: " << ssmart_ptr->size() << std::endl;

	ismart_ptr_sec.Release();
	ssmart_ptr.Release();
#ifdef _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtMemDumpAllObjectsSince(&_ms);
#endif
}