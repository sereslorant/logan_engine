
#include "lJSON_Util.h"

#include "lJSON_Parser.h"
#include "lJSON_Printer.h"

#include <iostream>

class lJSON_RecursiveParser
{
private:
	
	static char NextChar;
	
	static void ParseString(std::istream &in,std::string &dest)
	{
		/*in >> NextChar;*/
		in.get(NextChar);
		while(NextChar != '\"')
		{
			dest += NextChar;
			in.get(NextChar);
		}

		in.get(NextChar);
	}

	static bool ParsePair(std::istream &in,std::string &key,liJSON_Value *&value)
	{
		//in.get(NextChar);

		while(std::isspace(NextChar))
			{in.get(NextChar);}

		if(NextChar == '\"')
		{
			ParseString(in,key);
		}
		else
		{
			return true;
		}

		in.get(NextChar);
		while(std::isspace(NextChar))
			{in.get(NextChar);}

		if(NextChar !=  ':')
		{
			value = nullptr;
			return true;
		}

		if(ParseValue(in,value))
		{
			return true;
		}

		return false;
	}

	static void ParseString(std::istream &in,liJSON_Value *&dest)
	{
		std::cout << "Started parsing string" << std::endl;
		std::string Value;

		ParseString(in,Value);

		in.get(NextChar);

		std::cout << "Finished parsing string" << std::endl;
		dest = new lJSON_String(Value);
	}

	static void ParseArray(std::istream &in,liJSON_Value *&dest)
	{
		std::cout << "Started parsing array" << std::endl;
		liJSON_Value *NextValue;
		ParseValue(in,NextValue);
		//
		lJSON_Array::ArrayBuilder ArrayBuilder;
		ArrayBuilder.AddValue(NextValue);
		//
		bool Error = false;
		//in.get(NextChar);
		std::cout << "NextChar == " << (int)NextChar << ";";
		//
		while(NextChar != ']')
		{
			//std::cout << in.tellg() << std::endl;
			std::cout << "NextChar == " << (int)NextChar << ";";
			if(!std::isspace(NextChar))
			{
				if(NextChar == ',')
				{
					Error = ParseValue(in,NextValue);
					if(Error)
					{
						std::cout << "Error while parsing array" << std::endl;
						break;
					}
					//
					ArrayBuilder.AddValue(NextValue);
				}
			}
			else
			{
				if(!in.eof())
				{
					in.get(NextChar);
					//std::cout << "Next char loaded" << std::endl;
				}
				else
				{
					std::cout << "Error: end of file reacher prematurely" << std::endl;
					break;
				}
			}
		}

		std::cout << "Finished parsing array" << std::endl;
		dest = ArrayBuilder.Construct();
	}

