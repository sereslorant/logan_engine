#ifndef L_JSON_TYPE_RETRIEVER_H
#define L_JSON_TYPE_RETRIEVER_H

#include "liJSON.h"

class lJSON_TypeRetriever : private liJSON_Visitor
{
private:
	bool Null = false;
	
	const liJSON_Array		*ConstArray		= nullptr;
	const liJSON_Integer	*ConstInteger	= nullptr;
	const liJSON_Double		*ConstDouble	= nullptr;
	const liJSON_Bool		*ConstBool		= nullptr;
	const liJSON_String		*ConstString	= nullptr;
	const liJSON_Object		*ConstObject	= nullptr;
	
	liJSON_Array	*Array = nullptr;
	liJSON_Integer	*Integer = nullptr;
	liJSON_Double	*Double = nullptr;
	liJSON_Bool		*Bool = nullptr;
	liJSON_String	*String = nullptr;
	liJSON_Object	*Object = nullptr;
	
public:
	
	bool IsNull() const
	{
		return Null;
	}
	
	const liJSON_Array *ToConstArray() const
	{
		return ConstArray;
	}
	
	const liJSON_Integer *ToConstInteger() const
	{
		return ConstInteger;
	}
	
	const liJSON_Double *ToConstDouble() const
	{
		return ConstDouble;
	}
	
	const liJSON_Bool *ToConstBool() const
	{
		return ConstBool;
	}
	
	const liJSON_String *ToConstString() const
	{
		return ConstString;
	}
	
	const liJSON_Object *ToConstObject() const
	{
		return ConstObject;
	}
	
	liJSON_Array *ToArray() const
	{
		return Array;
	}
	
	liJSON_Integer *ToInteger() const
	{
		return Integer;
	}
	
	liJSON_Double *ToDouble() const
	{
		return Double;
	}
	
	liJSON_Bool *ToBool() const
	{
		return Bool;
	}
	
	liJSON_String *ToString() const
	{
		return String;
	}
	
	liJSON_Object *ToObject() const
	{
		return Object;
	}
	
	/*
	 * Functions inherited from liJSON_Visitor
	 */
	
	virtual void Visit(const liJSON_Array &param) override
	{
		ConstArray = &param;
	}
	
	virtual void Visit(const liJSON_Integer &param) override
	{
		ConstInteger = &param;
	}
	
	virtual void Visit(const liJSON_Double &param) override
	{
		ConstDouble	= &param;
	}
	
	virtual void Visit(const liJSON_Bool &param) override
	{
		ConstBool	= &param;
	}
	
	virtual void Visit(const liJSON_String &param) override
	{
		ConstString	= &param;
	}
	
	virtual void Visit(const liJSON_Object &param) override
	{
		ConstObject	= &param;
	}
	
	virtual void Visit(liJSON_Array &param) override
	{
		ConstArray	= &param;
		Array		= &param;
	}
	
	virtual void Visit(liJSON_Integer &param) override
	{
		ConstInteger = &param;
		Integer		 = &param;
	}
	
	virtual void Visit(liJSON_Double &param) override
	{
		ConstDouble	= &param;
		Double		= &param;
	}
	
	virtual void Visit(liJSON_Bool &param) override
	{
		ConstBool	= &param;
		Bool		= &param;
	}
	
	virtual void Visit(liJSON_String &param) override
	{
		ConstString	= &param;
		String		= &param;
	}
	
	virtual void Visit(liJSON_Object &param) override
	{
		ConstObject	= &param;
		Object		= &param;
	}
	
	virtual void VisitNull() override
	{
		Null = true;
	}
	
	lJSON_TypeRetriever(liJSON_Value *value)
	{
		if(value != nullptr)
		{
			value->Accept(*this);
		}
	}
	
	lJSON_TypeRetriever(const liJSON_Value *value)
	{
		if(value != nullptr)
		{
			value->Accept(*this);
		}
	}

	virtual ~lJSON_TypeRetriever() override
	{
		//
	}
};

bool IsNull(const liJSON_Value *value);

const liJSON_Array *ToConstArray(const liJSON_Value *value);

const liJSON_Integer *ToConstInteger(const liJSON_Value *value);

const liJSON_Double *ToConstDouble(const liJSON_Value *value);

const liJSON_Bool *ToConstBool(const liJSON_Value *value);

const liJSON_String *ToConstString(const liJSON_Value *value);

const liJSON_Object *ToConstObject(const liJSON_Value *value);

liJSON_Array *ToArray(liJSON_Value *value);

liJSON_Integer *ToInteger(liJSON_Value *value);

liJSON_Double *ToDouble(liJSON_Value *value);

liJSON_Bool *ToBool(liJSON_Value *value);

liJSON_String *ToString(liJSON_Value *value);

liJSON_Object *ToObject(liJSON_Value *value);

#endif // L_JSON_TYPE_RETRIEVER_H
