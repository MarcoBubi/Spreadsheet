#pragma once
#include <string>

class FieldBase;
class Spreadsheet;

class FieldFactory
{
public:
	FieldBase* Create(const Spreadsheet& spreadSheet, const std::string value) const;

private:
	FieldFactory() = default;

	friend class Spreadsheet;
};
