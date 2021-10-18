#include "Spreadsheet.h"
#include <iostream>

int main()
{
	Spreadsheet spreadsheet("spreadsheet.txt");
	spreadsheet.ParseData();
	std::cout << spreadsheet;
}
