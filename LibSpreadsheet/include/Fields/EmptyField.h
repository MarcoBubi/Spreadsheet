#pragma once
#include "IField.h"
#include <string>

class EmptyField : public IField
{
public:
	void PrintValue() override;
	std::string GetValue() override;
	IField* Clone() override;

private:
	EmptyField() = delete;
	explicit EmptyField(const std::string value);
	~EmptyField() = default;
	EmptyField(const EmptyField & other) = default;
	EmptyField& operator=(const EmptyField & other) = default;

	const std::string _value;

	friend class FieldFactory;
};
