#pragma once

#include <filesystem>
#include "FileInfo.h"
#include "SearchFilterCriteria.h"
#include "ComparisonUtils.h"

namespace SyncServ
{
	class FileSystemHelper
	{
	public:
		std::vector<FileInfo_SP> GetFlatList(const std::string path);
		std::vector<FileInfo_SP> GetFlatList(const SearchFilterCriteria& criteria);
		FileInfo_SP GetFileInfo(const std::string path);
	private:
		Utils::ComparisonUtils comparer;
		size_t DefaultBufferSize;
		void ValidatePath(const std::filesystem::path& dir);
	};
}