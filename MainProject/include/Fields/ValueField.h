#pragma once
#include "IField.h"
#include <string>

class ValueField : public IField
{
public:
	ValueField() = delete;
	explicit ValueField(const std::string value);
	~ValueField() = default;
	ValueField(const ValueField& other) = delete;
	ValueField& operator=(const ValueField& other) = delete;

	void PrintValue() override;
	std::string GetValue() override;

private:
	const std::string _value;
};
