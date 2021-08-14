#include "../include/engine/flat_index_builder.h"
#include <filesystem>
#include <Windows.h>
#include <WinBase.h>

using namespace std;
using namespace boost::filesystem;
namespace stdfs = std::experimental::filesystem;

flat_index_builder::flat_index_builder(std::string root_path)
{
	if (root_path == "" || root_path == ".")
		root_folder = current_path();
	else
		root_folder = root_path;
}

vector<shared_ptr<file_info>> flat_index_builder::build_index()
{
	return get(root_folder);
}

//vector<shared_ptr<file_info>> flat_index_builder::get(const path &dir_path)
//{
//	vector<shared_ptr<file_info>> file_info_list;
//
//	//If not found return empty vector
//	if (!exists(dir_path))
//		return {};
//
//	directory_iterator end_iter;
//	directory_iterator iter(dir_path);
//
//	for (; iter != end_iter; ++iter)
//	{
//		if (is_directory(iter->status()))
//		{
//			//Recursive callback and merge
//			auto vect = get(iter->path());
//			file_info_list.insert(file_info_list.end(), vect.begin(), vect.end());
//		}
//		else
//		{
//			//Obtain file information
//			try
//			{
//				if (exists(iter->path()))
//				{
//					shared_ptr<file_info> finfo(new file_info());
//					finfo->full_path = iter->path().string();
//					finfo->size = file_size(finfo->full_path);
//					finfo->name = basename(finfo->full_path);
//					finfo->extension = extension(finfo->full_path);
//					file_info_list.push_back(finfo);
//				}
//			}
//			catch (exception &exp)
//			{
//				//std::cout << "Error:" << iter->path() << std::endl;
//				OutputDebugString(iter->path().string().c_str());
//				OutputDebugString("\n");
//				OutputDebugString(exp.what());
//				OutputDebugString("\n");
//			}
//		}
//	}
//
//	return file_info_list;
//}

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
		if (!is_directory(iter->path()) && exists(iter->path()))
		{
			try
			{
				shared_ptr<file_info> finfo(new file_info());
				finfo->full_path = iter->path().string();
				finfo->size = file_size(finfo->full_path);
				finfo->name = basename(finfo->full_path);
				finfo->extension = extension(finfo->full_path);
				file_info_list.push_back(finfo);
			}
			catch (exception &exp)
			{
				OutputDebugString(iter->path().string().c_str());
				OutputDebugString("\n");
				OutputDebugString(exp.what());
				OutputDebugString("\n");
			}
		}

		error_code err;
		iter.increment(err);
		//else
		//{
		//	//To skip iteration over this directory
		//	//iter.no_push();
		//	//iter.disable_recursion_pending();
		//}
	}

	return file_info_list;
}