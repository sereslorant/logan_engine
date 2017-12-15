#ifndef LP_COMPOSITE_BUILDER_H
#define LP_COMPOSITE_BUILDER_H

#include "lpShapeBuilder.h"

#include "../lpCollisionShapes/lpCompositeShape.h"

class lpCompositeBuilder : public lpShapeBuilder
{
private:
	std::list<std::unique_ptr<lpShapeBuilder> > ChildBuilders;
	
public:
	
	lpCompositeBuilder &SetPosition(const lmVector3D &position)
	{
		State.Position = position;
		return *this;
	}
	
	lpSphereBuilder &CreateSphere()
	{
		lpSphereBuilder *SphereBuilder = new lpSphereBuilder;
		ChildBuilders.emplace_back(SphereBuilder);
		
		return *SphereBuilder;
	}
	
	lpCompositeBuilder &CreateComposite()
	{
		lpCompositeBuilder *CompositeBuilder = new lpCompositeBuilder;
		ChildBuilders.emplace_back(CompositeBuilder);
		
		return *CompositeBuilder;
	}
	
	virtual lpCompositeBuilder &Construct()
	{
		std::list<lpCollisionShape *> Children;
		for(auto &ChildBuilder : ChildBuilders)
		{
			if(ChildBuilder->IsConstructed())
			{
				lpCollisionShape *Child = ChildBuilder->GetCollisionShape();
				Children.push_back(Child);
			}
		}
		
		CollisionShape = new lpCompositeShape(State,Children.begin(),Children.end());
		return *this;
	}
	
	
	lpCompositeBuilder()
	{}
	
	virtual ~lpCompositeBuilder() override
	{}
	/*
	 * End of class
	 */
};

#endif // LP_COMPOSITE_BUILDER_H
