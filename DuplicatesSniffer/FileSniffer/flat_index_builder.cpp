#include "../include/engine/flat_index_builder.h"
#include <filesystem>
#include <Windows.h>
#include <WinBase.h>

using namespace std;
using namespace boost::filesystem;
namespace stdfs = std::experimental::filesystem;

flat_index_builder::flat_index_builder(std::string root_path, const vector<string>& filter, const vector<string>& exclude, const vector<string>& extension)
	: filters(filter), exclusions(exclude), extensions(extension)
{
	if (filter.size() > 0 || exclude.size() > 0 || extension.size() > 0)
		flag_filter_exists = true;

	if (root_path == "" || root_path == ".")
		root_folder = current_path();
	else
		root_folder = root_path;
}

vector<shared_ptr<file_info>> flat_index_builder::build_index()
{
	return get(root_folder);
}

bool match(char const *needle, char const *haystack) 
{
	for (; *needle != '\0'; ++needle) 
	{
		switch (*needle) 
		{
		case '?':
			if (*haystack == '\0')
				return false;
			++haystack;
			break;
		case '*': {
			if (needle[1] == '\0')
				return true;
			size_t max = strlen(haystack);
			for (size_t i = 0; i < max; i++)
				if (match(needle + 1, haystack + i))
					return true;
			return false;
		}
		default:
			if (toupper(*haystack) != toupper(*needle))
				return false;
			++haystack;
		}
	}
	return *haystack == '\0';
}

vector<shared_ptr<file_info>> flat_index_builder::get(const path &dir_path)
{
	vector<shared_ptr<file_info>> file_info_list;

	//If not found return empty vector
	if (!exists(dir_path) || !is_directory(dir_path))
		return {};

	stdfs::recursive_directory_iterator end_iter;
	stdfs::recursive_directory_iterator iter(dir_path.string());

	while (iter != end_iter)
	{
		if (!exists(iter->path()))
		{
			error_code err;
			iter.increment(err);
			continue;
		}

		bool consider_item = false;

		if (flag_filter_exists && !is_directory(iter->path()))
		{
			bool is_excluded = false, is_ok_filter = true, is_ok_type = true;

			//Check if exclusions exist and if present filter by them
			if (exclusions.size() > 0)
			{
				is_excluded = satisfies_exclusion(iter->path().string());
			}

			//Check if filters exist and if present filter by them
			if (filters.size() > 0)
			{
				is_ok_filter = satisfies_filter(iter->path().string());
			}

			//Check if extensions exist and if present filter by them
			if (extensions.size() > 0)
			{
				is_ok_type = satisfies_type(iter->path().string());
			}

			//If all conditions are ok then consider this file
			if (!is_excluded && is_ok_filter && is_ok_type)
			{
				consider_item = true;
			}
		}
		else
		{
			//If there are no filters then consider this item
			consider_item = true;
		}
		
		//if (is_directory(iter->path()))
		//{
		//	//If the file is a directory then check if we must ignore this else continue
		//	if (!consider_item)
		//		iter.disable_recursion_pending();
		//}

		try
		{
			shared_ptr<file_info> finfo(new file_info());
			finfo->full_path = iter->path().string();
			finfo->name = basename(finfo->full_path);
			
			if (consider_item)
			{
				finfo->size = file_size(finfo->full_path);
				finfo->extension = extension(finfo->full_path);
			}
			else
			{
				finfo->is_enabled = false;
			}
			
			file_info_list.push_back(finfo);
		}
		catch (exception &exp)
		{
			OutputDebugString(iter->path().string().c_str());
			OutputDebugString("\n");
			OutputDebugString(exp.what());
			OutputDebugString("\n");
		}

		error_code err;
		iter.increment(err);
	}

	return file_info_list;
}

bool flat_index_builder::satisfies_exclusion(const boost::filesystem::path& file_path)
{
	for (const auto& item : exclusions)
	{
		if (match(item.c_str(), file_path.filename().string().c_str()))
		{
			return true;
		}
		else if ((item.find(PATH_SEPARATOR) != string::npos)
			&& match(item.c_str(), file_path.string().c_str()))
		{
			return true;
		}
	}

	return false;
}

bool flat_index_builder::satisfies_filter(const boost::filesystem::path& file_path)
{
	for (const string& item : filters)
	{
		if (match(item.c_str(), file_path.filename().string().c_str()))
		{
			return true;
		}
		else if ((item.find(PATH_SEPARATOR) != string::npos)
			&& match(item.c_str(), file_path.string().c_str()))
		{
			return true;
		}
	}

	return false;
}

bool flat_index_builder::satisfies_type(const boost::filesystem::path& file_path)
{
	for (const auto& item : extensions)
	{
		if (match(item.c_str(), file_path.extension().string().c_str()))
		{
			return true;
		}
	}

	return false;
}
