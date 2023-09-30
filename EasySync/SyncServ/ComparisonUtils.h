#pragma once

#include <string>

namespace SyncServ::Utils
{
	using namespace std;

	class ComparisonUtils
	{
		
	public:

		bool wildcardMatch(char const* needle, char const* haystack)
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
						if (wildcardMatch(needle + 1, haystack + i))
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
	};
}