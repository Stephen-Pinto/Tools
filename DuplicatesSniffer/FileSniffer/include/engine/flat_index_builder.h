#include "../common//common.h"
#include <boost/filesystem.hpp>
#include "file_info.h"

class flat_index_builder
{
public:
	flat_index_builder(std::string root_path, 
		const std::vector<std::string>& filters, 
		const std::vector<std::string>& excludes, 
		const std::vector<std::string>& extensions);

	std::vector<std::shared_ptr<file_info>> build_index();

private:
	std::vector<std::shared_ptr<file_info>> get(const boost::filesystem::path &dir_path);
	bool satisfies_exclusion(const boost::filesystem::path& file_path);
	bool satisfies_filter(const boost::filesystem::path& file_path);
	bool satisfies_type(const boost::filesystem::path& file_path);

	boost::filesystem::path root_folder;
	const std::vector<std::string>& filters;
	const std::vector<std::string>& exclusions;
	const std::vector<std::string>& extensions;
	bool flag_filter_exists = false;
};