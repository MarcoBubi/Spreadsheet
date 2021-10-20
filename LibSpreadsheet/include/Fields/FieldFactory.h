#pragma once
#include <string>

class IField;
class Spreadsheet;

class FieldFactory
{
public:
	IField* Create(const Spreadsheet& spreadSheet, const std::string value) const;

private:
	FieldFactory() = default;

	friend class Spreadsheet;
};
