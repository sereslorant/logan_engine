#ifndef L_JSON_H
#define L_JSON_H

#include "liJSON.h"

#include <vector>
#include <list>
#include <map>

class lJSON_Array : public liJSON_Value, public liJSON_Array
{
private:
	std::vector<liJSON_Value *> Elements;

	template<class Iterator>
	lJSON_Array(Iterator begin,Iterator end,unsigned int size)
		:Elements(size)
	{
		Iterator I = begin;
		for(unsigned int i=0;i < size && I != end;i++,I++)
		{
			Elements[i] = *I;
		}
	}

public:

	virtual void Accept(liJSON_Visitor &visitor) const override
	{
		visitor.Visit(*this);
	}

	virtual void Accept(liJSON_Visitor &visitor) override
	{
		visitor.Visit(*this);
	}

	virtual unsigned int Size() const override
	{
		return Elements.size();
	}

	virtual const liJSON_Value *GetElement(unsigned int i) const override
	{
		if(i < Elements.size())
		{
			return Elements[i];
		}

		return nullptr;
	}

	virtual liJSON_Value *GetElement(unsigned int i) override
	{
		if(i < Elements.size())
		{
			return Elements[i];
		}

		return nullptr;
	}

	class ArrayBuilder
	{
	private:
		std::list<liJSON_Value *> Variables;
	public:

		ArrayBuilder &AddValue(liJSON_Value *value)
		{
			Variables.push_back(value);
			return *this;
		}

		liJSON_Value *Construct()
		{
			return new lJSON_Array(Variables.begin(),Variables.end(),Variables.size());
		}

		ArrayBuilder(){}
		~ArrayBuilder(){}
	};

	virtual ~lJSON_Array()
	{
		for(liJSON_Value *Element : Elements)
		{
			delete Element;
		}
	}
};

class lJSON_Integer : public liJSON_Value, public liJSON_Integer
{
private:
	int Value;

public:

	virtual void Accept(liJSON_Visitor &visitor) const override
	{
		visitor.Visit(*this);
	}

	virtual void Accept(liJSON_Visitor &visitor) override
	{
		visitor.Visit(*this);
	}

	virtual int GetValue() const override
	{
		return Value;
	}

	lJSON_Integer(int value)
		:Value(value)
	{}
	virtual ~lJSON_Integer(){}
};

class lJSON_Double : public liJSON_Value, public liJSON_Double
{
private:
	double Value;

public:

	virtual void Accept(liJSON_Visitor &visitor) const override
	{
		visitor.Visit(*this);
	}

	virtual void Accept(liJSON_Visitor &visitor) override
	{
		visitor.Visit(*this);
	}

	virtual double GetValue() const override
	{
		return Value;
	}

	lJSON_Double(double value)
		:Value(value)
	{}

	virtual ~lJSON_Double(){}
};

class lJSON_Bool : public liJSON_Value, public liJSON_Bool
{
private:
	bool Value;

public:

	virtual void Accept(liJSON_Visitor &visitor) const override
	{
		visitor.Visit(*this);
	}

	virtual void Accept(liJSON_Visitor &visitor) override
	{
		visitor.Visit(*this);
	}

	virtual bool GetValue() const override
	{
		return Value;
	}

	lJSON_Bool(bool value)
		:Value(value)
	{}

	virtual ~lJSON_Bool(){}
};

class lJSON_String : public liJSON_Value, public liJSON_String
{
private:
	std::string Value;

public:

	virtual void Accept(liJSON_Visitor &visitor) const override
	{
		visitor.Visit(*this);
	}

	virtual void Accept(liJSON_Visitor &visitor) override
	{
		visitor.Visit(*this);
	}

	virtual const std::string &GetValue() const override
	{
		return Value;
	}

	lJSON_String(const std::string &value)
		:Value(value)
	{}

	virtual ~lJSON_String(){}
};

class lJSON_Object : public liJSON_Value, public liJSON_Object
{
private:
	std::map<std::string,liJSON_Value  *> Variables;

public:

	virtual void Accept(liJSON_Visitor &visitor) const override
	{
		visitor.Visit(*this);
	}

	virtual void Accept(liJSON_Visitor &visitor) override
	{
		visitor.Visit(*this);
	}

	virtual const liJSON_Value *GetVariable(const std::string &name) const override
	{
		auto I = Variables.find(name);

		if(I != Variables.end())
		{
			return I->second;
		}

		return nullptr;
	}

	virtual liJSON_Value *GetVariable(const std::string &name) override
	{
		auto I = Variables.find(name);

		if(I != Variables.end())
		{
			return I->second;
		}

		return nullptr;
	}

