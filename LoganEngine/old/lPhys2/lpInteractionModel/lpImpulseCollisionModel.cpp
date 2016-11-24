
#include <iostream>

#include "lpImpulseCollisionModel.h"

void lpImpulseCollisionModel::lpSpringDistConstraint::ApplyForces(lmScalar dt)
{
	//Elkérjük a Bi és a Bj test pillanatnyi sebességét.
	lmVector3D biVelocity = State_i->GetMomentum() * Bi->GetInvMass();
	lmVector3D bjVelocity = State_j->GetMomentum() * Bj->GetInvMass();

	//Kiszámítjuk a Bj testnek a Bi-éhez viszonyított relatív sebességét
	lmVector3D RelativeLinearVelocity  = biVelocity - bjVelocity;

	lmMatrix3x3 biRotation(lmMatrix3x3::IDENTITY);
	lmRotate3x3(biRotation,State_i->GetOrientationQuaternion());
/*
	std::cout << biRotation[0][0] << ";" << biRotation[1][0] << ";" << biRotation[2][0] << std::endl;
	std::cout << biRotation[0][1] << ";" << biRotation[1][1] << ";" << biRotation[2][1] << std::endl;
	std::cout << biRotation[0][2] << ";" << biRotation[1][2] << ";" << biRotation[2][2] << std::endl;
*/
	lmMatrix3x3 bjRotation(lmMatrix3x3::IDENTITY);
	lmRotate3x3(bjRotation,State_j->GetOrientationQuaternion());
/*
	std::cout << bjRotation[0][0] << ";" << bjRotation[1][0] << ";" << bjRotation[2][0] << std::endl;
	std::cout << bjRotation[0][1] << ";" << bjRotation[1][1] << ";" << bjRotation[2][1] << std::endl;
	std::cout << bjRotation[0][2] << ";" << bjRotation[1][2] << ";" << bjRotation[2][2] << std::endl;
*/
	lmMatrix3x3 biInvInertiaTensor_t = biRotation * Bi->GetInvInertiaTensor() * biRotation.Transpose();
	lmMatrix3x3 bjInvInertiaTensor_t = bjRotation * Bj->GetInvInertiaTensor() * bjRotation.Transpose();

	//Elkérjük a Bi test pillanatnyi szögsebességét.
	lmVector3D  biAngularVelocity = biInvInertiaTensor_t * State_i->GetAngularMomentum();
	//Elkérjük a Bj test pillanatnyi szögsebességét.
	lmVector3D	bjAngularVelocity = bjInvInertiaTensor_t * State_j->GetAngularMomentum();

	//Kiszámoljuk a Bj test Bi-éhez viszonyított relatív szögsebességét a támadáspontban a v = omega x r képlettel.
	lmVector3D RotPi = biRotation * Pi;
	lmVector3D RotPj = bjRotation * Pj;
	lmVector3D RelativeAngularVelocity = lmCross(biAngularVelocity,RotPi) - lmCross(bjAngularVelocity,RotPj);

	//A támadáspontban a relatív sebesség:
	lmVector3D RelativeVelocity = RelativeLinearVelocity + RelativeAngularVelocity;

	lmVector3D ForceDirection = (RotPi + State_i->GetPosition()) - (RotPj + State_j->GetPosition());
	//Innen a relatív normál sebesség - a sebesség normálvektorra vett vetülete

	lmScalar Distance_t = ForceDirection.LengthSquared();
	if(std::abs(Distance_t - (Distance * Distance)) > 1e-8)
	{
		Distance_t = (std::sqrt(Distance_t) - Distance);
		ForceDirection.Normalize();

		lmScalar RelativeNormalVelocity = lmDot(ForceDirection,RelativeVelocity);

		lmScalar SpringForceMagnitude = Stiffness * Distance_t;
		lmScalar DampingMagnitude = DampingCoefficient * RelativeNormalVelocity;

		 /*
		if(sgn(DampingMagnitude) == sgn(SpringForceMagnitude))
		{
			DampingMagnitude = 0.0;
			std::cout << "Szercsy!" << std::endl;
		}
		else
		{
			std::cout << "Lavcsy!" << std::endl;
		}
		// */

		//Rugóerő kiszámítása
		lmVector3D SpringForce = (-SpringForceMagnitude - DampingMagnitude) * ForceDirection;

		Derivative_i->IncNetForce(SpringForce);
		Derivative_j->IncNetForce(-1.0 * SpringForce);

		Derivative_i->IncNetTorque(lmCross(RotPi,SpringForce));
		Derivative_j->IncNetTorque(lmCross(RotPj,-1.0 * SpringForce));
	}
}

