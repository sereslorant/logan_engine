#ifndef LJSON_PARSER_H
#define LJSON_PARSER_H

#include <istream>

#include <iomanip>

#include "lJSON_Data.h"

#include <cctype>

#include <iostream>

class lJSON_Parser
{
private:
	std::istream &In;
	
	void ParseString(liJSON_Value *&dest)
	{
		std::string Value;
		In >> std::quoted(Value);
		
		dest = new lJSON_String(Value);
	}

	void ParseTrue(liJSON_Value *&dest)
	{
		char Chars[5];
		In.get(Chars,5);
		
		std::cout << Chars << std::endl;
		
		if(Chars[0] == 't' && Chars[1] == 'r' && Chars[2] == 'u' && Chars[3] == 'e')
		{
			dest = new lJSON_Bool(true);
		}
	}

	void ParseFalse(liJSON_Value *&dest)
	{
		char Chars[6];
		In.get(Chars,6);
		
		std::cout << Chars << std::endl;
		
		if(Chars[0] == 'f' && Chars[1] == 'a' && Chars[2] == 'l' && Chars[3] == 's' && Chars[4] == 'e')
		{
			dest = new lJSON_Bool(false);
		}
	}

	void ParseNull(liJSON_Value *&dest)
	{
		char Chars[5];
		In.get(Chars,5);
		
		std::cout << Chars << std::endl;
		
		if(Chars[0] == 'n' && Chars[1] == 'u' && Chars[2] == 'l' && Chars[3] == 'l')
		{
			dest = new lJSON_Null;
		}
	}

	bool ParseNumber(liJSON_Value *&dest)
	{
		char NextChar;
		In >> NextChar;
		
		if(std::isdigit(NextChar) || (NextChar == '+') || (NextChar == '-'))
		{
			std::string NumberStr;
			
			bool IsFloat = false;
			
			while((std::isdigit(NextChar) || (NextChar == '.') || (NextChar == '+') || (NextChar == '-') || (std::tolower(NextChar) == 'e')) && !(In.eof()))
			{
				NumberStr += NextChar;
				
				if(NextChar == '.' || std::tolower(NextChar) == 'e')
				{
					IsFloat = true;
				}
				
				In >> NextChar;
			}
			
			if(!In.eof())
				{In.seekg(-1,std::ios_base::cur);}
			
			if(IsFloat)
			{
				double Value = std::stod(NumberStr);
				//std::cout << "Value = " + std::to_string(Value) << std::endl;
				dest = new lJSON_Double(Value);//= nullptr;
			}
			else
			{
				int Value = std::stoi(NumberStr);
				//std::cout << "Value = " + std::to_string(Value) << std::endl;
				dest = new lJSON_Integer(Value);//= nullptr;
			}
			
			return false;
		}
		else
		{
			dest = nullptr;
			std::cout << "Error while parsing number." << std::endl;
			
			return true;
		}
	}
	
	void ParseCommaSeparatedValues(std::function<void(void)> read_function,char delimiter)
	{
		char NextChar;
		In >> NextChar;
		
		bool Running = true;
		while(Running)
		{
			In >> NextChar;
			
			if(NextChar != delimiter)
			{
				In.seekg(-1,std::ios_base::cur);
				
				read_function();
				
				In >> NextChar;
				if(NextChar == delimiter)
				{
					Running = false;
				}
				else if(NextChar != ',')
				{
					std::cout << "Error, expected comma! " << NextChar << std::endl;
				}
			}
			else
			{
				Running = false;
			}
		}
	}
	
	void ParseArray(liJSON_Value *&dest)
	{
		lJSON_Array::ArrayBuilder ArrayBuilder;
		
		ParseCommaSeparatedValues([this,&ArrayBuilder]()
			{
				liJSON_Value *NextValue;
				ParseValue(NextValue);
				
				ArrayBuilder.AddValue(NextValue);
			},
			']'
		);
		
		dest = ArrayBuilder.Construct();
	}

	void ParseObject(liJSON_Value *&dest)
	{
		lJSON_Object *Object = new lJSON_Object;
		
		ParseCommaSeparatedValues([this,Object]()
			{
				std::string NextKey;
				In >> std::quoted(NextKey);
				
				//char Junk;
				//In >> Junk;
				
				char Colon;
				In >> Colon;
				
				if(Colon != ':')
					{std::cout << "Error, expected colon! " << Colon << std::endl;}
				
				liJSON_Value *NextValue;
				ParseValue(NextValue);
				
				Object->AddVariable(NextKey,NextValue);
			},
			'}'
		);
		
		dest = Object;
	}
	
	bool ParseValue(liJSON_Value *&dest)
	{
		bool Error = false;
		
		char NextChar;
		In >> NextChar;
		
		In.seekg(-1,std::ios_base::cur);
		
		switch(NextChar)
		{
		case '\"':
			ParseString(dest);
			break;
		case '[':
			ParseArray(dest);
			break;
		case '{':
			ParseObject(dest);
			break;
		case 't':
			ParseTrue(dest);
			break;
		case 'f':
			ParseFalse(dest);
			break;
		case 'n':
			ParseNull(dest);
			break;
		default:
			Error = ParseNumber(dest);
		}
		
		return Error;
	}
	
public:
	
	void Parse(liJSON_Value *&dest)
	{
		ParseValue(dest);
	}

	lJSON_Parser(std::istream &in)
		:In(in)
	{}
};

#endif // LJSON_PARSER_H
