#include "Fields/ValueField.h"
#include <iostream>

ValueField::ValueField(const std::string value)
	: FieldBase()
	, _value { value }
{
}

ValueField::~ValueField()
{

}

void ValueField::PrintValue()
{
	std::cout << _value;
}

std::string ValueField::GetValue()
{
	return _value;
}
