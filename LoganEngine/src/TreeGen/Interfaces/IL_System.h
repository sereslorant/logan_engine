#ifndef IL_SYSTEM_H
#define IL_SYSTEM_H

#include <string>

class IL_System
{
public:

	virtual void ApplyProductions(const std::string &str,std::string &dest) = 0;

	IL_System()
	{
		/*
		 * Üres függvénytörzs
		 */
	}

	virtual ~IL_System()
	{
		/*
		 * Üres függvénytörzs
		 */
	}
};

#endif // IL_SYSTEM_H
