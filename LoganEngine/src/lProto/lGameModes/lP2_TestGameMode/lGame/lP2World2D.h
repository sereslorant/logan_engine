#ifndef LP2_WORLD_2D_H
#define LP2_WORLD_2D_H

#include "liWorld2D.h"

#include <list>

struct lP2BodyState : public liState2D
{
	lmVector2D Position;
	lmVector2D Velocity;
	//
	virtual const lmVector2D &GetPosition() const override
	{
		return Position;
	}
	//
	virtual const lmVector2D &GetVelocity() const override
	{
		return Velocity;
	}
	//
	lP2BodyState() {}
	virtual ~lP2BodyState() {}
	/*
	 * End of class
	 */
};

class lP2Body2D : public liBody2D
{
protected:
	//
	bool Removable = false;
	//
	COLLISION_RESPONSE CollisionResponse = MOVABLE;
	//
	std::list<liCollisionCallback2D *> CollisionCallbacks;
	//
	void *UserData = nullptr;
	//
	lP2BodyState State;
	//
	liCollisionShape2D *Shape = nullptr;
	//
public:
	//
	virtual void SetCollisionResponse(COLLISION_RESPONSE collision_response) override
	{
		CollisionResponse = collision_response;
	}
	//
	COLLISION_RESPONSE GetCollisionResponse()
	{
		return CollisionResponse;
	}
	//
	virtual void AddCollisionCallback(liCollisionCallback2D *callback) override
	{
		CollisionCallbacks.push_back(callback);
	}
	//
	virtual void SetUserData(void *user_data) override
	{
		UserData = user_data;
	}
	//
	virtual void *GetUserData() override
	{
		return UserData;
	}
	//
	virtual void MarkRemovable() override
	{
		Removable = true;
	}
	//
	bool IsRemovable()
	{
		return Removable;
	}
	//
	virtual const lmVector2D &GetPosition() const override
	{
		return State.Position;
	}
	//
	virtual const lmVector2D &GetVelocity() const override
	{
		return State.Velocity;
	}
	//
	virtual void SetState(const liState2D &state) override
	{
		State.Position = state.GetPosition();
		State.Velocity = state.GetVelocity();
	}
	//
	void AddVelocity(const lmVector2D &velocity)
	{
		State.Velocity += velocity;
	}
	//
	virtual liCollisionShape2D *GetCollisionShape() override
	{
		return Shape;
	}
	//
	void SetCollisionShape(liCollisionShape2D *shape)
	{
		Shape = shape;
	}
	//
	void Step(float dt)
	{
		State.Position += State.Velocity * dt;
		State.Velocity = lmVector2D({0.0,0.0});
	}
	//
	void Collide(lP2Body2D *other_body)
	{
		for(liCollisionCallback2D *CollisionCallback : CollisionCallbacks)
		{
			CollisionCallback->Callback(other_body);
		}
	}
	//
	lP2Body2D()
	{
		//
	}
	//
	virtual ~lP2Body2D() override
	{
		delete Shape;
	}
	/*
	 * End of class
	 */
};

/*
template<typename lmScalar_T>
class lpDiscreteCollisionDetector2D
{
private:
	struct lpShapeContainer : public li2DShapeFactory
	{
		std::vector<lP2Circle *> Circles;
		//std::vector<lP2Particle *> Particles;
		//
		virtual liCollisionShape2D *NewCircle() override
		{

		}
		//
		lpShapeContainer()
			{}
		//
		virtual ~lpShapeContainer() override
			{}
		/*
		 * End of class
		 * /
	};
	//
	lpShapeContainer ShapeContainer;
	//
public:
	//
	li2DShapeFactory &GetShapeFactory()
	{
		return ShapeContainer;
	}
	//
	void CheckCollision(lmScalar_T dt)
	{

	}
	//
	lpDiscreteCollisionDetector2D(){}
	//
	virtual ~lpDiscreteCollisionDetector2D() override
	{}
};
*/

class lP2Circle : public liCollisionShape2D, public liCircle
{
protected:
	lP2Body2D *Body;
	lmScalar Radius = 0.0;

public:
	//
	virtual void Accept(liShapeVisitor *visitor) override
	{
		visitor->VisitCircle(this);
	}
	//
	virtual const lmVector2D &GetPosition() override
	{
		return Body->GetPosition();
	}
	//
	virtual lmScalar GetRadius() override
	{
		return Radius;
	}
	//
	virtual void SetRadius(lmScalar radius) override
	{
		Radius = radius;
	}
	//
	lP2Circle(lP2Body2D *body)
		:Body(body)
	{
		//
	}
	//
	virtual ~lP2Circle() override
	{
		//
	}
};

/*
liBody2D *lP2World2D::CreateBody(const lmVector2D &position,const lmVector2D &velocity)
{
	lP2Body2D *NewBody = new lP2Body2D(position,velocity);
	NewBody->SetCollisionShape(new lP2Circle(NewBody));
	//
	return NewBody;
}
*/


