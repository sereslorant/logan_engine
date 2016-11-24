#ifndef LI_COLLISION_MODEL
#define LI_COLLISION_MODEL

#include "liRigidBody.h"

/** \brief Ennek a classnak a feladata, hogy egy általános interfészt nyújtson a különböző ütközésmodellekre.
 * Az interaction model feladata, hogy nyilvántartsa, hogy mely testek ütköznek, és meghatározza, hogy egy
 * testre milyen erők hatnak.
 *
 * \todo INTERFÉSSZÉ KELL TENNI!
 *
 * \author Seres Lóránt Gábor
 *
 */
class liInteractionModel
{
public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
	liInteractionModel(const liInteractionModel &i) = delete;
	liInteractionModel &operator=(const liInteractionModel &i) = delete;

    /** \brief Az lpContact egy bejegyzés két testről, amik ütköznek.
     *
     */
    class liConstraint
    {
	public:
        virtual bool IsRemovable() = 0;

        virtual liRigidBody *GetBody_i() = 0;
        virtual liRigidBody *GetBody_j() = 0;

        virtual void SetState_i(liRigidBody::liState *state_i) = 0;
        virtual void SetState_j(liRigidBody::liState *state_j) = 0;

        virtual void SetDerivative_i(liRigidBody::liDerivative *derivative_i) = 0;
        virtual void SetDerivative_j(liRigidBody::liDerivative *derivative_j) = 0;

        virtual void ApplyForces(lmScalar dt) = 0;

        /** \brief Konstruktor
         *
         */
        liConstraint()
        {

        }

        /** \brief Virtuális destruktor
         *
         * Nem csinál semmit
         *
         */
        virtual ~liConstraint()
        {

        }

		/*
		 * Class vége
		 */
    };

    class liContact : public liConstraint
    {
	public:

    	virtual void AddContact(const lmVector3D &intersection_i,const lmVector3D &intersection_j,const lmVector3D &normal,lmScalar penetration_depth) = 0;

        /** \brief Konstruktor
         *
         */
        liContact()
        {

        }

        /** \brief Virtuális destruktor
         *
         * Nem csinál semmit
         *
         */
        virtual ~liContact()
        {

        }

		/*
		 * Class vége
		 */
    };

    virtual const lmVector3D &GetGravAcceleration() = 0;
    virtual lmScalar GetAirDensity() = 0;
    virtual lmScalar GetCoefficientOfFriction() = 0;

    virtual void SetGravAcceleration(const lmVector3D &grav_acceleration) = 0;
    virtual void SetAirDensity(lmScalar air_density) = 0;
    virtual void SetCoefficientOfFriction(lmScalar coefficient_of_friction) = 0;

    /** \brief Az összes ütköző test között lehívja a virtuális ApplyForces() függvényt.
     *
     * \param dt lmScalar: a szimuláció lépésköze.
     * \return void
     *
     */
    virtual void ApplyConstraintForces(lmScalar dt) = 0;

    /** \brief Ez a függvény dönti el, hogy egy testre milyen "univerzális" erők hatnak. (pl. gravitáció, légellenállás, stb.) Tisztán virtuális függvény.
     *
     * \param Bi lpRigidBody &:                          Az i-edik testre mutató referencia
     * \param State_i const lpRigidBody::lpState &:      Az i-edik test állapotára mutató referencia
     * \param Derivative_i lpRigidBody::lpDerivative &:  Az i-edik test állapotának deriváltjára mutató referencia. Az erők és forgatónyomatékok ide íródnak be.
     * \return void
     *
     */
    virtual void ApplyForces(liRigidBody &Bi,const liRigidBody::liState &State_i,liRigidBody::liDerivative &Derivative_i,lmScalar dt) = 0;

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
    virtual liConstraint *AddDistConstraint(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance) = 0;

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
    virtual liConstraint *AddSpring(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance,lmScalar stiffness,lmScalar damping_coefficient) = 0;

    /** \brief Új ütközés hozzáadása a nyilvántartáshoz
     *
     * \param bi lpRigidBody &:                          Az i-edik testre mutató referencia
     * \param bj lpRigidBody &:                          A j-edik testre mutató referencia
     * \param state_i const lpRigidBody::lpState &:      Az i-edik test állapotára mutató referencia
     * \param state_j const lpRigidBody::lpState &:      A j-edik test állapotára mutató referencia
     * \param derivative_i lpRigidBody::lpDerivative &:  Az i-edik test állapotának deriváltjára mutató referencia. Az erők és forgatónyomatékok ide íródnak be.
     * \param derivative_j lpRigidBody::lpDerivative &:  A j-edik test állapotának deriváltjára mutató referencia. Az erők és forgatónyomatékok ide íródnak be.
     * \param pi lmVector3D &:                           Az i-edik test támadáspontja.
     * \param pj lmVector3D &:                           A j-edik test támadáspontja.
     * \param normal lmVector3D &:                       Az ütközés normálvektora.
     * \param penetration_depth lmScalar:                A két test metszetének mélysége.
     *
     */
    virtual liContact *AddContact(liRigidBody *bi,liRigidBody *bj,liRigidBody::liState *state_i,liRigidBody::liState *state_j,liRigidBody::liDerivative *derivative_i,liRigidBody::liDerivative *derivative_j) = 0;

    /** \brief Default konstruktor - nem csinál semmit.
     *
     */
    liInteractionModel()
    {

    }

    /** \brief Virtuális destruktor
     *
     * Törli az esetlegesen bent felejtődött ütközéseket.
     *
     */
    virtual ~liInteractionModel()
    {

    }

    /*
     * Class vége
     */
};

#endif
