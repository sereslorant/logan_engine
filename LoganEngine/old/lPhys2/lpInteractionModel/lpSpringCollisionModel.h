#ifndef LP_SPRING_COLLISION_MODEL
#define LP_SPRING_COLLISION_MODEL

#include "lpInteractionModel.h"
#include "../../lInterfaces/lPhys/liCollisionDetector.h"

/** \brief Olyan ütközésmodell, amely a testek közötti erőt egy rugóerővel modellezi, amihez hozzájárul egy sebességtől függő csillapítás.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lpSpringCollisionModel : public lpInteractionModel
{
private:
    lmScalar Stiffness;
    lmScalar DampingCoefficient;

    //lmScalar DistConstraintStiffness;
    //lmScalar DistConstraintDampingCoefficient;

    class lpSpringDistConstraint : public lpDistConstraint
	{
	protected:
		lmScalar Stiffness;
		lmScalar DampingCoefficient;

	public:

		virtual void ApplyForces(lmScalar dt) override
		{
			//Elkérjük a Bi és a Bj test pillanatnyi sebességét.
			lmVector3D biVelocity = State_i->GetMomentum() * Bi->GetInvMass();
			lmVector3D bjVelocity = State_j->GetMomentum() * Bj->GetInvMass();

			//Kiszámítjuk a Bj testnek a Bi-éhez viszonyított relatív sebességét
			lmVector3D RelativeLinearVelocity  = biVelocity - bjVelocity;

			lmMatrix3x3 biRotation = lmRotate3x3(State_i->GetOrientationQuaternion());
			lmMatrix3x3 bjRotation = lmRotate3x3(State_j->GetOrientationQuaternion());

			lmMatrix3x3 biInvInertiaTensor_t = biRotation * Bi->GetInvInertiaTensor() * biRotation.Transpose();
			lmMatrix3x3 bjInvInertiaTensor_t = bjRotation * Bj->GetInvInertiaTensor() * bjRotation.Transpose();

			//Elkérjük a Bi test pillanatnyi szögsebességét.
			lmVector3D  biAngularVelocity = biInvInertiaTensor_t * State_i->GetAngularMomentum();
			//Elkérjük a Bj test pillanatnyi szögsebességét.
			lmVector3D	bjAngularVelocity = bjInvInertiaTensor_t * State_j->GetAngularMomentum();

			//Kiszámoljuk a Bj test Bi-éhez viszonyított relatív szögsebességét a támadáspontban a v = omega x r képlettel.
			lmVector3D RelativeAngularVelocity = lmCross(biAngularVelocity,Pi) - lmCross(bjAngularVelocity,Pj);

			//A támadáspontban a relatív sebesség:
			lmVector3D RelativeVelocity        = RelativeLinearVelocity + RelativeAngularVelocity;

			lmVector3D ForceDirection = (biRotation * Pi + State_i->GetPosition()) - (biRotation * Pj + State_j->GetPosition());

			lmScalar Distance_t = ForceDirection.LengthSquared();
			if(std::abs(Distance_t - (Distance * Distance)) > 1e-8)
			{
				Distance_t = (std::sqrt(Distance_t) - Distance);
				ForceDirection.Normalize();

				lmScalar   RelativeNormalVelocity = lmDot(ForceDirection,RelativeVelocity);

				//Rugóerő kiszámítása
				lmVector3D SpringForce	= -1.0f * Stiffness * Distance_t * ForceDirection;
				lmVector3D Damping		= DampingCoefficient * RelativeNormalVelocity * ForceDirection;

				if(Distance_t > 0.0)
				{
					Damping *= -1.0;
				}

				lmVector3D Force = SpringForce + Damping;

				Derivative_i->IncNetForce(Force);
				Derivative_j->IncNetForce(-1.0f * Force);

				Derivative_i->IncNetTorque(lmCross(Pi,Force));
				Derivative_j->IncNetTorque(lmCross(Pj,-1.0f * Force));
			}
		}

		lpSpringDistConstraint(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance,lmScalar stiffness,lmScalar damping_coefficient)
			:lpDistConstraint(bi,bj,state_i,state_j,derivative_i,derivative_j,pi,pj,distance),Stiffness(stiffness),DampingCoefficient(damping_coefficient)
		{

		}

		virtual ~lpSpringDistConstraint() override
		{

		}

		/*
		 * Class vége
		 */
	};

	class lpSpringContact : public lpContact
	{
	protected:
		lmScalar CoefficientOfFriction;

		lmScalar Stiffness;
		lmScalar DampingCoefficient;

	public:

		virtual void ApplyForces(lmScalar dt) override
		{
			lmScalar SumPenetrationDepth = 0.0;

			for(lpCollisionData &I : CollisionData)
			{
				SumPenetrationDepth += I.PenetrationDepth;
			}

			for(lpCollisionData &I : CollisionData)
			{
				/* Ütközés során fellépő erők modellezése Penalty-féle módszerrel.
				 *
				 * A modell lényege, hogy a két test között fellépő erőt egy rugóerővel modellezzük.
				 * Mivel a sima rugóerő esetén a test vagy szimplán csak pattogna, vagy egyre nagyobbat pattogna,
				 * ezért a rugóerőt egy sebességtől függő, ellentétes irányú erővel csillapítjuk.
				 *
				 * Itt a két együttható egyszerűen csak a két testben eltárolt együtthatók összege.
				 */

				//Rugóerő kiszámítása
				lmVector3D SpringForce = Stiffness * I.Normal * I.PenetrationDepth;
				//Rugóerő-csillapítás kiszámítása
				lmVector3D Damping = I.Normal * (DampingCoefficient * I.RelativeNormalVelocity);

				lmVector3D Force = SpringForce - Damping;
				lmScalar Weight = I.PenetrationDepth / SumPenetrationDepth;
				Force *= Weight;

				//Súrlódási erő kiszámítása - itt a sebesség a normálvektorra merőleges komponensével ellentétes irányú.
				lmVector3D FrictionDirection = I.RelativeVelocity - (I.RelativeNormalVelocity * I.Normal);
				FrictionDirection.Normalize();

				lmVector3D FrictionForce = -CoefficientOfFriction * std::abs(lmDot(Force,I.Normal)) * FrictionDirection;

				/* Az erőt és a forgatónyomatékot hozzáadjuk a testre ható eredő erőkhöz és forgatónyomatékokhoz.
				 */
				Force += FrictionForce;

				Derivative_i->IncNetForce(Force);
				Derivative_j->IncNetForce(-1.0f * Force);

				Derivative_i->IncNetTorque(lmCross(I.Pi,Force));
				Derivative_j->IncNetTorque(lmCross(I.Pj,-1.0f * Force));
			}

			Removable = true;
		}

		lpSpringContact(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j,
						/*const lmVector3D &pi,const lmVector3D &pj,lmVector3D &normal,lmScalar penetration_depth,lmScalar multiplier,*/
						lmScalar coefficient_of_friction,lmScalar stiffness,lmScalar damping_coefficient)
			:lpContact(bi,bj,state_i,state_j,derivative_i,derivative_j/*,pi,pj,normal,penetration_depth,multiplier*/),CoefficientOfFriction(coefficient_of_friction),Stiffness(stiffness),DampingCoefficient(damping_coefficient)
		{

		}

		/*
		 * Class vége
		 */
	};

