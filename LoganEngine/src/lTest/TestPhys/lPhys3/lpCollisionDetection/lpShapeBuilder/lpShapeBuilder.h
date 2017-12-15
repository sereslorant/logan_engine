#ifndef LP_SHAPE_BUILDER_H
#define LP_SHAPE_BUILDER_H

#include "../lpCollisionShapes/lpCollisionShape.h"

class lpShapeBuilder
{
public:
	
	class liShapeContainer
	{
	public:
		virtual void AddShape(lpCollisionShape *new_shape) = 0;
		
		liShapeContainer(){}
		virtual ~liShapeContainer(){}
		/*
		 * End of class
		 */
	};
	
protected:
	lpShapeState State;
	lpCollisionShape *CollisionShape = nullptr;
	
public:
	
	bool IsConstructed()
	{
		return CollisionShape != nullptr;
	}
	
	lpCollisionShape *GetCollisionShape()
	{
		return CollisionShape;
	}
	
	lpShapeBuilder()
	{}
	
	virtual ~lpShapeBuilder()
	{}
	/*
	 * End of class
	 */
};

#endif // LP_SHAPE_BUILDER_H
