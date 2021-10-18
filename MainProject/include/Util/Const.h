#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

using basicOperation = void (*)(std::string& result, std::string first, std::string second);

namespace Const
{
	void MultiplyStrings(std::string& result,  std::string first, std::string second);
	void DivideStrings(std::string& result,  std::string first, std::string second);
	void AddStrings(std::string& result,  std::string first, std::string second);
	void SubstractStrings(std::string& result,  std::string first, std::string second);

	void CalculateAndMerge(std::vector<std::string>& expressionVector, unsigned int targetIndex, basicOperation operation);
	void CalculateExpression(std::vector<std::string>& expressionVector);

	bool IsNumber(const std::string& str);
	bool IsSmaller(const std::string& first, const std::string& second);

	int GetIndexOfDelimiter(std::string str, std::string const delimiters);

	std::vector<std::string> SplitStringExpression(std::string str, const std::string& delimiters);
	std::vector<std::string> SplitStringWithDelimiters(std::string str, const std::string& delimiters);
	std::vector<std::string> SplitStringByDelimiter(std::string str, const std::string& delimiter);
	std::map<uint64_t, std::string> SplitStringWithIndex(std::string str, const std::string& delimiter);

	const std::string HTAG_NAN_STRING = "#NAN";
	const std::string HTAG_ERROR_STRING = "#ERROR";
	const std::string DATA_PATH = "\\data\\";
	const std::string COLON_SIGN = ":";
	const std::string BASIC_OPERATORS{ "*/+-" };

	const char MULTIPLICATION_SIGN = '*';
	const char DIVISION_SIGN = '/';
	const char ADDITION_SIGN = '+';
	const char SUBSTRACTION_SIGN = '-';
	const char EQUAL_SIGN = '=';
	const char INITIAL_COLUMN_LETTER = 'A';
	const char FINAL_COLUMN_LETTER = 'Z';
};
