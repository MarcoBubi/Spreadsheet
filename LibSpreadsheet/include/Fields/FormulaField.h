#pragma once
#include "IField.h"
#include <string>

class Spreadsheet;

class FormulaField : public IField
{
public:
	void PrintValue() override;
	std::string GetValue() override;
	IField* Clone() override;

private:
	FormulaField() = delete;
	FormulaField(const Spreadsheet& spreadSheet, const std::string value);
	~FormulaField() = default;
	FormulaField(const FormulaField & other) = default;
	FormulaField& operator=(const FormulaField & other) = default;

	const Spreadsheet& _spreadSheet;
	const std::string _value;

	friend class FieldFactory;
};
