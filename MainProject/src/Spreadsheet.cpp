#include "Util/Const.h"
#include "Spreadsheet.h"
#include <fstream>
#include <map>
#include <string>

#include "Fields/FieldBase.h"

Spreadsheet::Spreadsheet(const std::string path)
	: _path{ path }
	, _fieldFactory{ FieldFactory() }
{
}

Spreadsheet::Spreadsheet(const Spreadsheet& other)
	: _path { other._path }
	, _fieldsMap { other._fieldsMap }
	, _fieldFactory { other._fieldFactory }
{
}

Spreadsheet::Spreadsheet(Spreadsheet&& other)
	: _path{ other._path }
	, _fieldsMap{ other._fieldsMap }
	, _fieldFactory { other._fieldFactory }
{
}

Spreadsheet::~Spreadsheet()
{
	//TODO: dealocate map etc.
}

Spreadsheet& Spreadsheet::operator=(const Spreadsheet& other)
{
	_path = other._path;
	_fieldsMap = other._fieldsMap;
	_fieldFactory = other._fieldFactory;

	return *this;
}

Spreadsheet& Spreadsheet::operator=(Spreadsheet&& other)
{
	_path = other._path;
	_fieldsMap = other._fieldsMap;
	_fieldFactory = other._fieldFactory;

	return *this;
}

void Spreadsheet::ParseData()
{
	std::ifstream file(_path);
	char column = *_defaultColumnLetter;

	if(file.is_open() && file.good())
	{
		for(std::string line; std::getline(file, line);)
		{
			// pair - index, value
			std::map<int, std::string> stringsMap = Const::SplitStringWithIndex(line, _defaultTabulator);

			for (auto& entry : stringsMap)
			{
				if(entry.second != "")
				{
					std::string coordinate = column + std::to_string(entry.first + 1);
					_fieldsMap.insert(
						std::make_pair(
							coordinate,
							_fieldFactory.Create(coordinate, entry.second)));
				}
			}
			++column;
		}
	}

	file.close();
}
