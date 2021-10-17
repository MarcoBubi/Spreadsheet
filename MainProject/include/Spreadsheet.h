#pragma once
#include "Fields/FieldBase.h"
#include "Fields/FieldCoordinate.h"
#include "Fields/FieldFactory.h"
#include <map>
#include <string>
#include <vector>

class Spreadsheet
{
public:
	Spreadsheet() = delete;
	explicit Spreadsheet(const std::string path);
	Spreadsheet(const Spreadsheet& other);
	Spreadsheet(Spreadsheet&& other);
	~Spreadsheet();

	Spreadsheet& operator=(const Spreadsheet& other);
	Spreadsheet& operator=(Spreadsheet&& other);

	FieldBase* operator[](const std::string& coordinate) const;

	void ParseData();
	void PrintData();

	std::string GetValueForField(const std::string& fieldCoordinate) const;
	std::string CalculateValueForField(std::string value) const;

private:
	std::string GetValueInternal(const std::string& fieldCoordinate, std::vector<std::string>& usedFields) const;

	void CalculateField(std::vector<std::string>& expression) const;
	void IncrementColumn(std::string& column, int incrementValue = 1);

	bool IsFormula(const std::string& coordinate) const;

	const char* _defaultTabulator = "\t";
	const char* _defaultNewline = "\n";

	FieldFactory _fieldFactory;

	std::string _path;
	std::map<FieldCoordinate, FieldBase*> _fieldsMap;
};
