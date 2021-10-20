#include "pch.h"

#include "Util/Const.h"
#include <algorithm>
#include <cassert>

namespace Const
{
	void MultiplyStrings(std::string& result, std::string first, std::string second)
	{
		assert(!first.empty() && !second.empty());
		assert(IsNumber(first) && IsNumber(second));

		bool isProductNegative = false;
		if (first[0] == '-')
		{
			first = first.substr(1);
			isProductNegative = !isProductNegative;
		}

		if (second[0] == '-')
		{
			second = second.substr(1);
			isProductNegative = !isProductNegative;
		}

		if (first == "0" || second == "0")
		{
			result = "0";
			return;
		}

		std::string product(first.size() + second.size(), 0);

		for (int i = first.size() - 1; i >= 0; i--)
		{
			for (int j = second.size() - 1; j >= 0; j--)
			{
				int n = (first[i] - '0') * (second[j] - '0') + product[i + j + 1];
				product[i + j + 1] = n % 10;
				product[i + j] += n / 10;
			}
		}

		for (char& i : product)
		{
			i += '0';
		}

		if (product[0] == '0')
		{
			product = product.substr(1);
		}

		if (isProductNegative)
		{
			product = "-" + product;
		}

		result = product;
	}

	void DivideStrings(std::string& result, std::string first, std::string second)
	{
		assert(!first.empty() && !second.empty());
		assert(IsNumber(first) && IsNumber(second));
		assert(!second.compare("0") == 0);

		result = std::to_string(std::stoi(first) / std::stoi(second));
	}

	void AddStrings(std::string& result, std::string first, std::string second)
	{
		assert(!first.empty() && !second.empty());
		assert(IsNumber(first) && IsNumber(second));

		bool isResultNegative = false;
		if (first[0] == '-' && second[0] == '-')
		{
			first = first.substr(1);
			second = second.substr(1);
			isResultNegative = true;
		}
		else if (first[0] == '-')
		{
			first = first.substr(1);
			if (IsSmaller(first, second))
			{
				SubstractStrings(result, second, first);
			}
			else
			{
				SubstractStrings(result, first, second);
				result = "-" + result;
			}

			return;
		}
		else if (second[0] == '-')
		{
			second = second.substr(1);
			if (IsSmaller(second, first))
			{
				SubstractStrings(result, first, second);
			}
			else
			{
				SubstractStrings(result, second, first);
				result = "-" + result;
			}
			return;
		}

		if (first.size() < second.size())
		{
			swap(first, second);
		}

		int j = first.size() - 1;
		for (int i = second.size() - 1; i >= 0; --i, --j)
		{
			first[j] += (second[i] - '0');
		}

		for (int i = first.size() - 1; i > 0; i--)
		{
			if (first[i] > '9')
			{
				int d = first[i] - '0';
				first[i - 1] = ((first[i - 1] - '0') + d / 10) + '0';
				first[i] = (d % 10) + '0';
			}
		}

		if (first[0] > '9')
		{
			std::string k;
			k += first[0];
			first[0] = ((first[0] - '0') % 10) + '0';
			k[0] = ((k[0] - '0') / 10) + '0';
			first = k + first;
		}

		result = first;

		if (isResultNegative)
		{
			result = "-" + result;
		}
	}

	void SubstractStrings(std::string& result, std::string first, std::string second)
	{
		assert(!first.empty() && !second.empty());
		assert(IsNumber(first) && IsNumber(second));

		if (first[0] == '-' && second[0] == '-')
		{
			AddStrings(result, first, second.substr(1));
			return;
		}
		else if (first[0] == '-')
		{
			AddStrings(result, first.substr(1), second);
			result = "-" + result;
			return;
		}
		else if (second[0] == '-')
		{
			AddStrings(result, first, second.substr(1));
			return;
		}

		if (first == "0")
		{
			result = "-" + second;
			return;
		}

		if (IsSmaller(first, second))
		{
			swap(first, second);
		}

		std::string substraction;
		int firstSize = first.length();
		int secondSize = second.length();

		reverse(first.begin(), first.end());
		reverse(second.begin(), second.end());

		int carry = 0;
		for (int i = 0; i < secondSize; i++)
		{
			int sub = (first[i] - '0') - (second[i] - '0') - carry;

			if (sub < 0)
			{
				sub = sub + 10;
				carry = 1;
			}
			else
			{
				carry = 0;
			}

			substraction.push_back(sub + '0');
		}

		for (int i = secondSize; i < firstSize; i++)
		{
			int sub = (first[i] - '0') - carry;

			if (sub < 0)
			{
				sub = sub + 10;
				carry = 1;
			}
			else
			{
				carry = 0;
			}

			substraction.push_back(sub + '0');
		}

		reverse(substraction.begin(), substraction.end());

		result = substraction;
		if (result.size() > 1)
		{
			while (result[0] == '0')
			{
				result = result.substr(1);
			}
		}
	}

