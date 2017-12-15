#ifndef LP_SPACE_PARTITION_H
#define LP_SPACE_PARTITION_H

#include "lpCollisionShapes/lpSphere.h"
#include "lpCollisionShapes/lpCompositeShape.h"

/*
 * UTIL!!!!!!!!!!!!!!!!!!!!!!!
 * TODO: Ki kell venni a program ezen részből!!!
 */

class lShapeGetter : public liShapeVisitor
{
private:
	lpSphere *Sphere = nullptr;
	const lpSphere *ConstSphere = nullptr;
	lpCompositeShape *Composite = nullptr;
	const lpCompositeShape *ConstComposite = nullptr;
	
public:
	
	lpSphere *GetSphere()
	{
		return Sphere;
	}
	
	const lpSphere *GetConstSphere()
	{
		return ConstSphere;
	}
	
	lpCompositeShape *GetComposite()
	{
		return Composite;
	}
	
	const lpCompositeShape *GetConstComposite()
	{
		return ConstComposite;
	}
	
	virtual void VisitSphere(lpSphere *sphere) override
	{
		Sphere = sphere;
		ConstSphere = sphere;
	}
	
	virtual void VisitSphere(const lpSphere *sphere) override
	{
		ConstSphere = sphere;
	}
	
	virtual void VisitComposite(lpCompositeShape *composite) override
	{
		Composite = composite;
		ConstComposite = composite;
	}
	
	virtual void VisitComposite(const lpCompositeShape *composite) override
	{
		ConstComposite = composite;
	}
	
	lShapeGetter()
	{}
	
	virtual ~lShapeGetter() override
	{}
	/*
	 * End of class
	 */
};

#include "lpCollisionTable/lpSymmetricCollisionTable.h"

class lpSpacePartition
{
private:
	lpSymmetricCollisionTable SphereCollisionTable;
	
	void AddSphere(unsigned int shape_id,const lpSphere *sphere)
	{
		SphereCollisionTable.AddShape(shape_id,sphere);
	}
	
	void AddComposite(unsigned int shape_id,const lpCompositeShape *composite)
	{
		for(int id=0;id < composite->GetNumChildren();id++)
		{
			AddCollisionShape(shape_id,composite->GetChild(id));
		}
	}
	
public:
	
	void AddCollisionShape(unsigned int shape_id,const lpCollisionShape *new_collision_shape)
	{
		lShapeGetter ShapeGetter;
		new_collision_shape->Accept(ShapeGetter);
		if(ShapeGetter.GetConstSphere() != nullptr)
		{
			AddSphere(shape_id,ShapeGetter.GetConstSphere());
		}
		if(ShapeGetter.GetConstComposite() != nullptr)
		{
			AddComposite(shape_id,ShapeGetter.GetConstComposite());
		}
	}
	
	void CheckCollision(float dt)
	{
		SphereCollisionTable.CheckCollision(dt);
	}
	
	unsigned int GetNumCollisions()
	{
		return SphereCollisionTable.GetNumCollisions();
	}
	
	void GetCollisionData(lpCollisionData collision_data[],unsigned int num_collision_data)
	{
		SphereCollisionTable.GetCollisionData(collision_data,num_collision_data);
	}
	
	lpSpacePartition()
	{}
	
	~lpSpacePartition()
	{}
	/*
	 * End of class
	 */
};

#endif // LP_SPACE_PARTITION_H
