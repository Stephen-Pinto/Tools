#pragma once

#include <filesystem>
#include "FileInfo.h"

namespace SyncServ
{
	class FileSystemHelper
	{
	public:
		std::vector<FileInfo_SP> GetFlatList(const std::string path);
		FileInfo_SP GetFileInfo(const std::string path);
	private:
		size_t DefaultBufferSize;
		void ValidatePath(const std::filesystem::path& dir);
	};
}