	void CalculateAndMerge(StringsVector& expression, unsigned int index, BasicOperation operation)
	{
		assert(expression.size() > 2);
		assert(((index - 1) >= 0) && ((index + 1) <= expression.size()) && (index != 0) && (index != (expression.size() - 1)));

		const unsigned int pIndex = index - 1;
		const unsigned int nIndex = index + 1;

		operation(expression[pIndex], expression[pIndex], expression[nIndex]);
		expression.erase(expression.begin() + nIndex);
		expression.erase(expression.begin() + index);
	}

	void CalculateExpression(StringsVector& expression)
	{
		if (expression.size() == 1)
		{
			return;
		}

		assert(expression.size() % 2 == 1);

		const StringsVector prioDels{ "/", "*" };
		const StringsVector dels{ "+", "-" };

		auto result = std::find_first_of(
			expression.begin(), expression.end(), prioDels.begin(), prioDels.end());
		if (result != expression.end())
		{
			const unsigned int foundAt = std::distance(expression.begin(), result);

			const char operation = expression.at(foundAt)[0];
			if (operation == Const::MULTIPLICATION_SIGN)
			{
				CalculateAndMerge(expression, foundAt, MultiplyStrings);
				CalculateExpression(expression);
				return;
			}

			if (operation == Const::DIVISION_SIGN)
			{
				CalculateAndMerge(expression, foundAt, DivideStrings);
				CalculateExpression(expression);
				return;
			}
		}

		result = std::find_first_of(expression.begin(), expression.end(), dels.begin(), dels.end());
		if (result != expression.end())
		{
			const unsigned int foundAt = std::distance(expression.begin(), result);
			const char operation = expression.at(foundAt)[0];
			if (operation == Const::ADDITION_SIGN)
			{
				CalculateAndMerge(expression, foundAt, AddStrings);
				CalculateExpression(expression);
				return;
			}

			if (operation == Const::SUBSTRACTION_SIGN)
			{
				CalculateAndMerge(expression, foundAt, SubstractStrings);
				CalculateExpression(expression);
				return;
			}
		}
	}

	bool IsNumber(const std::string& str)
	{
		for (char const& c : str)
		{
			if (isdigit(c) == 0)
			{
				if (str[0] == SUBSTRACTION_SIGN)
				{
					continue;
				}
				return false;
			}
		}

		return true;
	}

	bool IsSmaller(const std::string& first, const std::string& second)
	{
		if (first[0] == SUBSTRACTION_SIGN && second[0] == SUBSTRACTION_SIGN)
		{
			return IsSmaller(second.substr(1), first.substr(1));
		}
		else if (first[0] == SUBSTRACTION_SIGN)
		{
			return true;
		}
		else if (second[0] == SUBSTRACTION_SIGN)
		{
			return false;
		}

		int firstSize = first.length();
		int secondSize = second.length();

		if (firstSize < secondSize)
		{
			return true;
		}

		if (secondSize < firstSize)
		{
			return false;
		}

		for (int i = 0; i < firstSize; ++i)
		{
			if (first[i] < second[i])
			{
				return true;
			}

			if (first[i] > second[i])
			{
				return false;
			}
		}

		return false;
	}

	bool IsFormula(const std::string& value)
	{
		if (value.empty())
		{
			return false;
		}
		return value[0] == EQUAL_SIGN;
	}

	StringsVector SplitStringExpression(std::string str, const std::string& delimiters)
	{
		std::vector<std::string> stringsVector;
		size_t beg, next = 0;
		std::string dataString;
		while ((beg = str.find_first_of(delimiters, next)) != std::string::npos)
		{
			if (beg == next)
			{
				dataString = str.substr(beg, 1);
				if (str[beg] == Const::SUBSTRACTION_SIGN)
				{
					if (!stringsVector.empty())
					{
						std::string lastAddedElement = stringsVector.back();
						bool hasDelimiter
							= lastAddedElement.find_first_of(delimiters) != std::string::npos;

						if (hasDelimiter && lastAddedElement.size() == 1)
						{
							next = str.find_first_of(delimiters, 1);

							if (next != std::string::npos)
							{
								dataString = str.substr(beg, next);
							}
							else
							{
								continue;
							}
						}
					}
					else
					{
						next = str.find_first_of(delimiters, 1);
						dataString = str.substr(beg, next);
					}
				}

				stringsVector.push_back(dataString);
				str = str.substr(beg + dataString.size());
				next = 0;
				continue;
			}

			dataString = str.substr(next, beg - next);

			if (!dataString.empty())
			{
				stringsVector.push_back(dataString);
				str = str.substr(beg);
				next = str.find_first_of(delimiters, beg - dataString.size());
				continue;
			}
		}

		if (str.size() > 0)
		{
			stringsVector.push_back(str.substr(0));
		}

		return stringsVector;
	}

	std::map<uint64_t, std::string> SplitStringWithIndex(std::string str, const std::string& delimiter)
	{
		std::map<uint64_t, std::string> stringsMap;
		size_t index = 0;
		size_t foundAt = 0;
		while ((foundAt = str.find(delimiter)) != std::string::npos)
		{
			stringsMap.insert(std::make_pair(index, str.substr(0, foundAt)));
			str = str.substr(foundAt + delimiter.size());
			++index;
		}

		if (str.size() > 0)
		{
			stringsMap.insert(std::pair<uint64_t, std::string>{ index, str });
		}

		return stringsMap;
	}
};
