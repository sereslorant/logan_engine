#ifndef LP2_WORLD_2D_H
#define LP2_WORLD_2D_H

#include <lCore/lMath/lMath.h>

#include "lpCollisionDetection/lpCollisionShape.h"

class lpBody2D
{
public:
	enum lpCollisionResponse
	{
		MOVABLE,
		IMMOVABLE,
		GHOST,
	};
	//
	/*
	class liCollisionCallback2D
	{
	public:
		
	};
	*/
	//
protected:
	//
	bool Removable = false;
	//
	lpBody2DState State;
	float Mass;
	//
	lpCollisionResponse CollisionResponse = MOVABLE;
	//
	lpCollisionShape2D *Shape = nullptr;
	//
	//std::list<liCollisionCallback2D *> CollisionCallbacks;
	//void *UserData = nullptr;
	//
public:
	//
	void SetCollisionResponse(lpCollisionResponse collision_response)
	{
		CollisionResponse = collision_response;
	}
	//
	lpCollisionResponse GetCollisionResponse()
	{
		return CollisionResponse;
	}
	//
	/*
	virtual void AddCollisionCallback(liCollisionCallback2D *callback) override
	{
		CollisionCallbacks.push_back(callback);
	}
	//
	void Collide(lP2Body2D *other_body)
	{
		for(liCollisionCallback2D *CollisionCallback : CollisionCallbacks)
		{
			CollisionCallback->Callback(other_body);
		}
	}
	*/
	//
	/*
	virtual void SetUserData(void *user_data) override
	{
		UserData = user_data;
	}
	//
	virtual void *GetUserData() override
	{
		return UserData;
	}
	*/
	//
	void MarkRemovable()
	{
		Removable = true;
	}
	//
	bool IsRemovable()
	{
		return Removable;
	}
	//
	const lmVector2D &GetPosition() const
	{
		return State.Position;
	}
	//
	const lmVector2D &GetVelocity() const
	{
		return State.Velocity;
	}
	//
	float GetMass() const
	{
		return Mass;
	}
	//
	void SetState(const lmVector2D &position,const lmVector2D &velocity)
	{
		State.Position = position;
		State.Velocity = velocity;
	}
	//
	lpBody2DState &GetState()
	{
		return State;
	}
	//
	void SetMass(float mass)
	{
		Mass = mass;
	}
	//
	void AddVelocity(const lmVector2D &velocity)
	{
		State.Velocity += velocity;
	}
	//
	lpCollisionShape2D &GetCollisionShape()
	{
		return *Shape;
	}
	//
	void SetCollisionShape(lpCollisionShape2D &shape)
	{
		Shape = &shape;
	}
	//
	void Step(float dt)
	{
		State.Position += State.Velocity * dt;
	}
	//
	lpBody2D()
	{
		//
	}
	//
	~lpBody2D()
	{
		delete Shape;
	}
	/*
	 * End of class
	 */
};

class lpBody2DBuilder
{
public:
	class liObjectList
	{
	public:
		virtual void AddBody(lpBody2D *new_body) = 0;
		//
		liObjectList(){}
		virtual ~liObjectList(){}
		/*
		 * End of class
		 */
	};
	//
	class lpCircle2DBuilder
	{
	public:
		lpBody2DBuilder &Parent;
		//
		bool Finalized = false;
		float Radius = 0.0;
		//
		lpCircle2DBuilder &SetRadius(float radius)
		{
			Radius = radius;
			return *this;
		}
		//
		lpBody2DBuilder &Construct()
		{
			Finalized = true;
			return Parent;
		}
		//
		lpCircle2DBuilder(lpBody2DBuilder &parent)
			:Parent(parent)
		{}
		~lpCircle2DBuilder() {}
		/*
		 * End of class
		 */
	};
	//
private:
	//
	lpBody2DState BodyState;
	float Mass = 0.0;
	//
	lpBody2D::lpCollisionResponse CollisionResponse = lpBody2D::MOVABLE;
	lpCircle2DBuilder CircleBuilder;
	liObjectList &World;
	//
public:
	//
	lpBody2DBuilder &SetState(const lpBody2DState &state)
	{
		BodyState.Position = state.GetPosition();
		BodyState.Velocity = state.GetVelocity();
		//
		return *this;
	}
	//
	lpBody2DBuilder &SetMass(float mass)
	{
		Mass = mass;
		return *this;
	}
	//
	lpBody2DBuilder &SetCollisionResponse(lpBody2D::lpCollisionResponse collision_response)
	{
		CollisionResponse = collision_response;
		return *this;
	}
	//
	lpCircle2DBuilder &CreateCircle()
	{
		return CircleBuilder;
	}
	//
	lpBody2D &Construct()
	{
		lpBody2D *NewBody = new lpBody2D();
		NewBody->SetState(BodyState.GetPosition(),BodyState.GetVelocity());
		NewBody->SetMass(Mass);
		NewBody->SetCollisionResponse(CollisionResponse);
		//
		if(CircleBuilder.Finalized)
		{
			lpCircle *NewCircle = new lpCircle(&NewBody->GetState());
			NewCircle->SetRadius(CircleBuilder.Radius);
			//
			NewBody->SetCollisionShape(*NewCircle);
		}
		//
		World.AddBody(NewBody);
		//
		return *NewBody;
	}
	//
	lpBody2DBuilder(liObjectList &world)
		:CircleBuilder(*this),World(world)
	{}
	//
	virtual ~lpBody2DBuilder(){}
	/*
	 * End of class
	 */
};

