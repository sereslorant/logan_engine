#ifndef LP2_WORLD_2D_H
#define LP2_WORLD_2D_H

#include "liWorld2D.h"

#include "lP2Body2D.h"

#include <list>

class lP2World2D : public liWorld2D
{
private:

	std::list<lP2Body2D *> Bodies;

	void CollisionResponse(lP2Body2D *body_1,lP2Body2D *body_2,lmScalar dt,const lmVector2D &normal,float overlap);

public:
	//
	virtual liBody2D *CreateBody(const lmVector2D &position,const lmVector2D &velocity) override;
	//
	virtual void Simulate(lmScalar dt) override;
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

#endif // LP2_WORLD_2D_H
