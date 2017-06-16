#ifndef LJSON_DATA_H
#define LJSON_DATA_H

#include "liJSON.h"

#include <vector>
#include <list>
#include <map>

class lJSON_Array : public liJSON_Value, public liJSON_Array
{
private:
	std::vector<liJSON_Value *> Elements;

	/*
	 * TODO: Meg kell nézni, hogy az unsigned int vs std::vector<akarmi>::size_type warningot érdemes-e kijavítani.
	 */
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

	virtual void Accept(liJSON_Visitor &visitor) override;

	virtual void Accept(liJSON_Visitor &visitor) const override;

	virtual unsigned int Size() const override;

	virtual const liJSON_Value *GetElement(unsigned int i) const override;

	virtual liJSON_Value *GetElement(unsigned int i) override;

	class ArrayBuilder
	{
	private:
		std::list<liJSON_Value *> Variables;
	public:

		ArrayBuilder &AddValue(liJSON_Value *value);

		liJSON_Value *Construct();

		ArrayBuilder();
		~ArrayBuilder();
	};

	virtual ~lJSON_Array() override;
	/*
	 * End of class
	 */
};

class lJSON_Integer : public liJSON_Value, public liJSON_Integer
{
private:
	int Value;
	
public:
	
	virtual void Accept(liJSON_Visitor &visitor) override;
	
	virtual void Accept(liJSON_Visitor &visitor) const override;
	
	virtual int GetValue() const override;
	
	lJSON_Integer(int value);
	
	virtual ~lJSON_Integer() override;
	/*
	 * End of class
	 */
};

class lJSON_Double : public liJSON_Value, public liJSON_Double
{
private:
	double Value;
	
public:
	
	virtual void Accept(liJSON_Visitor &visitor) override;
	
	virtual void Accept(liJSON_Visitor &visitor) const override;
	
	virtual double GetValue() const override;
	
	lJSON_Double(double value);
	
	virtual ~lJSON_Double() override;
	/*
	 * End of class
	 */
};

class lJSON_Bool : public liJSON_Value, public liJSON_Bool
{
private:
	bool Value;
	
public:
	
	virtual void Accept(liJSON_Visitor &visitor) override;
	
	virtual void Accept(liJSON_Visitor &visitor) const override;
	
	virtual bool GetValue() const override;
	
	lJSON_Bool(bool value);
	
	virtual ~lJSON_Bool() override;
	/*
	 * End of class
	 */
};

class lJSON_String : public liJSON_Value, public liJSON_String
{
private:
	std::string Value;
	
public:
	
	virtual void Accept(liJSON_Visitor &visitor) override;
	
	virtual void Accept(liJSON_Visitor &visitor) const override;
	
	virtual const std::string &GetValue() const override;
	
	lJSON_String(const std::string &value);
	
	virtual ~lJSON_String() override;
	/*
	 * End of class
	 */
};

class lJSON_Object : public liJSON_Value, public liJSON_Object
{
private:
	std::map<std::string,liJSON_Value  *> Variables;
	
public:
	
	virtual void Accept(liJSON_Visitor &visitor) override;
	
	virtual void Accept(liJSON_Visitor &visitor) const override;
	
	virtual const liJSON_Value *GetVariable(const std::string &name) const override;
	
	virtual liJSON_Value *GetVariable(const std::string &name) override;
	
	virtual void Forall(std::function<void (const std::string&,liJSON_Value *)> function) override;
	
	virtual void Forall(std::function<void (const std::string&,const liJSON_Value *)> function) const override;
	
	bool AddVariable(const std::string &name,liJSON_Value *value);
	
	lJSON_Object();
	
	virtual ~lJSON_Object() override;
	/*
	 * End of class
	 */
};

/*
 * Null object
 */
class lJSON_Null : public liJSON_Value
{
public:
	
	virtual void Accept(liJSON_Visitor &visitor) override;
	
	virtual void Accept(liJSON_Visitor &visitor) const override;
	
	lJSON_Null();
	
	virtual ~lJSON_Null() override;
	/*
	 * End of class
	 */
};

#endif // LJSON_DATA_H
