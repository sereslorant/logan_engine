
#include "lJSON_Data.h"


void lJSON_Array::Accept(liJSON_Visitor &visitor)
{
	visitor.Visit(*this);
}

void lJSON_Array::Accept(liJSON_Visitor &visitor) const
{
	visitor.Visit(*this);
}


/*
 * TODO Meg kell nézni, hogy érdemes-e a size_type vs unsigned int dolgot javítani.
 */
unsigned int lJSON_Array::Size() const
{
	return Elements.size();
}

const liJSON_Value *lJSON_Array::GetElement(unsigned int i) const
{
	if(i < Elements.size())
	{
		return Elements[i];
	}

	return nullptr;
}

liJSON_Value *lJSON_Array::GetElement(unsigned int i)
{
	if(i < Elements.size())
	{
		return Elements[i];
	}

	return nullptr;
}

lJSON_Array::ArrayBuilder &lJSON_Array::ArrayBuilder::AddValue(liJSON_Value *value)
{
	Variables.push_back(value);
	return *this;
}

liJSON_Value *lJSON_Array::ArrayBuilder::Construct()
{
	return new lJSON_Array(Variables.begin(),Variables.end(),Variables.size());
}

lJSON_Array::ArrayBuilder::ArrayBuilder()
{
	//
}

lJSON_Array::ArrayBuilder::~ArrayBuilder()
{
	//
}

lJSON_Array::~lJSON_Array()
{
	for(liJSON_Value *Element : Elements)
	{
		delete Element;
	}
}


void lJSON_Integer::Accept(liJSON_Visitor &visitor)
{
	visitor.Visit(*this);
}

void lJSON_Integer::Accept(liJSON_Visitor &visitor) const
{
	visitor.Visit(*this);
}

int lJSON_Integer::GetValue() const
{
	return Value;
}

lJSON_Integer::lJSON_Integer(int value)
	:Value(value)
{}

lJSON_Integer::~lJSON_Integer()
{}


void lJSON_Double::Accept(liJSON_Visitor &visitor)
{
	visitor.Visit(*this);
}

void lJSON_Double::Accept(liJSON_Visitor &visitor) const
{
	visitor.Visit(*this);
}

double lJSON_Double::GetValue() const
{
	return Value;
}

lJSON_Double::lJSON_Double(double value)
	:Value(value)
{}

lJSON_Double::~lJSON_Double(){}



void lJSON_Bool::Accept(liJSON_Visitor &visitor)
{
	visitor.Visit(*this);
}

void lJSON_Bool::Accept(liJSON_Visitor &visitor) const
{
	visitor.Visit(*this);
}

bool lJSON_Bool::GetValue() const
{
	return Value;
}

lJSON_Bool::lJSON_Bool(bool value)
	:Value(value)
{
	//
}

lJSON_Bool::~lJSON_Bool()
{
	//
}

void lJSON_String::Accept(liJSON_Visitor &visitor)
{
	visitor.Visit(*this);
}

void lJSON_String::Accept(liJSON_Visitor &visitor) const
{
	visitor.Visit(*this);
}

const std::string &lJSON_String::GetValue() const
{
	return Value;
}

lJSON_String::lJSON_String(const std::string &value)
	:Value(value)
{
	
}

lJSON_String::~lJSON_String()
{
	
}

void lJSON_Object::Accept(liJSON_Visitor &visitor)
{
	visitor.Visit(*this);
}

void lJSON_Object::Accept(liJSON_Visitor &visitor) const
{
	visitor.Visit(*this);
}

const liJSON_Value *lJSON_Object::GetVariable(const std::string &name) const
{
	auto I = Variables.find(name);

	if(I != Variables.end())
	{
		return I->second;
	}

	return nullptr;
}

liJSON_Value *lJSON_Object::GetVariable(const std::string &name)
{
	auto I = Variables.find(name);

	if(I != Variables.end())
	{
		return I->second;
	}

	return nullptr;
}

void lJSON_Object::Forall(std::function<void (const std::string&,liJSON_Value *)> function)
{
	for(auto I : Variables)
	{
		function(I.first,I.second);
	}
}

void lJSON_Object::Forall(std::function<void (const std::string&,const liJSON_Value *)> function) const
{
	for(auto I : Variables)
	{
		function(I.first,I.second);
	}
}

bool lJSON_Object::AddVariable(const std::string &name,liJSON_Value *value)
{
	auto I = Variables.find(name);

	if(I == Variables.end())
	{
		Variables[name] = value;
		return true;
	}

	return false;
}

lJSON_Object::lJSON_Object(){}

lJSON_Object::~lJSON_Object()
{
	for(auto I : Variables)
	{
		delete I.second;
	}
}


void lJSON_Null::Accept(liJSON_Visitor &visitor)
{
	visitor.VisitNull();
}

void lJSON_Null::Accept(liJSON_Visitor &visitor) const
{
	visitor.VisitNull();
}

lJSON_Null::lJSON_Null()
{
	//
}

lJSON_Null::~lJSON_Null()
{
	//
}
