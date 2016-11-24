#ifndef L_TECH_DEMO
#define L_TECH_DEMO

#include "../lGame.h"


#include <SDL2/SDL.h>

#include "../lParticleSystem.h"

using namespace std;




#include "../../lRenderer/lrSceneGraph/lrsStaticMeshNode.h"
#include "../../lRenderer/lrSceneGraph/lrsSkeletalMeshNode.h"

#include "../../lInterfaces/lPhys/lPhys.h"


class lPhysicalObject : public liAgent
{
protected:
	bool Removable = false;

    liRigidBody::liState *State;

    lrsSceneNode *SceneNode;

public:

	virtual void MarkRemovable() override {Removable = true;}
	virtual bool IsRemovable() override {return Removable;}

    virtual void Update(double dt) override
    {
       SceneNode->SetPosition(State->GetPosition());
       SceneNode->SetOrientation(State->GetOrientationQuaternion());
    }

    lPhysicalObject(liRigidBody::liState *state,lrsSceneNode *scene_node)
        :State(state),SceneNode(scene_node)
    {

    }

    virtual ~lPhysicalObject()
    {

    }
};

class lPhysicalObject_StaticMesh : public lPhysicalObject
{
protected:
    lrsStaticMeshNode *StaticMeshNode;

public:

    liStaticMesh *GetMesh()
    {
        return StaticMeshNode->GetMesh();
    }

    liMultiMaterial *GetMaterial()
    {
        return StaticMeshNode->GetMaterial();
    }

    lPhysicalObject_StaticMesh(liRigidBody::liState *state,lrsStaticMeshNode *static_mesh_node)
        :lPhysicalObject(state,static_mesh_node),StaticMeshNode(static_mesh_node)
    {

    }

    virtual ~lPhysicalObject_StaticMesh() override
    {
    	delete StaticMeshNode;
    }
};

class lPhysicalObject_SkeletalMesh : public lPhysicalObject
{
protected:
    lrsSkeletalMeshNode *SkeletalMeshNode;

public:

	virtual void Update(double dt) override
	{
		lPhysicalObject::Update(dt);
	}

    liCharacter *GetCharacter()
    {
        return SkeletalMeshNode->GetCharacter();
    }

    liMultiMaterial *GetMaterial()
    {
        return SkeletalMeshNode->GetMaterial();
    }

    liSkeleton &GetSkeleton()
    {
        return SkeletalMeshNode->GetSkeleton();
    }

    void SetSkeleton(const liSkeleton &skeleton)
    {
        SkeletalMeshNode->SetSkeleton(skeleton);
    }
	/*
    virtual void Add(lrRenderer *Renderer) override
    {
        Renderer->AddSkeletalMesh(*GetCharacter(),GetSkeleton(),*GetMaterial(),*SkeletalMeshNode);
    }
	*/
    lPhysicalObject_SkeletalMesh(liRigidBody::liState *state,lrsSkeletalMeshNode *skeletal_mesh_node)
        :lPhysicalObject(state,skeletal_mesh_node),SkeletalMeshNode(skeletal_mesh_node)//,Character(character),Material(material),Skeleton(skeleton)
    {
    	//SkeletalMeshNode = new lrsSkeletalMeshNode(state->GetPosition(),scale,state->GetOrientationQuaternion(),character,material,skeleton);
    }

    virtual ~lPhysicalObject_SkeletalMesh() override
    {
    	delete SkeletalMeshNode;
    }
};












class lPlayer// : public liAgent
{
private:
	liRigidBody *Body;


	//lmScalar Acceleration = 5.0;

	lmScalar Radius;
	lmScalar IdealSpeed = 10.0;
	lmScalar AngularFrequency;

	bool GradualAcceleration = false;
	lmScalar IdealAcceleration = 40.0;
	lmScalar AngularAcceleration;// = 80.0;

	lmScalar TurnAngularFrequency = 2.0*PI*0.25;

	lmVector3D Up			= lmVector3D({0.0,1.0,0.0});
	lmVector3D Direction	= lmVector3D({0.0,0.0,-1.0});
	lmVector3D Szercsy;