class lP2Body2DBuilder : public liBody2DBuilder
{
public:
	class liObjectList
	{
	public:
		virtual void AddBody(lP2Body2D *new_body) = 0;
		//
		liObjectList(){}
		virtual ~liObjectList(){}
		/*
		 * End of class
		 */
	};
	//
private:
	//
	class lP2Circle2DBuilder : public liCircle2DBuilder
	{
	public:
		bool Finalized = false;
		lmScalar Radius = 0.0;
		//
		virtual liCircle2DBuilder &SetRadius(lmScalar radius) override
		{
			Radius = radius;
			return *this;
		}
		//
		virtual void Construct() override
		{
			Finalized = true;
		}
		//
		lP2Circle2DBuilder(){}
		virtual ~lP2Circle2DBuilder() override {}
		/*
		 * End of class
		 */
	};
	//
	lP2BodyState BodyState;
	lP2Circle2DBuilder CircleBuilder;
	liObjectList &World;
	//
public:
	//
	virtual liBody2DBuilder &SetState(const liState2D &state) override
	{
		BodyState.Position = state.GetPosition();
		BodyState.Velocity = state.GetVelocity();
		//
		return *this;
	}
	//
	virtual liCircle2DBuilder &CreateCircle() override
	{
		return CircleBuilder;
	}
	//
	virtual liBody2D &Construct() override
	{
		lP2Body2D *NewBody = new lP2Body2D();
		NewBody->SetState(BodyState);
		//
		if(CircleBuilder.Finalized)
		{
			lP2Circle *NewCircle = new lP2Circle(NewBody);
			NewCircle->SetRadius(CircleBuilder.Radius);
			//
			NewBody->SetCollisionShape(NewCircle);
		}
		//
		World.AddBody(NewBody);
		//
		return *NewBody;
	}
	//
	lP2Body2DBuilder(liObjectList &world)
		:World(world)
	{}
	//
	virtual ~lP2Body2DBuilder(){}
	/*
	 * End of class
	 */
};

class lP2Body2DFactory : public liBody2DFactory
{
private:
	std::list<lP2Body2DBuilder> Builders;
	//
	lP2Body2DBuilder::liObjectList &World;
	//
public:
	//
	virtual liBody2DBuilder &CreateBody() override
	{
		Builders.emplace_back(World);
		//
		return Builders.back();
	}
	//
	lP2Body2DFactory(lP2Body2DBuilder::liObjectList &world)
		:World(world)
	{}
	virtual ~lP2Body2DFactory(){}
	/*
	 * End of class
	 */
};

class lP2World2D : public liWorld2D, private lP2Body2DBuilder::liObjectList
{
private:
	bool Paused = false;
	std::list<lP2Body2D *> Bodies;
	//
	lP2Body2DFactory BodyFactory;
	//
	void CollisionResponse(lP2Body2D *body_1,lP2Body2D *body_2,lmScalar dt,const lmVector2D &normal,float overlap)
	{
		if(body_1->GetCollisionResponse() == liBody2D::MOVABLE && body_2->GetCollisionResponse() == liBody2D::MOVABLE)
		{
			float Speed = overlap/(2.0f*dt);

			body_2->AddVelocity(normal * (Speed));
			body_1->AddVelocity(normal * (-Speed));
		}
		else if(body_2->GetCollisionResponse() == liBody2D::MOVABLE && body_1->GetCollisionResponse() == liBody2D::IMMOVABLE)
		{
			float Speed = overlap/dt;

			body_2->AddVelocity(normal * Speed);
		}
		else if(body_2->GetCollisionResponse() == liBody2D::IMMOVABLE && body_1->GetCollisionResponse() == liBody2D::MOVABLE)
		{
			float Speed = overlap/dt;

			body_1->AddVelocity(normal * (-Speed));
		}
	}
	//
public:
	//
	virtual void AddBody(lP2Body2D *new_body) override
	{
		Bodies.push_back(new_body);
	}
	//
	virtual void Pause() override
	{
		Paused = true;
	}
	//
	virtual void Continue() override
	{
		Paused = false;
	}
	//
	virtual liBody2DFactory &GetBodyFactory() override
	{
		return BodyFactory;
	}
	//
	virtual void Simulate(lmScalar dt) override
	{
		if(!Paused)
		{
			//Detect collision
			for(auto I = Bodies.begin();I != Bodies.end();)
			{
				for(auto J = std::next(I);J != Bodies.end();J++)
				{
					lGetCircle GetCircle_I;
					lGetCircle GetCircle_J;
					//
					(*I)->GetCollisionShape()->Accept(&GetCircle_I);
					(*J)->GetCollisionShape()->Accept(&GetCircle_J);
					//
					liCircle *Circle_I = GetCircle_I.GetCircle();
					liCircle *Circle_J = GetCircle_J.GetCircle();
					//
					if(lmSphereSphereIntersection(
													Circle_J->GetPosition(),
													Circle_J->GetRadius(),
													Circle_I->GetPosition(),
													Circle_I->GetRadius())
						)
					{
						(*I)->Collide(*J);
						(*J)->Collide(*I);
						//
						lmVector2D Normal = Circle_J->GetPosition() - Circle_I->GetPosition();
						float Dist = std::sqrt(Normal.LengthSquared());
						float SumRadius = Circle_J->GetRadius() + Circle_I->GetRadius();
						//
						float Overlap = SumRadius - Dist;
						Normal *= 1.0f/Dist;
						//
						CollisionResponse(*I,*J,dt,Normal,Overlap);
					}
				}
				//
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
			//Step
			for(auto I : Bodies)
			{
				I->Step(dt);
			}
		}
	}
	//
	lP2World2D()
		:BodyFactory(*this)
	{
		//
	}
	//
	virtual ~lP2World2D() override
	{
		for(auto I : Bodies)
		{
			delete I;
		}
	}
};

#endif // LP2_WORLD_2D_H