	virtual void Forall(liForallStrategy *forall_strategy) override
	{
		for(auto I : Variables)
		{
			forall_strategy->PerformAction(I.first,I.second);
		}
	}

	virtual void Forall(liForallStrategy *forall_strategy) const override
	{
		for(auto I : Variables)
		{
			forall_strategy->PerformAction(I.first,I.second);
		}
	}

	bool AddVariable(const std::string &name,liJSON_Value *value)
	{
		auto I = Variables.find(name);

		if(I == Variables.end())
		{
			Variables[name] = value;
			return true;
		}

		return false;
	}

	lJSON_Object(){}

	virtual ~lJSON_Object()
	{
		for(auto I : Variables)
		{
			delete I.second;
		}
	}
};

/*
 * Null object
 */
class lJSON_Null : public liJSON_Value
{
public:

	virtual void Accept(liJSON_Visitor &visitor) override
	{
		visitor.VisitNull(*this);
	}

	virtual void Accept(liJSON_Visitor &visitor) const override
	{
		visitor.VisitNull(*this);
	}

	lJSON_Null()
		{}

	virtual ~lJSON_Null() override
		{}
};

#include <string>

#include <iostream>


class PrintJSON : public liJSON_Visitor
{
private:

	class PrintObjectVariables : public liJSON_Object::liForallStrategy
	{
	private:
		PrintJSON *Printer;

	public:
		virtual void PerformAction(const std::string &key,liJSON_Value *value)
		{
			std::cout << key << ":";
			value->Accept(*Printer);
		}

		virtual void PerformAction(const std::string &key,const liJSON_Value *value)
		{
			std::cout << key << ":";
			value->Accept(*Printer);
		}

		PrintObjectVariables(PrintJSON *printer)
			:Printer(printer)
		{}
		virtual ~PrintObjectVariables() override
		{}
	};

public:
	virtual void Visit(const liJSON_Array &param) override
	{
		for(int i=0;i < param.Size();i++)
		{
			param.GetElement(i)->Accept(*this);
		}
	}

	virtual void Visit(const liJSON_Integer &param) override
	{
		std::cout << param.GetValue() << std::endl;
	}

	virtual void Visit(const liJSON_Double &param) override
	{
		std::cout << param.GetValue() << std::endl;
	}

	virtual void Visit(const liJSON_Bool &param) override
	{
		std::cout << param.GetValue() << std::endl;
	}

	virtual void Visit(const liJSON_String &param) override
	{
		std::cout << param.GetValue() << std::endl;
	}

	virtual void Visit(const liJSON_Object &param) override
	{
		PrintObjectVariables Print(this);
		param.Forall(&Print);
	}

	virtual void VisitNull(const liJSON_Value &param) override
	{
		std::cout << "null" << std::endl;
	}

	virtual void Visit(liJSON_Array &param) override
	{
		for(int i=0;i < param.Size();i++)
		{
			param.GetElement(i)->Accept(*this);
		}
	}

	virtual void Visit(liJSON_Integer &param) override
	{
		std::cout << param.GetValue() << std::endl;
	}

	virtual void Visit(liJSON_Double &param) override
	{
		std::cout << param.GetValue() << std::endl;
	}

	virtual void Visit(liJSON_Bool &param) override
	{
		std::cout << param.GetValue() << std::endl;
	}

	virtual void Visit(liJSON_String &param) override
	{
		std::cout << param.GetValue() << std::endl;
	}

	virtual void Visit(liJSON_Object &param) override
	{
		PrintObjectVariables Print(this);
		param.Forall(&Print);
	}

	virtual void VisitNull(liJSON_Value &param) override
	{
		std::cout << "null" << std::endl;
	}

	PrintJSON(){}
	virtual ~PrintJSON() override {}
};

#include <istream>

class lJSON_StatefulParser
{
private:
	char NextChar;
	std::istream &In;

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
		std::cout << "Started parsing string" << std::endl;
		std::string Value;

		ParseString(Value);

