#pragma once

#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include "FileInfo.h"
#include "DuplicateFinder.h"

namespace EasySync
{
	class CmdProgram
	{
	public:
		int Run(int argc, char** argv);
	private:
		int ExecuteTool(const boost::program_options::variables_map& vmap);
		int PrintList(const boost::program_options::variables_map& vmap, const SyncServ::DuplicateFileList_SP& list);
		int ExtractFiles(const boost::program_options::variables_map& vmap, const SyncServ::DuplicateFileList_SP& list);
		std::vector<std::string> SplitList(const std::string& arg);
	};
}