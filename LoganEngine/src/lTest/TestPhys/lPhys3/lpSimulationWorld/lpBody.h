#ifndef LP_BODY_H
#define LP_BODY_H

#include <lCore/lMath/lMath.h>

#include "../lpCollisionDetection/lpCollisionShapes/lpCollisionShape.h"

#include <memory>

class lpBody
{
public:
	enum lpCollisionResponse
	{
		MOVABLE,
		IMMOVABLE,
		GHOST,
	};
	
protected:
	
	bool Removable = false;
	
	lpShapeState State;
	float Mass;
	
	lpCollisionResponse CollisionResponse = MOVABLE;
	
	std::unique_ptr<lpCollisionShape> Shape;
	
	//std::list<liCollisionCallback2D *> CollisionCallbacks;
	//void *UserData = nullptr;
	
public:
	
	void SetCollisionResponse(lpCollisionResponse collision_response)
	{
		CollisionResponse = collision_response;
	}
	
	lpCollisionResponse GetCollisionResponse()
	{
		return CollisionResponse;
	}
	
	/*
	virtual void AddCollisionCallback(liCollisionCallback2D *callback) override
	{
		CollisionCallbacks.push_back(callback);
	}
	
	void Collide(lP2Body2D *other_body)
	{
		for(liCollisionCallback2D *CollisionCallback : CollisionCallbacks)
		{
			CollisionCallback->Callback(other_body);
		}
	}
	*/
	
	/*
	virtual void SetUserData(void *user_data) override
	{
		UserData = user_data;
	}
	
	virtual void *GetUserData() override
	{
		return UserData;
	}
	*/
	
	void MarkRemovable()
	{
		Removable = true;
	}
	
	bool IsRemovable()
	{
		return Removable;
	}
	
	const lmVector3D &GetPosition() const
	{
		return State.Position;
	}
	
	const lmVector3D &GetVelocity() const
	{
		return State.Velocity;
	}
	
	float GetMass() const
	{
		return Mass;
	}
	
	void SetState(const lmVector3D &position,const lmVector3D &velocity)
	{
		State.Position = position;
		State.Velocity = velocity;
	}
	
	lpShapeState &GetState()
	{
		return State;
	}
	
	void SetMass(float mass)
	{
		Mass = mass;
	}
	
	void AddVelocity(const lmVector3D &velocity)
	{
		State.Velocity += velocity;
	}
	
	lpCollisionShape &GetCollisionShape()
	{
		return *Shape;
	}
	
	void SetCollisionShape(lpCollisionShape *shape)
	{
		Shape = std::unique_ptr<lpCollisionShape>(shape);
		Shape->SetState(State);
	}
	
	void Step(float dt)
	{
		State.Position += State.Velocity * dt;
		Shape->SetState(State);
	}
	
	lpBody()
	{}
	
	~lpBody()
	{}
	/*
	 * End of class
	 */
};

#endif // LP_BODY_H
