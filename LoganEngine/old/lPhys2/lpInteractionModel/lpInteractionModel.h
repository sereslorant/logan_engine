#ifndef LP_COLLISION_MODEL
#define LP_COLLISION_MODEL

#include "../../lInterfaces/lPhys/liInteractionModel.h"

#include <list>

/** \brief Ennek a classnak a feladata, hogy egy általános interfészt nyújtson a különböző ütközésmodellekre.
 * Az interaction model feladata, hogy nyilvántartsa, hogy mely testek ütköznek, és meghatározza, hogy egy
 * testre milyen erők hatnak.
 *
 * \todo INTERFÉSSZÉ KELL TENNI!
 *
 * \author Seres Lóránt Gábor
 *
 */
class lpInteractionModel : public liInteractionModel
{
public:

	/** \brief Az lpContact egy bejegyzés két testről, amik ütköznek.
     *
     */
    class lpDistConstraint : public liConstraint
    {
	protected:
    	bool Removable;

        liRigidBody *Bi;/**< Az egyik testre mutató referencia */
        liRigidBody *Bj;/**< A másik testre mutató referencia */

        liRigidBody::liState *State_i;/**< Az egyik test állapota */
        liRigidBody::liState *State_j;/**< A másik test állapota */

        liRigidBody::liDerivative *Derivative_i;/**< Az egyik test állapotának deriváltjai */
        liRigidBody::liDerivative *Derivative_j;/**< A másik test állapotának deriváltjai */

        lmVector3D Pi;
        lmVector3D Pj;

        lmScalar Distance;

	public:

        virtual bool IsRemovable() override
        {
        	return Removable;
        }

        virtual liRigidBody *GetBody_i() override
        {
        	return Bi;
		}

        virtual liRigidBody *GetBody_j() override
        {
        	return Bj;
		}

        virtual void SetState_i(liRigidBody::liState *state_i) override
        {
        	State_i = state_i;
		}

        virtual void SetState_j(liRigidBody::liState *state_j) override
        {
        	State_j = state_j;
		}

        virtual void SetDerivative_i(liRigidBody::liDerivative *derivative_i) override
        {
        	Derivative_i = derivative_i;
        }

        virtual void SetDerivative_j(liRigidBody::liDerivative *derivative_j) override
        {
        	Derivative_j = derivative_j;
        }

        /** \brief Konstruktor
         *
         * \param bi lpRigidBody&:                          Az egyik testre mutató referencia
         * \param bj lpRigidBody&:                          A másik testre mutató referencia
         * \param state_i lpRigidBody::lpState&:            Az egyik test állapota
         * \param state_j lpRigidBody::lpState&:            A másik test állapota
         * \param derivative_i lpRigidBody::lpDerivative&:  Az egyik test állapotának deriváltjai
         * \param derivative_j lpRigidBody::lpDerivative&:  A másik test állapotának deriváltjai
         * \param pi lmVector3D&:                           Az egyik test támadáspontja (a tömegközéppontjához viszonyítva)
         * \param pj lmVector3D&:                           A másik test támadáspontja (a tömegközéppontjához viszonyítva)
         * \param distance lmScalar:               			A két pont távolsága
         *
         */
        lpDistConstraint(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance)
            :Bi(bi),Bj(bj),State_i(state_i),State_j(state_j),Derivative_i(derivative_i),Derivative_j(derivative_j),Pi(pi),Pj(pj),Distance(distance)
        {
        	Removable = false;
        }

        /** \brief Virtuális destruktor
         *
         * Nem csinál semmit
         *
         */
        virtual ~lpDistConstraint()
        {
        	//Üres függvénytörzs
        }

		/*
		 * Class vége
		 */
    };

    /** \brief Az lpContact egy bejegyzés két testről, amik ütköznek.
     *
     */
    class lpContact : public liContact
    {
	protected:
    	bool Removable;

        liRigidBody *Bi;/**< Az egyik testre mutató referencia */
        liRigidBody *Bj;/**< A másik testre mutató referencia */

        liRigidBody::liState *State_i;/**< Az egyik test állapota */
        liRigidBody::liState *State_j;/**< A másik test állapota */

        liRigidBody::liDerivative *Derivative_i;/**< Az egyik test állapotának deriváltjai */
        liRigidBody::liDerivative *Derivative_j;/**< A másik test állapotának deriváltjai */

		lmVector3D RelativeLinearVelocity;

		lmMatrix3x3 biInvInertiaTensor_t;
		lmMatrix3x3 bjInvInertiaTensor_t;

        struct lpCollisionData
        {
        	lmVector3D Pi;              /**< Az egyik test támadáspontja (a tömegközéppontjához viszonyítva) */
			lmVector3D Pj;              /**< A másik test támadáspontja (a tömegközéppontjához viszonyítva) */
			lmVector3D Normal;          /**< Az ütközés normálvektora */
			lmScalar PenetrationDepth;  /**< Az ütközés mélysége */

			lmVector3D RelativeVelocity;
			lmScalar RelativeNormalVelocity;
			bool RestingContact;

			//lmScalar Multiplier;

