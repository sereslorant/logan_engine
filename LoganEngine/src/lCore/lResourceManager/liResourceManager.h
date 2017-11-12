#ifndef LI_RESOURCE_MANAGER_H
#define LI_RESOURCE_MANAGER_H

/*
 * Egyelőre még nincs használatban.
 */
template<class lResource_T>
class liResource
{
public:
	virtual bool IsReady() = 0;
	virtual bool ErrorHappened() = 0;
	//
	virtual lResource_T &GetResource() = 0;
	//
	liResource(){}
	virtual ~liResource(){}
	/*
	 * End of class
	 */
};


#include "liBitmap2D.h"
#include "liCubemap.h"

#include "lLegacyResourceManager/lrmStaticMesh.h"
#include "lLegacyResourceManager/lrmSkeletalMesh.h"

class liResourceManager
{
public:
	//
	virtual lrmStaticMesh *GetStaticMesh(const std::string &resource_identifier) = 0;
	virtual lrmSkeletalMesh *GetSkeletalMesh(const std::string &resource_identifier) = 0;
	virtual liBitmap2D *GetBitmap(const std::string &resource_identifier) = 0;
	virtual liCubemap *GetCubemap(const std::string &resource_identifier) = 0;
	//
	liResourceManager(){}
	virtual ~liResourceManager() {}
};

#endif // LI_RESOURCE_MANAGER_H
