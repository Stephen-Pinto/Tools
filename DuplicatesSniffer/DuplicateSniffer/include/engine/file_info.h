#pragma once
#include "../common/common.h"

struct file_info
{
public:
	int crc = 0;

	std::string name;
	std::string full_path;
	std::string extension;
	size_t size;

	bool operator ==(const file_info& item)
	{
		if (crc != 0 && crc == item.crc)
			return true;
		else if (name == item.name)
			return true;
		else 
			return false;
	}
};