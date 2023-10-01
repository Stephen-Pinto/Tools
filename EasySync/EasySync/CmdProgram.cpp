#include <iostream>
#include <fstream>
#include "TimerUtil.h"
#include "CmdProgram.h"
#include "FileInfo.h"
#include "DuplicateFinder.h"
#include "progressbar.hpp"
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

using namespace boost::program_options;
using namespace std;
using namespace SyncServ;
using namespace EasySync;

int CmdProgram::Run(int argc, char** argv)
{
	options_description options("Duplicate sniffer options.");
	options.add_options()
		("help", "Produces this help document.")
		("version,v", "Prints version string.")
		("root,r", value<string>()->required(), "Sets the root folder for sniffer.")
		("include,i", value<string>(), "Look for specified files and directories only. Split multiple values with '|'.")
		("exclude,x", value<string>(), "Exclude specified files or directories. Split multiple values with '|'.")
		("out,o", value<string>(), "Set output file for results.")
		("extract,e", value<string>(), "Move unique copies to specified directory without duplicates");

	variables_map vmap;

	try
	{
		store(parse_command_line(argc, argv, options), vmap);
		notify(vmap);
	}
	catch (exception& exp)
	{
		//If not help or version then notify the user about the error 
		if (!vmap.count("help") && !vmap.count("version"))
			cerr << "ERROR: " << exp.what() << endl;
	}

	//Verify if the mandatory arguments are provided if not then show help and exit
	if (vmap.count("help") || vmap.count("root") == 0)
	{
		cout << options;
		return EXIT_FAILURE;
	}

	if (vmap.count("version"))
	{
		cout << "Version: 0.95 (alpha)";
		return EXIT_SUCCESS;
	}

	//If all is ok then execute the tool
	auto exitCode = ExecuteTool(vmap);

	//cout << endl << "Program complete... Press enter to continue";
	//getchar();
	return exitCode;
}

int CmdProgram::ExecuteTool(const variables_map& vmap)
{
	SearchFilterCriteria criteria;
	criteria.Path = vmap["root"].as<string>();

	if (vmap.count("include") > 0)
		criteria.HavingNames = SplitList(vmap["include"].as<string>());

	if (vmap.count("exclude") > 0)
		criteria.NotHavingNames = SplitList(vmap["exclude"].as<string>());

	DuplicateFinder dupFinder(criteria);

	//Building index
	TimerUtil timer;
	timer.Start();

	cout << "Indexing files" << endl;

	boost::thread t{
		[&]
		{
			while (dupFinder.AnalayzedFiles == 0)
				boost::this_thread::sleep_for(boost::chrono::milliseconds{ 100 });

			ldouble perc = 0.0;
			int lastPerc = 0;
			int curPerc = 0;
			progressbar bar(100);

			while (dupFinder.AnalayzedSize > dupFinder.ProcessedSize)
			{
				perc = (ldouble)dupFinder.ProcessedSize / (ldouble)dupFinder.AnalayzedSize;
				curPerc = (int)(perc * 100);

				for (int i = 0; i < curPerc - lastPerc; i++)
					bar.update();

				lastPerc = curPerc;
				boost::this_thread::sleep_for(boost::chrono::milliseconds{ 50 });
			}

			for (int i = 0; i < 100 - lastPerc; i++)
				bar.update();

			cout << endl;
		}
	};

	auto duplicates = dupFinder.GetDuplicates();

	timer.Stop();
	t.join();

	cout << endl << "Indexing Complete. Took "
		<< timer.ElapsedSeconds() << " seconds"
		<< " to index " << dupFinder.ProcessedFiles << " files" << endl << endl;

	if (vmap.count("extract"))
		return ExtractFiles(vmap, duplicates);
	return PrintList(vmap, duplicates);
}

int CmdProgram::PrintList(const variables_map& vmap, const DuplicateFileList_SP& list)
{
	ostream* ostrm = nullptr;
	ofstream* ofstrm = nullptr;

	//Prepare output stream based on options specified
	if (vmap.count("out"))
	{
		string outfile = vmap["out"].as<string>();
		ofstrm = new ofstream();
		ofstrm->open(outfile);
		ostrm = ofstrm;
	}
	else
	{
		ostrm = &cout;
	}

	//Print out the whole list
	for (auto& [key, val] : *list)
	{
		if (val.size() == 1)
		{
			*ostrm << "File: " << val[0]->Name
				<< " Crc: " << val[0]->Crc
				<< endl;
		}
		else
		{
			*ostrm << "Crc: " << key << " :" << endl;
			for (auto item : val)
			{
				*ostrm << "File: " << val[0]->Name
					<< endl;
			}
			*ostrm << endl;
		}
	}

	if (ofstrm != nullptr)
	{
		ofstrm->flush();
		ofstrm->close();
		delete(ofstrm);
		cout << "Wrote to file successfully" << endl;
	}

	return EXIT_SUCCESS;
}

int CmdProgram::ExtractFiles(const variables_map& vmap, const DuplicateFileList_SP& list)
{
	if (!vmap.count("extract"))
	{
		cerr << "No proper extraction directory provided" << endl;
		return EXIT_FAILURE;
	}

	string path = vmap["extract"].as<string>();
	cout << "Extracting files to " << path << endl;

	filesystem::create_directories(path);

	progressbar bar(list->size());
	TimerUtil timer;
	timer.Start();

	for (auto& [key, val] : *list)
	{
		//Copy only first versions of each file
		filesystem::copy_file(
			val[0]->Path,
			path + "\\" + val[0]->Name,
			filesystem::copy_options::overwrite_existing);

		bar.update();
	}

	timer.Stop();
	cout << endl << endl << "Extraction complete. Took "
		<< timer.ElapsedSeconds() << " seconds"
		<< " to copy " << list->size() << " files" << endl << endl;

	return EXIT_SUCCESS;
}

vector<string> CmdProgram::SplitList(const string& arg)
{
	vector<string> list;
	if (arg == "")
		return list;

	istringstream strm(arg);
	string val;

	while (getline(strm, val, '|'))
		list.push_back(val);

	return list;
}