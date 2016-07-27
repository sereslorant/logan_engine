#ifndef LP2_WORLD_2D_H
#define LP2_WORLD_2D_H

#include "liWorld2D.h"

#include "lP2Body2D.h"

#include <list>

class li2DShapeFactory
{
public:
	//
	virtual liCollisionShape2D *NewCircle() = 0;
	//
	li2DShapeFactory()
		{}
	//
	virtual ~li2DShapeFactory()
		{}
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
class lP2World2D : public liWorld2D
{
private:
	bool Paused = false;
	std::list<lP2Body2D *> Bodies;

	void CollisionResponse(lP2Body2D *body_1,lP2Body2D *body_2,lmScalar dt,const lmVector2D &normal,float overlap);

public:
	//
	virtual void Pause() override {Paused = true;}
	virtual void Continue() override {Paused = false;}
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
