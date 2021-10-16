#pragma once
#include "FieldBase.h"
#include <string>

class FormulaField : public FieldBase
{
public:
	FormulaField(const std::string coordinate, const std::string value);
	~FormulaField();

private:

};
