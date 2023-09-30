#pragma once

#include <string>
#include <filesystem>

namespace SyncServ
{
	struct SearchFilterCriteria
	{
		std::string Path;
		std::vector<std::string> HavingNames;
		std::vector<std::string> NotHavingNames;
	};
}