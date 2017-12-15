#ifndef LP_SYMMETRIC_COLLISION_TABLE_H
#define LP_SYMMETRIC_COLLISION_TABLE_H

#include "../lpCollisionData.h"

#include <list>
#include <vector>

#include <tuple>

class lpSymmetricCollisionTable
{
private:
	std::list<std::tuple<unsigned int,const lpSphere *> > Shapes;
	std::vector<int> CollisionTable;
	
	void ResizeCollisionTable()
	{
		if(CollisionTable.size() != (Shapes.size() * Shapes.size()))
		{
			CollisionTable.resize(Shapes.size() * Shapes.size());
		}
	}
	
	void ClearCollisionTable()
	{
		for(int &Entry : CollisionTable)
		{
			Entry = 0;
		}
	}
	
protected:
	
	bool CheckCollision(const lpSphere &shape1,const lpSphere &shape2,float dt)
	{
		return lmSphereSphereIntersection(	shape2.GetPosition(),
											shape2.GetRadius(),
											shape1.GetPosition(),
											shape1.GetRadius());
	}
	
	lpCollisionData GetCollisionData(std::tuple<unsigned int,const lpSphere *> &shape_1,std::tuple<unsigned int,const lpSphere *> &shape_2)
	{
		lpCollisionData CollisionData;
		
		unsigned int Id1;
		unsigned int Id2;
		const lpSphere *Sphere1;
		const lpSphere *Sphere2;
		std::tie(Id1,Sphere1) = shape_1;
		std::tie(Id2,Sphere2) = shape_2;
		
		CollisionData.Shape1Id = Id1;
		CollisionData.Shape2Id = Id2;
		CollisionData.Normal = Sphere2->GetPosition() - Sphere1->GetPosition();
		CollisionData.NormalOverlap = (Sphere1->GetRadius() + Sphere2->GetRadius()) - CollisionData.Normal.Length();
		CollisionData.Normal.Normalize();
		CollisionData.IntersectionPoint1 = Sphere1->GetPosition() + CollisionData.Normal * Sphere1->GetRadius();
		CollisionData.IntersectionPoint2 = Sphere2->GetPosition() - CollisionData.Normal * Sphere2->GetRadius();
		
		return CollisionData;
	}
	
public:
	
	void AddShape(unsigned int shape_id,const lpSphere *shape)
	{
		Shapes.emplace_back(shape_id,shape);
	}
	
	void CheckCollision(float dt)
	{
		ResizeCollisionTable();
		ClearCollisionTable();
		
		int i = 0;
		for(auto I = Shapes.begin();I != Shapes.end();I++)
		{
			int j = i + 1;
			for(auto J = std::next(I);J != Shapes.end();J++)
			{
				const lpSphere *bodies[2];
				std::tie(std::ignore,bodies[0]) = *I;
				std::tie(std::ignore,bodies[1]) = *J;
				
				CollisionTable[i*Shapes.size() + j] = CheckCollision(*bodies[0],*bodies[1],dt);
				j++;
			}
			i++;
		}
	}
	
	unsigned int GetNumCollisions()
	{
		unsigned int NumCollisions = 0;
		int i = 0;
		for(auto I = Shapes.begin();I != Shapes.end();I++)
		{
			int j = i + 1;
			for(auto J = std::next(I);J != Shapes.end();J++)
			{
				if(CollisionTable[i*Shapes.size() + j])
				{
					unsigned int id_i;
					unsigned int id_j;
					std::tie(id_i,std::ignore) = *I;
					std::tie(id_j,std::ignore) = *J;
					if(id_i != id_j)
					{
						NumCollisions++;
					}
				}
				j++;
			}
			i++;
		}
		return NumCollisions;
	}
	
	void GetCollisionData(lpCollisionData collision_data[],unsigned int num_collision_data)
	{
		unsigned int NextId = 0;
		int i = 0;
		for(auto I = Shapes.begin();I != Shapes.end();I++)
		{
			int j = i + 1;
			for(auto J = std::next(I);J != Shapes.end();J++)
			{
				if(CollisionTable[i*Shapes.size() + j])
				{
					unsigned int id_i;
					unsigned int id_j;
					std::tie(id_i,std::ignore) = *I;
					std::tie(id_j,std::ignore) = *J;
					if(id_i != id_j)
					{
						collision_data[NextId] = GetCollisionData(*I,*J);
						NextId++;
						if(NextId == num_collision_data)
						{
							return;
						}
					}
				}
				j++;
			}
			i++;
		}
	}
	
	lpSymmetricCollisionTable()
	{}
	
	~lpSymmetricCollisionTable()
	{}
	/*
	 * End of class
	 */
};

#endif // LP_SYMMETRIC_COLLISION_TABLE_H
