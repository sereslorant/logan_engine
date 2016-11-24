#ifndef L_PARTICLE_SYSTEM_H
#define L_PARTICLE_SYSTEM_H

#include "../lInterfaces/lPhys/lPhys.h"

#include "../lInterfaces/lRenderer/liPointLight.h"

#include "../lInterfaces/lGameMode/liAgent.h"

#include "lTimer.h"

class lParticle : public liAgent
{
private:
	bool Removable = false;

protected:
	liRigidBody *Body;
	liRigidBody::liState *State;

	liPointLight *PointLight;

	bool Alive;
	bool Immortal;

	float Lifespan;
	float TimeElapsed;

public:

	virtual void MarkRemovable() override
	{
		Removable = true;
	}

	virtual bool IsRemovable() override
	{
		return Removable;
	}

	liRigidBody *GetBody();

	liRigidBody::liState *GetState();

	bool IsAlive();

	void Reset();

	virtual void Update(double dt) override;

	lParticle(liRigidBody *body,liPointLight *point_light,bool immortal,float lifespan)
		:Body(body),State(body->GetState()),PointLight(point_light),Alive(true),Immortal(immortal),Lifespan(lifespan),TimeElapsed(0.0)
	{
		//Üres függvénytörzs
	}

	virtual ~lParticle() override
	{
		Body->MarkRemovable();

		PointLight->Disable();
		PointLight->MarkRemovable();
	}
};

class lParticleSystem : public liAgent
{
private:
	bool Removable = false;

public:

	class liParticleFactory
	{
	public:

		virtual liRigidBody *CreateParticle(const lmVector3D &position,const lmVector3D &velocity,const lmVector3D &orientation,const lmVector3D &angular_momentum) = 0;
		virtual liPointLight *CreateLight(float ambient[],float diffuse[],float specular[],float attenuation[]) = 0;

		liParticleFactory()
		{
			//Üres függvénytörzs
		}

		virtual ~liParticleFactory()
		{
			//Üres függvénytörzs
		}

		/*
		 * Class vége
		 */
	};

protected:
	std::list<lParticle *> Particles;

	int nParticles;

	bool Immortal;

	lmVector3D	Position;
	lmScalar	ParticleSpeed;
	lmVector3D	ParticleVelocityDirection;

	float MinLifespan;

	//A randomizált dolgokhoz tartozó változók
	bool RandomizeVelocity;
	float MaxRandomVelocityAngle;

	bool RandomizeLifespan;
	float MaxLifespan;

	bool HasLight;
	float LightAmbient[3];
	float LightDiffuse[3];
	float LightSpecular[3];
	float LightAttenuation[3];

	liTimer *Timer;

	class lSpawnParticleCallback : public liEventCallback
	{
	private:
		lParticleSystem		*ParticleSystem;
		liParticleFactory	*ParticleFactory;

	public:
		virtual void Callback() override;

		lSpawnParticleCallback(lParticleSystem *particle_system,liParticleFactory *particle_factory)
			:ParticleSystem(particle_system),ParticleFactory(particle_factory)
		{
			//Üres függvénytörzs
		}

		virtual ~lSpawnParticleCallback() override
		{
			//Üres függvénytörzs
			delete ParticleFactory;
		}
	};

	lmVector3D GetVelocity();
	float GetLifespan();

	lSpawnParticleCallback *SpawnParticleCallback;
public:

	virtual void MarkRemovable() override
	{
		Removable = true;
	}

	virtual bool IsRemovable() override
	{
		return Removable;
	}

	virtual void Update(double dt) override;

	lParticleSystem(liParticleFactory* particle_factory,unsigned int n_particles,bool immortal,const lmVector3D &position,lmScalar particle_speed,const lmVector3D &particle_velocity_direction,float min_lifespan,
					bool randomize_velocity,float max_random_velocity_angle,bool randomize_lifespan,float max_lifespan,
					bool has_light,float light_ambient[],float light_diffuse[],float light_specular[],float light_attenuation[],
					float spawn_delay);

	virtual ~lParticleSystem() override;

	/*
	 * Class vége
	 */
};

#endif // L_PARTICLE_SYSTEM_H
