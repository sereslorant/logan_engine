#ifndef LP_COLLISION_SHAPE_2D_H
#define LP_COLLISION_SHAPE_2D_H

#include <lCore/lMath/lMath.h>

struct lpBody2DState
{
	lmVector2D Position;
	lmVector2D Velocity;
	//
	const lmVector2D &GetPosition() const
	{
		return Position;
	}
	//
	const lmVector2D &GetVelocity() const
	{
		return Velocity;
	}
	//
	lpBody2DState() {}
	lpBody2DState(const lmVector2D &position,const lmVector2D &velocity)
		:Position(position),Velocity(velocity)
	{}
	virtual ~lpBody2DState() {}
	/*
	 * End of class
	 */
};

class lpCircle;

class liShapeVisitor
{
public:
	virtual void VisitCircle(lpCircle *circle) = 0;
	virtual void VisitCircle(const lpCircle *circle) = 0;
	//virtual void VisitParticle(liParticle *circle) = 0;
	//
	liShapeVisitor(){}
	virtual ~liShapeVisitor(){}
};

class lpCollisionShape2D
{
public:
	//
	virtual void Accept(liShapeVisitor &visitor) = 0;
	virtual void Accept(liShapeVisitor &visitor) const = 0;
	//
	lpCollisionShape2D(){}
	virtual ~lpCollisionShape2D(){}
};

class lpCircle : public lpCollisionShape2D
{
protected:
	lpBody2DState *BodyState;
	float Radius;

public:
	//
	virtual void Accept(liShapeVisitor &visitor) override
	{
		visitor.VisitCircle(this);
	}
	//
	virtual void Accept(liShapeVisitor &visitor) const override
	{
		visitor.VisitCircle(this);
	}
	//
	const lmVector2D &GetPosition() const
	{
		return BodyState->GetPosition();
	}
	//
	float GetRadius() const
	{
		return Radius;
	}
	//
	void SetRadius(float radius)
	{
		Radius = radius;
	}
	//
	lpCircle(lpBody2DState *body_state,float radius = 0.0f)
		:BodyState(body_state),Radius(radius)
	{
		//
	}
	//
	virtual ~lpCircle() override
	{
		//
	}
};

/*
 * UTIL!!!!!!!!!!!!!!!!!!!!!!!
 * TODO: Ki kell venni a program ezen részből!!!
 */

class lShapeGetter : public liShapeVisitor
{
private:
	lpCircle *Circle = nullptr;
	const lpCircle *ConstCircle = nullptr;

public:
	//
	lpCircle *GetCircle()
	{
		return Circle;
	}
	//
	const lpCircle *GetConstCircle()
	{
		return ConstCircle;
	}
	//
	virtual void VisitCircle(lpCircle *circle) override
	{
		Circle = circle;
	}
	//
	virtual void VisitCircle(const lpCircle *circle) override
	{
		ConstCircle = circle;
	}
	//
	lShapeGetter(){}
	virtual ~lShapeGetter(){}
	/*
	 * End of class
	 */
};

#include <list>
#include <vector>

struct lpCollisionData
{
	unsigned int Shape1Id;
	unsigned int Shape2Id;
	float NormalOverlap;
	lmVector2D Normal;
	lmVector2D IntersectionPoint1;
	lmVector2D IntersectionPoint2;
};

class lpCollisionTable
{
private:
	std::list<lpCircle *> Bodies;
	std::vector<int> CollisionTable;
	//
public:
	//
	
	//
	lpCollisionTable()
	{}
	//
	~lpCollisionTable()
	{}
	/*
	 * End of class
	 */
};

