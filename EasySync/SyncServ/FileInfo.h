#pragma once

#include <string>
#include <vector>
#include <memory>
#include "LocalTypes.h"

namespace SyncServ
{
	struct FileInfo
	{
	public:
		uint Crc = 0;

		std::string Name;
		std::string Path;
		std::string Extension;
		ullong Size = 0;

		bool operator ==(const FileInfo& item)
		{
			if (Crc != 0 && Crc == item.Crc)
				return true;
			else if (Name == item.Name)
				return true;
			else
				return false;
		}
	};

	typedef std::shared_ptr<FileInfo> FileInfo_SP;
	typedef std::unique_ptr<FileInfo> FileInfo_UP;	
}