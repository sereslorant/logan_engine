#ifndef LP_IMPULSE_COLLISION_MODEL
#define LP_IMPULSE_COLLISION_MODEL

#include <iostream>

#include "lpInteractionModel.h"

/** \brief Impulzus modellt használó interaction model.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lpImpulseCollisionModel : public lpInteractionModel
{
private:

	class lpSpringDistConstraint : public lpDistConstraint
	{
	protected:
		lmScalar Stiffness;
		lmScalar DampingCoefficient;

	public:

		virtual void ApplyForces(lmScalar dt) override;

		lpSpringDistConstraint(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance,lmScalar stiffness,lmScalar damping_coefficient)
			:lpDistConstraint(bi,bj,state_i,state_j,derivative_i,derivative_j,pi,pj,distance),Stiffness(stiffness),DampingCoefficient(damping_coefficient)
		{
			//Üres függvénytörzs
		}

		virtual ~lpSpringDistConstraint() override
		{
			//Üres függvénytörzs
		}

		/*
		 * Class vége
		 */
	};

    class lpImpulseContact : public lpContact
	{
	protected:
		lmScalar CoefficientOfFriction;

	public:

		virtual void ApplyForces(lmScalar dt) override;

		lpImpulseContact(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j,lmScalar coefficient_of_friction)
			:lpContact(bi,bj,state_i,state_j,derivative_i,derivative_j),CoefficientOfFriction(coefficient_of_friction)
		{
			//Üres függvénytörzs
		}

		virtual ~lpImpulseContact() override
		{
			//Üres függvénytörzs
		}

		/*
		 * Class vége
		 */
	};

public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
    lpImpulseCollisionModel(const lpImpulseCollisionModel &i) = delete;
    lpImpulseCollisionModel &operator=(const lpImpulseCollisionModel &i) = delete;

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
    	lpSpringDistConstraint *DistConstraint = new lpSpringDistConstraint(bi,bj,state_i,state_j,derivative_i,derivative_j,pi,pj,distance,/*DistConstraint*/4000,/*DistConstraint*/100);
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
		lpImpulseContact *Contact = new lpImpulseContact(bi,bj,state_i,state_j,derivative_i,derivative_j/*,pi,pj,normal,penetration_depth,multiplier*/,CoefficientOfFriction);

		Constraints.push_back(Contact);

		return Contact;
	}

    lpImpulseCollisionModel(const lmVector3D &grav_acceleration,lmScalar air_density,lmScalar coefficient_of_friction)
		:lpInteractionModel(grav_acceleration,air_density,coefficient_of_friction)
    {
		//Üres függvénytörzs
    }

    virtual ~lpImpulseCollisionModel() override
    {
		//Üres függvénytörzs
    }

	/*
	 * Class vége
	 */
};

#endif
