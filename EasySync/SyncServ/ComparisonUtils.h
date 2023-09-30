#pragma once

#include <string>

namespace SyncServ::Utils
{
	using namespace std;

	class ComparisonUtils
	{
		
	public:

		bool WildcardMatch(const vector<string>& patterns, char const* str)
		{
			for (auto pattern : patterns)
			{
				if (WildcardMatch(pattern.c_str(), str))
					return true;				
			}

			return false;
		}

		bool WildcardMatch(char const* pattern, char const* str)
		{
			for (; *pattern != '\0'; ++pattern)
			{
				switch (*pattern)
				{
				case '?':
					if (*str == '\0')
						return false;
					++str;
					break;
				case '*': {
					if (pattern[1] == '\0')
						return true;
					size_t max = strlen(str);
					for (size_t i = 0; i < max; i++)
						if (WildcardMatch(pattern + 1, str + i))
							return true;
					return false;
				}
				default:
					if (toupper(*str) != toupper(*pattern))
						return false;
					++str;
				}
			}
			return *str == '\0';
		}
	};
}