	void CalculateSzercsy()
	{
		Szercsy = lmCross(Direction,Up);
		Szercsy.Normalize();
	}

	void Rotate(const lmVector3D &axle,lmScalar angle)
	{
		lmQuaternion q(axle,angle);
		/*
		lmQuaternion qDirection(Direction.X,Direction.Y,Direction.Z,0.0);

		qDirection = q*qDirection*q.GetInverse();

		Direction.X = qDirection.X;
		Direction.Y = qDirection.Y;
		Direction.Z = qDirection.Z;
		*/
		Direction = lmQuaternionRotate(Direction,q);

		CalculateSzercsy();

		lmVector3D AngularVelocity = Body->GetInvInertiaTensor() * Body->GetAngularMomentum();
		/*
		lmQuaternion qAngularVelocity(AngularVelocity.X,AngularVelocity.Y,AngularVelocity.Z,0.0);
		qAngularVelocity = q*qAngularVelocity*q.GetInverse();

		AngularVelocity.X = qAngularVelocity.X;
		AngularVelocity.Y = qAngularVelocity.Y;
		AngularVelocity.Z = qAngularVelocity.Z;
		*/
		AngularVelocity = lmQuaternionRotate(AngularVelocity,q);

		Body->GetState()->SetAngularMomentum(Body->GetInertiaTensor() * AngularVelocity);
	}

	void Accelerate(const lmVector3D &axle,lmScalar dt)
	{
		liRigidBody::liState *State = Body->GetState();

		lmScalar MomentOfInertia = lmDot(axle,Body->GetInertiaTensor()*axle);
		State->IncAngularMomentum(MomentOfInertia * AngularAcceleration * dt * axle);
	}

	void Dampen(const lmVector3D &axle,lmScalar dt)
	{
		liRigidBody::liState *State = Body->GetState();
		lmVector3D AngularFrequencyVector = Body->GetInvInertiaTensor() * State->GetAngularMomentum();

		lmScalar Dot = lmDot(AngularFrequencyVector,axle);
		if(std::abs(Dot) > 0.0)
		{
			liRigidBody::liState *State = Body->GetState();

			lmScalar MomentOfInertia = lmDot(axle,Body->GetInertiaTensor()*axle);
			//Trükk, hogy a csillapítás csökkenjen, ha a szögsebesség kicsi.
			State->IncAngularMomentum(MomentOfInertia * -1.0 * sgn(Dot) * std::min(Dot*Dot * AngularAcceleration,AngularAcceleration) * dt * axle);
		}
	}

	void SetAngularFrequency(lmScalar angular_frequency,const lmVector3D &axle)
	{
		liRigidBody::liState *State = Body->GetState();

		lmScalar RemovableComponent = lmDot(axle,State->GetAngularMomentum());
		State->IncAngularMomentum(-RemovableComponent * axle);

		lmScalar MomentOfInertia = lmDot(axle,Body->GetInertiaTensor()*axle);
		State->IncAngularMomentum(MomentOfInertia * angular_frequency * axle);
	}

	void ZeroAngularMomentumComponent(const lmVector3D &axle)
	{
		liRigidBody::liState *State = Body->GetState();

		lmScalar Magnitude = lmDot(axle,State->GetAngularMomentum());
		State->SetAngularMomentum(State->GetAngularMomentum() - Magnitude*axle);
	}

public:

	bool MoveForward	= false;
	bool MoveBackward	= false;
	bool MoveLeft		= false;
	bool MoveRight		= false;

	bool TurnLeft		= false;
	bool TurnRight		= false;

	bool Jump			= false;



	/* 8===========================D
	 * TMP!!!!!!!!
	 * 8===========================D
	 */
	liRigidBody *GetBody()
	{
		return Body;
	}
	const lmVector3D &GetDirection()
	{
		return Direction;
	}
	const lmVector3D &GetSzercsy()
	{
		return Szercsy;
	}
	/* 8===========================D
	 * TMP!!!!!!!!
	 * 8===========================D
	 */


