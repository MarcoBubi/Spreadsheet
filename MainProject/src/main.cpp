#include "Spreadsheet.h"

int main()
{
	Spreadsheet spreadsheet(R"(C:\Users\Marco\source\repos\Cetitec\Spreadsheet\MainProject\data\spreadsheet.txt)");
	spreadsheet.ParseData();
}
