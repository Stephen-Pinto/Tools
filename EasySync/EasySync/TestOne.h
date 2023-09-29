#include <iostream>
#include "Common.h"
#include "CrcListGenerator.h"

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

inline void TestMethodTwo()
{
    CrcListGenerator gen;
    auto list = gen.GenerateCrcForDir(DIR_PATH);
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    for (auto item : list)
    {
        std::cout << item->Path << " __ CRC: __ " << item->Crc << std::endl;
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto tm_duration = duration_cast<microseconds>(t2 - t1).count();
    std::cout << "Time elapsed: " << tm_duration << std::endl;
}

inline void TestMethodThree()
{
    CrcListGenerator gen;
    DuplicateFileList list;
    size_t count = gen.GenerateDuplicateList(list, DIR_PATH);
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    for(auto & [key, val] : list)
    {
        std::cout << "With CRC: " << key << ":" << std::endl;

        for (auto item : val)
        {
            std::cout << ">> " << item->Crc << std::endl;
        }
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto tm_duration = duration_cast<microseconds>(t2 - t1).count();
    std::cout << "Time elapsed: " << tm_duration << std::endl;
}

#endif // ! _TEST_ONE__
