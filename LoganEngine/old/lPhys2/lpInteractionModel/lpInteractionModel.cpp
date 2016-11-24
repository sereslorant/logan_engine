
#include <iostream>

#include "lpInteractionModel.h"

void lpInteractionModel::lpContact::AddContact(const lmVector3D &intersection_i,const lmVector3D &intersection_j,const lmVector3D &normal,lmScalar penetration_depth)
{
	//Elkérjük a Bi test pillanatnyi szögsebességét.
	lmVector3D  biAngularVelocity = biInvInertiaTensor_t * State_i->GetAngularMomentum();
	//Elkérjük a Bj test pillanatnyi szögsebességét.
	lmVector3D	bjAngularVelocity = bjInvInertiaTensor_t * State_j->GetAngularMomentum();

	//Kiszámoljuk a Bj test Bi-éhez viszonyított relatív szögsebességét a támadáspontban a v = omega x r képlettel.
	lmVector3D RelativeAngularVelocity = lmCross(biAngularVelocity,intersection_i) - lmCross(bjAngularVelocity,intersection_j);

	lmVector3D RelativeVelocity = RelativeLinearVelocity + RelativeAngularVelocity;
	lmScalar RelativeNormalVelocity = lmDot(normal,RelativeVelocity);

	//Az, hogy ezt eredetileg nem ellenőriztem le, olyan epic bugokhoz vezetett, hogy arra nincs emberi kifejezés. :P
	if(RelativeNormalVelocity < 0.0)
	{
		CollisionData.push_back({intersection_i,intersection_j,normal,penetration_depth,RelativeVelocity,RelativeNormalVelocity,false});
	}
	else
	{
		CollisionData.push_back({intersection_i,intersection_j,normal,penetration_depth,RelativeVelocity,RelativeNormalVelocity,true});
	}
}

lpInteractionModel::lpContact::lpContact(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j
		/*,const lmVector3D &pi,const lmVector3D &pj,lmVector3D &normal,lmScalar penetration_depth,lmScalar multiplier*/)
	:Bi(bi),Bj(bj),State_i(state_i),State_j(state_j),Derivative_i(derivative_i),Derivative_j(derivative_j)//,Pi(pi),Pj(pj),Normal(normal),PenetrationDepth(penetration_depth),Multiplier(multiplier)
{
	Removable = false;

	//Elkérjük a Bi és a Bj test pillanatnyi sebességét.
	lmVector3D biVelocity = State_i->GetMomentum() * Bi->GetInvMass();
	lmVector3D bjVelocity = State_j->GetMomentum() * Bj->GetInvMass();

	//Kiszámítjuk a Bj testnek a Bi-éhez viszonyított relatív sebességét
	RelativeLinearVelocity  = biVelocity - bjVelocity;

	//Kiszámoljuk a Bi test tehetetlenségi tenzorát.
	lmMatrix3x3 biRotation(lmMatrix3x3::IDENTITY);// = lmRotate3x3(State_i->GetOrientationQuaternion());
	lmRotate3x3(biRotation,State_i->GetOrientationQuaternion());

	biInvInertiaTensor_t = biRotation * Bi->GetInvInertiaTensor() * biRotation.Transpose();
	/*
	std::cout << biInvInertiaTensor_t[0][0] << ";" << biInvInertiaTensor_t[1][0] << ";" << biInvInertiaTensor_t[2][0] << std::endl;
	std::cout << biInvInertiaTensor_t[0][1] << ";" << biInvInertiaTensor_t[1][1] << ";" << biInvInertiaTensor_t[2][1] << std::endl;
	std::cout << biInvInertiaTensor_t[0][2] << ";" << biInvInertiaTensor_t[1][2] << ";" << biInvInertiaTensor_t[2][2] << std::endl;
	*/

	//Kiszámoljuk a Bj test tehetetlenségi tenzorát.
	lmMatrix3x3 bjRotation(lmMatrix3x3::IDENTITY);// = lmRotate3x3(State_j->GetOrientationQuaternion());
	lmRotate3x3(bjRotation,State_j->GetOrientationQuaternion());

	bjInvInertiaTensor_t = bjRotation * Bj->GetInvInertiaTensor() * bjRotation.Transpose();
	/*
	std::cout << bjInvInertiaTensor_t[0][0] << ";" << bjInvInertiaTensor_t[1][0] << ";" << bjInvInertiaTensor_t[2][0] << std::endl;
	std::cout << bjInvInertiaTensor_t[0][1] << ";" << bjInvInertiaTensor_t[1][1] << ";" << bjInvInertiaTensor_t[2][1] << std::endl;
	std::cout << bjInvInertiaTensor_t[0][2] << ";" << bjInvInertiaTensor_t[1][2] << ";" << bjInvInertiaTensor_t[2][2] << std::endl;
	*/
}

void lpInteractionModel::ApplyConstraintForces(lmScalar dt)
{
	for(auto I = Constraints.begin(); I != Constraints.end();)
	{
		if(!(*I)->IsRemovable())
		{
			(*I)->ApplyForces(dt);
		}

		if(!(*I)->IsRemovable())
		{
			I++;
		}
		else
		{
			if(true) //Itt majd azt kell lecsekkolni, hogy a constraintet egy poolból vettük e, és csak meg kell jelölni, hogy érvénytelen, vagy delete-elni kell.
			{
				liConstraint *Constraint = *I;
				delete Constraint;
			}
			else
			{
				//Majd itt meg kell jelölni, hogy "érvénytelen" és vissza lehet rakni a poolba.
			}

			auto J = I;
			I++;
			Constraints.erase(J);
		}
	}
}

void lpInteractionModel::ApplyForces(liRigidBody &Bi,const liRigidBody::liState &State_i,liRigidBody::liDerivative &Derivative_i,lmScalar dt)
{
	//Kiszámoljuk a gravitációs erő nagyságát
	lmVector3D GravForce   = Bi.GetMass() * GravAcceleration;
	//Kiszámoljuk a légellenállás nagyságát
	lmVector3D AirFriction = -AirDensity * (State_i.GetMomentum() * Bi.GetInvMass());

	//Hozzáadjuk az eredő erőhöz
	Derivative_i.IncNetForce(GravForce);
	Derivative_i.IncNetForce(AirFriction);
}

lpInteractionModel::lpInteractionModel(const lmVector3D &grav_acceleration,lmScalar air_density,lmScalar coefficient_of_friction)
	:GravAcceleration(grav_acceleration),AirDensity(air_density),CoefficientOfFriction(coefficient_of_friction)
{

}

lpInteractionModel::~lpInteractionModel()
{
	for(auto &I : Constraints)
	{
		delete I;
	}

	Constraints.clear();
}
