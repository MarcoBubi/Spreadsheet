#include "pch.h"

#include "gtest/gtest.h"
#include "Fields/EmptyField.h"
#include "Fields/FormulaField.h"
#include "Fields/ValueField.h"
#include "Spreadsheet.h"
#include "Util/Const.h"
#include <iostream>
#include <string>

namespace SpreadsheetTests
{
	TEST(SpreadsheetParsing, NoParsingFileFoundAssert)
	{
		Spreadsheet spreadsheet("IncorrectFile.txt");
		ASSERT_DEATH(spreadsheet.ParseData(), "");
	}

	TEST(SpreadsheetParsing, CorrectParsingWithCircularReference)
	{
		Spreadsheet spreadsheet("CircularReferenceSpreadsheet.txt");
		spreadsheet.ParseData();

		testing::internal::CaptureStdout();
		std::cout << spreadsheet;
		std::string output = testing::internal::GetCapturedStdout();

		EXPECT_TRUE(output.find(Const::HTAG_ERROR_STRING));
	}

	TEST(SpreadsheetParsing, CorrectParsingWithNaNValue)
	{
		Spreadsheet spreadsheet("NaNSpreadsheet.txt");
		spreadsheet.ParseData();

		testing::internal::CaptureStdout();
		std::cout << spreadsheet;
		std::string output = testing::internal::GetCapturedStdout();

		EXPECT_TRUE(output.find(Const::HTAG_NAN_STRING));
	}

	TEST(SpreadsheetParsing, CorrectParsing)
	{
		Spreadsheet spreadsheet("CorrectSpreadsheet.txt");
		spreadsheet.ParseData();

		testing::internal::CaptureStdout();
		std::cout << spreadsheet;
		std::string output = testing::internal::GetCapturedStdout();

		EXPECT_TRUE(output.find(Const::HTAG_NAN_STRING) == std::string::npos);
		EXPECT_TRUE(output.find(Const::HTAG_ERROR_STRING) == std::string::npos);
		// Last calculation is 6
		EXPECT_TRUE(output.find('6'));
	}

	TEST(SpreadsheetParsing, CorrectParsingPriorities)
	{
		Spreadsheet spreadsheet("CorrectSpreadsheetWithPriorityOperations.txt");
		spreadsheet.ParseData();

		testing::internal::CaptureStdout();
		std::cout << spreadsheet;
		std::string output = testing::internal::GetCapturedStdout();

		EXPECT_TRUE(output.find(Const::HTAG_NAN_STRING) == std::string::npos);
		EXPECT_TRUE(output.find(Const::HTAG_ERROR_STRING) == std::string::npos);
		// Last calculation is 0
		EXPECT_TRUE(output.find('0'));
	}

	TEST(SpreadsheetFunctionality, GetEmptyFieldValue)
	{
		Spreadsheet spreadsheet("CorrectSpreadsheetWithPriorityOperations.txt");
		spreadsheet.ParseData();

		EXPECT_TRUE(spreadsheet.GetValueForField("A2").empty());
	}

	TEST(SpreadsheetFunctionality, GetFormulaFieldValue)
	{
		Spreadsheet spreadsheet("CorrectSpreadsheetWithPriorityOperations.txt");
		spreadsheet.ParseData();

		EXPECT_TRUE(spreadsheet.GetValueForField("B1").at(0) == '=');
	}

	TEST(SpreadsheetFunctionality, GetValueFieldValue)
	{
		Spreadsheet spreadsheet("CorrectSpreadsheetWithPriorityOperations.txt");

		EXPECT_FALSE(spreadsheet["A1"]);

		spreadsheet.ParseData();

		EXPECT_TRUE(spreadsheet["A1"]);
	}

	TEST(SpreadsheetFunctionality, GetValueThroughBoxBracketsOperator)
	{
		Spreadsheet spreadsheet("CorrectSpreadsheetWithPriorityOperations.txt");
		spreadsheet.ParseData();

		EXPECT_TRUE(spreadsheet.GetValueForField("A2").empty());
	}

