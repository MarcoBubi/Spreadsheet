#include "Spreadsheet.h"
#include "Fields/FormulaField.h"
#include "Util/Const.h"
#include <iostream>
#include <string>

FormulaField::FormulaField(const Spreadsheet& spreadSheet, const std::string value)
	: _spreadSheet{ spreadSheet }
	, _value{ value }
{
}

void FormulaField::PrintValue()
{
	std::string value = _spreadSheet.CalculateValueFromString(_value);
	std::cout << value;
}

std::string FormulaField::GetValue()
{
	return _value;
}

IField* FormulaField::Clone()
{
	return new FormulaField(*this);
}
