#include "Spreadsheet.h"
#include "Fields/EmptyField.h"
#include "Fields/FieldFactory.h"
#include "Fields/FormulaField.h"
#include "Fields/ValueField.h"
#include "Util/Const.h"
#include <cctype>
#include <stdexcept>

FieldBase* FieldFactory::Create(const Spreadsheet& spreadSheet, const std::string value) const
{
	if (value.empty())
	{
		return new EmptyField(value);
	}

	const char firstElement = value.at(0);
	if (firstElement == Const::EQUAL_SIGN)
	{
		return new FormulaField(spreadSheet, value);
	}

	if(isdigit(firstElement))
	{
		return new ValueField(value);
	}

	throw std::invalid_argument("The value given for the FieldFactory is not valid!");
}
