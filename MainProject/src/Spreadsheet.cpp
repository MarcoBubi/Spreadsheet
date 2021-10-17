#include "Fields/FieldBase.h"
#include "Fields/FieldCoordinate.h"
#include "Spreadsheet.h"
#include "Util/Const.h"
#include <direct.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

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


FieldBase* Spreadsheet::operator[](const std::string& coordinate) const
{
	std::map<FieldCoordinate, FieldBase*>::const_iterator it;
	for (it = _fieldsMap.cbegin(); it != _fieldsMap.cend(); ++it)
	{
		if (it->first.coordinate == coordinate)
		{
			return it->second;
		}
	}

	return nullptr;
}

void Spreadsheet::ParseData()
{
	char workingDirectory[256];
	_getcwd(workingDirectory, 256);

	std::ifstream file(workingDirectory + Const::DATA_PATH + _path);
	std::string column;
	column = Const::INITIAL_COLUMN_LETTER;
	uint64_t index = 1;

	if(file.is_open() && file.good())
	{
		for(std::string line; std::getline(file, line);)
		{
			// pair - index, value
			std::map<uint64_t, std::string> stringsMap = Const::SplitStringWithIndex(line, _defaultTabulator);

			for (auto& entry : stringsMap)
			{
				FieldCoordinate coordinate{ column, index, column + std::to_string(index) }; // refactor!

				_fieldsMap.insert(std::make_pair(coordinate, _fieldFactory.Create(*this, entry.second)));
				IncrementColumn(column);
			}
			++index;
			column = Const::INITIAL_COLUMN_LETTER;
		}
	}

	file.close();
}

void Spreadsheet::PrintData()
{
	uint64_t currentRow = 1;
	std::map<FieldCoordinate, FieldBase*>::iterator it;
	for (it = _fieldsMap.begin(); it != _fieldsMap.end(); ++it)
	{
		if(currentRow < it->first.row)
		{
			std::cout << std::endl;
			currentRow++;
		}
		it->second->PrintValue();
		std::cout << "\t";
	}
}

std::string Spreadsheet::CalculateValueForField(std::string value) const
{
	const std::string delimiters{ "+-*/" };
	const char firstNegativeNumber = value[1];
	std::vector<std::string> fieldsVector = Const::SplitStringWithDelimiters(value.substr(1), delimiters);

	if(firstNegativeNumber == Const::SUBSTRACTION_SIGN)
	{
		fieldsVector.at(0) = firstNegativeNumber + fieldsVector.at(0);
	}

	for (std::string& entry : fieldsVector)
	{
		if ((entry.find_first_of(delimiters) != std::string::npos) || Const::IsNumber(entry))
		{
			continue;
		}

		std::vector<std::string> usedFields;
		entry = GetValueInternal(entry, usedFields);
	}
	CalculateField(fieldsVector);

	return fieldsVector[0];
}

std::string Spreadsheet::GetValueInternal(const std::string& fieldCoordinate, std::vector<std::string>& usedFields) const
{
	std::string value = GetValueForField(fieldCoordinate);
	if(!IsFormula(fieldCoordinate))
	{
		return value;
	}
	usedFields.push_back(fieldCoordinate);

	const std::string delimiters{ "+-*/" };
	std::vector<std::string> fieldsVector = Const::SplitStringWithDelimiters(value.substr(1), delimiters);

	for (std::string& entry : fieldsVector)
	{
		if (entry.empty())
		{
			return Const::HTAG_NAN_STRING;
		}

		if ((entry.find_first_of(delimiters) != std::string::npos) || Const::IsNumber(entry))
		{
			continue;
		}

		if (std::find(usedFields.begin(), usedFields.end(), entry) != usedFields.end())
		{
			return Const::HTAG_ERROR_STRING;
		}

		entry = GetValueInternal(entry, usedFields);
	}

	for (std::string& entry : fieldsVector)
	{
		if (entry == Const::HTAG_ERROR_STRING)
		{
			return entry;
		}

		if(entry == Const::HTAG_NAN_STRING)
		{
			return entry;
		}
	}

	CalculateField(fieldsVector);

	return fieldsVector[0];
}

std::string Spreadsheet::GetValueForField(const std::string& fieldCoordinate) const
{
	FieldBase* field = (*this)[fieldCoordinate];
	if(field == nullptr)
	{
		return Const::HTAG_NAN_STRING;
	}

	return field->GetValue();
}

void Spreadsheet::CalculateField(std::vector<std::string>& expression) const
{
	for (std::string& entry : expression)
	{
		if (entry == Const::HTAG_ERROR_STRING)
		{
			expression[0] = Const::HTAG_ERROR_STRING;
			return;
		}

		if(entry == Const::HTAG_NAN_STRING || entry.empty())
		{
			expression[0] = Const::HTAG_NAN_STRING;
			return;
		}
	}

	Const::CalculateExpression(expression);
}

bool Spreadsheet::IsFormula(const std::string& coordinate) const
{
	std::map<FieldCoordinate, FieldBase*>::const_iterator it;
	for (it = _fieldsMap.cbegin(); it != _fieldsMap.cend(); ++it)
	{
		if(it->first.coordinate == coordinate)
		{
			std::string value = it->second->GetValue();

			if(value != "")
			{
				return value.at(0) == Const::EQUAL_SIGN;
			}

		}
	}

	return false;
}

void Spreadsheet::IncrementColumn(std::string& column, int incrementValue)
{
	if(column.empty())
	{
		return;
	}

	size_t index = column.length() - 1;
	size_t columnValue = column.at(index);
	if(columnValue >= Const::FINAL_COLUMN_LETTER)
	{
		column.at(index) = Const::INITIAL_COLUMN_LETTER;
		column += Const::INITIAL_COLUMN_LETTER;
		return;
	}

	column.at(index) += incrementValue;
}
