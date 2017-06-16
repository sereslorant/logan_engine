#ifndef LJSON_PRINTER_H
#define LJSON_PRINTER_H

#include "liJSON.h"

#include <ostream>

class lJSON_Printer : public liJSON_Visitor
{
private:
	std::ostream &Out;
	
	class PrintObjectVariables //: public liJSON_Object::liForallStrategy
	{
	private:
		lJSON_Printer *Printer;
		
	public:
		virtual void PerformAction(const std::string &key,liJSON_Value *value) //override
		{
			Printer->Out << key << ":";
			value->Accept(*Printer);
		}
		
		virtual void PerformAction(const std::string &key,const liJSON_Value *value) //override
		{
			Printer->Out << key << ":";
			value->Accept(*Printer);
		}
		
		PrintObjectVariables(lJSON_Printer *printer)
			:Printer(printer)
		{}
		virtual ~PrintObjectVariables() //override
		{}
	};
	
public:
	virtual void Visit(const liJSON_Array &param) override
	{
		Out << "[" << std::endl;
		
		for(unsigned int i=0;i < param.Size();i++)
		{
			param.GetElement(i)->Accept(*this);
		}
		
		Out << "]" << std::endl;
	}
	
	virtual void Visit(const liJSON_Integer &param) override
	{
		Out << param.GetValue() << std::endl;
	}
	
	virtual void Visit(const liJSON_Double &param) override
	{
		Out << param.GetValue() << std::endl;
	}
	
	virtual void Visit(const liJSON_Bool &param) override
	{
		Out << param.GetValue() << std::endl;
	}
	
	virtual void Visit(const liJSON_String &param) override
	{
		Out << param.GetValue() << std::endl;
	}
	
	virtual void Visit(const liJSON_Object &param) override
	{
		Out << "{" << std::endl;
		
		PrintObjectVariables Print(this);
		
		param.Forall([&Print](const std::string &key,const liJSON_Value *value)
		{
			Print.PerformAction(key,value);
		}
		);
		
		Out << "}" << std::endl;
	}
	
	virtual void Visit(liJSON_Array &param) override
	{
		Out << "[" << std::endl;
		
		for(unsigned int i=0;i < param.Size();i++)
		{
			param.GetElement(i)->Accept(*this);
		}
		
		Out << "]" << std::endl;
	}
	
	virtual void Visit(liJSON_Integer &param) override
	{
		Out << param.GetValue() << std::endl;
	}
	
	virtual void Visit(liJSON_Double &param) override
	{
		Out << param.GetValue() << std::endl;
	}
	
	virtual void Visit(liJSON_Bool &param) override
	{
		Out << param.GetValue() << std::endl;
	}
	
	virtual void Visit(liJSON_String &param) override
	{
		Out << param.GetValue() << std::endl;
	}
	
	virtual void Visit(liJSON_Object &param) override
	{
		Out << "{" << std::endl;
		
		PrintObjectVariables Print(this);
		param.Forall([&Print](const std::string &key,const liJSON_Value *value)
		{
			Print.PerformAction(key,value);
		}
		);
		
		Out << "}" << std::endl;
	}
	
	virtual void VisitNull() override
	{
		Out << "null" << std::endl;
	}
	
	lJSON_Printer(std::ostream &out)
		:Out(out)
	{}
	
	virtual ~lJSON_Printer() override
	{}
};

#endif // LJSON_PRINTER_H
