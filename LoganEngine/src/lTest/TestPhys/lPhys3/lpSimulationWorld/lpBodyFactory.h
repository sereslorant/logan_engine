#ifndef LP_BODY_FACTORY_H
#define LP_BODY_FACTORY_H

#include "lpBodyBuilder.h"

#include <list>

class lpBodyFactory
{
private:
	std::list<lpBodyBuilder> Builders;
	
	lpBodyBuilder::liBodyContainer &World;
	
public:
	
	lpBodyBuilder &CreateBody()
	{
		Builders.emplace_back(World);
		
		return Builders.back();
	}
	
	lpBodyFactory(lpBodyBuilder::liBodyContainer &world)
		:World(world)
	{}
	
	~lpBodyFactory()
	{}
	/*
	 * End of class
	 */
};

#endif // LP_BODY_FACTORY_H
