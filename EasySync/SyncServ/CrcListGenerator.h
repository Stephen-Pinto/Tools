#pragma once

#include "FileInfo.h"
#include "DuplicateFileList.h"
#include <boost/filesystem.hpp>
#include <filesystem>

namespace SyncServ
{
	class CrcListGenerator
	{
	public:
		CrcListGenerator();
		~CrcListGenerator();

		FileInfo_SP GenerateCrcForFile(const std::string& file);
		void		GenerateCrcForFile(const FileInfo_SP& finfo);
		std::vector<FileInfo_SP> GenerateCrcForDir(const std::string& dir);
		//DuplicateFileList& GenerateDuplicateList(const std::string& dir);
		size_t GenerateDuplicateList(DuplicateFileList& duplicateList, const std::string& dir);

		std::vector<FileInfo_SP> GetFlatList(const std::filesystem::path& dir);
	private:
		void ValidatePath(const std::filesystem::path& dir);
		uint CalculateCrc(const std::string& file);
	};
}