	TEST(SpreadsheetFunctionality, GetValueOutOfBound)
	{
		Spreadsheet spreadsheet("CorrectSpreadsheetWithPriorityOperations.txt");
		spreadsheet.ParseData();

		EXPECT_TRUE(spreadsheet.GetValueForField("Z1") == Const::HTAG_NAN_STRING);
	}

	TEST(SpreadsheetFunctionality, CalculateFormulaValue)
	{
		Spreadsheet spreadsheet("CorrectSpreadsheetWithPriorityOperations.txt");
		spreadsheet.ParseData();
		std::string result = spreadsheet.CalculateValueFromString("=A1*2");
		std::string coordinateValue = spreadsheet["B1"]->GetValue();
		std::string result2 = spreadsheet.CalculateValueFromString(coordinateValue);
		std::string result3 = spreadsheet.CalculateValueFromString("2");

		EXPECT_TRUE(result == "2");
		EXPECT_TRUE(result == result2);
	}

	TEST(SpreadsheetConstructorTest, CopyConstructorTest)
	{
		Spreadsheet spreadsheet("CorrectSpreadsheetWithPriorityOperations.txt");
		spreadsheet.ParseData();
		Spreadsheet spreadsheet2(spreadsheet);

		EXPECT_TRUE(spreadsheet["A1"]->GetValue() == spreadsheet2["A1"]->GetValue());
	}

	TEST(SpreadsheetConstructorTest, MoveConstructorTest)
	{
		Spreadsheet spreadsheet(std::move(Spreadsheet("CorrectSpreadsheetWithPriorityOperations.txt")));
		spreadsheet.ParseData();

		EXPECT_TRUE(spreadsheet["A1"]->GetValue() == "1");
	}

}

namespace FieldsTests
{
	class FieldTest : public ::testing::Test
	{
	protected:
		FieldTest()
		{
			_spreadSheet = new Spreadsheet("CorrectSpreadsheetWithPriorityOperations.txt");
		}

		void SetUp() override
		{
			_spreadSheet->ParseData();
		}

		Spreadsheet* _spreadSheet;
	};

	TEST_F(FieldTest, EmptyFieldGetValue)
	{
		IField* eField = (*_spreadSheet)["A2"];
		EXPECT_TRUE(eField->GetValue().empty());
	}

	TEST_F(FieldTest, EmptyFieldPrint)
	{
		IField* eField = (*_spreadSheet)["A2"];
		testing::internal::CaptureStdout();
		eField->PrintValue();
		std::string output = testing::internal::GetCapturedStdout();
		EXPECT_TRUE(output.empty());
	}

	TEST_F(FieldTest, EmptyFieldClone)
	{
		IField* eField = (*_spreadSheet)["A2"];
		IField* emptyField = eField->Clone();
		EXPECT_TRUE(emptyField != nullptr);
		EXPECT_TRUE(emptyField != eField);
		EXPECT_TRUE(eField->GetValue().compare(emptyField->GetValue()) == 0);
	}

	TEST_F(FieldTest, ValueFieldGetValue)
	{
		IField* vField = (*_spreadSheet)["A1"];
		EXPECT_TRUE(vField->GetValue().compare("1") == 0);
	}

	TEST_F(FieldTest, ValueFieldPrint)
	{
		IField* vField = (*_spreadSheet)["A1"];
		testing::internal::CaptureStdout();
		vField->PrintValue();
		std::string output = testing::internal::GetCapturedStdout();
		EXPECT_FALSE(output.empty());
		EXPECT_TRUE(output.compare("1") == 0);
	}

	TEST_F(FieldTest, ValueFieldClone)
	{
		IField* vField = (*_spreadSheet)["A1"];
		IField* valueField = vField->Clone();
		EXPECT_TRUE(valueField != nullptr);
		EXPECT_TRUE(valueField != vField);
		EXPECT_TRUE(vField->GetValue().compare(valueField->GetValue()) == 0);
	}

	TEST_F(FieldTest, FormulaFieldGetValue)
	{
		IField* fField = (*_spreadSheet)["B1"];
		EXPECT_TRUE(fField->GetValue().compare("=A1*2") == 0);
	}