public:
    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
    lpSpringCollisionModel(const lpSpringCollisionModel &i) = delete;
    lpSpringCollisionModel &operator=(const lpSpringCollisionModel &i) = delete;

    /** \brief Új távolság constraint hozzáadása a nyilvántartáshoz
     *
     * \param bi lpRigidBody &:                          Az i-edik testre mutató referencia
     * \param bj lpRigidBody &:                          A j-edik testre mutató referencia
     * \param state_i const lpRigidBody::lpState &:      Az i-edik test állapotára mutató referencia
     * \param state_j const lpRigidBody::lpState &:      A j-edik test állapotára mutató referencia
     * \param derivative_i lpRigidBody::lpDerivative &:  Az i-edik test állapotának deriváltjára mutató referencia. Az erők és forgatónyomatékok ide íródnak be.
     * \param derivative_j lpRigidBody::lpDerivative &:  A j-edik test állapotának deriváltjára mutató referencia. Az erők és forgatónyomatékok ide íródnak be.
     * \param pi lmVector3D &:
     * \param pj lmVector3D &:
     * \param distance lmScalar:
     *
     */
    virtual liConstraint *AddDistConstraint(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance) override
    {
    	lpSpringDistConstraint *DistConstraint = new lpSpringDistConstraint(bi,bj,state_i,state_j,derivative_i,derivative_j,pi,pj,distance,/*DistConstraint*/Stiffness,/*DistConstraint*/DampingCoefficient);

		Constraints.push_back(DistConstraint);

		return DistConstraint;
    }

    /** \brief Új távolság constraint hozzáadása a nyilvántartáshoz
     *
     * \param bi lpRigidBody &:                          Az i-edik testre mutató referencia
     * \param bj lpRigidBody &:                          A j-edik testre mutató referencia
     * \param state_i const lpRigidBody::lpState &:      Az i-edik test állapotára mutató referencia
     * \param state_j const lpRigidBody::lpState &:      A j-edik test állapotára mutató referencia
     * \param derivative_i lpRigidBody::lpDerivative &:  Az i-edik test állapotának deriváltjára mutató referencia. Az erők és forgatónyomatékok ide íródnak be.
     * \param derivative_j lpRigidBody::lpDerivative &:  A j-edik test állapotának deriváltjára mutató referencia. Az erők és forgatónyomatékok ide íródnak be.
     * \param pi lmVector3D &:
     * \param pj lmVector3D &:
     * \param distance lmScalar:
     *
     */
    virtual liConstraint *AddSpring(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance,lmScalar stiffness,lmScalar damping_coefficient) override
    {
    	lpSpringDistConstraint *DistConstraint = new lpSpringDistConstraint(bi,bj,state_i,state_j,derivative_i,derivative_j,pi,pj,distance,stiffness,damping_coefficient);

		Constraints.push_back(DistConstraint);

		return DistConstraint;
    }

	virtual liContact *AddContact(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j) override
	{
		lpSpringContact *Contact = new lpSpringContact(bi,bj,state_i,state_j,derivative_i,derivative_j,CoefficientOfFriction,Stiffness,DampingCoefficient);

		Constraints.push_back(Contact);

		return Contact;
	}

    /** \brief Konstruktor, amely beállítja az összes adatot az ütközésmodellben.
     *
     * \param grav_acceleration const lmVector3D &:  A gravitációs gyorsulás vektor
     * \param air_density lmScalar:                  A levegő sűrűsége.
     * \param coefficient_of_friction lmScalar:      A két test közti súrlódási együttható. Csúnya dolog ezt itt tárolni, az ütközés saját adata kéne, hogy legyen.
     *
     */
    lpSpringCollisionModel(const lmVector3D &grav_acceleration,lmScalar air_density,lmScalar coefficient_of_friction,lmScalar stiffness,lmScalar damping_coefficient)
        :lpInteractionModel(grav_acceleration,air_density,coefficient_of_friction),Stiffness(stiffness),DampingCoefficient(damping_coefficient)
    {

    }

    /** \brief Destruktor
     *
     */
    virtual ~lpSpringCollisionModel() override
    {

    }

    /*
     * Class vége
	 */
};

#endif
