#include <iostream>
#include <fstream>
#include "TimerUtil.h"
#include "CmdProgram.h"
#include "FileInfo.h"
#include "DuplicateFinder.h"

using namespace boost::program_options;
using namespace std;
using namespace SyncServ;
using namespace EasySync;

int CmdProgram::Run(int argc, char** argv)
{
	options_description options("Duplicate sniffer options");
	options.add_options()
		("help", "produces this help document")
		("version,v", "prints version string")
		("root,r", value<string>()->required(), "sets the root folder for sniffer")
		("include,i", value<vector<string>>(), "look for specified files and directories only")
		("exclude,e", value<vector<string>>(), "exclude specified files or directories")
		("out", value<string>(), "set output file for results");

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
		criteria.HavingNames = vmap["include"].as<vector<string>>();

	if (vmap.count("exclude") > 0)
		criteria.HavingNames = vmap["exclude"].as<vector<string>>();

	DuplicateFinder dupFinder;

	//Building index
	TimerUtil timer;
	timer.Start();

	auto duplicates = dupFinder.GenerateDuplicateList(criteria);

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
			cout << "File: " << val[0]->Path
				<< " Crc: " << val[0]->Crc
				<< endl;
		}
		else
		{
			cout << "Crc: " << key << " :" << endl;
			for (auto item : val)
			{
				cout << "File: " << val[0]->Path
					<< endl;
			}
			cout << endl;
		}
	}

	if (ofstrm != nullptr)
	{
		ofstrm->close();
		delete(ofstrm);
	}

	return EXIT_SUCCESS;
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
