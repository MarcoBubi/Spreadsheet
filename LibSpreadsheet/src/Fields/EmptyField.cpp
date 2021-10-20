#include "pch.h"

#include "Fields/EmptyField.h"
#include <iostream>

EmptyField::EmptyField(const std::string value)
	: _value{ value }
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

IField* EmptyField::Clone()
{
	return new EmptyField(*this);
}