		std::cout << "Finished parsing string" << std::endl;
		dest = new lJSON_String(Value);
	}

	void ParseArray(liJSON_Value *&dest)
	{
		std::cout << "Started parsing array" << std::endl;
		liJSON_Value *NextValue;
		ParseValue(NextValue);
		//
		lJSON_Array::ArrayBuilder ArrayBuilder;
		ArrayBuilder.AddValue(NextValue);
		//
		bool Error = false;
		std::cout << "NextChar:" << NextChar << ";";
		//
		while(NextChar != ']')
		{
			//std::cout << in.tellg() << std::endl;
			std::cout << "NextChar:" << NextChar << ";";
			if(!std::isspace(NextChar))
			{
				if(NextChar == ',')
				{
					Error = ParseValue(NextValue);
					if(Error)
					{
						std::cout << "Error while parsing array" << std::endl;
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
						std::cout << "Error: end of file reacher prematurely" << std::endl;
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
					std::cout << "Error: end of file reacher prematurely" << std::endl;
					break;
				}
			}
		}

		if(!In.eof())
		{
			In.get(NextChar);
		}

		std::cout << "Finished parsing array" << std::endl;
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
		//In.get(NextChar);
		while(std::isspace(NextChar))
			{In.get(NextChar);}
		//
		if(NextChar !=  ':')
		{
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
		std::cout << "Started parsing object" << std::endl;
		std::string NextKey;
		liJSON_Value *NextValue;
		ParsePair(NextKey,NextValue);

		//char NextChar;
		bool Error = false;
		//in.get(NextChar);
		std::cout << "NextChar:" << NextChar << ";";

		lJSON_Object *NewObject = new lJSON_Object;
		NewObject->AddVariable(NextKey,NextValue);
		NextKey.clear();

		while(NextChar != '}')
		{
			//std::cout << in.tellg() << std::endl;
			std::cout << "NextChar:" << NextChar << ";";
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
						std::cout << "Error: end of file reacher prematurely" << std::endl;
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
					std::cout << "Error: end of file reacher prematurely" << std::endl;
					break;
				}
			}
		}
		//
		std::cout << "Finished parsing object" << std::endl;
		dest = NewObject;
		//std::cerr << "Parsing an object is not yet implemented." << std::endl;
	}

	void ParseTrue(liJSON_Value *&dest)
	{
		std::cout << "Started parsing true" << std::endl;
		char Chars[3];
		//
		In.get(Chars[0]);
		if(Chars[0] != 'r')
		{std::cerr << "Error while parsing true" << std::endl;}
		//
		In.get(Chars[1]);
		if(Chars[1] != 'u')
		{std::cerr << "Error while parsing true" << std::endl;}
		//
		In.get(Chars[2]);
		if(Chars[2] != 'e')
		{std::cerr << "Error while parsing true" << std::endl;}
		//
		In.get(NextChar);
		//
		std::cout << "Finished parsing true" << std::endl;
		dest = new lJSON_Bool(true);
	}

	void ParseFalse(liJSON_Value *&dest)
	{
		std::cout << "Started parsing false" << std::endl;
		char Chars[4];
		//
		In.get(Chars[0]);
		if(Chars[0] != 'a')
		{std::cerr << "Error while parsing false" << std::endl;}
		//
		In.get(Chars[1]);
		if(Chars[1] != 'l')
		{std::cerr << "Error while parsing false" << std::endl;}
		//
		In.get(Chars[2]);
		if(Chars[2] != 's')
		{std::cerr << "Error while parsing false" << std::endl;}
		//
		In.get(Chars[3]);
		if(Chars[3] != 'e')
		{std::cerr << "Error while parsing false" << std::endl;}
		//
		In.get(NextChar);
		//
		std::cout << "Finished parsing false" << std::endl;
		dest = new lJSON_Bool(false);
	}

	void ParseNull(liJSON_Value *&dest)
	{
		std::cout << "Started parsing null" << std::endl;
		char Chars[3];
		//
		In.get(Chars[0]);
		if(Chars[0] != 'u')
		{std::cerr << "Error while parsing null" << std::endl;}
		//
		In.get(Chars[1]);
		if(Chars[1] != 'l')
		{std::cerr << "Error while parsing null" << std::endl;}
		//
		In.get(Chars[2]);
		if(Chars[2] != 'l')
		{std::cerr << "Error while parsing null" << std::endl;}
		//
		In.get(NextChar);
		//
		std::cout << "Finished parsing null" << std::endl;
		dest = new lJSON_Null;
	}

	bool ParseNumber(liJSON_Value *&dest)
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
		std::cout << "NextChar:" << (int)NextChar << std::endl;
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


class lJSON_Util
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

public:

	static void Parse(std::istream &in,liJSON_Value *&dest)
	{
		std::cout << "Starting parsing" << std::endl;

		//ParseValue(in,dest);
		lJSON_StatefulParser Parser(in);
		Parser.Parse(dest);

		std::cout << "Finished parsing" << std::endl;
	}

	lJSON_Util(){}
	~lJSON_Util(){}
};

#endif // L_JSON_H
