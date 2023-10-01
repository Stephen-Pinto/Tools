#include <iostream>
#include <fstream>
#include "TimerUtil.h"
#include "CmdProgram.h"
#include "FileInfo.h"
#include "DuplicateFinder.h"
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

using namespace boost::chrono;
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
		("exclude,e", value<string>(), "Exclude specified files or directories. Split multiple values with '|'.")
		("out,o", value<string>(), "Set output file for results.");

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

int CmdProgram::ExecuteTool(variables_map& vmap)
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

	boost::thread t{ 
		[&] 
		{
			while(dupFinder.AnalayzedFiles == 0)
				boost::this_thread::sleep_for(milliseconds{ 100 });

			ldouble perc;

			while (dupFinder.AnalayzedSize > dupFinder.ProcessedSize)
			{
				perc = (ldouble)dupFinder.ProcessedSize / (ldouble)dupFinder.AnalayzedSize;
				cout << "Completed: " << (int)(perc * 100) << "%" << endl;
				boost::this_thread::sleep_for(milliseconds{ 100 });
			}
		} 
	};

	auto duplicates = dupFinder.GetDuplicates();

	timer.Stop();

	cout << "Took "
		<< timer.ElapsedSeconds() << " seconds"
		<< " to index " << dupFinder.ProcessedFiles << " files" << endl;

	return PrintList(vmap, duplicates);
}

int CmdProgram::PrintList(variables_map& vmap, DuplicateFileList_SP list)
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

vector<string> CmdProgram::SplitList(string arg)
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

void PrintFiles(variables_map& vmap, const vector<FileInfo_SP>& flatIndex)
{
	ofstream ostrm;

	if (vmap.count("out"))
	{
		string outfile = vmap["out"].as<string>();
		ostrm.open(outfile);
	}

	for (auto const& item : flatIndex)
	{
		if (!ostrm.is_open())
			cout << item->Path << endl;
		else
			ostrm << item->Path << endl;
	}

	if (ostrm.is_open())
	{
		ostrm.flush();
		ostrm.close();
		cout << "Successfully wrote to file";
	}
}
