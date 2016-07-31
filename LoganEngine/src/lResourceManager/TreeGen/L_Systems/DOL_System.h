#ifndef DOL_SYSTEM_H
#define DOL_SYSTEM_H

#include <map>
#include "../Interfaces/IL_System.h"

class DOL_System : public IL_System
{
public:

	class Production
	{
	private:
		std::string Successor;

	public:

		const std::string &GetSuccessor()
		{
			return Successor;
		}

		Production()
		{}

		Production(const std::string &successor)
			:Successor(successor)
		{}

		~Production()
		{}
	};

private:
	std::map<char,Production> Productions;

public:

	void AddProduction(char predecessor,const Production &production)
	{
		Productions[predecessor] = production;
	}

	virtual void ApplyProductions(const std::string &str,std::string &dest) override
	{
		dest.clear();

		for(unsigned int i=0;i < str.size();i++)
		{
			char CurrentChar = str[i];

			auto I = Productions.find(CurrentChar);

			if(I != Productions.end())
			{
				dest += I->second.GetSuccessor();
			}
			else
			{
				dest += CurrentChar;
			}
		}
	}

	DOL_System()
	{
		/*
		 * Üres függvénytörzs
		 */
	}

	virtual ~DOL_System() override
	{
		/*
		 * Üres függvénytörzs
		 */
	}
};

#endif // DOL_SYSTEM_H

