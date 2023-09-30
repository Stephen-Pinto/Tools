#pragma once

#include "Common.h"
#include "DuplicateFinder.h"
#include <chrono>
#include <iostream>

using namespace SyncServ;
using namespace std::chrono;

#define DIR_PATH "D:\\Temporary"

inline void TestOne()
{
    FileSystemHelper helper;
    SearchFilterCriteria criteria;    
    criteria.Path = DIR_PATH;
    criteria.HavingNames.push_back("*.pdf");
    auto list = helper.GetFlatList(criteria);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    for (auto& info : list)
    {
        std::cout << ">> " << info->Path << std::endl;
        std::cout << std::endl;
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto tm_duration = duration_cast<microseconds>(t2 - t1).count();
    std::cout << "Time elapsed: " << tm_duration * 1e-6 << " secs" << std::endl;
}

inline void TestTwo()
{
    FileSystemHelper helper;
    SearchFilterCriteria criteria;
    criteria.Path = DIR_PATH;
    //criteria.NotHavingNames.push_back("*.epub");
    criteria.HavingNames.push_back("*.epub");
    criteria.NotHavingNames.push_back("*.pdf");
    auto list = helper.GetFlatList(criteria);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    for (auto& info : list)
    {
        std::cout << ">> " << info->Path << std::endl;
        std::cout << std::endl;
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto tm_duration = duration_cast<microseconds>(t2 - t1).count();
    std::cout << "Time elapsed: " << tm_duration * 1e-6 << " secs" << std::endl;
}