	TEST_F(FieldTest, FormulaFieldPrint)
	{
		IField* fField = (*_spreadSheet)["B1"];
		testing::internal::CaptureStdout();
		fField->PrintValue();
		std::string output = testing::internal::GetCapturedStdout();
		EXPECT_FALSE(output.empty());
		EXPECT_TRUE(output.compare("2") == 0);
	}

	TEST_F(FieldTest, FormulaFieldClone)
	{
		IField* fField = (*_spreadSheet)["B1"];
		IField* formulaField = fField->Clone();
		EXPECT_TRUE(formulaField != nullptr);
		EXPECT_TRUE(formulaField != fField);
		EXPECT_TRUE(fField->GetValue().compare(formulaField->GetValue()) == 0);
	}
}

namespace UtilTests
{
	TEST(ArithmeticOperations, MultiplySameSignStrings)
	{
		std::string result;
		std::string first = "120";
		std::string second = "105";
		Const::MultiplyStrings(result, first, second);

		// 120 * 105 = 12600
		EXPECT_EQ(result, "12600");

		result = "";
		first = "-120";
		second = "-105";
		Const::MultiplyStrings(result, first, second);

		// 120 * 105 = 12600
		EXPECT_EQ(result, "12600");
	}

	TEST(ArithmeticOperations, MultiplyDifferentSignStrings)
	{
		std::string result;
		std::string first = "120";
		std::string second = "-105";
		Const::MultiplyStrings(result, first, second);

		// 120 * 105 = -12600
		EXPECT_EQ(result, "-12600");

		result = "";
		Const::MultiplyStrings(result, second, first);

		// 120 * 105 = -12600
		EXPECT_EQ(result, "-12600");
	}

	TEST(ArithmeticOperations, MultiplyWithZero)
	{
		std::string result;
		std::string first = "120";
		std::string second = "0";
		Const::MultiplyStrings(result, first, second);

		// 120 * 0 = 0
		EXPECT_EQ(result, "0");

		result = "";
		Const::MultiplyStrings(result, second, first);

		// 0 * 120 = 0
		EXPECT_EQ(result, "0");
	}

	TEST(ArithmeticOperations, MultiplyWithNonNumber)
	{
		std::string result;
		std::string first = "120";
		std::string second = "abc";
		ASSERT_DEATH(Const::MultiplyStrings(result, first, second), "");
		ASSERT_DEATH(Const::MultiplyStrings(result, second, first), "");
	}

	TEST(ArithmeticOperations, MultiplyEmptyStringsAsert)
	{
		std::string result;
		std::string first = "120";
		std::string second;

		ASSERT_DEATH(Const::MultiplyStrings(result, first, second), "");
		ASSERT_DEATH(Const::MultiplyStrings(result, second, first), "");
		ASSERT_DEATH(Const::MultiplyStrings(result, second, second), "");
	}

	TEST(ArithmeticOperations, DivideSameSignStrings)
	{
		std::string result;
		std::string first = "120";
		std::string second = "10";
		Const::DivideStrings(result, first, second);

		// 120 / 10 = 12
		EXPECT_EQ(result, "12");

		result = "";
		first = "-120";
		second = "-10";
		Const::DivideStrings(result, first, second);

		// -120 / -10 = 12
		EXPECT_EQ(result, "12");
	}

	TEST(ArithmeticOperations, DivideDifferentSignStrings)
	{
		std::string result;
		std::string first = "120";
		std::string second = "-10";
		Const::DivideStrings(result, first, second);

		// 120 / -10 = -12
		EXPECT_EQ(result, "-12");

		result = "";
		first = "-120";
		second = "10";
		Const::DivideStrings(result, first, second);

		// -120 * 10 = -12
		EXPECT_EQ(result, "-12");
	}

	TEST(ArithmeticOperations, DivideWithZero)
	{
		std::string result;
		std::string first = "120";
		std::string second = "0";
		// 120 / 0 = ERROR
		ASSERT_DEATH(Const::DivideStrings(result, first, second), "");

		result = "";
		Const::DivideStrings(result, second, first);

		// 0 / 120 = 0
		EXPECT_EQ(result, "0");
	}

