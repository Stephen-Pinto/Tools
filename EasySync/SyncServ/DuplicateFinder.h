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
		DuplicateFinder();
		~DuplicateFinder();

		DuplicateFileList_SP GenerateDuplicateList(const std::string& dir);
		DuplicateFileList_SP GenerateDuplicateList(const SearchFilterCriteria& criteria);

		std::atomic_size_t ProcessedFiles;
		std::size_t AnalayzedFiles;

	private:
		FileSystemHelper fsHelper;
		CrcCalculator crcCalculator;

		DuplicateFileList_SP GetDuplicates(const std::vector<FileInfo_SP>& fileList);
	};
}