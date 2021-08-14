#include "../common//common.h"
#include <boost/filesystem.hpp>
#include "file_info.h"

class flat_index_builder
{
public:
	flat_index_builder(std::string root_path);
	std::vector<std::shared_ptr<file_info>> build_index();

private:
	std::vector<std::shared_ptr<file_info>> get(const boost::filesystem::path &dir_path);

	boost::filesystem::path root_folder;
};