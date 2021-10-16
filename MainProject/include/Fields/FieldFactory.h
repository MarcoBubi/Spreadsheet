#pragma once
#include <string>

class FieldBase;

class FieldFactory
{
public:
	FieldBase* Create(const std::string coordinate, const std::string value) const;

private:
	FieldFactory() = default;

	const char* _equalSign = "=";

	friend class Spreadsheet;
};
