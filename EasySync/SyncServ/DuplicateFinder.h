#pragma once

#include "FileInfo.h"
#include "DuplicateFileList.h"
#include "FileSystemHelper.h"
#include "CrcCalculator.h"
#include <boost/filesystem.hpp>
#include <filesystem>

namespace SyncServ
{
	class DuplicateFinder
	{
	public:
		DuplicateFinder(SearchFilterCriteria& criteria);
		~DuplicateFinder();

		DuplicateFileList_SP GetDuplicates();

		std::atomic_size_t ProcessedFiles;
		std::atomic_ullong ProcessedSize;
		std::size_t AnalayzedFiles;
		ullong AnalayzedSize;

	private:
		FileSystemHelper fsHelper;
		CrcCalculator crcCalculator;
		const SearchFilterCriteria& searchCriteria;
	};
}