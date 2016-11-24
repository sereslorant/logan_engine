#ifndef LI_SPHERE_H
#define LI_SPHERE_H

#include "../../lMath/lMath.h"

class liSphere
{
public:

	virtual const lmVector3D &GetPosition() const = 0;
	virtual void SetPosition(const lmVector3D &position) = 0;

	virtual lmScalar GetRadius() const = 0;
	virtual void SetRadius(lmScalar radius) = 0;

	liSphere(){}
	virtual ~liSphere(){}
};

#endif // LI_SPHERE_H
