#pragma once
#include "IField.h"
#include <string>

class ValueField : public IField
{
public:
	void PrintValue() override;
	std::string GetValue() override;
	IField* Clone() override;

private:
	ValueField() = delete;
	explicit ValueField(const std::string value);
	~ValueField() = default;
	ValueField(const ValueField & other) = default;
	ValueField& operator=(const ValueField & other) = default;

	const std::string _value;

	friend class FieldFactory;
};
