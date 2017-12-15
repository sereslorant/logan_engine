#ifndef LP_SIMULATION_WORLD_H
#define LP_SIMULATION_WORLD_H

#include "lpBodyFactory.h"

#include <memory>

class lpSimulationWorld : private lpBodyBuilder::liBodyContainer
{
private:
	bool Paused = false;
	std::vector<std::unique_ptr<lpBody> > Bodies;
	
	lpSpacePartition SpacePartition;
	
	lpBodyFactory BodyFactory;
	
	void CollisionResponse(lpBody *body_1,lpBody *body_2,float dt,const lmVector3D &normal,float overlap)
	{
		if(body_1->GetCollisionResponse() == lpBody::MOVABLE && body_2->GetCollisionResponse() == lpBody::MOVABLE)
		{
			float Speed = overlap/(2.0f*dt);

			body_2->AddVelocity(normal * (Speed));
			body_1->AddVelocity(normal * (-Speed));
		}
		else if(body_2->GetCollisionResponse() == lpBody::MOVABLE && body_1->GetCollisionResponse() == lpBody::IMMOVABLE)
		{
			float Speed = overlap/dt;

			body_2->AddVelocity(normal * Speed);
		}
		else if(body_2->GetCollisionResponse() == lpBody::IMMOVABLE && body_1->GetCollisionResponse() == lpBody::MOVABLE)
		{
			float Speed = overlap/dt;

			body_1->AddVelocity(normal * (-Speed));
		}
	}
	
public:
	
	void AddBody(lpBody *new_body)
	{
		int body_id = Bodies.size();
		Bodies.emplace_back(new_body);
		SpacePartition.AddCollisionShape(body_id,&new_body->GetCollisionShape());
	}
	
	void Pause()
	{
		Paused = true;
	}
	
	void Continue()
	{
		Paused = false;
	}
	
	lpBodyFactory &GetBodyFactory()
	{
		return BodyFactory;
	}
	
	void Simulate(float dt)
	{
		if(!Paused)
		{
			//Detect collision
			SpacePartition.CheckCollision(dt);
			
			//Collision response
			std::vector<lpCollisionData> CollisionData(SpacePartition.GetNumCollisions());
			SpacePartition.GetCollisionData(&CollisionData[0],CollisionData.size());
			for(lpCollisionData &Collision : CollisionData)
			{
				lpBody *Body1 = Bodies[Collision.Shape1Id].get();
				lpBody *Body2 = Bodies[Collision.Shape2Id].get();
				CollisionResponse(Body1,Body2,dt,Collision.Normal,Collision.NormalOverlap);
			}
			
			//Step
			for(auto &I : Bodies)
			{
				if(I->GetCollisionResponse() == lpBody::MOVABLE)
				{
					I->AddVelocity({0.0f,-9.81f * dt,0.0});
				}
				I->Step(dt);
			}
		}
	}
	
	void RemoveRemovable()
	{
		for(auto I = Bodies.begin();I != Bodies.end();)
		{
			if((*I)->IsRemovable())
			{
				auto K = I;
				I++;
				
				Bodies.erase(K);
				continue;
			}
			else
			{
				I++;
			}
		}
	}
	
	lpSimulationWorld()
		:BodyFactory(*this)
	{}
	
	~lpSimulationWorld()
	{}
	/*
	 * End of class
	 */
};

#endif // LP_SIMULATION_WORLD_H