#include <list>

class lpBody2DFactory
{
private:
	std::list<lpBody2DBuilder> Builders;
	//
	lpBody2DBuilder::liObjectList &World;
	//
public:
	//
	lpBody2DBuilder &CreateBody()
	{
		Builders.emplace_back(World);
		//
		return Builders.back();
	}
	//
	lpBody2DFactory(lpBody2DBuilder::liObjectList &world)
		:World(world)
	{}
	//
	~lpBody2DFactory()
	{}
	/*
	 * End of class
	 */
};

#include <iostream>

class lpWorld2D : private lpBody2DBuilder::liObjectList
{
private:
	bool Paused = false;
	std::vector<lpBody2D *> Bodies;
	//
	lpSpacePartition SpacePartition;
	//
	lpBody2DFactory BodyFactory;
	//
	void CollisionResponse(lpBody2D *body_1,lpBody2D *body_2,float dt,const lmVector2D &normal,float overlap)
	{
		if(body_1->GetCollisionResponse() == lpBody2D::MOVABLE && body_2->GetCollisionResponse() == lpBody2D::MOVABLE)
		{
			float Speed = overlap/(2.0f*dt);

			body_2->AddVelocity(normal * (Speed));
			body_1->AddVelocity(normal * (-Speed));
		}
		else if(body_2->GetCollisionResponse() == lpBody2D::MOVABLE && body_1->GetCollisionResponse() == lpBody2D::IMMOVABLE)
		{
			float Speed = overlap/dt;

			body_2->AddVelocity(normal * Speed);
		}
		else if(body_2->GetCollisionResponse() == lpBody2D::IMMOVABLE && body_1->GetCollisionResponse() == lpBody2D::MOVABLE)
		{
			float Speed = overlap/dt;

			body_1->AddVelocity(normal * (-Speed));
		}
	}
	//
public:
	//
	void AddBody(lpBody2D *new_body)
	{
		Bodies.push_back(new_body);
		SpacePartition.AddCollisionShape(new_body->GetCollisionShape());
	}
	//
	void Pause()
	{
		Paused = true;
	}
	//
	void Continue()
	{
		Paused = false;
	}
	//
	lpBody2DFactory &GetBodyFactory()
	{
		return BodyFactory;
	}
	//
	void Simulate(float dt)
	{
		if(!Paused)
		{
			//Detect collision
			SpacePartition.CheckCollision(dt);
			//
			//Collision response
			std::vector<lpCollisionData> CollisionData(SpacePartition.GetNumCollisions());
			SpacePartition.GetCollisionData(&CollisionData[0],CollisionData.size());
			for(lpCollisionData &Collision : CollisionData)
			{
				lpBody2D *Body1 = Bodies[Collision.Shape1Id];
				lpBody2D *Body2 = Bodies[Collision.Shape2Id];
				CollisionResponse(Body1,Body2,dt,Collision.Normal,Collision.NormalOverlap);
			}
			//
			//Step
			for(auto I : Bodies)
			{
				if(I->GetCollisionResponse() == lpBody2D::MOVABLE)
				{
					I->AddVelocity(lmVector2D({0.0f,-9.81f * dt}));
				}
				I->Step(dt);
			}
		}
	}
	//
	void RemoveRemovable()
	{
		for(auto I = Bodies.begin();I != Bodies.end();)
		{
			if((*I)->IsRemovable())
			{
				auto K = I;
				I++;
				//
				Bodies.erase(K);
				continue;
			}
			else
			{
				I++;
			}
		}
	}
	//
	lpWorld2D()
		:BodyFactory(*this)
	{
		//
	}
	//
	~lpWorld2D()
	{
		for(lpBody2D *Body : Bodies)
		{
			delete Body;
		}
	}
};

#endif // LP2_WORLD_2D_H
