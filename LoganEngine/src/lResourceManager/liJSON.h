#ifndef LI_JSON_H
#define LI_JSON_H

#include <string>

class liJSON_Visitor;

class liJSON_Value
{
public:
	virtual void Accept(liJSON_Visitor &visitor) = 0;
	virtual void Accept(liJSON_Visitor &visitor) const = 0;

	liJSON_Value()
		{}

	virtual ~liJSON_Value()
		{}
};

class liJSON_Array
{
public:
	virtual unsigned int Size() const = 0;

	virtual const liJSON_Value *GetElement(unsigned int i) const = 0;
	virtual liJSON_Value *GetElement(unsigned int i) = 0;

	liJSON_Array()
		{}

	virtual ~liJSON_Array()
		{}
};

class liJSON_Integer
{
public:
	virtual int GetValue() const = 0;

	liJSON_Integer()
		{}

	virtual ~liJSON_Integer()
		{}
};

class liJSON_Double
{
public:
	virtual double GetValue() const = 0;

	liJSON_Double()
		{}

	virtual ~liJSON_Double()
		{}
};

class liJSON_Bool
{
public:
	virtual bool GetValue() const = 0;

	liJSON_Bool()
		{}

	virtual ~liJSON_Bool()
		{}
};

class liJSON_String
{
public:
	virtual const std::string &GetValue() const = 0;

	liJSON_String()
		{}

	virtual ~liJSON_String()
		{}
};

class liJSON_Object
{
public:
	class liForallStrategy
	{
	public:
		virtual void PerformAction(const std::string &key,liJSON_Value *value) = 0;
		virtual void PerformAction(const std::string &key,const liJSON_Value *value) = 0;

		liForallStrategy(){}
		virtual ~liForallStrategy(){}
	};

	virtual const liJSON_Value *GetVariable(const std::string &name) const = 0;
	virtual liJSON_Value *GetVariable(const std::string &name) = 0;

	virtual void Forall(liForallStrategy *forall_strategy) = 0;
	virtual void Forall(liForallStrategy *forall_strategy) const = 0;

	liJSON_Object()
		{}

	virtual ~liJSON_Object()
		{}
};

class liJSON_Visitor
{
public:
	virtual void Visit(const liJSON_Array &param) = 0;
	virtual void Visit(const liJSON_Integer &param) = 0;
	virtual void Visit(const liJSON_Double &param) = 0;
	virtual void Visit(const liJSON_Bool &param) = 0;
	virtual void Visit(const liJSON_String &param) = 0;
	virtual void Visit(const liJSON_Object &param) = 0;
	virtual void VisitNull(const liJSON_Value &param) = 0;

	virtual void Visit(liJSON_Array &param) = 0;
	virtual void Visit(liJSON_Integer &param) = 0;
	virtual void Visit(liJSON_Double &param) = 0;
	virtual void Visit(liJSON_Bool &param) = 0;
	virtual void Visit(liJSON_String &param) = 0;
	virtual void Visit(liJSON_Object &param) = 0;
	virtual void VisitNull(liJSON_Value &param) = 0;


	liJSON_Visitor()
		{}

	virtual ~liJSON_Visitor()
		{}
};

#endif // LI_JSON_H
