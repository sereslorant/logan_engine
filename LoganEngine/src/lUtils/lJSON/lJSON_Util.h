#ifndef LJSON_UTIL_H
#define LJSON_UTIL_H

#include "liJSON.h"

class lJSON_Util
{
public:
	
	static void Parse(std::istream &in,liJSON_Value *&dest);
	
	static void Print(const liJSON_Integer &value,std::ostream &out);
	static void Print(const liJSON_Double &value,std::ostream &out);
	static void Print(const liJSON_Bool &value,std::ostream &out);
	static void Print(const liJSON_String &value,std::ostream &out);
	static void Print(const liJSON_Object &value,std::ostream &out);
	static void Print(const liJSON_Array &value,std::ostream &out);
	
	static void Print(const liJSON_Value &value,std::ostream &out);
	
	lJSON_Util(){}
	~lJSON_Util(){}
};




#endif // LJSON_UTIL_H