	static void ParseObject(std::istream &in,liJSON_Value *&dest)
	{
		std::cout << "Started parsing object" << std::endl;
		std::string NextKey;
		liJSON_Value *NextValue;
		ParsePair(in,NextKey,NextValue);

		//char NextChar;
		bool Error = false;
		//in.get(NextChar);
		std::cout << "NextChar == " << (int)NextChar << ";";

		lJSON_Object *NewObject = new lJSON_Object;
		NewObject->AddVariable(NextKey,NextValue);
		NextKey.clear();

		while(NextChar != '}')
		{
			//std::cout << in.tellg() << std::endl;
			//std::cout << "NextChar == " << (int)NextChar << ";";
			if(!std::isspace(NextChar))
			{
				if(NextChar == ',')
				{
					Error = ParsePair(in,NextKey,NextValue);
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
			}
			//
			//in >> NextChar;
			if(!in.eof())
			{
				//in.get(NextChar);
				//std::cout << "Next char loaded" << std::endl;
			}
			else
			{
				std::cout << "Error: end of file reacher prematurely" << std::endl;
				break;
			}
		}

		std::cout << "Finished parsing object" << std::endl;
		dest = NewObject;
		//std::cerr << "Parsing an object is not yet implemented." << std::endl;
	}

	static void ParseTrue(std::istream &in,liJSON_Value *&dest)
	{
		std::cout << "Started parsing true" << std::endl;
		char Chars[3];

		in.get(Chars[0]);
		if(Chars[0] != 'r')
		{std::cerr << "Error while parsing true" << std::endl;}

		in.get(Chars[1]);
		if(Chars[1] != 'u')
		{std::cerr << "Error while parsing true" << std::endl;}

		in.get(Chars[2]);
		if(Chars[2] != 'e')
		{std::cerr << "Error while parsing true" << std::endl;}

		in.get(NextChar);

		std::cout << "Finished parsing true" << std::endl;
		dest = new lJSON_Bool(true);
	}

	static void ParseFalse(std::istream &in,liJSON_Value *&dest)
	{
		std::cout << "Started parsing false" << std::endl;
		char Chars[4];

		in.get(Chars[0]);
		if(Chars[0] != 'a')
		{std::cerr << "Error while parsing false" << std::endl;}

		in.get(Chars[1]);
		if(Chars[1] != 'l')
		{std::cerr << "Error while parsing false" << std::endl;}

		in.get(Chars[2]);
		if(Chars[2] != 's')
		{std::cerr << "Error while parsing false" << std::endl;}

		in.get(Chars[3]);
		if(Chars[3] != 'e')
		{std::cerr << "Error while parsing false" << std::endl;}

		in.get(NextChar);

		std::cout << "Finished parsing false" << std::endl;
		dest = new lJSON_Bool(false);
	}

	static void ParseNull(std::istream &in,liJSON_Value *&dest)
	{
		std::cout << "Started parsing null" << std::endl;
		char Chars[3];

		in.get(Chars[0]);
		if(Chars[0] != 'u')
		{std::cerr << "Error while parsing null" << std::endl;}

		in.get(Chars[1]);
		if(Chars[1] != 'l')
		{std::cerr << "Error while parsing null" << std::endl;}

		in.get(Chars[2]);
		if(Chars[2] != 'l')
		{std::cerr << "Error while parsing null" << std::endl;}

		in.get(NextChar);

		std::cout << "Finished parsing null" << std::endl;
		dest = new lJSON_Null;
	}

	static bool ParseNumber(std::istream &in/*,char first_character*/,liJSON_Value *&dest)
	{
		std::cout << "Started parsing number" << std::endl;
		if(std::isdigit(NextChar) || (NextChar == '+') || (NextChar == '-'))
		{
			std::string NumberStr;
			//int StreamPos;
			//char NextChar;

			bool IsFloat = false;

			//NumberStr += first_character;
			//StreamPos = in.tellg();
			//in.get(NextChar);
			while((std::isdigit(NextChar) || (NextChar == '.') || (std::tolower(NextChar) == 'e')) && !(in.eof()))
			{
				NumberStr += NextChar;

				if(NextChar == '.' || std::tolower(NextChar) == 'e')
				{
					IsFloat = true;
				}

				//StreamPos = in.tellg();
				in >> NextChar;
			}

			//std::cout << in.tellg() << std::endl;
			//in.seekg(StreamPos);
			//std::cout << in.tellg() << std::endl;

			if(IsFloat)
			{
				double Value = std::stod(NumberStr);
				std::cerr << "Value = " + std::to_string(Value) << std::endl;
				dest = new lJSON_Double(Value);//= nullptr;
			}
			else
			{
				int Value = std::stoi(NumberStr);
				std::cerr << "Value = " + std::to_string(Value) << std::endl;
				dest = new lJSON_Integer(Value);//= nullptr;
			}

			std::cout << "Finished parsing number" << std::endl;
			//
			return false;
		}
		else
		{
			dest = nullptr;
			std::cerr << "Error while parsing number." << std::endl;
			//
			return true;
		}
	}
	
public:
	
	static bool ParseValue(std::istream &in,liJSON_Value *&dest)
	{
		std::cout << "Started parsing variable" << std::endl;

		bool Error = false;
		//char NextChar;
		//
		in.get(NextChar);
		std::cout << "NextChar =" << NextChar << std::endl;
		//
		while(std::isspace(NextChar))
			{in>>NextChar;}
		//
		switch(NextChar)
		{
		case '\"':
			ParseString(in,dest);
			break;
		case '[':
			ParseArray(in,dest);
			break;
		case '{':
			ParseObject(in,dest);
			break;
		case 't':
			ParseTrue(in,dest);
			break;
		case 'f':
			ParseFalse(in,dest);
			break;
		case 'n':
			ParseNull(in,dest);
			break;
		default:
			Error = ParseNumber(in/*,NextChar*/,dest);
		}

		std::cout << "Finished parsing variable" << std::endl;

		return Error;
	}
};

void lJSON_Util::Parse(std::istream &in,liJSON_Value *&dest)
{
	std::cout << "Starting parsing" << std::endl;

	//ParseValue(in,dest);
	lJSON_Parser Parser(in);
	Parser.Parse(dest);

	std::cout << "Finished parsing" << std::endl;
}


void lJSON_Util::Print(const liJSON_Integer &value,std::ostream &out)
{
	lJSON_Printer Printer(out);
	Printer.Visit(value);
}

void lJSON_Util::Print(const liJSON_Double &value,std::ostream &out)
{
	lJSON_Printer Printer(out);
	Printer.Visit(value);
}

void lJSON_Util::Print(const liJSON_Bool &value,std::ostream &out)
{
	lJSON_Printer Printer(out);
	Printer.Visit(value);
}

void lJSON_Util::Print(const liJSON_String &value,std::ostream &out)
{
	lJSON_Printer Printer(out);
	Printer.Visit(value);
}

void lJSON_Util::Print(const liJSON_Object &value,std::ostream &out)
{
	lJSON_Printer Printer(out);
	Printer.Visit(value);
}

void lJSON_Util::Print(const liJSON_Array &value,std::ostream &out)
{
	lJSON_Printer Printer(out);
	Printer.Visit(value);
}

void lJSON_Util::Print(const liJSON_Value &value,std::ostream &out)
{
	lJSON_Printer Printer(out);
	value.Accept(Printer);
}
