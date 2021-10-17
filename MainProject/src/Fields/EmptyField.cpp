#include "Fields/EmptyField.h"
#include <iostream>

EmptyField::EmptyField(const std::string value)
	: FieldBase()
	, _value{ value }
{

}

EmptyField::~EmptyField()
{

}

void EmptyField::PrintValue()
{
	std::cout << _value;
}

std::string EmptyField::GetValue()
{
	return _value;
}