	TEST(ArithmeticOperations, DivideWithNonNumber)
	{
		std::string result;
		std::string first = "120";
		std::string second = "abc";
		ASSERT_DEATH(Const::DivideStrings(result, first, second), "");
		ASSERT_DEATH(Const::DivideStrings(result, second, first), "");
	}

	TEST(ArithmeticOperations, DivideEmptyStringsAsert)
	{
		std::string result;
		std::string first = "120";
		std::string second;

		ASSERT_DEATH(Const::DivideStrings(result, first, second), "");
		ASSERT_DEATH(Const::DivideStrings(result, second, first), "");
		ASSERT_DEATH(Const::DivideStrings(result, second, second), "");
	}

	TEST(ArithmeticOperations, AddSameSignStrings)
	{
		std::string result;
		std::string first = "120";
		std::string second = "10";
		Const::AddStrings(result, first, second);

		// 120 + 10 = 130
		EXPECT_EQ(result, "130");

		result = "";
		first = "-120";
		second = "-10";
		Const::AddStrings(result, first, second);

		// -120 + -10 = -130
		EXPECT_EQ(result, "-130");
	}

	TEST(ArithmeticOperations, AddDifferentSignStrings)
	{
		std::string result;
		std::string first = "120";
		std::string second = "-10";
		Const::AddStrings(result, first, second);

		// 120 + -10 = 110
		EXPECT_EQ(result, "110");

		result = "";
		Const::AddStrings(result, second, first);

		// -10 + 120 = 110
		EXPECT_EQ(result, "110");
	}

	TEST(ArithmeticOperations, AddWithZero)
	{
		std::string result;
		std::string first = "120";
		std::string second = "0";
		Const::AddStrings(result, first, second);

		// 120 + 0 = 120
		EXPECT_EQ(result, "120");

		result = "";
		Const::AddStrings(result, second, first);

		// 0 + 120 = 0
		EXPECT_EQ(result, "120");
	}

	TEST(ArithmeticOperations, AddWithNonNumber)
	{
		std::string result;
		std::string first = "120";
		std::string second = "abc";
		ASSERT_DEATH(Const::AddStrings(result, first, second), "");
		ASSERT_DEATH(Const::AddStrings(result, second, first), "");
	}

	TEST(ArithmeticOperations, AddEmptyStringsAsert)
	{
		std::string result;
		std::string first = "120";
		std::string second;

		ASSERT_DEATH(Const::AddStrings(result, first, second), "");
		ASSERT_DEATH(Const::AddStrings(result, second, first), "");
		ASSERT_DEATH(Const::AddStrings(result, second, second), "");
	}

	TEST(ArithmeticOperations, SubstractSameSignStrings)
	{
		std::string result;
		std::string first = "120";
		std::string second = "10";
		Const::SubstractStrings(result, first, second);

		// 120 - 10 = 110
		EXPECT_EQ(result, "110");

		result = "";
		first = "-120";
		second = "-10";
		Const::SubstractStrings(result, first, second);

		// -120 - -10 = -110
		EXPECT_EQ(result, "-110");
	}

	TEST(ArithmeticOperations, SubstractDifferentSignStrings)
	{
		std::string result;
		std::string first = "120";
		std::string second = "-10";
		Const::SubstractStrings(result, first, second);

		// 120 - - 10 = 130
		EXPECT_EQ(result, "130");

		result = "";
		Const::SubstractStrings(result, second, first);

		// -10 - 120 = -130
		EXPECT_EQ(result, "-130");
	}

	TEST(ArithmeticOperations, SubstractWithZero)
	{
		std::string result;
		std::string first = "120";
		std::string second = "0";
		Const::SubstractStrings(result, first, second);

		// 120 - 0 = 120
		EXPECT_EQ(result, "120");

		result = "";
		Const::SubstractStrings(result, second, first);

		// 0 - 120 = -120
		EXPECT_EQ(result, "-120");
	}

	TEST(ArithmeticOperations, SubstractWithNonNumber)
	{
		std::string result;
		std::string first = "120";
		std::string second = "abc";
		ASSERT_DEATH(Const::SubstractStrings(result, first, second), "");
		ASSERT_DEATH(Const::SubstractStrings(result, second, first), "");
	}

