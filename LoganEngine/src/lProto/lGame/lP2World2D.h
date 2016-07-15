#ifndef L_JRPG_MAP
#define L_JRPG_MAP

#include "liWorld2D.h"

#include <list>

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
	lmVector2D Position;
	lmVector2D Velocity;
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
	virtual const lmVector2D &GetPosition() override
	{
		return Position;
	}
	//
	virtual const lmVector2D &GetVelocity() override
	{
		return Velocity;
	}
	//
	virtual void SetPosition(const lmVector2D &position) override
	{
		Position = position;
	}
	//
	virtual void SetVelocity(const lmVector2D &velocity) override
	{
		Velocity = velocity;
	}
	//
	void AddVelocity(const lmVector2D &velocity)
	{
		Velocity += velocity;
	}
	//
	virtual liCollisionShape2D *GetCollisionShape() override
	{
		return Shape;
	}
	//
	virtual void SetCollisionShape(liCollisionShape2D *shape) override
	{
		Shape = shape;
	}
	//
	void Step(float dt)
	{
		Position += Velocity * dt;
		Velocity = lmVector2D({0.0,0.0});
	}
	//
	void Collide(lP2Body2D *other_body)
	{
		for(liCollisionCallback2D *CollisionCallback : CollisionCallbacks)
		{
			CollisionCallback->Callback(other_body);
		}
	}
	/*
	void DBG_Draw()
	{
		glLoadIdentity();

		glPointSize(5);
			glColor4f(1.0,0.0,0.0,1.0);

		glBegin(GL_POINTS);
			glVertex2f(Position[0],Position[1]);
		glEnd();

		glBegin(GL_LINE_LOOP);

		float PI = 3.14159265359;
		float dAngle = 2.0*PI / 8;

		for(int i=0;i < 8;i++)
		{
			glVertex2f(Position[0] + Radius * cos(i*dAngle),Position[1] + Radius * sin(i*dAngle));
		}

		glEnd();
	}
	*/
	lP2Body2D(const lmVector2D &position,const lmVector2D &velocity)
		:Position(position),Velocity(velocity)
	{
		//
	}

	virtual ~lP2Body2D() override
	{
		delete Shape;
	}
};

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

class lP2World2D : public liWorld2D
{
private:

	std::list<lP2Body2D *> Bodies;

	void CollisionResponse(lP2Body2D *body_1,lP2Body2D *body_2,lmScalar dt,const lmVector2D &normal,float overlap)
	{
		if(body_1->GetCollisionResponse() == liBody2D::MOVABLE && body_2->GetCollisionResponse() == liBody2D::MOVABLE)
		{
			float Speed = overlap/(2.0*dt);

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

public:
	//
	virtual liBody2D *CreateBody(const lmVector2D &position,const lmVector2D &velocity) override
	{
		lP2Body2D *NewBody = new lP2Body2D(position,velocity);
		NewBody->SetCollisionShape(new lP2Circle(NewBody));
		//
		Bodies.push_back(NewBody);
		return NewBody;
	}
	//
	virtual void Step(lmScalar dt) override
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
					Normal *= 1.0/Dist;
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
	//
	lP2World2D()
		{}
	//
	virtual ~lP2World2D() override
	{
		for(auto I : Bodies)
		{
			delete I;
		}
	}
};

#endif
