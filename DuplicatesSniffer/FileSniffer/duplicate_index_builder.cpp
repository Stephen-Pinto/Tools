#include <boost/crc.hpp>
#include <fstream>
#include <algorithm>
#include <functional>
#include <boost/thread.hpp>
#include "duplicate_index_builder.h"
#include <tbb/task_group.h>

using namespace std;

#ifndef PRIVATE_BUFFER_SIZE
#define PRIVATE_BUFFER_SIZE  1024
#endif

streamsize const  buffer_size = PRIVATE_BUFFER_SIZE;

duplicate_index_builder::duplicate_index_builder(const vector<shared_ptr<file_info>> &index) : flat_index(index)
{}

vector<identical_file_group> duplicate_index_builder::get_duplicates()
{
	//Calculate all CRC values in independent tbb tasks
	tbb::task_group tsk_grp;
	for (const auto& item : flat_index)
	{
		tsk_grp.run([&] { calculate_crc(item); });
	}

	//Wait for all the CRC values to be calculated
	tsk_grp.wait();

	//Organize all files according to their CRC values
	for (const auto& item : flat_index)
	{
		add_to_appropriate_group(item);
	}

	return grouped_file_list;
}

void duplicate_index_builder::calculate_crc(const shared_ptr<file_info>& info)
{
	info->crc = get_crc(info->full_path);
}

void duplicate_index_builder::add_to_appropriate_group(const shared_ptr<file_info>& item)
{
	//Look for this matching crc in the 
	auto matched_item = find_if(
		grouped_file_list.begin(), grouped_file_list.end(),
		bind(&identical_file_group::compare, placeholders::_1, *item));

	if (matched_item != grouped_file_list.end())
	{
		//If a match found then update this item in that group
		matched_item[0].file_list.push_back(item);
	}
	else
	{
		//If not exists already then add it to the group
		identical_file_group grp;
		grp.crc = item->crc;
		grp.file_list.push_back(item);
		grouped_file_list.push_back(grp);
	}
}

int duplicate_index_builder::get_crc(string file_path)
{
	boost::crc_32_type crc_calcuator;

	ifstream input_fstream(file_path, ios_base::binary);

	if (input_fstream)
	{
		do
		{
			//Load buffer for checksum
			char buffer[buffer_size];
			input_fstream.read(buffer, buffer_size);
			crc_calcuator.process_bytes(buffer, input_fstream.gcount());

		} while (input_fstream);
	}

	return crc_calcuator.checksum();
}