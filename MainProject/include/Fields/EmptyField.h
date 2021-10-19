#pragma once
#include "IField.h"
#include <string>

class EmptyField : public IField
{
public:
	EmptyField() = delete;
	explicit EmptyField(const std::string value);
	~EmptyField() = default;
	EmptyField(const EmptyField& other) = default;
	EmptyField& operator=(const EmptyField& other) = default;

	void PrintValue() override;
	std::string GetValue() override;
	IField* Clone() override;

private:
	const std::string _value;
};
