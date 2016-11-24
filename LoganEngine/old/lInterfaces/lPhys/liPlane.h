#ifndef LI_PLANE_H
#define LI_PLANE_H

#include "../../lMath/lMath.h"

class liPlane
{
public:

	virtual const lmVector3D &GetPosition() const = 0;
	virtual void SetPosition(const lmVector3D &position) = 0;

	virtual const lmVector3D &GetNormal() const = 0;
	virtual void SetNormal(const lmVector3D &normal) = 0;

	liPlane(){}
	virtual ~liPlane(){}
};

#endif // LI_PLANE_H
