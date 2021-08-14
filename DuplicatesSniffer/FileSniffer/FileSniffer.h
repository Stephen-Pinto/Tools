// DuplicateSniffer.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <chrono>
#include <cstdlib>    // for EXIT_SUCCESS, EXIT_FAILURE
#include <exception>  // for std::exception
#include <fstream>    // for std::ifstream
#include <ios>        // for std::ios_base, etc.
#include <iostream>   // for std::cerr, std::cout
#include <boost/crc.hpp>
#include <boost/program_options.hpp>
#include "flat_index_builder.h"

#ifndef PRIVATE_BUFFER_SIZE
#define PRIVATE_BUFFER_SIZE  1024
#endif

#define INTERVAL(start, end) ((end - start) / CLOCKS_PER_SEC)

// Global objects
std::streamsize const  buffer_size = PRIVATE_BUFFER_SIZE;
typedef std::chrono::high_resolution_clock chr_clock;

namespace po = boost::program_options;

int ExecuteTool(po::variables_map& vmap);

void FindDuplicates(po::variables_map& vmap, const std::vector<std::shared_ptr<file_info>>& flat_index);

void PrintFiles(po::variables_map& vmap, const std::vector<std::shared_ptr<file_info>>& flat_index);

void FindFiles(po::variables_map& vmap, const std::vector<std::shared_ptr<file_info>>& flat_index);

void FindExtension(po::variables_map& vmap, const std::vector<std::shared_ptr<file_info>>& flat_index);

void FindSizes(po::variables_map& vmap, const std::vector<std::shared_ptr<file_info>>& flat_index);

void FindFiles(po::variables_map& vmap, const std::vector<std::shared_ptr<file_info>>& flat_index);

// TODO: Reference additional headers your program requires here.
