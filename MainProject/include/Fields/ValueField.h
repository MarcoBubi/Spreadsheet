#pragma once
#include "FieldBase.h"
#include <string>

class ValueField : public FieldBase
{
public:
	ValueField(const std::string value);
	~ValueField();

	void PrintValue() override;
	std::string GetValue() override;

private:
	std::string _value;
};
