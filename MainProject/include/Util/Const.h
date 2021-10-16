#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Const
{
	static std::vector<std::string> SplitStringByDelimiter(std::string str, const char* delimiter)
	{
		size_t start;
		size_t end = 0;

		std::vector<std::string> stringsVector;

		while((start = str.find_first_not_of(delimiter, end)) != std::string::npos)
		{
			end = str.find(delimiter, start);
			stringsVector.push_back(str.substr(start, end - start));
		};

		return stringsVector;
	}

	static std::map<int, std::string> SplitStringWithIndex(std::string str, const char* delimiter)
	{
		size_t delimiterLength = 0;
		while (delimiter[delimiterLength] != '\0')
		{
			++delimiterLength;
		}

		std::map<int, std::string> stringsMap;
		size_t index = 0;
		size_t foundAt = 0;
		while ((foundAt = str.find(delimiter)) != std::string::npos)
		{
			stringsMap.insert(std::make_pair(index, str.substr(0, foundAt)));
			str = str.substr(foundAt + delimiterLength);
			++index;
		}

		if(str.size() > 0)
		{
			stringsMap.insert(std::pair<int, std::string>{ index, str });
		}

		return stringsMap;
	}
};
