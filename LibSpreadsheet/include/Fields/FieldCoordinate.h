#pragma once
#include <string>

struct FieldCoordinate
{
	bool operator<(const FieldCoordinate& other) const
	{
		return (row < other.row || (row == other.row && column < other.column));
	}

	const std::string column;
	const uint64_t row;
	const std::string coordinate;
};
