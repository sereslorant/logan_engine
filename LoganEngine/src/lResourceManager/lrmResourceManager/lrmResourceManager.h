#ifndef LRM_RESOURCE_MANAGER_H
#define LRM_RESOURCE_MANAGER_H

#include <lResourceManager/liJSON.h>

#include <lResourceManager/liBitmap2D.h>
#include <lResourceManager/liCubemap.h>

template<class lResource_T>
class liResourceManagerModule
{
public:
	virtual lResource_T &GetResource(const std::string &resource_id) = 0;
	//
	liResourceManagerModule()
	{}
	//
	virtual ~liResourceManagerModule()
	{}
	/*
	 * End of class
	 */
};

class lrmResourceManager
{
private:
	//
	//
public:
	//
	//
	lrmResourceManager()
	{}
	//
	virtual ~lrmResourceManager() //override
	{}
	/*
	 * End of class
	 */
};

#endif
