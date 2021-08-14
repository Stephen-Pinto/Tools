// DuplicateSniffer.cpp : Defines the entry point for the application.
//

#include "DuplicateSniffer.h"
#include <boost/crc.hpp>
#include <cstdlib>    // for EXIT_SUCCESS, EXIT_FAILURE
#include <exception>  // for std::exception
#include <fstream>    // for std::ifstream
#include <ios>        // for std::ios_base, etc.
#include <iostream>   // for std::cerr, std::cout

#include "flat_index_builder.h"
#include "duplicate_index_builder.h"
#include "Test.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int ExecuteTool(po::variables_map& vmap);

int main(int argc, char** argv)
{
	po::options_description sniffer_options("Duplicate sniffer options");
	sniffer_options.add_options()
		("help", "produces help document")
		("version,v", "prints version string")
		("root", po::value<std::string>()->required(), "set root folder for sniffer")
		("out", po::value<std::string>(), "set output file for results")
		("maxs", po::value<int>(), "(In bytes) Files above this size will be ignored");

	po::variables_map vmap;

	try
	{
		/*po::command_line_parser parser{ argc, argv };
		parser.options(sniffer_options).allow_unregistered().style(
			po::command_line_style::default_style |
			po::command_line_style::allow_slash_for_short);
		po::parsed_options parsed_options = parser.run();*/

		po::store(po::parse_command_line(argc, argv, sniffer_options), vmap);
		po::notify(vmap);
	}
	catch (exception& exp)
	{
		//If not help or version then notify the user about the error 
		if(!vmap.count("help") && !vmap.count("version"))
			std::cerr << "ERROR: " << exp.what() << std::endl;
	}

	//Verify if the mandatory arguments are provided if not then show help and exit
	if (vmap.count("help") || vmap.count("root") == 0)
	{
		std::cout << sniffer_options;
		return EXIT_FAILURE;
	}

	//If all is ok then execute the tool
	auto exit_code = ExecuteTool(vmap);

	std::cout << std::endl << "Program complete... Press enter to continue";
	std::getchar();
	return exit_code;
}

int ExecuteTool(po::variables_map& vmap)
{
	flat_index_builder builder(vmap["root"].as<string>());

	//Building index
	auto start = chr_clock::now();

	auto index = builder.build_index();

	std::cout
		<< "Took "
		<< std::chrono::duration_cast<std::chrono::seconds>(chr_clock::now() - start).count() << " seconds"
		<< " to index " << index.size() << " files" << std::endl;

	duplicate_index_builder ident_grps(index);

	//Scanning for duplicates
	start = chr_clock::now();

	auto dup_grp_index = ident_grps.get_duplicates();

	std::cout
		<< "Took "
		<< std::chrono::duration_cast<std::chrono::seconds>(chr_clock::now() - start).count() << " seconds"
		<< " to scan " << index.size() << " files" << std::endl;

	return 0;

	for (auto const& item : dup_grp_index)
	{
		if (item.file_list.size() > 1)
		{
			std::cout << std::endl << "+++++++++++++++++++++++++++++++++++" << std::endl << std::endl;

			for (auto const& file : item.file_list)
			{
				std::cout << file->full_path << std::endl;
			}
		}
	}

	return EXIT_SUCCESS;
}