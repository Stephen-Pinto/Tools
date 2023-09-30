#include "CrcCalculator.h"
#include <boost/crc.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

using namespace SyncServ;

uint CrcCalculator::CalculateCrc(const std::filesystem::path& file, const size_t bufferSize)
{
	boost::crc_32_type cc;
	std::ifstream input_fstream(file, std::ios_base::binary);

	if (input_fstream)
	{
		do
		{
			//Load buffer for checksum
			std::vector<char> buffer(bufferSize);
			input_fstream.read(buffer.data(), bufferSize);
			cc.process_bytes(buffer.data(), input_fstream.gcount());

		} while (input_fstream);
	}

	return cc.checksum();
}

uint CrcCalculator::CalculateCrc(const std::filesystem::path& file)
{
	return CalculateCrc(file, DefaultBufferSize);
}
