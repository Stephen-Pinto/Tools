#include <chrono>
#ifndef PRIVATE_BUFFER_SIZE
#define PRIVATE_BUFFER_SIZE  1024
#endif

#define INTERVAL(start, end) ((end - start) / CLOCKS_PER_SEC)

// Global objects
std::streamsize const  buffer_size = PRIVATE_BUFFER_SIZE;
typedef std::chrono::high_resolution_clock chr_clock;

using namespace std;

void Test3()
{
	flat_index_builder builder("D:\\Resources\\Books");

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

	return;
}

void Test2()
{
	flat_index_builder builder("D:");

	auto start = chr_clock::now();

	auto index = builder.build_index();

	std::cout
		<< "Took "
		<< std::chrono::duration_cast<std::chrono::seconds>(chr_clock::now() - start).count() << " seconds"
		<< " for " << index.size() << " files";

	/*for (auto const& item : index)
	{
		std::cout << item->full_path << std::endl;
	}*/
}

int Test1(int argc, char const * argv[])
{
	try
	{
		boost::crc_32_type  result;

		for (int i = 1; i < argc; ++i)
		{
			std::ifstream  ifs(argv[i], std::ios_base::binary);

			if (ifs)
			{
				do
				{
					char  buffer[buffer_size];

					ifs.read(buffer, buffer_size);
					result.process_bytes(buffer, ifs.gcount());

				} while (ifs);
			}
			else
			{
				std::cerr << "Failed to open file '" << argv[i] << "'." << std::endl;
			}
		}

		std::cout << std::hex << std::uppercase << result.checksum() << std::endl;
		return EXIT_SUCCESS;
	}
	catch (std::exception &e)
	{
		std::cerr << "Found an exception with '" << e.what() << "'." << std::endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "Found an unknown exception." << std::endl;
		return EXIT_FAILURE;
	}

	cout << "Hello CMake." << endl;
	return 0;
}