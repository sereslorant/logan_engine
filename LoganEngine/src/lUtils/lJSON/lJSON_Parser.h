#ifndef LJSON_PARSER_H
#define LJSON_PARSER_H

#include <istream>

#include "lJSON_Data.h"

#include <cctype>

#include <iostream>

class lJSON_StatefulParser
{
private:
	/*
	 * TODO: A compiler rinyál, hogy nincs inicializálva.
	 */
	char NextChar;
	std::istream &In;
	
	static void PrintChar(char next_char)
	{
		//std::cout << "NextChar:" << next_char << " " << next_char << ";";
	}

	void ParseString(std::string &dest)
	{
		In.get(NextChar);
		while(NextChar != '\"')
		{
			dest += NextChar;
			In.get(NextChar);
		}

		In.get(NextChar);
	}

	void ParseString(liJSON_Value *&dest)
	{
		//std::cout << "Started parsing string" << std::endl;
		std::string Value;

		ParseString(Value);

		//std::cout << "Finished parsing string" << std::endl;
		dest = new lJSON_String(Value);
	}

	void ParseArray(liJSON_Value *&dest)
	{
		//std::cout << "Started parsing array" << std::endl;
		liJSON_Value *NextValue;
		bool ParsingError = false;
		if(ParseValue(NextValue))
		{
			ParsingError = true;
		}
		//
		lJSON_Array::ArrayBuilder ArrayBuilder;
		if(!ParsingError)
		{
			ArrayBuilder.AddValue(NextValue);
		}
		//
		bool Error = false;
		PrintChar(NextChar);
		//
		while(NextChar != ']')
		{
			//std::cout << in.tellg() << std::endl;
			PrintChar(NextChar);
			if(!std::isspace(NextChar))
			{
				if(NextChar == ',')
				{
					Error = ParseValue(NextValue);
					if(Error)
					{
						//std::cout << "Error while parsing array" << std::endl;
						break;
					}
					//
					ArrayBuilder.AddValue(NextValue);
				}
				else
				{
					if(!In.eof())
					{
						In.get(NextChar);
						//std::cout << "Next char loaded" << std::endl;
					}
					else
					{
						//std::cout << "Error: end of file reacher prematurely" << std::endl;
						break;
					}
				}
			}
			else
			{
				if(!In.eof())
				{
					In.get(NextChar);
					//std::cout << "Next char loaded" << std::endl;
				}
				else
				{
					//std::cout << "Error: end of file reacher prematurely" << std::endl;
					break;
				}
			}
		}

		if(!In.eof())
		{
			In.get(NextChar);
		}

		//std::cout << "Finished parsing array" << std::endl;
		dest = ArrayBuilder.Construct();
	}

	bool ParsePair(std::string &key,liJSON_Value *&value)
	{
		In.get(NextChar);
		//
		while(std::isspace(NextChar))
			{In.get(NextChar);}
		//
		if(NextChar == '\"')
		{
			ParseString(key);
		}
		else
		{
			return true;
		}
		//
		std::cout << "Key = " << key << std::endl;
		//In.get(NextChar);
		while(std::isspace(NextChar))
			{In.get(NextChar);}
		//
		if(NextChar !=  ':')
		{
			std::cout << "Error: expected ':' instead of " << NextChar << std::endl;
			value = nullptr;
			return true;
		}
		//
		if(ParseValue(value))
		{
			return true;
		}
		//
		return false;
	}

	void ParseObject(liJSON_Value *&dest)
	{
		//std::cout << "Started parsing object" << std::endl;
		std::string NextKey;
		liJSON_Value *NextValue;
		ParsePair(NextKey,NextValue);

		//char NextChar;
		bool Error = false;
		//in.get(NextChar);
		PrintChar(NextChar);

		lJSON_Object *NewObject = new lJSON_Object;
		NewObject->AddVariable(NextKey,NextValue);
		NextKey.clear();

		while(NextChar != '}')
		{
			//std::cout << in.tellg() << std::endl;
			PrintChar(NextChar);
			if(!std::isspace(NextChar))
			{
				if(NextChar == ',')
				{
					Error = ParsePair(NextKey,NextValue);
					if(Error)
					{
						std::cout << "Error: couldn't parse pair" << std::endl;
						break;
					}
					//
					if(!NewObject->AddVariable(NextKey,NextValue))
					{
						Error = true;
					}
					NextKey.clear();
				}
				else
				{
					if(!In.eof())
					{
						In.get(NextChar);
						//std::cout << "Next char loaded" << std::endl;
					}
					else
					{
						//std::cout << "Error: end of file reacher prematurely" << std::endl;
						break;
					}
				}
			}
			else
			{
				//in >> NextChar;
				if(!In.eof())
				{
					In.get(NextChar);
					//std::cout << "Next char loaded" << std::endl;
				}
				else
				{
					//std::cout << "Error: end of file reacher prematurely" << std::endl;
					break;
				}
			}
		}
		//
		//std::cout << "Finished parsing object" << std::endl;
		dest = NewObject;
		//std::cerr << "Parsing an object is not yet implemented." << std::endl;
	}