	TEST(ArithmeticOperations, SubstractEmptyStringsAsert)
	{
		std::string result;
		std::string first = "120";
		std::string second;

		ASSERT_DEATH(Const::SubstractStrings(result, first, second), "");
		ASSERT_DEATH(Const::SubstractStrings(result, second, first), "");
		ASSERT_DEATH(Const::SubstractStrings(result, second, second), "");
	}

	TEST(CalculateUtilities, CalculateAndMergeMultiply)
	{
		std::vector<std::string> stringsVector{ "1", "*", "2" };
		Const::CalculateAndMerge(stringsVector, 1, Const::MultiplyStrings);
		EXPECT_EQ(stringsVector[0], "2");
		EXPECT_TRUE(stringsVector.size() == 1);
	}

	TEST(CalculateUtilities, CalculateAndMergeDivision)
	{
		std::vector<std::string> stringsVector{ "4", "/", "2" };
		Const::CalculateAndMerge(stringsVector, 1, Const::DivideStrings);
		EXPECT_EQ(stringsVector[0], "2");
		EXPECT_TRUE(stringsVector.size() == 1);
	}

	TEST(CalculateUtilities, CalculateAndMergeAddition)
	{
		std::vector<std::string> stringsVector{ "4", "+", "2" };
		Const::CalculateAndMerge(stringsVector, 1, Const::AddStrings);
		EXPECT_EQ(stringsVector[0], "6");
		EXPECT_TRUE(stringsVector.size() == 1);
	}

	TEST(CalculateUtilities, CalculateAndMergeSubstraction)
	{
		std::vector<std::string> stringsVector{ "4", "-", "2" };
		Const::CalculateAndMerge(stringsVector, 1, Const::SubstractStrings);
		EXPECT_EQ(stringsVector[0], "2");
		EXPECT_TRUE(stringsVector.size() == 1);
	}

	TEST(CalculateUtilities, CalculateAndMergeAssert)
	{
		std::vector<std::string> stringsVector{ "4", "-" };
		ASSERT_DEATH(Const::CalculateAndMerge(stringsVector, 1, Const::SubstractStrings), "");
		stringsVector = { "4", "-", "3" };
		ASSERT_DEATH(Const::CalculateAndMerge(stringsVector, 2, Const::SubstractStrings), "");
		ASSERT_DEATH(Const::CalculateAndMerge(stringsVector, 0, Const::SubstractStrings), "");
	}

	TEST(CalculateUtilities, CalculateExpressionMultiplication)
	{
		std::vector<std::string> stringsVector{ "4", "*" , "3"};
		Const::CalculateExpression(stringsVector);
		EXPECT_EQ(stringsVector[0], "12");
		EXPECT_TRUE(stringsVector.size() == 1);
	}

	TEST(CalculateUtilities, CalculateExpressionDivision)
	{
		std::vector<std::string> stringsVector{ "12", "/" , "4" };
		Const::CalculateExpression(stringsVector);
		EXPECT_EQ(stringsVector[0], "3");
		EXPECT_TRUE(stringsVector.size() == 1);
	}

	TEST(CalculateUtilities, CalculateExpressionAddition)
	{
		std::vector<std::string> stringsVector{ "12", "+" , "4" };
		Const::CalculateExpression(stringsVector);
		EXPECT_EQ(stringsVector[0], "16");
		EXPECT_TRUE(stringsVector.size() == 1);
	}

	TEST(CalculateUtilities, CalculateExpressionSubstraction)
	{
		std::vector<std::string> stringsVector{ "12", "-" , "4" };
		Const::CalculateExpression(stringsVector);
		EXPECT_EQ(stringsVector[0], "8");
		EXPECT_TRUE(stringsVector.size() == 1);
	}

	TEST(CalculateUtilities, CalculateExpressionWithSingleExpression)
	{
		std::vector<std::string> stringsVector{ "12"};
		Const::CalculateExpression(stringsVector);
		EXPECT_EQ(stringsVector[0], "12");
		EXPECT_TRUE(stringsVector.size() == 1);
	}

