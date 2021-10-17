#include "Spreadsheet.h"

int main()
{
	Spreadsheet spreadsheet("spreadsheet.txt");
	spreadsheet.ParseData();
	spreadsheet.PrintData();
}
