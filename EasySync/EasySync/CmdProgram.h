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
		int ExecuteTool(boost::program_options::variables_map& vmap);
		int PrintList(boost::program_options::variables_map& vmap, SyncServ::DuplicateFileList_SP list);
	};
}