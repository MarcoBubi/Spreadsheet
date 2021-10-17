#pragma once
#include <string>

class FieldBase
{
public:
	FieldBase() = default;
	virtual ~FieldBase() = default;

	virtual void PrintValue() = 0;
	virtual std::string GetValue() = 0;
protected:
private:
};
