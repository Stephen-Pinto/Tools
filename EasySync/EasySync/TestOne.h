#include <iostream>
#include "Common.h"
#include "DuplicateFinder.h"
#include <chrono>

#ifndef  _TEST_ONE__
#define _TEST_ONE__

using namespace SyncServ;
using namespace std::chrono;

#define DIR_PATH "D:\\Temporary\\Pdfs"

//inline void TestMethodOne()
//{
//    CrcListGenerator gen;
//    auto list = gen.GenerateCrcForDir2(DIR_PATH);
//    high_resolution_clock::time_point t1 = high_resolution_clock::now();
//
//    for (auto item : list)
//    {
//        std::cout << item->Path << " __ CRC: __ " << item->Crc << std::endl;
//    }
//
//    high_resolution_clock::time_point t2 = high_resolution_clock::now();
//    auto tm_duration = duration_cast<microseconds>(t2 - t1).count();
//    std::cout << "Time elapsed: " << tm_duration << std::endl;
//}

//inline void TestMethodTwo()
//{
//    CrcListGenerator gen;
//    auto list = gen.GenerateCrcForDir(DIR_PATH);
//    high_resolution_clock::time_point t1 = high_resolution_clock::now();
//
//    for (auto item : list)
//    {
//        std::cout << item->Path << " __ CRC: __ " << item->Crc << std::endl;
//    }
//
//    high_resolution_clock::time_point t2 = high_resolution_clock::now();
//    auto tm_duration = duration_cast<microseconds>(t2 - t1).count();
//    std::cout << "Time elapsed: " << tm_duration << std::endl;
//}

inline void TestMethodThree()
{
	DuplicateFinder finder;
	auto list = finder.GenerateDuplicateList(DIR_PATH);
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	for (auto& [key, val] : *list)
	{
		std::cout << "With CRC: " << key << ": [" << val.size() << "]" << std::endl;

		for (auto item : val)
		{
			std::cout << ">> " << item->Path << std::endl;
		}

		std::cout << std::endl;
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto tm_duration = duration_cast<microseconds>(t2 - t1).count();
	std::cout << "Time elapsed: " << tm_duration * 1e-6 << " secs" << std::endl;
}

#endif // ! _TEST_ONE__