	TEST(CalculateUtilities, CalculateExpressionComplexExpression)
	{
		std::vector<std::string> stringsVector{ "3", "+", "-4", "*", "12", "-", "3", "/", "1", "-", "3", "*", "4" };
		Const::CalculateExpression(stringsVector);
		EXPECT_EQ(stringsVector[0], "-60");
	}

	TEST(CalculateUtilities, CalculateExpressionAssert)
	{
		std::vector<std::string> stringsVector{ "12", "-" };
		ASSERT_DEATH(Const::CalculateExpression(stringsVector), "");
	}

	TEST(ComparisonUtilities, IsNumberNegative)
	{
		std::string number = "-12";
		ASSERT_TRUE(Const::IsNumber(number));
	}

	TEST(ComparisonUtilities, IsNumberPositive)
	{
		std::string number = "12";
		ASSERT_TRUE(Const::IsNumber(number));
	}

	TEST(ComparisonUtilities, IsNumberNotANumber)
	{
		std::string word = "abc";
		ASSERT_FALSE(Const::IsNumber(word));
	}

	TEST(ComparisonUtilities, IsSmallerBothPositive)
	{
		std::string first = "120";
		std::string second = "130";
		ASSERT_TRUE(Const::IsSmaller(first, second));
		ASSERT_FALSE(Const::IsSmaller(second, first));
	}

	TEST(ComparisonUtilities, IsSmallerBothNegative)
	{
		std::string first = "-120";
		std::string second = "-130";
		ASSERT_FALSE(Const::IsSmaller(first, second));
		ASSERT_TRUE(Const::IsSmaller(second, first));
	}

	TEST(ComparisonUtilities, IsSmallerDifferentSigns)
	{
		std::string first = "-120";
		std::string second = "130";
		ASSERT_TRUE(Const::IsSmaller(first, second));
		ASSERT_FALSE(Const::IsSmaller(second, first));
	}

	TEST(ComparisonUtilities, IsSmallerAreEqual)
	{
		std::string first = "120";
		std::string second = "120";
		ASSERT_FALSE(Const::IsSmaller(first, second));
		ASSERT_FALSE(Const::IsSmaller(second, first));
	}

	TEST(ComparisonUtilities, IsFormulaEmpty)
	{
		std::string formulaEmpty;
		ASSERT_FALSE(Const::IsFormula(formulaEmpty));
	}

	TEST(ComparisonUtilities, IsFormulaValue)
	{
		std::string formulaValue = "120";
		ASSERT_FALSE(Const::IsFormula(formulaValue));
	}

	TEST(ComparisonUtilities, IsFormulaFormula)
	{
		std::string formula = "=A1+B";
		ASSERT_TRUE(Const::IsFormula(formula));
	}

	TEST(SplitUtilities, SplitStringExpressionEmptyString)
	{
		std::string expression;
		std::vector<std::string> stringsVector = Const::SplitStringExpression(expression, Const::BASIC_OPERATORS);
		ASSERT_TRUE(stringsVector.empty());
	}

	TEST(SplitUtilities, SplitStringExpressionSimpleMultiplication)
	{
		std::string expression{ "A*B" };
		std::vector<std::string> stringsVector = Const::SplitStringExpression(expression, "*");
		std::vector<std::string> outputVector = { "A", "*", "B" };
		ASSERT_EQ(stringsVector, outputVector);
	}

	TEST(SplitUtilities, SplitStringExpressionSimpleDivision)
	{
		std::string expression{ "A/B" };
		std::vector<std::string> stringsVector = Const::SplitStringExpression(expression, "/");
		std::vector<std::string> outputVector = { "A", "/", "B" };
		ASSERT_EQ(stringsVector, outputVector);
	}

	TEST(SplitUtilities, SplitStringExpressionSimpleAddition)
	{
		std::string expression{ "A+B" };
		std::vector<std::string> stringsVector = Const::SplitStringExpression(expression, "+");
		std::vector<std::string> outputVector = { "A", "+", "B" };
		ASSERT_EQ(stringsVector, outputVector);
	}

