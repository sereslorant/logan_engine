#ifndef LP_COMPOSITE_SHAPE_H
#define LP_COMPOSITE_SHAPE_H

#include "lpCollisionShape.h"

#include <vector>

class lpCompositeShape : public lpCollisionShape
{
protected:
	std::vector<lpCollisionShape *> Children;
	
public:
	
	virtual void Accept(liShapeVisitor &visitor) override
	{
		visitor.VisitComposite(this);
	}
	
	virtual void Accept(liShapeVisitor &visitor) const override
	{
		visitor.VisitComposite(this);
	}
	
	unsigned int GetNumChildren() const
	{
		return Children.size();
	}
	
	lpCollisionShape *GetChild(unsigned int id)
	{
		return Children[id];
	}
	
	const lpCollisionShape *GetChild(unsigned int id) const
	{
		return Children[id];
	}
	
	template<class IteratorType>
	lpCompositeShape(const lpShapeState &shape_state,IteratorType begin,IteratorType end)
		:lpCollisionShape(shape_state),Children(begin,end)
	{
		for(lpCollisionShape *Child : Children)
		{
			Child->SetParent(this);
		}
	}
	
	virtual ~lpCompositeShape() override
	{}
	/*
	 * End of class
	 */
};

#endif // LP_COMPOSITE_SHAPE_H
