#pragma once
#include "FieldBase.h"
#include <string>

class EmptyField : public FieldBase
{
public:
	EmptyField(const std::string value);
	~EmptyField();

	void PrintValue() override;
	std::string GetValue() override;

private:
	std::string _value;
};
