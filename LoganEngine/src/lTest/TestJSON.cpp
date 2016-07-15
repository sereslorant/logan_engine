
#include <iostream>

#include "../lJSON/lJSON.h"

int main(int argc, char *argv[])
{
	PrintJSON Printer;
	liJSON_Value *Value;

	lJSON_Util::Parse(std::cin,Value);
	
	Value->Accept(Printer);
	
	delete Value;
	
	return 0;
} 
