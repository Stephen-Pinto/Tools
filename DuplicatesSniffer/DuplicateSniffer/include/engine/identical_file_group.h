#pragma once
#include "../common/common.h"
#include "file_info.h"

struct identical_file_group
{
public:
	int crc = 0;
	std::vector<std::shared_ptr<file_info>> file_list;
	
	//bool friend operator==(const identical_file_group& grp, const file_info& item);
	bool compare(const file_info& item)
	{
		if (crc != 0 && item.crc != 0 && crc == item.crc)
			return true;
		
		return false;
	}
};

//bool operator==(const identical_file_group & grp, const file_info & item)
//{
//	if (grp.crc != 0 && item.crc != 0 && grp.crc == item.crc)
//		return true;
//	else
//		return false;
//}