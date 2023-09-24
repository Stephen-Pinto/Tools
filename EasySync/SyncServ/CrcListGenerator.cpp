#include "pch.h"
#include "CrcListGenerator.h"
#include <boost/filesystem.hpp>
#include <filesystem>
#include <Windows.h>
#include <boost/crc.hpp>
#include <fstream>

using namespace SyncServ;

#ifndef PRIVATE_BUFFER_SIZE
#define PRIVATE_BUFFER_SIZE  1024
#endif

std::streamsize const  buffer_size = PRIVATE_BUFFER_SIZE;

CrcListGenerator::CrcListGenerator()
{
}

CrcListGenerator::~CrcListGenerator()
{
}

FileInfo_SP SyncServ::CrcListGenerator::GenerateCrcForFile(const std::string& file)
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

void SyncServ::CrcListGenerator::GenerateCrcForFile(const FileInfo_SP& finfo)
{
	finfo->Crc = CalculateCrc(finfo->Path);	
}

std::vector<FileInfo_SP> SyncServ::CrcListGenerator::GenerateCrcForDir(const std::string& dir)
{
	return std::vector<FileInfo_SP>();
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

void SyncServ::CrcListGenerator::ValidatePath(const std::filesystem::path& dir)
{
	if (dir.empty() || !std::filesystem::is_directory(dir) || !std::filesystem::exists(dir))
		throw std::invalid_argument("Expected a valid directory");
}

uint SyncServ::CrcListGenerator::CalculateCrc(const std::string& file)
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