	void ParseTrue(liJSON_Value *&dest)
	{
		//std::cout << "Started parsing true" << std::endl;
		char Chars[3];
		//
		In.get(Chars[0]);
		if(Chars[0] != 'r')
		{
			//std::cerr << "Error while parsing true" << std::endl;
		}
		//
		In.get(Chars[1]);
		if(Chars[1] != 'u')
		{
			//std::cerr << "Error while parsing true" << std::endl;
		}
		//
		In.get(Chars[2]);
		if(Chars[2] != 'e')
		{
			//std::cerr << "Error while parsing true" << std::endl;
		}
		//
		In.get(NextChar);
		//
		//std::cout << "Finished parsing true" << std::endl;
		dest = new lJSON_Bool(true);
	}

	void ParseFalse(liJSON_Value *&dest)
	{
		std::cout << "Started parsing false" << std::endl;
		char Chars[4];
		//
		In.get(Chars[0]);
		if(Chars[0] != 'a')
		{
			//std::cerr << "Error while parsing false" << std::endl;
		}
		//
		In.get(Chars[1]);
		if(Chars[1] != 'l')
		{
			//std::cerr << "Error while parsing false" << std::endl;
		}
		//
		In.get(Chars[2]);
		if(Chars[2] != 's')
		{
			//std::cerr << "Error while parsing false" << std::endl;
		}
		//
		In.get(Chars[3]);
		if(Chars[3] != 'e')
		{
			//std::cerr << "Error while parsing false" << std::endl;
		}
		//
		In.get(NextChar);
		//
		//std::cout << "Finished parsing false" << std::endl;
		dest = new lJSON_Bool(false);
	}

	void ParseNull(liJSON_Value *&dest)
	{
		std::cout << "Started parsing null" << std::endl;
		char Chars[3];
		//
		In.get(Chars[0]);
		if(Chars[0] != 'u')
		{
			//std::cerr << "Error while parsing null" << std::endl;
		}
		//
		In.get(Chars[1]);
		if(Chars[1] != 'l')
		{
			//std::cerr << "Error while parsing null" << std::endl;
		}
		//
		In.get(Chars[2]);
		if(Chars[2] != 'l')
		{
			//std::cerr << "Error while parsing null" << std::endl;
		}
		//
		In.get(NextChar);
		//
		//std::cout << "Finished parsing null" << std::endl;
		dest = new lJSON_Null;
	}

	bool ParseNumber(liJSON_Value *&dest)
	{
		//std::cout << "Started parsing number" << std::endl;
		if(std::isdigit(NextChar) || (NextChar == '+') || (NextChar == '-'))
		{
			std::string NumberStr;
			//int StreamPos;
			//char NextChar;

			bool IsFloat = false;

			//NumberStr += first_character;
			//StreamPos = in.tellg();
			//in.get(NextChar);
			while((std::isdigit(NextChar) || (NextChar == '.') || (NextChar == '+') || (NextChar == '-') || (std::tolower(NextChar) == 'e')) && !(In.eof()))
			{
				NumberStr += NextChar;

				if(NextChar == '.' || std::tolower(NextChar) == 'e')
				{
					IsFloat = true;
				}

				//StreamPos = in.tellg();
				In.get(NextChar);
			}

			//std::cout << in.tellg() << std::endl;
			//in.seekg(StreamPos);
			//std::cout << in.tellg() << std::endl;

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

			//std::cout << "Finished parsing number" << std::endl;
			//
			return false;
		}
		else
		{
			dest = nullptr;
			//std::cerr << "Error while parsing number." << std::endl;
			//
			return true;
		}
	}

	bool ParseValue(liJSON_Value *&dest)
	{
		//std::cout << "Started parsing value" << std::endl;
		bool Error = false;
		//
		In.get(NextChar);
		//
		while(std::isspace(NextChar))
			{In.get(NextChar);}
		//
		PrintChar(NextChar);
		//
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
		//
		//std::cout << "Finished parsing value" << std::endl;
		return Error;
	}

public:

	void Parse(liJSON_Value *&dest)
	{
		ParseValue(dest);
	}

	lJSON_StatefulParser(std::istream &in)
		:In(in)
	{

	}
};

#endif // LJSON_PARSER_H
