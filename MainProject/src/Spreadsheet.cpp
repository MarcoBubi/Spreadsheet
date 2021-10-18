#include "Fields/IField.h"
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

Spreadsheet::~Spreadsheet()
{
	FieldMap::iterator it;
	for (it = _fieldsMap.begin(); it != _fieldsMap.end(); ++it)
	{
		delete it->second;
	}
}

IField* Spreadsheet::operator[](const std::string& coordinate) const
{
	FieldMap::const_iterator it;
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


	if (file.is_open() && file.good())
	{
		std::string column;
		column = Const::INITIAL_COLUMN_LETTER;
		uint64_t index = 1;

		for (std::string line; std::getline(file, line);)
		{
			std::map<uint64_t, std::string> stringsMap
				= Const::SplitStringWithIndex(line, Const::DEFAULT_TABULATOR);

			for (auto& entry : stringsMap)
			{
				FieldCoordinate coordinate{ column, index, column + std::to_string(index) };

				_fieldsMap.insert(
					std::make_pair(coordinate, _fieldFactory.Create(*this, entry.second)));
				IncrementColumn(column);
			}
			++index;
			column = Const::INITIAL_COLUMN_LETTER;
		}
	}

	file.close();
}

std::string Spreadsheet::GetValueForField(const std::string& fieldCoordinate) const
{
	IField* field = (*this)[fieldCoordinate];
	if (field == nullptr)
	{
		return Const::HTAG_NAN_STRING;
	}

	return field->GetValue();
}

std::string Spreadsheet::CalculateValueForField(std::string value) const
{
	std::vector<std::string> fieldsVector
		= Const::SplitStringExpression(value.substr(1), Const::BASIC_OPERATORS);

	for (std::string& entry : fieldsVector)
	{
		bool isDelimiter
			= entry.find_first_of(Const::BASIC_OPERATORS) != std::string::npos && entry.size() == 1;
		if (isDelimiter || Const::IsNumber(entry))
		{
			continue;
		}

		std::vector<std::string> usedFields;
		if (entry.at(0) == (Const::SUBSTRACTION_SIGN))
		{
			entry = GetValueInternal(entry.substr(1), usedFields);
			entry = Const::SUBSTRACTION_SIGN + entry;
			continue;
		}

		entry = GetValueInternal(entry, usedFields);
	}
	CalculateField(fieldsVector);

	return fieldsVector[0];
}

std::string Spreadsheet::GetValueInternal(
	const std::string& fieldCoordinate, std::vector<std::string>& usedFields) const
{
	std::string value = GetValueForField(fieldCoordinate);
	if (!IsFormula(fieldCoordinate))
	{
		return value;
	}
	usedFields.push_back(fieldCoordinate);

	std::vector<std::string> fieldsVector
		= Const::SplitStringExpression(value.substr(1), Const::BASIC_OPERATORS);

	for (std::string& entry : fieldsVector)
	{
		if (entry.empty())
		{
			return Const::HTAG_NAN_STRING;
		}

		if ((entry.find_first_of(Const::BASIC_OPERATORS) != std::string::npos)
			|| Const::IsNumber(entry))
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

		if (entry == Const::HTAG_NAN_STRING)
		{
			return entry;
		}
	}

	CalculateField(fieldsVector);

	return fieldsVector[0];
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

		if (entry == Const::HTAG_NAN_STRING || entry.empty())
		{
			expression[0] = Const::HTAG_NAN_STRING;
			return;
		}
	}

	Const::CalculateExpression(expression);
}

void Spreadsheet::IncrementColumn(std::string& column, int incrementValue)
{
	if (column.empty())
	{
		return;
	}

	size_t index = column.length() - 1;
	size_t columnValue = column.at(index);
	if (columnValue >= Const::FINAL_COLUMN_LETTER)
	{
		column.at(index) = Const::INITIAL_COLUMN_LETTER;
		column += Const::INITIAL_COLUMN_LETTER;
		return;
	}

	column.at(index) += incrementValue;
}

bool Spreadsheet::IsFormula(const std::string& coordinate) const
{
	FieldMap::const_iterator it;
	for (it = _fieldsMap.cbegin(); it != _fieldsMap.cend(); ++it)
	{
		if (it->first.coordinate == coordinate)
		{
			std::string value = it->second->GetValue();

			if (!value.empty())
			{
				return value.at(0) == Const::EQUAL_SIGN;
			}
		}
	}

	return false;
}

std::ostream& operator<<(std::ostream& os, const Spreadsheet& spreadsheet)
{
	uint64_t currentRow = 1;
	FieldMap::const_iterator it;
	for (it = spreadsheet._fieldsMap.cbegin(); it != spreadsheet._fieldsMap.cend(); ++it)
	{
		if (currentRow < it->first.row)
		{
			std::cout << std::endl;
			currentRow++;
		}
		it->second->PrintValue();
		std::cout << "\t";
	}

	return os;
}
