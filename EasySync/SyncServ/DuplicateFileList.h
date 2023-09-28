#pragma once

#include "FileInfo.h"
#include <map>

namespace SyncServ
{
	struct DuplicateFileList : public std::map<uint, std::vector<FileInfo_SP>>
	{
	};

	typedef std::shared_ptr<DuplicateFileList> DuplicateFileList_SP;
	typedef std::unique_ptr<DuplicateFileList> DuplicateFileList_UP;
}