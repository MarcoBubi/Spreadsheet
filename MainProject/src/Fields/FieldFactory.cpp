#include "Fields/FieldFactory.h"
#include "Fields/FormulaField.h"
#include "Fields/ValueField.h"
#include <cctype>
#include <stdexcept>

FieldBase* FieldFactory::Create(const std::string coordinate, const std::string value) const
{
	const char firstElement = value.at(0);

	if (firstElement == *_equalSign)
	{
		FormulaField* field = new FormulaField(coordinate, value);
		return field;
	}
	else if(isdigit(firstElement))
	{
		ValueField* field = new ValueField(coordinate, value);
		return field;
	}
	else
	{
		throw std::invalid_argument("The value given for the FieldFactory is not valid!");
	}
}
