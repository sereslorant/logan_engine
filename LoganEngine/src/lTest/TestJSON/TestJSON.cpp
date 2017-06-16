
#include <iostream>

#include "../../lUtils/lJSON/lJSON.h"

int main(int argc, char *argv[])
{
	liJSON_Value *Value;
	lJSON_Util::Parse(std::cin,Value);
	
	//Value->Accept(Printer);
	lJSON_Util::Print(*Value,std::cout);
	
	delete Value;
	
	return 0;
} 
