#pragma once

#include "FileInfo.h"

namespace SyncServ
{
	struct DuplicateFileList
	{
	public:
		int Crc = 0;
		std::vector<FileInfo_SP> FileList;
	};

	typedef std::shared_ptr<DuplicateFileList> DuplicateFileList_SP;
	typedef std::unique_ptr<DuplicateFileList> DuplicateFileList_UP;
}