void lpImpulseCollisionModel::lpImpulseContact::ApplyForces(lmScalar dt)
{
	/*
	lmScalar SumPenetrationDepth = 0.0;

	for(lpCollisionData &I : CollisionData)
	{
		SumPenetrationDepth += I.PenetrationDepth;
	}
	*/

	lmScalar Weight = 1.0/CollisionData.size();

	/*
	 * Impulse
	 */

	for(lpCollisionData &I : CollisionData)
	{
		lmScalar Impulse;
		if(!I.RestingContact) //Azaz ha ütközés történi, és a két test nem csak "pihen" egymáson.
		{
			lmScalar e = 1.0;
			Impulse =	 -1.0 * (1.0 + e) * I.RelativeNormalVelocity /
						 ((Bi->GetInvMass() + Bj->GetInvMass())*lmDot(I.Normal,I.Normal) + lmDot(lmCross(biInvInertiaTensor_t * lmCross(I.Pi,I.Normal),I.Pi) + lmCross(bjInvInertiaTensor_t * lmCross(I.Pj,I.Normal),I.Pj),I.Normal));
		}
		else
		{
			/*
			 * Érdemes lehet elgondolkozni azon, hogy ezt az egyenletet használjuk a fenti helyett. Ez figyelembe veszi a
			 * két test metszésének a mélységét.
			 *
			 * Sőt, ezt fogom használni, mert így nem süllyednek egymásba a meshek.
			 * NEM mert ezt inkább akkor célszerű használni, ha a két test sebességvektora ellentétes irányú.
			 * Resting contactnek tökéletes.
			 *
			 */

			Impulse =	(I.PenetrationDepth/dt) - I.RelativeNormalVelocity /
						((Bi->GetInvMass() + Bj->GetInvMass())*lmDot(I.Normal,I.Normal) + lmDot(lmCross(biInvInertiaTensor_t * lmCross(I.Pi,I.Normal),I.Pi) + lmCross(bjInvInertiaTensor_t * lmCross(I.Pj,I.Normal),I.Pj),I.Normal));
		}
		//lmScalar Weight = I.PenetrationDepth / SumPenetrationDepth;
		lmVector3D Force = ((Impulse * Weight)/dt) * I.Normal;

		//Súrlódási erő kiszámítása - itt a sebesség a normálvektorra merőleges komponensével ellentétes irányú.
		lmVector3D FrictionDirection = I.RelativeVelocity - (I.RelativeNormalVelocity * I.Normal);
		FrictionDirection.Normalize();

		lmVector3D FrictionForce = -1.0 * CoefficientOfFriction * std::abs((Impulse * Weight)/dt) * FrictionDirection;
		Force += FrictionForce;

		/*
		 * Az erőt és a forgatónyomatékot hozzáadjuk a testre ható eredő erőhöz és forgatónyomatékhoz.
		 */

		Derivative_i->IncNetForce(Force);
		Derivative_j->IncNetForce(-1.0 * Force);

		Derivative_i->IncNetTorque(lmCross(I.Pi,Force));
		Derivative_j->IncNetTorque(lmCross(I.Pj,-1.0 * Force));
	}

	Removable = true;
}