class lpSpacePartition
{
private:
	std::list<lpCollisionShape2D *> CollisionShapes;
	std::vector<int> CollisionTable;
	//
	void ResizeCollisionTable()
	{
		if(CollisionTable.size() != (CollisionShapes.size() * CollisionShapes.size()))
		{
			CollisionTable.resize(CollisionShapes.size() * CollisionShapes.size());
		}
	}
	//
	void ClearCollisionTable()
	{
		for(int &Entry : CollisionTable)
		{
			Entry = 0;
		}
	}
	//
	bool CheckCollision(const lpCollisionShape2D &collision_shape_1,const lpCollisionShape2D &collision_shape_2)
	{
		lShapeGetter GetCircle_I;
		lShapeGetter GetCircle_J;
		//
		collision_shape_1.Accept(GetCircle_I);
		collision_shape_2.Accept(GetCircle_J);
		//
		const lpCircle &Circle_1 = *GetCircle_I.GetConstCircle();
		const lpCircle &Circle_2 = *GetCircle_J.GetConstCircle();
		//
		return lmSphereSphereIntersection(	Circle_2.GetPosition(),
											Circle_2.GetRadius(),
											Circle_1.GetPosition(),
											Circle_1.GetRadius());
	}
	//
	void FillSphereSphereCollisionData(const lpCircle &circle_1,const lpCircle &circle_2,lpCollisionData &collision_data)
	{
		collision_data.Normal = circle_2.GetPosition() - circle_1.GetPosition();
		collision_data.NormalOverlap = (circle_1.GetRadius() + circle_2.GetRadius()) - collision_data.Normal.Length();
		collision_data.Normal.Normalize();
		collision_data.IntersectionPoint1 = circle_1.GetPosition() + collision_data.Normal * circle_1.GetRadius();
		collision_data.IntersectionPoint2 = circle_2.GetPosition() - collision_data.Normal * circle_2.GetRadius();
	}
	//
	void FillCollisionData(const lpCollisionShape2D &collision_shape_1,const lpCollisionShape2D &collision_shape_2,lpCollisionData &collision_data)
	{
		lShapeGetter GetCircle_I;
		lShapeGetter GetCircle_J;
		//
		collision_shape_1.Accept(GetCircle_I);
		collision_shape_2.Accept(GetCircle_J);
		//
		const lpCircle &Circle_1 = *GetCircle_I.GetConstCircle();
		const lpCircle &Circle_2 = *GetCircle_J.GetConstCircle();
		//
		FillSphereSphereCollisionData(Circle_1,Circle_2,collision_data);
	}
	//
public:
	//
	void AddCollisionShape(lpCollisionShape2D &new_collision_shape)
	{
		CollisionShapes.push_back(&new_collision_shape);
	}
	//
	void CheckCollision(float dt)
	{
		ResizeCollisionTable();
		ClearCollisionTable();
		//
		int i = 0;
		for(auto I = CollisionShapes.begin();I != CollisionShapes.end();I++)
		{
			int j = i + 1;
			for(auto J = std::next(I);J != CollisionShapes.end();J++)
			{
				CollisionTable[i*CollisionShapes.size() + j] = CheckCollision(*(*I),*(*J));
				j++;
			}
			i++;
		}
	}
	//
	unsigned int GetNumCollisions()
	{
		unsigned int NumCollisions = 0;
		for(int i=0;i < CollisionShapes.size();i++)
		{
			for(int j = i+1;j < CollisionShapes.size();j++)
			{
				if(CollisionTable[i*CollisionShapes.size() + j])
				{
					NumCollisions++;
				}
			}
		}
		return NumCollisions;
	}
	//
	void GetCollisionData(lpCollisionData collision_data[],unsigned int num_collision_data)
	{
		unsigned int NextId = 0;
		int i = 0;
		for(auto I = CollisionShapes.begin();I != CollisionShapes.end();I++)
		{
			int j = i + 1;
			for(auto J = std::next(I);J != CollisionShapes.end();J++)
			{
				if(CollisionTable[i*CollisionShapes.size() + j])
				{
					collision_data[NextId].Shape1Id = i;
					collision_data[NextId].Shape2Id = j;
					FillCollisionData(*(*I),*(*J),collision_data[NextId]);
					NextId++;
					if(NextId == num_collision_data)
					{
						return;
					}
				}
				j++;
			}
			i++;
		}
	}
	//
	lpSpacePartition()
	{
		//
	}
	//
	~lpSpacePartition()
	{
		//
	}
	/*
	 * End of class
	 */
};

#endif // LP_COLLISION_SHAPE_2D_H
