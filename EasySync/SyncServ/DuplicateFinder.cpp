#ifdef _WIN32
#define NOMINMAX
#endif // _WIN32

#include "pch.h"
#include "DuplicateFinder.h"
#include <boost/filesystem.hpp>
#include <filesystem>
#include <Windows.h>
#include <boost/crc.hpp>
#include <fstream>
#include <oneapi/tbb.h>
#include <iostream>

using namespace SyncServ;

#define AS_SIZE(x) (size_t) x

#ifndef PRIVATE_BUFFER_SIZE
#define PRIVATE_BUFFER_SIZE  2048
#endif

std::streamsize const buffer_size = PRIVATE_BUFFER_SIZE;

DuplicateFinder::DuplicateFinder(SearchFilterCriteria& criteria) : searchCriteria(criteria), fsHelper(), crcCalculator()
{
	AnalayzedFiles = 0;
	AnalayzedSize = 0;
	ProcessedFiles = 0;
	ProcessedSize = 0;
}

DuplicateFinder::~DuplicateFinder()
{
}

DuplicateFileList_SP DuplicateFinder::GetDuplicates()
{
	auto fileList = fsHelper.GetFlatList(searchCriteria);

	AnalayzedFiles = fileList.size();
	for (auto& item : fileList)
		AnalayzedSize += item->Size;
	
	using namespace oneapi::tbb;
	task_group tskGrp;

	for (auto& item : fileList)
	{
		tskGrp.run([&]
			{
				//Since we dont care about how different 2 files are we can choose to
				//set boundries for sizes and then calculate crc which can improve speed
				if (item->Size < AS_SIZE(PRIVATE_BUFFER_SIZE))
					item->Crc = crcCalculator.CalculateCrc(item->Path, 512);
				else
					item->Crc = crcCalculator.CalculateCrc(item->Path, PRIVATE_BUFFER_SIZE);

				//Increment counters which can be used for tracking progress
				ProcessedFiles++;
				ProcessedSize += item->Size;
			});
	}

	tskGrp.wait();

	DuplicateFileList_SP duplicateList(new DuplicateFileList());

	for (auto& item : fileList)
	{
		if (duplicateList->find(item->Crc) == duplicateList->end())
		{
			std::pair<uint, std::vector<FileInfo_SP>> pair = std::make_pair(item->Crc, std::vector<FileInfo_SP>());
			duplicateList->insert(pair);
		}

		(*duplicateList)[item->Crc].push_back(item);
	}

	return duplicateList;
}