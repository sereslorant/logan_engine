#ifndef LI_CUBEMAP_H
#define LI_CUBEMAP_H

#include <lResourceManager/liBitmap2D.h>

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

#endif // LI_CUBEMAP_H
