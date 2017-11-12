
#include "lJSON_TypeRetriever.h"


bool IsNull(const liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).IsNull();
}

const liJSON_Array *ToConstArray(const liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToConstArray();
}

const liJSON_Integer *ToConstInteger(const liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToConstInteger();
}

const liJSON_Double *ToConstDouble(const liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToConstDouble();
}

const liJSON_Bool *ToConstBool(const liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToConstBool();
}

const liJSON_String *ToConstString(const liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToConstString();
}

const liJSON_Object *ToConstObject(const liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToConstObject();
}

liJSON_Array *ToArray(liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToArray();
}

liJSON_Integer *ToInteger(liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToInteger();
}

liJSON_Double *ToDouble(liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToDouble();
}

liJSON_Bool *ToBool(liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToBool();
}

liJSON_String *ToString(liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToString();
}

liJSON_Object *ToObject(liJSON_Value *value)
{
	return lJSON_TypeRetriever(value).ToObject();
}
