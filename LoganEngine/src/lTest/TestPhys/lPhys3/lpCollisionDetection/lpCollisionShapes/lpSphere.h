#ifndef LP_SPHERE_H
#define LP_SPHERE_H

#include "lpCollisionShape.h"

class lpSphere : public lpCollisionShape
{
protected:
	float Radius;
	
public:
	
	virtual void Accept(liShapeVisitor &visitor) override
	{
		visitor.VisitSphere(this);
	}
	
	virtual void Accept(liShapeVisitor &visitor) const override
	{
		visitor.VisitSphere(this);
	}
	
	float GetRadius() const
	{
		return Radius;
	}
	
	void SetRadius(float radius)
	{
		Radius = radius;
	}
	
	lpSphere(const lpShapeState &shape_state,float radius = 0.0f)
		:lpCollisionShape(shape_state),Radius(radius)
	{}
	
	virtual ~lpSphere() override
	{}
	/*
	 * End of class
	 */
};

#endif // LP_SPHERE_H
