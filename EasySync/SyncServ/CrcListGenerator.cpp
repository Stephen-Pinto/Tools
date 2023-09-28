#ifdef _WIN32
#define NOMINMAX
#endif // _WIN32
#include "pch.h"
#include "CrcListGenerator.h"
#include <boost/filesystem.hpp>
#include <filesystem>
#include <Windows.h>
#include <boost/crc.hpp>
#include <fstream>
#include <oneapi/tbb.h>

using namespace SyncServ;

#ifndef PRIVATE_BUFFER_SIZE
#define PRIVATE_BUFFER_SIZE  2048
#endif

std::streamsize const buffer_size = PRIVATE_BUFFER_SIZE;

CrcListGenerator::CrcListGenerator()
{
}

CrcListGenerator::~CrcListGenerator()
{
}

void CrcListGenerator::GenerateCrcForFile(const FileInfo_SP& finfo)
{
	finfo->Crc = CalculateCrc(finfo->Path);	
}

FileInfo_SP CrcListGenerator::GenerateCrcForFile(const std::string& file)
{
	auto path = std::filesystem::path(file);
	FileInfo_SP finfo(new FileInfo());
	finfo->Path = path.string();
	finfo->Size = std::filesystem::file_size(finfo->Path);
	finfo->Name = path.stem().string();
	finfo->Extension = path.extension().string();
	finfo->Crc = CalculateCrc(finfo->Path);
	return finfo;
}

std::vector<FileInfo_SP> CrcListGenerator::GenerateCrcForDir(const std::string& dir)
{
	using namespace oneapi::tbb;

	auto list = GetFlatList(dir);

	task_group tsk_grp;

	for (auto item : list)
	{
		tsk_grp.run([&] { GenerateCrcForFile(item); });
	}

	tsk_grp.wait();
	return list;
}

size_t CrcListGenerator::GenerateDuplicateList(DuplicateFileList& duplicateList, const std::string& dir)
{
	auto list = GenerateCrcForDir(dir);
	for (auto item : list)
	{
		if (!duplicateList.contains(item->Crc))
			duplicateList.insert(item->Crc, {});
		
		duplicateList[item->Crc].push_back(item);
	}

	return duplicateList.size();
}

std::vector<FileInfo_SP> CrcListGenerator::GetFlatList(const std::filesystem::path& dir)
{
	std::vector<FileInfo_SP> fileList;

	ValidatePath(dir);

	std::filesystem::recursive_directory_iterator end_iter;
	std::filesystem::recursive_directory_iterator iter(dir.string());

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
				OutputDebugStringA(iter->path().string().c_str());
				OutputDebugStringA("\n");
				OutputDebugStringA(exp.what());
				OutputDebugStringA("\n");
			}
		}

		std::error_code err;
		iter.increment(err);
	}

	return fileList;
}

void CrcListGenerator::ValidatePath(const std::filesystem::path& dir)
{
	if (dir.empty() || !std::filesystem::is_directory(dir) || !std::filesystem::exists(dir))
		throw std::invalid_argument("Expected a valid directory");
}

uint CrcListGenerator::CalculateCrc(const std::string& file)
{
	boost::crc_32_type cc;
	std::ifstream input_fstream(file, std::ios_base::binary);

	if (input_fstream)
	{
		do
		{
			//Load buffer for checksum
			char buffer[buffer_size];
			input_fstream.read(buffer, buffer_size);
			cc.process_bytes(buffer, input_fstream.gcount());

		} while (input_fstream);
	}

	return cc.checksum();
}