	void Update(float dt)
	{
		if(TurnLeft || TurnRight)
		{
			if(TurnLeft && !TurnRight)
			{
				Rotate(Up,TurnAngularFrequency*dt);
			}

			if(!TurnLeft && TurnRight)
			{
				Rotate(Up,-1.0*TurnAngularFrequency*dt);
			}
		}

		liRigidBody::liState *State = Body->GetState();

		/*
		 * Az alábbi változók alapján döntjük el, hogy az avatár a vezérlés alapján melyik irányba megy.
		 *
		 * Jelmagyarázat:
		 * Legyen az "előre" irány északra, a "hátra" délre, a "balra" nyugat "jobbra" pedig kelet.
		 *
		 * N: North - észak
		 * S: South - dél
		 * W: West  - nyugat
		 * E: East  - kelet
		 *
		 */

		bool NotMoving = (MoveForward && MoveBackward && MoveLeft && MoveRight) || (!MoveForward && !MoveBackward && !MoveLeft && !MoveRight);

		// Ez a változó true, ha jobbra vagy balra mozgunk
		bool CanMoveWorE = (MoveForward && MoveBackward) || (!MoveForward && !MoveBackward);
		// Ez a változó true, ha előre vagy hátrafele mozgunk.
		bool CanMoveNorS = (MoveLeft && MoveRight) || (!MoveLeft && !MoveRight);

		//Ez a változó true, ha 45 fokban balra előre haladunk.
		bool CanMoveNE = MoveForward && MoveRight;
		//Ez a változó true, ha 45 fokban balra hátra haladunk.
		bool CanMoveSE = MoveBackward && MoveRight;

		//Ez a változó true, ha 45 fokban jobbra előre haladunk.
		bool CanMoveNW = MoveForward && MoveLeft;
		//Ez a változó true, ha 45 fokban jobbra hátra haladunk.
		bool CanMoveSW = MoveBackward && MoveLeft;

		/*
		 * Ami itt történik, azt kurvára csiszolni kéne.
		 */
		if(NotMoving)
		{
			lmVector3D AngularFrequencyAxle = ((lmScalar)-1.0) * Body->GetInvInertiaTensor() * State->GetAngularMomentum();
			AngularFrequencyAxle.Normalize();

			if(GradualAcceleration)
			{Dampen(AngularFrequencyAxle,dt);}
			else
			{State->SetAngularMomentum({0.0,0.0,0.0});}
		}
		else
		{
			if(CanMoveWorE && !CanMoveNorS)
			{
				if(MoveLeft)
				{
					if(lmDot(Body->GetInvInertiaTensor() * Body->GetAngularMomentum(),-1.0*Direction) < AngularFrequency)
					{
						if(GradualAcceleration)
						{Accelerate(-1.0*Direction,dt);}
						else
						{SetAngularFrequency(-1.0*AngularFrequency,Direction);}
					}
				}
				else if(MoveRight)
				{
					if(lmDot(Body->GetInvInertiaTensor() * Body->GetAngularMomentum(),Direction) < AngularFrequency)
					{
						if(GradualAcceleration)
						{Accelerate(Direction,dt);}
						else
						{SetAngularFrequency(AngularFrequency,Direction);}
					}
				}

				if(GradualAcceleration)
				{Dampen(Szercsy,dt);}
				else
				{ZeroAngularMomentumComponent(Szercsy);}
			}
			else if(CanMoveNorS && !CanMoveWorE)
			{
				if(MoveForward)
				{
					if(lmDot(Body->GetInvInertiaTensor() * Body->GetAngularMomentum(),-1.0*Szercsy) < AngularFrequency)
					{
						if(GradualAcceleration)
						{Accelerate(-1.0*Szercsy,dt);}
						else
						{SetAngularFrequency(-1.0*AngularFrequency,Szercsy);}
					}
				}
				else if(MoveBackward)
				{
					if(lmDot(Body->GetInvInertiaTensor() * Body->GetAngularMomentum(),Szercsy) < AngularFrequency)
					{
						if(GradualAcceleration)
						{Accelerate(Szercsy,dt);}
						else
						{SetAngularFrequency(AngularFrequency,Szercsy);}
					}
				}

				if(GradualAcceleration)
				{Dampen(Direction,dt);}
				else
				{ZeroAngularMomentumComponent(Direction);}
			}
			else
			{
				lmVector3D Vec_NE = (1.0/2.0)*Direction + (1.0/2.0)*Szercsy;
				lmVector3D Vec_SE = (-1.0/2.0)*Direction + (1.0/2.0)*Szercsy;

				if(CanMoveNE)
				{
					lmVector3D Axle = -1.0 * Vec_SE;

					if(lmDot(Body->GetInvInertiaTensor() * Body->GetAngularMomentum(),Axle) < AngularFrequency)
					{
						if(GradualAcceleration)
						{Accelerate(Axle,dt);}
						else
						{SetAngularFrequency(AngularFrequency,Axle);}
					}

					if(GradualAcceleration)
					{Dampen(Vec_NE,dt);}
					else
					{ZeroAngularMomentumComponent(Vec_NE);}
				}
				else if(CanMoveSE)
				{
					lmVector3D Axle = Vec_NE;

					if(lmDot(Body->GetInvInertiaTensor() * Body->GetAngularMomentum(),Axle) < AngularFrequency)
					{
						if(GradualAcceleration)
						{Accelerate(Axle,dt);}
						else
						{SetAngularFrequency(AngularFrequency,Axle);}
					}

					if(GradualAcceleration)
					{Dampen(Vec_SE,dt);}
					else
					{ZeroAngularMomentumComponent(Vec_SE);}
				}
				else if(CanMoveSW)
				{
					lmVector3D Axle = Vec_SE;

					if(lmDot(Body->GetInvInertiaTensor() * Body->GetAngularMomentum(),Axle) < AngularFrequency)
					{
						if(GradualAcceleration)
						{Accelerate(Axle,dt);}
						else
						{SetAngularFrequency(AngularFrequency,Axle);}
					}

					if(GradualAcceleration)
					{Dampen(Vec_NE,dt);}
					else
					{ZeroAngularMomentumComponent(Vec_NE);}
				}
				else if(CanMoveNW)
				{
					lmVector3D Axle = -1.0 * Vec_NE;

					if(lmDot(Body->GetInvInertiaTensor() * Body->GetAngularMomentum(),Axle) < AngularFrequency)
					{
						if(GradualAcceleration)
						{Accelerate(Axle,dt);}
						else
						{SetAngularFrequency(AngularFrequency,Axle);}
					}

					if(GradualAcceleration)
					{Dampen(Vec_SE,dt);}
					else
					{ZeroAngularMomentumComponent(Vec_SE);}
				}
			}
		}
	}

