#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

using BasicOperation = void (*)(std::string& result, std::string first, std::string second);
using StringsVector = std::vector<std::string>;

namespace Const
{
	void MultiplyStrings(std::string& result, std::string first, std::string second);
	void DivideStrings(std::string& result, std::string first, std::string second);
	void AddStrings(std::string& result, std::string first, std::string second);
	void SubstractStrings(std::string& result, std::string first, std::string second);

	void CalculateAndMerge(StringsVector& expression, unsigned int index, BasicOperation operation);
	void CalculateExpression(StringsVector& expression);

	bool IsNumber(const std::string& str);
	bool IsSmaller(const std::string& first, const std::string& second);
	bool IsFormula(const std::string& value);

	StringsVector SplitStringExpression(std::string str, const std::string& delimiters);
	std::map<uint64_t, std::string> SplitStringWithIndex(std::string str, const std::string& delimiter);

	const std::string HTAG_NAN_STRING = "#NAN";
	const std::string HTAG_ERROR_STRING = "#ERROR";
	const std::string DATA_PATH = "\\data\\";
	const std::string COLON_SIGN = ":";
	const std::string BASIC_OPERATORS{ "*/+-" };
	const std::string DEFAULT_TABULATOR = "\t";
	const std::string DEFAULT_NEWLINE = "\n";

	const char MULTIPLICATION_SIGN = '*';
	const char DIVISION_SIGN = '/';
	const char ADDITION_SIGN = '+';
	const char SUBSTRACTION_SIGN = '-';
	const char EQUAL_SIGN = '=';
	const char INITIAL_COLUMN_LETTER = 'A';
	const char FINAL_COLUMN_LETTER = 'Z';
};
