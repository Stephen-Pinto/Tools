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
	private:
		FileSystemHelper fsHelper;
		CrcCalculator crcCalculator;
	};
}