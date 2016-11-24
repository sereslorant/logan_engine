
#include "../lRandomGenerator.h"

#include "lParticleSystem.h"

liRigidBody *lParticle::GetBody()
{
	return Body;
}

liRigidBody::liState *lParticle::GetState()
{
	return State;
}

bool lParticle::IsAlive()
{
	return Alive;
}

void lParticle::Reset()
{
	Alive = true;
	TimeElapsed = 0.0;

	if(PointLight != nullptr)
	{
		PointLight->SetPosition(Body->GetPosition());
	}
}

void lParticle::Update(double dt)
{
	if(Alive && !Immortal)
	{
		TimeElapsed += dt;
		if(TimeElapsed > Lifespan)
		{
			Alive = false;
		}

		if(PointLight != nullptr)
		{
			PointLight->SetPosition(Body->GetPosition());
		}
	}
}

void lParticleSystem::lSpawnParticleCallback::Callback()
{
	if(ParticleSystem->Particles.size() < ParticleSystem->nParticles)
	{
		liRigidBody *Body = ParticleFactory->CreateParticle(ParticleSystem->Position,
															ParticleSystem->GetVelocity(),
															{0.0,0.0,0.0},
															{0.0,0.0,0.0}
															);

		liPointLight *Light;
		if(ParticleSystem->HasLight)
		{
			Light = ParticleFactory->CreateLight(ParticleSystem->LightAmbient,
												 ParticleSystem->LightDiffuse,
												 ParticleSystem->LightSpecular,
												 ParticleSystem->LightAttenuation
												 );

			Light->SetPosition(Body->GetPosition());
		}
		else
		{
			Light = nullptr;
		}

		ParticleSystem->Particles.push_back(new lParticle(Body,Light,ParticleSystem->Immortal,ParticleSystem->GetLifespan()));
	}
	else
	{
		ParticleSystem->Timer->Disable();
	}
}

lmVector3D lParticleSystem::GetVelocity()
{
	if(RandomizeVelocity)
	{
		lRandomGenerator *RandGen = lRandomGenerator::GetInstance();

		lmVector3D RandomVelocityDirection;

		float AzimuthalAngle = RandGen->RandomFloat(0.0,PI);
		float PolarAngle = RandGen->RandomFloat(0.0,MaxRandomVelocityAngle);

		lmVector3D RandVector = {RandGen->RandomFloat(-1.0,1.0),
								 RandGen->RandomFloat(-1.0,1.0),
								 RandGen->RandomFloat(-1.0,1.0)
								 };

		//TODO: A bázisvektorok irányát egyszer le kéne ellenőrizni papíron.
		lmVector3D Base1 = lmCross(RandVector,ParticleVelocityDirection);
		lmVector3D Base2 = lmCross(Base1,ParticleVelocityDirection);
		lmVector3D Base3 = ParticleVelocityDirection;

		Base1.Normalize();
		Base2.Normalize();

		lmScalar X = std::cos(AzimuthalAngle)*std::sin(PolarAngle);
		lmScalar Y = std::sin(AzimuthalAngle)*std::sin(PolarAngle);
		lmScalar Z = std::cos(PolarAngle);

		RandomVelocityDirection = X*Base1 + Y*Base2 + Z*Base3;
		RandomVelocityDirection.Normalize();

		return ParticleSpeed * RandomVelocityDirection;
	}
	else
	{
		return ParticleSpeed  * ParticleVelocityDirection;
	}
}

float lParticleSystem::GetLifespan()
{
	if(RandomizeLifespan)
	{
		lRandomGenerator *RandGen = lRandomGenerator::GetInstance();

		return RandGen->RandomFloat(MinLifespan,MaxLifespan);
	}
	else
	{
		return MinLifespan;
	}
}

void lParticleSystem::Update(double dt)
{
	for(lParticle *I : Particles)
	{
		if(!I->IsAlive())
		{
			liRigidBody *Body_I = I->GetBody();
			liRigidBody::liState *State_I = I->GetState();

			State_I->SetPosition(Position);
			State_I->SetMomentum(Body_I->GetMass() * GetVelocity());

			State_I->SetOrientation({0.0,0.0,0.0});
			State_I->SetAngularMomentum({0.0,0.0,0.0});

			I->Reset();
		}
		else
		{
			I->Update(dt);
		}
	}

	Timer->Update(dt);
}

lParticleSystem::lParticleSystem(liParticleFactory* particle_factory,unsigned int n_particles,bool immortal,const lmVector3D &position,lmScalar particle_speed,const lmVector3D &particle_velocity_direction,float min_lifespan,
				bool randomize_velocity,float max_random_velocity_angle,bool randomize_lifespan,float max_lifespan,
				bool has_light,float light_ambient[],float light_diffuse[],float light_specular[],float light_attenuation[],
				float spawn_delay)
	:nParticles(n_particles),Immortal(immortal),
	 Position(position),ParticleSpeed(particle_speed),ParticleVelocityDirection(particle_velocity_direction),MinLifespan(min_lifespan),
	 RandomizeVelocity(randomize_velocity),MaxRandomVelocityAngle(max_random_velocity_angle),RandomizeLifespan(randomize_lifespan),MaxLifespan(max_lifespan),
	 HasLight(has_light)
{
	ParticleVelocityDirection.Normalize();

	if(HasLight)
	{
		for(int i=0;i < 3;i++)
		{
			LightAmbient[i]		= light_ambient[i];
			LightDiffuse[i]		= light_diffuse[i];
			LightSpecular[i]	= light_specular[i];
			LightAttenuation[i]	= light_attenuation[i];
		}
	}
	else
	{
		for(int i=0;i < 3;i++)
		{
			LightAmbient[i]		= 0.0;
			LightDiffuse[i]		= 0.0;
			LightSpecular[i]	= 0.0;
			LightAttenuation[i]	= 0.0;
		}
	}

	Timer = new lTimer(spawn_delay,true);

	SpawnParticleCallback = new lSpawnParticleCallback(this,particle_factory);
	Timer->GetLimitReachedEvent()->AddCallback(SpawnParticleCallback);
	Timer->Enable();
}

lParticleSystem::~lParticleSystem()
{
	for(lParticle *I : Particles)
	{
		delete I;
	}

	delete Timer;
	delete SpawnParticleCallback;
}
