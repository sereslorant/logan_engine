#ifndef LI_COLLISION_CALLBACK
#define LI_COLLISION_CALLBACK

class liBody2D;

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

class liShapeVisitor
{
public:
	virtual void VisitCircle(liCircle *circle) = 0;
	//
	liShapeVisitor(){}
	virtual ~liShapeVisitor(){}
};

class lGetCircle : public liShapeVisitor
{
private:
	liCircle *Circle;

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
};

class liCollisionShape2D
{
public:
	virtual void Accept(liShapeVisitor *visitor) = 0;
	//
	liCollisionShape2D() {}
	virtual ~liCollisionShape2D() {}
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
	virtual void SetCollisionResponse(COLLISION_RESPONSE collision_response) = 0;
	//
	virtual void AddCollisionCallback(liCollisionCallback2D *callback) = 0;
	//
	virtual void SetUserData(void *user_data) = 0;
	virtual void *GetUserData() = 0;
	//
	virtual void MarkRemovable() = 0;
	//
	virtual const lmVector2D &GetPosition() = 0;
	virtual const lmVector2D &GetVelocity() = 0;
	//
	virtual void SetPosition(const lmVector2D &position) = 0;
	virtual void SetVelocity(const lmVector2D &velocity) = 0;
	//
	virtual liCollisionShape2D *GetCollisionShape() = 0;
	virtual void SetCollisionShape(liCollisionShape2D *shape) = 0;
	//
	liBody2D()
		{}
	//
	virtual ~liBody2D()
		{}
};

class liWorld2D
{
public:
	//
	virtual liBody2D *CreateBody(const lmVector2D &position,const lmVector2D &velocity) = 0;
	//
	virtual void Step(lmScalar dt) = 0;
	//
	liWorld2D()
		{}
	//
	virtual ~liWorld2D()
		{}
};

#endif // LI_COLLISION_CALLBACK
