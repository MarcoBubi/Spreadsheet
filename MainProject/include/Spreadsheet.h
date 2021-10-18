#pragma once
#include "Fields/IField.h"
#include "Fields/FieldCoordinate.h"
#include "Fields/FieldFactory.h"
#include <map>
#include <string>
#include <vector>

using FieldMap = std::map<FieldCoordinate, IField*>;

class Spreadsheet
{
public:
	Spreadsheet() = delete;
	explicit Spreadsheet(const std::string path);
	~Spreadsheet();
	Spreadsheet(const Spreadsheet& other) = delete;
	Spreadsheet(Spreadsheet&& other) = delete;
	Spreadsheet& operator=(const Spreadsheet& other) = delete;
	Spreadsheet& operator=(Spreadsheet&& other) = delete;

	IField* operator[](const std::string& coordinate) const;

	void ParseData();

	std::string GetValueForField(const std::string& fieldCoordinate) const;
	std::string CalculateValueForField(std::string value) const;

	friend std::ostream& operator<<(std::ostream& os, const Spreadsheet& dt);

private:
	std::string GetValueInternal(
		const std::string& fieldCoordinate, std::vector<std::string>& usedFields) const;

	void CalculateField(std::vector<std::string>& expression) const;
	void IncrementColumn(std::string& column, int incrementValue = 1);

	bool IsFormula(const std::string& coordinate) const;

	const std::string _path;
	const FieldFactory _fieldFactory;
	FieldMap _fieldsMap;
};
