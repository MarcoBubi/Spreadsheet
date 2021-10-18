#pragma once
#include "IField.h"
#include <string>

class EmptyField : public IField
{
public:
	EmptyField() = delete;
	explicit EmptyField(const std::string value);
	~EmptyField() = default;
	EmptyField(const EmptyField& other) = delete;
	EmptyField& operator=(const EmptyField& other) = delete;

	void PrintValue() override;
	std::string GetValue() override;

private:
	const std::string _value;
};
