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

		size_t GenerateDuplicateList(DuplicateFileList& duplicateList, const std::string& dir);
	private:
		FileSystemHelper fsHelper;
		CrcCalculator crcCalculator;
	};
}