	TEST(SplitUtilities, SplitStringExpressionSimpleSubstraction)
	{
		std::string expression{ "A-B" };
		std::vector<std::string> stringsVector = Const::SplitStringExpression(expression, "-");
		std::vector<std::string> outputVector = { "A", "-", "B" };
		ASSERT_EQ(stringsVector, outputVector);
	}

	TEST(SplitUtilities, SplitStringExpressionComplexMultiplication)
	{
		std::string expression{ "A*-B" };
		std::vector<std::string> stringsVector = Const::SplitStringExpression(expression, "*");
		std::vector<std::string> outputVector = { "A", "*", "-B" };
		ASSERT_EQ(stringsVector, outputVector);
	}

	TEST(SplitUtilities, SplitStringExpressionComplexDivision)
	{
		std::string expression{ "A/-B" };
		std::vector<std::string> stringsVector = Const::SplitStringExpression(expression, "/");
		std::vector<std::string> outputVector = { "A", "/", "-B" };
		ASSERT_EQ(stringsVector, outputVector);
	}

	TEST(SplitUtilities, SplitStringExpressionComplexAddition)
	{
		std::string expression{ "A+-B" };
		std::vector<std::string> stringsVector = Const::SplitStringExpression(expression, "+");
		std::vector<std::string> outputVector = { "A", "+", "-B" };
		ASSERT_EQ(stringsVector, outputVector);
	}

	TEST(SplitUtilities, SplitStringExpressionComplexSubstraction)
	{
		std::string expression{ "A--B" };
		std::vector<std::string> stringsVector = Const::SplitStringExpression(expression, "-");
		std::vector<std::string> outputVector = { "A", "-", "-B" };
		ASSERT_EQ(stringsVector, outputVector);
	}

	TEST(SplitUtilities, SplitStringExpressionComplexExpressionWithBasicOperators)
	{
		std::string expression{ "A--B+2/C-B1+72--3/4" };
		std::vector<std::string> stringsVector = Const::SplitStringExpression(expression, Const::BASIC_OPERATORS);
		std::vector<std::string> outputVector = { "A", "-", "-B", "+", "2", "/", "C", "-", "B1", "+", "72", "-", "-3", "/", "4" };
		ASSERT_EQ(stringsVector, outputVector);
	}

	TEST(SplitUtilities, SplitStringExpressionNoDelimitersFound)
	{
		std::string expression{ "A+B+C" };
		std::vector<std::string> stringsVector = Const::SplitStringExpression(expression, "-");
		std::vector<std::string> outputVector = { "A+B+C" };
		ASSERT_EQ(stringsVector, outputVector);
	}

	TEST(SplitUtilities, SplitStringWithIndexNotFound)
	{
		std::string expression{ "A+B+C" };
		std::map<uint64_t, std::string> targetMap = Const::SplitStringWithIndex(expression, "-");

		EXPECT_TRUE(targetMap.size() == 1);
		EXPECT_TRUE(targetMap[0] == expression);
	}

	TEST(SplitUtilities, SplitStringWithIndexWithIndexFound)
	{
		std::string expression{ "A+B+C-D" };
		std::map<uint64_t, std::string> targetMap = Const::SplitStringWithIndex(expression, "-");

		EXPECT_TRUE(targetMap.size() == 2);
		EXPECT_TRUE(targetMap[0] == "A+B+C");
		EXPECT_TRUE(targetMap[1] == "D");
	}

	TEST(SplitUtilities, SplitStringWithIndexWithMultipleIndexesFound)
	{
		std::string expression{ "A+B+C-D+E+F" };
		std::map<uint64_t, std::string> targetMap = Const::SplitStringWithIndex(expression, "+");

		EXPECT_TRUE(targetMap.size() == 5);
		EXPECT_TRUE(targetMap[0] == "A");
		EXPECT_TRUE(targetMap[1] == "B");
		EXPECT_TRUE(targetMap[2] == "C-D");
		EXPECT_TRUE(targetMap[3] == "E");
		EXPECT_TRUE(targetMap[4] == "F");
	}
}
