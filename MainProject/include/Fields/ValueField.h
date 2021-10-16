#pragma once
#include "FieldBase.h"
#include <string>

class ValueField : public FieldBase
{
public:
	ValueField(const std::string coordinate, const std::string value);
	~ValueField();

private:

};
