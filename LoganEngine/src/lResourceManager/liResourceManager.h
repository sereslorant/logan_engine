#ifndef LI_RESOURCE_MANAGER_H
#define LI_RESOURCE_MANAGER_H

#include "../lMath/lMath.h"

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

#include "lrmStaticMesh.h"

/*
 * Bitképekhez kötkető interfész
 */

#include <inttypes.h>

class liBitmap2D
{
public:
	//
    class liSampler
    {
	public:
    	//
		virtual float GetRed(int i,int j) = 0;
		virtual float GetGreen(int i,int j) = 0;
		virtual float GetBlue(int i,int j) = 0;
		virtual float GetAlpha(int i,int j) = 0;
		//
		liSampler(){}
		virtual ~liSampler(){}
		/*
		 * End of class
		 */
    };
    //
    virtual int32_t GetWidth() const = 0;
    virtual int32_t GetHeight() const = 0;
    //
    virtual const char *GetPixelArray() const = 0;
    //virtual liSampler &GetSampler() = 0;
    //
	liBitmap2D(){}
	virtual ~liBitmap2D(){}
	/*
	 * End of class
	 */
};

class liCubemap
{
public:
	//
	enum LI_CUBEMAP_SIDE
	{
		LI_CUBEMAP_UP		= 0,
		LI_CUBEMAP_DOWN		= 1,
		LI_CUBEMAP_FRONT	= 2,
		LI_CUBEMAP_BACK		= 3,
		LI_CUBEMAP_LEFT		= 4,
		LI_CUBEMAP_RIGHT	= 5,
	};
	//
	virtual int32_t GetSize() const = 0;
	//
	virtual const liBitmap2D &GetSide(LI_CUBEMAP_SIDE cubemap_side) const = 0;
	//
	liCubemap(){}
	virtual ~liCubemap(){}
	/*
	 * End of class
	 */
};

class liResourceManager
{
public:
	//
	virtual lrmStaticMesh *GetStaticMesh(const std::string &resource_identifier) = 0;
	virtual liBitmap2D *GetBitmap(const std::string &resource_identifier) = 0;
	virtual liCubemap *GetCubemap(const std::string &resource_identifier) = 0;
	//
	liResourceManager(){}
	virtual ~liResourceManager() {}
};

#endif // LI_RESOURCE_MANAGER_H
