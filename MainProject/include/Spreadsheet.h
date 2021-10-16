#pragma once
#include "Fields/FieldBase.h"
#include "Fields/FieldFactory.h"
#include <map>
#include <string>

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

	void ParseData();

private:
	const char* _defaultColumnLetter = "A";
	const char* _defaultTabulator = "\t";
	const char* _defaultNewline = "\n";

	FieldFactory _fieldFactory;

	std::string _path;
	std::map<std::string, FieldBase*> _fieldsMap;
};
