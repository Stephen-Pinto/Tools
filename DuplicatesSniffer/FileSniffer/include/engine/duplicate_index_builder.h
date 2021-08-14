#include "../common/common.h"
#include "identical_file_group.h"

class duplicate_index_builder
{
public:
	duplicate_index_builder(const std::vector<std::shared_ptr<file_info>> &index);
	std::vector<identical_file_group> get_duplicates();
	void calculate_crc(const std::shared_ptr<file_info>& info);
	void add_to_appropriate_group(const std::shared_ptr<file_info>& item);

private:
	int get_crc(std::string file_path);
	
	const std::vector<std::shared_ptr<file_info>> &flat_index;
	std::vector<identical_file_group> grouped_file_list;
};