
#include <iostream>

#include <string>
#include <sstream>

#include "../../lUtils/lJSON/lJSON.h"

int main(int argc, char *argv[])
{
	std::string FileContents;
	while(!std::cin.eof())
	{
		char NextChar;
		std::cin.get(NextChar);
		
		FileContents += NextChar;
	}
	
	std::stringstream FileStream(FileContents);
	
	liJSON_Value *Value;
	lJSON_Util::Parse(FileStream,Value);
	
	//Value->Accept(Printer);
	lJSON_Util::Print(*Value,std::cout);
	
	std::cout << std::endl;
	
	delete Value;
	
	return 0;
} 
