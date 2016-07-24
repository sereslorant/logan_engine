#ifndef L_SPACE_SHOOTER_H
#define L_SPACE_SHOOTER_H

#include "../lGame/lP2Game.h"

class lSS_Agent : public liAgent
{
protected:
	liBody2D *Body;
	bool Removable = false;

public:
	//
	virtual void MarkRemovable() override
	{
		Removable = true;
	}
	//
	virtual bool IsRemovable() override
	{
		return Removable;
	}
	//
	//virtual void Update(double dt) = 0;
	//
	lSS_Agent(liBody2D *body)
		:Body(body)
	{}
	//
	virtual ~lSS_Agent() override
	{
		Body->MarkRemovable();
	}
};

class lSS_Projectile : public lSS_Agent
{
protected:
	unsigned int Species;
	lmVector2D Velocity;
	bool Alive = true;
	//
public:
	//
	virtual unsigned int GetSpecies() //override
	{
		return Species;
	}
	//
	virtual void Update(double dt) override
	{
		if(Alive)
		{
			Body->SetVelocity(Velocity);
		}
		else
		{
			Removable = true;
		}
	}
	//
	lSS_Projectile(liBody2D *body,unsigned int species,const lmVector2D &velocity)
		:lSS_Agent(body),Species(species),Velocity(velocity)
	{}
	//
	virtual ~lSS_Projectile() override
	{}
};

class lSS_LivingObject : public lSS_Agent
{
protected:
	unsigned int Species;
	bool Alive = true;
	const float MaxHealth;
	float Health;
	//
	virtual void PerformAction(double dt) = 0;
	//
public:
	//
	virtual unsigned int GetSpecies() //override
	{
		return Species;
	}
	//
	virtual void Damage(float damage) //override
	{
		if(Health > damage)
		{
			Health -= damage;
		}
		else
		{
			Health = 0.0;
			Alive = false;
		}
	}
	//
	virtual void Update(double dt) override
	{
		if(Alive)
		{
			PerformAction(dt);
		}
		else
		{
			Removable = true;
		}
	}
	//
	lSS_LivingObject(liBody2D *body,unsigned int species,float max_health,float health)
		:lSS_Agent(body),Species(species),MaxHealth(max_health),Health(health)
	{}
	//
	virtual ~lSS_LivingObject() override
	{}
};

#include "../../../lInterfaces/lRenderer/li2DScene.h"


#include "../../../lRenderer/lr2DScene/lr2DScene.h"

class lSS_GameMode : public lP2ProtoGameMode
{
protected:
	//
	liController *Controller = nullptr;
	//
	li2DScene *Scene;
	//
public:
	//
	lSS_GameMode(liInput &input,liWorld2D &world,liRenderer &renderer)
		:lP2ProtoGameMode(input,world,renderer)
	{
		if(Input.GetNumControllers() != 0)
		{
			Controller = Input.GetController(0);
		}
		//
		Scene = new lr2DScene;
	}
	//
	virtual ~lSS_GameMode() override
	{
		delete Scene;
	}
};

#endif // L_SPACE_SHOOTER_H
