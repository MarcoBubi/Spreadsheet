#pragma once
#include "IField.h"
#include <string>

class ValueField : public IField
{
public:
	ValueField() = delete;
	explicit ValueField(const std::string value);
	~ValueField() = default;
	ValueField(const ValueField& other) = default;
	ValueField& operator=(const ValueField& other) = default;

	void PrintValue() override;
	std::string GetValue() override;
	IField* Clone() override;

private:
	const std::string _value;
};