	const lmVector3D &GetPosition()
	{
		return Body->GetPosition();
	}

	lPlayer(liRigidBody *body,lmScalar radius)
		:Body(body),Radius(radius)
	{
		AngularFrequency = IdealSpeed / Radius;
		AngularAcceleration = IdealAcceleration / Radius;

		CalculateSzercsy();
	}

	virtual ~lPlayer() //override
	{
		Body->MarkRemovable();
	}
};

#include "../../lRenderer/lrCharacterAnim/lrCharacterAnim.h"

class lTechDemo : public lGame
{
protected:
    //Game related
    float DefaultMass;
	const float PLAYER_RADIUS = 0.8;

    //lParticleSystem *ParticleSystem;

    //A doboz, amiben a szimuláció játszódik.
    lrsStaticMeshNode *FalObj;

    //A testek meshe
    liPhMesh *BodyMesh;

    //Skeletal mesh
    liCharacter				*Character;
    //lrKeyframeAnimation	*Animation;
    lrCharacterAnim	*CharacterAnim;

    //TMP!!!!!!!!!
    bool Anyad = false;
    bool NagyAnyad = false;
    bool Apad = false;
    bool NagyApad = false;
    //TMP!!!!!!!

    lrsSkeletalMeshNode		*SkeletalMeshNode;

    lPlayer *Player;

    double t = 0.0;

    bool MeshOrBall = true;
    bool BallConstraint = false;


    void AddBall();
    void AddMesh();

    virtual void Logic() override;

public:
    lTechDemo(liApiAdapter *api_adapter,std::string &next_map);

    virtual ~lTechDemo() override;
};

#endif
