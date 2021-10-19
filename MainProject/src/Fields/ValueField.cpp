#include "Fields/ValueField.h"
#include <iostream>

ValueField::ValueField(const std::string value)
	: _value{ value }
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

IField* ValueField::Clone()
{
	return new ValueField(*this);
}
