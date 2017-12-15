#ifndef LP_COLLISION_SHAPE_H
#define LP_COLLISION_SHAPE_H

#include <lCore/lMath/lMath.h>

struct lpShapeState
{
	lmVector3D Position;
	lmVector3D Velocity;
	
	lpShapeState()
	{}
	
	lpShapeState(const lmVector3D &position,const lmVector3D &velocity)
		:Position(position),Velocity(velocity)
	{}
	
	virtual ~lpShapeState()
	{}
	/*
	 * End of class
	 */
};

class lpSphere;
class lpCompositeShape;

class liShapeVisitor
{
public:
	virtual void VisitSphere(lpSphere *sphere) = 0;
	virtual void VisitSphere(const lpSphere *sphere) = 0;
	virtual void VisitComposite(lpCompositeShape *composite) = 0;
	virtual void VisitComposite(const lpCompositeShape *composite) = 0;
	
	liShapeVisitor(){}
	virtual ~liShapeVisitor(){}
	/*
	 * End of class
	 */
};

class lpCollisionShape
{
protected:
	lpCollisionShape *Parent = nullptr;
	lpShapeState ShapeState;
	
public:
	
	void SetParent(lpCollisionShape *new_parent)
	{
		Parent = new_parent;
	}
	
	void SetState(const lpShapeState &new_state)
	{
		ShapeState = new_state;
	}
	
	lmVector3D GetPosition() const
	{
		if(Parent != nullptr)
		{
			return Parent->GetPosition() + ShapeState.Position;
		}
		
		return ShapeState.Position;
	}
	
	const lmVector3D &GetLocalPosition() const
	{
		return ShapeState.Position;
	}
	
	virtual void Accept(liShapeVisitor &visitor) = 0;
	virtual void Accept(liShapeVisitor &visitor) const = 0;
	
	lpCollisionShape(const lpShapeState &shape_state)
		:ShapeState(shape_state)
	{}
	
	virtual ~lpCollisionShape()
	{}
	/*
	 * End of class
	 */
};

#endif // LP_COLLISION_SHAPE_2D_H
