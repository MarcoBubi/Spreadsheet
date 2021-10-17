#pragma once
#include "FieldBase.h"
#include <string>

class Spreadsheet;

class FormulaField : public FieldBase
{
public:
	FormulaField(const Spreadsheet& spreadSheet, const std::string value);
	~FormulaField();

	void PrintValue() override;
	std::string GetValue() override;

private:
	const Spreadsheet& _spreadSheet;
	std::string _value;
};
