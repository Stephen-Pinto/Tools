// DuplicateSniffer.cpp : Defines the entry point for the application.
//

#include "duplicate_index_builder.h"
#include "FileSniffer.h"

using namespace std;

int main(int argc, char** argv)
{
	using namespace std;

	po::options_description sniffer_options("Duplicate sniffer options");
	sniffer_options.add_options()
		("help", "produces this help document")
		("version,v", "prints version string")
		("root,r", po::value<string>()->required(), "sets the root folder for sniffer")
		("filter,f", po::value<vector<string>>(), "look for specified files and directories only")
		("exclude,e", po::value<vector<string>>(), "exclude specified files or directories")
		("type,t", po::value<vector<string>>(), "lists all the files having these extensions")
		("dup,d", "find all duplicates or only duplicates of file specified by -f option")
		("lsize,l", po::value<int>(), "(in bytes) files below this size will be ignored")
		("msize,m", po::value<int>(), "(in bytes) files above this size will be ignored")
		("out", po::value<string>(), "set output file for results");

	po::variables_map vmap;

	try
	{
		po::store(po::parse_command_line(argc, argv, sniffer_options), vmap);
		po::notify(vmap);
	}
	catch (exception& exp)
	{
		//If not help or version then notify the user about the error 
		if(!vmap.count("help") && !vmap.count("version"))
			cerr << "ERROR: " << exp.what() << endl;
	}

	//Verify if the mandatory arguments are provided if not then show help and exit
	if (vmap.count("help") || vmap.count("root") == 0)
	{
		cout << sniffer_options;
		return EXIT_FAILURE;
	}

	//If all is ok then execute the tool
	auto exit_code = ExecuteTool(vmap);

	//cout << endl << "Program complete... Press enter to continue";
	//getchar();
	return exit_code;
}

int ExecuteTool(po::variables_map& vmap)
{
	vector<string> filter;
	vector<string> exclusion;
	vector<string> extensions;

	if(vmap.count("filter") > 0)
		filter = vmap["filter"].as<vector<string>>();

	if(vmap.count("exclude") > 0)
		exclusion = vmap["exclude"].as<vector<string>>();

	if (vmap.count("type") > 0)
		extensions = vmap["type"].as<vector<string>>();

	flat_index_builder builder(vmap["root"].as<string>(), filter, exclusion, extensions);

	//Building index
	auto start = chr_clock::now();

	auto index = builder.build_index();

	cout
		<< "Took "
		<< chrono::duration_cast<chrono::seconds>(chr_clock::now() - start).count() << " seconds"
		<< " to index " << index.size() << " files" << endl;

	if (vmap.count("dup"))
	{
		FindDuplicates(vmap, index);
	}
	else if (vmap.count("lsize") || vmap.count("msize"))
	{
		FindSizes(vmap, index);
	}
	else
	{
		PrintFiles(vmap, index);
	}

	return EXIT_SUCCESS;
}

void FindDuplicates(po::variables_map& vmap, const vector<shared_ptr<file_info>>& flat_index)
{
	duplicate_index_builder ident_grps(flat_index);

	//Scanning for duplicates
	auto start = chr_clock::now();

	auto dup_grp_index = ident_grps.get_duplicates();

	cout
		<< "Took "
		<< chrono::duration_cast<chrono::seconds>(chr_clock::now() - start).count() << " seconds"
		<< " to scan " << flat_index.size() << " files" << endl;

	bool first_duplicates_grp = true;

	ofstream ostrm;

	if (vmap.count("out"))
	{
		string outfile = vmap["out"].as<string>();
		ostrm.open(outfile);
	}

	for (auto const& item : dup_grp_index)
	{
		if (item.file_list.size() > 1)
		{
			//If it has atleast one enabled item then list duplicates
			if(any_of(
				item.file_list.begin(), 
				item.file_list.end(), 
				[](const shared_ptr<file_info>& info) { return info->is_enabled; }))
			{
				if (first_duplicates_grp)
				{
					first_duplicates_grp = false;
					
					if (!ostrm.is_open())
						cout << endl << "Listing duplicates: " << endl << endl;
				}
				else
				{
					if(!ostrm.is_open())
						cout << endl << "+++++++++++++++++++++++++++++++++++" << endl << endl;
					else
						ostrm << endl << "+++++++++++++++++++++++++++++++++++" << endl << endl;
				}

				for (auto const& file : item.file_list)
				{
					if (!ostrm.is_open())
						cout << file->full_path << endl;
					else
						ostrm << file->full_path << endl;
				}
			}
		}
	}

	if (ostrm.is_open())
	{
		ostrm.flush();
		ostrm.close();
		cout << "Successfully wrote to file";
	}
}

void PrintFiles(po::variables_map& vmap, const vector<shared_ptr<file_info>>& flat_index)
{
	ofstream ostrm;

	if (vmap.count("out"))
	{
		string outfile = vmap["out"].as<string>();
		ostrm.open(outfile);
	}

	for (auto const& item : flat_index)
	{
		if (item->is_enabled)
		{
			if (!ostrm.is_open())
				cout << item->full_path << endl;
			else
				ostrm << item->full_path << endl;
		}
	}

	if (ostrm.is_open())
	{
		ostrm.flush();
		ostrm.close();
		cout << "Successfully wrote to file";
	}
}

void FindSizes(po::variables_map& vmap, const vector<shared_ptr<file_info>>& flat_index)
{
	if (!vmap.count("lsize"))
		return;

	int lsize = -1, msize = -1;
	
	if(vmap.count("msize"))
		msize =	vmap["msize"].as<int>();

	if (vmap.count("lsize"))
		lsize = vmap["lsize"].as<int>();

	ofstream ostrm;

	if (vmap.count("out"))
	{
		string outfile = vmap["out"].as<string>();
		ostrm.open(outfile);
	}

	for (auto const& item : flat_index)
	{
		//If it matches size filter then return else continue
		if ((lsize == -1 || item->size > lsize) && (msize == -1 || item->size < msize))
		{
			if (!ostrm.is_open())
				cout << item->full_path << endl;
			else 
				ostrm << item->full_path << endl;
		}
	}

	if (ostrm.is_open())
	{
		ostrm.flush();
		ostrm.close();
		cout << "Successfully wrote to file";
	}
}

void FindFiles(po::variables_map& vmap, const vector<shared_ptr<file_info>>& flat_index)
{
}

void FindExtension(po::variables_map& vmap, const vector<shared_ptr<file_info>>& flat_index)
{
	if (!vmap.count("type"))
		return;

	//vector<string> extension_list = vmap["type"].as<vector<string>>();

	//for (auto const& item : flat_index)
	//{
	//	//Find if such extension is specified under filters
	//	auto matched_extn = find_if(extension_list.begin(), extension_list.end(), [&item](const string& extn) {
	//		return extn == item->extension;
	//});

	//	//If there is any match then print this information else continue
	//	if (matched_extn->size() > 0)  
	//	{
	//		cout << item->full_path << endl;
	//	}
	//}
}