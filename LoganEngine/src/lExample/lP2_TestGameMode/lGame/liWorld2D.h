#ifndef LI_COLLISION_CALLBACK
#define LI_COLLISION_CALLBACK

#include <lMath/lMath.h>

class liCircle
{
public:
	virtual const lmVector2D &GetPosition() = 0;
	//
	virtual lmScalar GetRadius() = 0;
	virtual void SetRadius(lmScalar radius) = 0;
	//
	liCircle(){}
	virtual ~liCircle(){}
};

/*
class liParticle
{
public:
	virtual const lmVector2D &GetPosition() = 0;
	virtual const lmVector2D &GetVelocity() = 0;
	//
	liParticle(){}
	virtual ~liParticle(){}
};
*/
class liShapeVisitor
{
public:
	virtual void VisitCircle(liCircle *circle) = 0;
	//virtual void VisitParticle(liParticle *circle) = 0;
	//
	liShapeVisitor(){}
	virtual ~liShapeVisitor(){}
};

class lGetCircle : public liShapeVisitor
{
private:
	liCircle *Circle = nullptr;

public:
	//
	liCircle *GetCircle()
	{
		return Circle;
	}
	//
	virtual void VisitCircle(liCircle *circle) override
	{
		Circle = circle;
	}
	//
	lGetCircle(){}
	virtual ~lGetCircle(){}
	/*
	 * End of class
	 */
};

class liCollisionShape2D
{
public:
	virtual void Accept(liShapeVisitor *visitor) = 0;
	//
	liCollisionShape2D() {}
	virtual ~liCollisionShape2D() {}
	/*
	 * End of class
	 */
};

class liState2D
{
public:
	virtual const lmVector2D &GetPosition() const = 0;
	virtual const lmVector2D &GetVelocity() const = 0;
	//
	liState2D() {}
	virtual ~liState2D() {}
	/*
	 * End of class
	 */
};

class liBody2D
{
public:
	//
	enum COLLISION_RESPONSE
	{
		IMMOVABLE,
		MOVABLE,
		GHOST,
	};
	//
	class liCollisionCallback2D
	{
	public:
		//
		virtual void MarkRemovable() = 0;
		//
		virtual void Callback(liBody2D *body) = 0;
		//
		liCollisionCallback2D()
			{}
		//
		virtual ~liCollisionCallback2D()
			{}
	};
	//
	virtual void SetCollisionResponse(COLLISION_RESPONSE collision_response) = 0;
	//
	virtual void AddCollisionCallback(liCollisionCallback2D *callback) = 0;
	//
	virtual void SetUserData(void *user_data) = 0;
	virtual void *GetUserData() = 0;
	//
	virtual void MarkRemovable() = 0;
	//
	virtual const lmVector2D &GetPosition() const = 0;
	virtual const lmVector2D &GetVelocity() const = 0;
	//
	virtual void SetState(const liState2D &state) = 0;
	//
	virtual liCollisionShape2D *GetCollisionShape() = 0;
	//
	liBody2D()
		{}
	//
	virtual ~liBody2D()
		{}
	/*
	 * End of class
	 */
};

class liCircle2DBuilder
{
public:
	virtual liCircle2DBuilder &SetRadius(lmScalar radius) = 0;
	virtual void Construct() = 0;
	//
	liCircle2DBuilder(){}
	virtual ~liCircle2DBuilder(){}
	/*
	 * End of class
	 */
};

class liBody2DBuilder
{
public:
	virtual liBody2DBuilder &SetState(const liState2D &state) = 0;
	//
	virtual liCircle2DBuilder &CreateCircle() = 0;
	//
	virtual liBody2D &Construct() = 0;
	//
	liBody2DBuilder(){}
	virtual ~liBody2DBuilder(){}
	/*
	 * End of class
	 */
};

class liBody2DFactory
{
public:
	virtual liBody2DBuilder &CreateBody() = 0;
	//
	liBody2DFactory(){}
	virtual ~liBody2DFactory(){}
	/*
	 * End of class
	 */
};

class liWorld2D
{
public:
	virtual void Pause() = 0;
	virtual void Continue() = 0;
	//
	virtual liBody2DFactory &GetBodyFactory() = 0;
	//
	virtual void Simulate(lmScalar dt) = 0;
	//
	liWorld2D()
		{}
	//
	virtual ~liWorld2D()
		{}
};

#endif // LI_COLLISION_CALLBACK
