#ifndef LP_SPHERE_BUILDER_H
#define LP_SPHERE_BUILDER_H

#include "lpShapeBuilder.h"

#include "../lpCollisionShapes/lpSphere.h"

class lpSphereBuilder : public lpShapeBuilder
{
public:
	float Radius = 0.0;
	
	lpSphereBuilder &SetPosition(const lmVector3D &position)
	{
		State.Position = position;
		return *this;
	}
	
	lpSphereBuilder &SetRadius(float radius)
	{
		Radius = radius;
		return *this;
	}
	
	virtual lpSphereBuilder &Construct()
	{
		CollisionShape = new lpSphere(State,Radius);
		return *this;
	}
	
	lpSphereBuilder()
	{}
	
	virtual ~lpSphereBuilder() override
	{}
	/*
	 * End of class
	 */
};

#endif // LP_SPHERE_BUILDER_H
