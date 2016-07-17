#ifndef LP2_BODY_2D_H
#define LP2_BODY_2D_H

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

#endif // LP2_BODY_2D_H
