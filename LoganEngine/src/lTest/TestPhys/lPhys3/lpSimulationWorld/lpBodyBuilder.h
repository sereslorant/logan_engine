#ifndef LP_BODY_BUILDER_H
#define LP_BODY_BUILDER_H

#include "lpBody.h"

#include "../lpCollisionDetection/lpShapeBuilder/lpSphereBuilder.h"
#include "../lpCollisionDetection/lpShapeBuilder/lpCompositeBuilder.h"

class lpBodyBuilder
{
public:
	
	class liBodyContainer
	{
	public:
		virtual void AddBody(lpBody *new_body) = 0;
		
		liBodyContainer(){}
		virtual ~liBodyContainer(){}
		/*
		 * End of class
		 */
	};
	
private:
	
	lpShapeState BodyState;
	float Mass = 0.0;
	
	lpBody::lpCollisionResponse CollisionResponse = lpBody::MOVABLE;
	std::unique_ptr<lpShapeBuilder> ShapeBuilder;
	liBodyContainer &World;
	
	lpBody *Body = nullptr;
	
public:
	
	lpBodyBuilder &SetState(const lpShapeState &state)
	{
		BodyState = state;
		
		return *this;
	}
	
	lpBodyBuilder &SetMass(float mass)
	{
		Mass = mass;
		return *this;
	}
	
	lpBodyBuilder &SetCollisionResponse(lpBody::lpCollisionResponse collision_response)
	{
		CollisionResponse = collision_response;
		return *this;
	}
	
	lpSphereBuilder &CreateSphere()
	{
		lpSphereBuilder *SphereBuilder = new lpSphereBuilder;
		ShapeBuilder = std::unique_ptr<lpShapeBuilder>(SphereBuilder);
		
		return *SphereBuilder;
	}
	
	lpCompositeBuilder &CreateComposite()
	{
		lpCompositeBuilder *CompositeBuilder = new lpCompositeBuilder;
		ShapeBuilder = std::unique_ptr<lpShapeBuilder>(CompositeBuilder);
		
		return *CompositeBuilder;
	}
	
	lpBodyBuilder &Construct()
	{
		Body = new lpBody();
		Body->SetState(BodyState.Position,BodyState.Velocity);
		Body->SetMass(Mass);
		Body->SetCollisionResponse(CollisionResponse);
		
		if(ShapeBuilder->IsConstructed())
		{
			Body->SetCollisionShape(ShapeBuilder->GetCollisionShape());
		}
		
		World.AddBody(Body);
		return *this;
	}
	
	bool IsConstructed()
	{
		return Body != nullptr;
	}
	
	lpBody *GetBody()
	{
		return Body;
	}
	
	lpBodyBuilder(liBodyContainer &world)
		:World(world)
	{}
	
	virtual ~lpBodyBuilder(){}
	/*
	 * End of class
	 */
};

#endif // LP_BODY_BUILDER_H
