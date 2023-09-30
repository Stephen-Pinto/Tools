#include "FileSystemHelper.h"
#include <iostream>

using namespace SyncServ;

std::vector<FileInfo_SP> FileSystemHelper::GetFlatList(const std::string path)
{
	std::vector<FileInfo_SP> fileList;

	ValidatePath(path);

	std::filesystem::recursive_directory_iterator end_iter;
	std::filesystem::recursive_directory_iterator iter(path);

	while (iter != end_iter)
	{
		if (!is_directory(iter->path()) && exists(iter->path()))
		{
			try
			{
				FileInfo_SP finfo(new FileInfo());
				auto& pathRef = iter->path();
				finfo->Path = pathRef.string();
				finfo->Size = std::filesystem::file_size(finfo->Path);
				finfo->Name = pathRef.stem().string();
				finfo->Extension = pathRef.extension().string();
				fileList.push_back(finfo);
			}
			catch (std::exception& exp)
			{
				std::cerr << "Exception: " << exp.what() << std::endl;
			}
		}

		std::error_code err;
		iter.increment(err);
	}

	return fileList;
}

std::vector<FileInfo_SP> SyncServ::FileSystemHelper::GetFlatList(const SearchFilterCriteria& criteria)
{
	std::vector<FileInfo_SP> fileList;

	ValidatePath(criteria.Path);

	std::filesystem::recursive_directory_iterator end_iter;
	std::filesystem::recursive_directory_iterator iter(criteria.Path);

	while (iter != end_iter)
	{
		if (!is_directory(iter->path()) && exists(iter->path()))
		{
			try
			{
				FileInfo_SP finfo(new FileInfo());
				auto& pathRef = iter->path();

				if (!(
						(criteria.HavingNames.size() > 0 
						&& !comparer.WildcardMatch(criteria.HavingNames, pathRef.string().c_str()))
						||
						(criteria.NotHavingNames.size() > 0
						&& comparer.WildcardMatch(criteria.NotHavingNames, pathRef.string().c_str()))
					))
				{
					finfo->Path = pathRef.string();
					finfo->Size = std::filesystem::file_size(finfo->Path);
					finfo->Name = pathRef.filename().string();
					finfo->Extension = pathRef.extension().string();
					fileList.push_back(finfo);
				}
			}
			catch (std::exception& exp)
			{
				std::cerr << "Exception: " << exp.what() << std::endl;
			}
		}

		std::error_code err;
		iter.increment(err);
	}

	return fileList;
}

FileInfo_SP FileSystemHelper::GetFileInfo(const std::string path)
{
	auto pathObj = std::filesystem::path(path);
	FileInfo_SP finfo(new FileInfo());
	finfo->Path = pathObj.string();
	finfo->Size = std::filesystem::file_size(finfo->Path);
	finfo->Name = pathObj.stem().string();
	finfo->Extension = pathObj.extension().string();
	return finfo;
}

void FileSystemHelper::ValidatePath(const std::filesystem::path& dir)
{
	if (dir.empty() || !std::filesystem::is_directory(dir) || !std::filesystem::exists(dir))
		throw std::invalid_argument("Expected a valid directory");
}
