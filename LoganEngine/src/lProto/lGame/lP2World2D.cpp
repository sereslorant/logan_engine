
#include "lP2World2D.h"

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

void lP2World2D::CollisionResponse(lP2Body2D *body_1,lP2Body2D *body_2,lmScalar dt,const lmVector2D &normal,float overlap)
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

liBody2D *lP2World2D::CreateBody(const lmVector2D &position,const lmVector2D &velocity)
{
	lP2Body2D *NewBody = new lP2Body2D(position,velocity);
	NewBody->SetCollisionShape(new lP2Circle(NewBody));
	//
	Bodies.push_back(NewBody);
	return NewBody;
}

void lP2World2D::Simulate(lmScalar dt)
{
	if(!Paused)
	{
		//Detect collision
		for(auto I = Bodies.begin();I != Bodies.end();)
		{
			//for(auto J = std::next(I);J != Bodies.end();J++) //Az Eclipse az std::next-re false pozitív compile error miatt rinyál.
			auto J = I; J++; //Sajnos egyelőre ez kell ide, hogy ne legyen false pozitív Eclipse rinyálás.
			for(;J != Bodies.end();J++)
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
