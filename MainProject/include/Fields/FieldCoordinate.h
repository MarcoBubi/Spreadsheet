#pragma once
#include <string>

struct FieldCoordinate
{
	//FieldCoordinate(const std::string& colValue, const uint64_t& rowValue)
	//	: column{ colValue }
	//	, row{ rowValue }
	//	, coordinate{ colValue + std::to_string(rowValue) }
	//{
	//};

	//FieldCoordinate(const FieldCoordinate& other) = default;

	//FieldCoordinate(FieldCoordinate&& other)
	//	: column{ other.column }
	//	, row{ other.row }
	//	, coordinate{ other.coordinate}
	//{
	//};

	//bool operator==(const FieldCoordinate& other) const
	//{
	//	return (this->coordinate == other.coordinate);
	//}

	bool operator<(const FieldCoordinate& other)  const
	{
		return (row < other.row || (row == other.row && column < other.column));
	}

	std::string column;
	uint64_t row;
	std::string coordinate;
};
