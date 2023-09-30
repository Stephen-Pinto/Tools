#pragma once

#include <string>
#include <filesystem>
#include "FileInfo.h"

namespace SyncServ
{
	class CrcCalculator
	{
	public:
		uint CalculateCrc(const std::string& file);
		uint CalculateCrc(const std::string& file, const size_t bufferSize);
	private:
		size_t DefaultBufferSize;
	};
}