        	lpCollisionData(const lmVector3D &intersection_i,const lmVector3D &intersection_j,const lmVector3D &normal,lmScalar penetration_depth,const lmVector3D &relative_velocity,lmScalar relative_normal_velocity,bool resting_contact)
				:Pi(intersection_i),Pj(intersection_j),Normal(normal),PenetrationDepth(penetration_depth),RelativeVelocity(relative_velocity),RelativeNormalVelocity(relative_normal_velocity),RestingContact(resting_contact)
        	{
        		//Üres függvénytörzs
        	}
        };

        std::list<lpCollisionData> CollisionData;

	public:

        virtual bool IsRemovable() override
        {
        	return Removable;
        }

        virtual liRigidBody *GetBody_i() override
        {
        	return Bi;
		}

        virtual liRigidBody *GetBody_j() override
        {
        	return Bj;
		}

        virtual void SetState_i(liRigidBody::liState *state_i) override
        {
        	State_i = state_i;
		}

        virtual void SetState_j(liRigidBody::liState *state_j) override
        {
        	State_j = state_j;
		}

        virtual void SetDerivative_i(liRigidBody::liDerivative *derivative_i) override
        {
        	Derivative_i = derivative_i;
        }

        virtual void SetDerivative_j(liRigidBody::liDerivative *derivative_j) override
        {
        	Derivative_j = derivative_j;
        }

        virtual void AddContact(const lmVector3D &intersection_i,const lmVector3D &intersection_j,const lmVector3D &normal,lmScalar penetration_depth) override;

        /** \brief Konstruktor
         *
         * \param bi lpRigidBody&:                          Az egyik testre mutató referencia
         * \param bj lpRigidBody&:                          A másik testre mutató referencia
         * \param state_i lpRigidBody::lpState&:            Az egyik test állapota
         * \param state_j lpRigidBody::lpState&:            A másik test állapota
         * \param derivative_i lpRigidBody::lpDerivative&:  Az egyik test állapotának deriváltjai
         * \param derivative_j lpRigidBody::lpDerivative&:  A másik test állapotának deriváltjai
         * \param pi lmVector3D&:                           Az egyik test támadáspontja (a tömegközéppontjához viszonyítva)
         * \param pj lmVector3D&:                           A másik test támadáspontja (a tömegközéppontjához viszonyítva)
         * \param normal lmVector3D&:                       Az ütközés normálvektora
         * \param penetration_depth lmScalar:               Az ütközés mélysége
         *
         */
        lpContact(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j);

        /** \brief Virtuális destruktor
         *
         * Nem csinál semmit
         *
         */
        virtual ~lpContact() override
        {
        	//Üres függvénytörzs
        }

		/*
		 * Class vége
		 */
    };

protected:
	lmVector3D	GravAcceleration;		/**< A gravitációs gyorsulás vektor */
    lmScalar	AirDensity;				/**< A levegő sűrűsége - a légellenálláshoz kell */
    lmScalar	CoefficientOfFriction;	/**< A testek közti súrlódási együttható - NAGYON csúnya megoldás ezt itt tárolni. Ez egy adott ütközés sajátja kéne, hogy legyen. */

    std::list<liConstraint *> Constraints; /**< Itt tartjuk nyilván a az adott ciklusban detektált ütközéseket. */

public:

    virtual const lmVector3D &GetGravAcceleration() override
    {return GravAcceleration;}

    virtual lmScalar GetAirDensity() override
    {return AirDensity;}

    virtual lmScalar GetCoefficientOfFriction() override
    {return CoefficientOfFriction;}

    virtual void SetGravAcceleration(const lmVector3D &grav_acceleration) override
    {GravAcceleration = grav_acceleration;}

    virtual void SetAirDensity(lmScalar air_density) override
    {AirDensity = air_density;}

    virtual void SetCoefficientOfFriction(lmScalar coefficient_of_friction) override
    {CoefficientOfFriction = coefficient_of_friction;}

    /** \brief Az összes ütköző test között lehívja a virtuális ApplyForces() függvényt.
     *
     * \param dt lmScalar: a szimuláció lépésköze.
     * \return void
     *
     */
    virtual void ApplyConstraintForces(lmScalar dt) override;

    /** \brief Ez a függvény dönti el, hogy egy testre milyen "univerzális" erők hatnak. (pl. gravitáció, légellenállás, stb.) Tisztán virtuális függvény.
     *
     * \param Bi lpRigidBody &:                          Az i-edik testre mutató referencia
     * \param State_i const lpRigidBody::lpState &:      Az i-edik test állapotára mutató referencia
     * \param Derivative_i lpRigidBody::lpDerivative &:  Az i-edik test állapotának deriváltjára mutató referencia. Az erők és forgatónyomatékok ide íródnak be.
     * \return void
     *
     */
    virtual void ApplyForces(liRigidBody &Bi,const liRigidBody::liState &State_i,liRigidBody::liDerivative &Derivative_i,lmScalar dt) override;

    /** \brief Default konstruktor - nem csinál semmit.
     *
     */
    lpInteractionModel(const lmVector3D &grav_acceleration,lmScalar air_density,lmScalar coefficient_of_friction);

    /** \brief Virtuális destruktor
     *
     * Törli az esetlegesen bent felejtődött ütközéseket.
     *
     */
    virtual ~lpInteractionModel() override;

    /*
     * Class vége
	 */
};

#endif // LP_COLLISION_MODEL
