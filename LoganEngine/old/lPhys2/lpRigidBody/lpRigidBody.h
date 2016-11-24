#ifndef LP_RIGID_BODY
#define LP_RIGID_BODY

#include "../../lMath/lMath.h"

#include "../../lInterfaces/lPhys/liRigidBody.h"

#include <iostream>

/** \brief Egy merev testet modellező objektum
 *
 * \author Seres Lóránt Gábor
 *
 */
class lpRigidBody : public liRigidBody
{
public:
    //Publikus adattípusok/struktúrák deklarációja.

    /** \brief A test geometriai típusa
     */
    //enum lpType
    //{
    //    LP_PARTICLE,/**< Pontszerű test */
    //    LP_SPHERE,/**< Gömb */
    //    LP_PLANE,/**< Sík */
    //    LP_MESH,
    //};

    /** \brief A test deriváltjait tartalmazó adatstruktúra, amelyből a következő ciklusbeli állapotot integráljuk.
     *
     * \author Seres Lóránt Gábor
     *
     */
    struct lpDerivative : public liDerivative
    {
        lmVector3D Momentum;/**< Impulzus*/
        lmVector3D NetForce;/**< Eredő erő*/

        lmVector3D AngularMomentum;/**< Impulzusmomentum*/
        lmVector3D NetTorque;/**< Forgatónyomaték*/

        virtual const lmVector3D &GetMomentum() const {return Momentum;}
        virtual const lmVector3D &GetNetForce() const {return NetForce;}
        virtual const lmVector3D &GetAngularMomentum() const {return AngularMomentum;}
        virtual const lmVector3D &GetNetTorque() const {return NetTorque;}


        virtual void SetMomentum(const lmVector3D &momentum) {Momentum = momentum;}
		virtual void SetNetForce(const lmVector3D &net_force) {NetForce = net_force;}
		virtual void SetAngularMomentum(const lmVector3D &angular_momentum) {AngularMomentum = angular_momentum;}
		virtual void SetNetTorque(const lmVector3D &net_torque) {NetTorque = net_torque;}


        virtual void IncMomentum(const lmVector3D &d_momentum) {Momentum += d_momentum;}
		virtual void IncNetForce(const lmVector3D &d_net_force) {NetForce += d_net_force;}
		virtual void IncAngularMomentum(const lmVector3D &d_angular_momentum) {AngularMomentum += d_angular_momentum;}
		virtual void IncNetTorque(const lmVector3D &d_net_torque) {NetTorque += d_net_torque;}

        /** \brief Default konstruktor, ami nullával tölti fel az adatokat.
         *
         */
        lpDerivative()
        {
			//Üres függvénytörzs
        }
    };

    /** \brief A test pillanatnyi állapotát leíró adatstruktúra
     *
     * \author Seres Lóránt Gábor
     *
     */
    struct lpState : public liState
    {
        lmVector3D Position;/**< Pozíció*/
        lmVector3D Momentum;/**< Impulzus*/

        lmVector3D Orientation;/**< Szögelfordulás*/
        lmVector3D AngularMomentum;/**< Impulzusmomentum*/

    	bool Movable;
    	bool Rotatable;

        virtual void SetPosition(const lmVector3D &position) override
        {
        	Position = position;
		}

    	virtual void SetMomentum(const lmVector3D &momentum) override
        {
        	Momentum = momentum;
		}

    	virtual void SetOrientation(const lmVector3D &orientation) override
        {
        	Orientation = orientation;
		}
		/*
		virtual void SetOrientation(const lmQuaternion &orientation) override
		{
			double q_Angle		= acos(lmClamp(-1.0,1.0,orientation.W));
			double q_SinAngle	= sin(q_Angle);

			lmVector3D q_axis;
			if(std::abs(q_SinAngle) > 1e-8)
			{
				q_axis = lmVector3D(orientation.X / q_SinAngle,
									orientation.Y / q_SinAngle,
									orientation.Z / q_SinAngle
									);
			}

			Orientation = 2.0 * q_Angle * q_axis;
		}
		*/

    	virtual void SetAngularMomentum(const lmVector3D &angular_momentum) override
        {
        	AngularMomentum = angular_momentum;
		}

		virtual void SetMovable(bool movable) override
		{
			Movable = movable;
		}

        virtual void SetRotatable(bool rotatable) override
        {
        	Rotatable = rotatable;
        }

    	virtual void IncPosition(const lmVector3D &delta_position) override
        {
        	Position += delta_position;
		}

    	virtual void IncMomentum(const lmVector3D &delta_momentum) override
        {
        	Momentum += delta_momentum;
		}

    	virtual void IncOrientation(const lmVector3D &delta_orientation) override
        {
        	//std::cout << std::endl << Orientation[0] << ";" << Orientation[1] << ";" << Orientation[2] << std::endl;
        	//std::cout << delta_orientation[0] << ";" << delta_orientation[1] << ";" << delta_orientation[2] << std::endl;

        	Orientation += delta_orientation;
        	//std::cout << Orientation[0] << ";" << Orientation[1] << ";" << Orientation[2] << std::endl << std::endl;
		}

    	virtual void IncAngularMomentum(const lmVector3D &delta_angular_momentum) override
        {
        	//std::cout << std::endl << AngularMomentum[0] << ";" << AngularMomentum[1] << ";" << AngularMomentum[2] << std::endl;
        	//std::cout << delta_angular_momentum[0] << ";" << delta_angular_momentum[1] << ";" << delta_angular_momentum[2] << std::endl;

        	AngularMomentum += delta_angular_momentum;

        	//std::cout << AngularMomentum[0] << ";" << AngularMomentum[1] << ";" << AngularMomentum[2] << std::endl << std::endl;
		}

        virtual const lmVector3D &GetPosition() const override
        {
        	return Position;
        }

        virtual const lmVector3D &GetMomentum() const override
        {
        	return Momentum;
        }

        virtual const lmVector3D &GetOrientation() const override
        {
        	return Orientation;
        }

        virtual const lmVector3D &GetAngularMomentum() const override
        {
        	return AngularMomentum;
        }

        virtual bool IsMovable() const override
        {
        	return Movable;
        }

		virtual bool IsRotatable() const override
		{
			return Rotatable;
		}

        /** \brief Kvaternió, amelyből ki lehet számolni a test elfordulását megadó mátrixot.
         *
         * \return lmQuaternion - a test elfordulását megadó kvaternió
         *
         */
        virtual lmQuaternion GetOrientationQuaternion() const override
        {
            lmVector3D RotationAxis = Orientation;
            lmScalar Rotation = std::sqrt(RotationAxis.LengthSquared());
            RotationAxis.Normalize();

            return lmQuaternion(RotationAxis,Rotation);
        }

        /** \brief Default konstruktor, ami nullával tölti fel az adatokat.
         *
         */
        lpState()
			:Rotatable(true)
        {
			//Üres függvénytörzs
        }

        /** \brief Konstruktor, amely feltölti az adatstruktúra minden adattagját.
         *
         * \param position const lmVector3D &:         Új pozíció.
         * \param momentum const lmVector3D &:         Új impulzus.
         * \param orientation const lmVector3D &:      Új szögelfordulás.
         * \param angular_momentum const lmVector3D &: Új impulzusmomentum.
         *
         */
        lpState(const lmVector3D &position,const lmVector3D &momentum,const lmVector3D &orientation,const lmVector3D &angular_momentum,bool movable,bool rotatable)
            :Position(position),Momentum(momentum),Orientation(orientation),AngularMomentum(angular_momentum),Movable(movable),Rotatable(rotatable)
		{
			//Üres függvénytörzs
		}

		virtual ~lpState() override
		{
			//Üres függvénytörzs
		}
    };

    //struct lpMaterialdata;
protected:

	bool Removable = false;

    const lpType Type;/**< A test geometriai típusa */
    bool Ghost;

    lmScalar Mass;/**< A test tömege */

    lmMatrix3x3 InertiaTensor;
    lmMatrix3x3 InvInertiaTensor;

    liState *State;/**< A test pillanatnyi állapotát leíró adatstruktúra */

    std::vector<liCollisionCallback *>CollisionCallbacks;
    void *UserData = nullptr;

    /** \brief Származtatott osztáyoknak fenntartott konstruktor - lényege az, hogy a származtatott osztály beállíthatja a test geometriai típusát.
     *
     * \param mass lmScalar:                    A tömeg új étéke. Ha nulla, akkor a testnek végtelen lesz a tömege.
     * \param stiffness lmScalar:               A rugóállandó új értéke
     * \param damping_coefficient lmScalar:     A rugóerő csillapításának új értéke
     * \param state lpRigidBody::lpState *:     A test új állapota. (pozíció,impulzus,szögelfordulás,impulzusmomentum) Dinamikusan allokált kell, hogy legyen, felszabadításáról az lpRigidBody class gondoskodik.
     * \param type lpRigidBody::lpType:         A test geometriai típusa
     *
     */
    lpRigidBody(bool ghost,lmScalar mass,lpRigidBody::liState *state,lpType type);
public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
    lpRigidBody(const lpRigidBody &r) = delete;
    lpRigidBody &operator=(const lpRigidBody &r) = delete;

    //Konstruktor, destruktor.

    /** \brief Pontszerű, kiterjedés nélküli testnek létrehozott konstruktor - a geometriai típust automatikusan LP_PARTICLE-re állítja
     *
     * \param mass lmScalar:                    A tömeg új étéke. Ha nulla, akkor a testnek végtelen lesz a tömege.
     * \param stiffness lmScalar:               A rugóállandó új értéke
     * \param damping_coefficient lmScalar:     A rugóerő csillapításának új értéke
     * \param state lpRigidBody::lpState *:     A test új állapota. (pozíció,impulzus,szögelfordulás,impulzusmomentum) Dinamikusan allokált kell, hogy legyen, felszabadításáról az lpRigidBody class gondoskodik.
     *
     */
    lpRigidBody(bool ghost,lmScalar mass,lpRigidBody::liState *state);

    /** \brief Virtuális destruktor
     *
     * Felszabadítja a test dinamikusan allokált állapotát
     *
     */
    virtual ~lpRigidBody() override;

    virtual bool IsRemovable() override
    {
    	return Removable;
    }

    virtual void MarkRemovable() override
    {
    	Removable = true;
    }

    virtual void AddCollisionCallback(liCollisionCallback *collision_callback) override
    {
    	CollisionCallbacks.push_back(collision_callback);
    }

    virtual void CollisionCallback(const liRigidBody *other_body,const liState *other_state,const lmVector3D &intersection,unsigned int collision_data_index) override
    {
    	for(liCollisionCallback *ColCallback : CollisionCallbacks)
		{
			ColCallback->Callback(other_body,other_state,intersection,collision_data_index);
		}
    }

    virtual void* GetUserData() override
    {
    	return UserData;
    }

    virtual void SetUserData(void *user_data) override
    {
    	UserData = user_data;
    }

    //Getter függvények

    /** \brief Getter függvény - lekérdezi a test típusát - konstans tagfüggvény
     *
     * \return lpRigidBody::lpType - a test típusa
     *
     */
    virtual lpType GetType() const override;

    virtual void SetGhost(bool ghost) override
    {
    	Ghost = ghost;
	}

    virtual bool IsGhost() const override
    {
    	return Ghost;
	}

    /** \brief Getter függvény - lekérdezi a test tömegét - konstans tagfüggvény
     *
     * \return lmScalar - a test tömege
     *
     */
    virtual lmScalar GetMass() const override;

    /** \brief Getter függvény - lekérdezi a test tömegének reciprokát - konstans tagfüggvény
     *
     * \return lmScalar - a test tömegének reciproka
     *
     */
    virtual lmScalar GetInvMass() const override;

    /** \brief Getter függvény - lekérdezi a test tehetetlenségi tenzorát - konstans tagfüggvény
     *
     * \return const lmMatrix3x3 & - a test tehetetlenségi tenzora. Ha a test tömege "végtelen" vagy nulla, akkor nullmátrixszal tér vissza.
     *
     */
    const lmMatrix3x3 &GetInertiaTensor() const;

    /** \brief Getter függvény - lekérdezi a test tehetetlenségi tenzorának inverzét - konstans tagfüggvény
     *
     * \return const lmMatrix3x3 & - a test tehetetlenségi tenzorának inverze. Ha a test tömege "végtelen" vagy nulla, akkor nullmátrixszal tér vissza.
     *
     */
    virtual const lmMatrix3x3 &GetInvInertiaTensor() const override;

    /** \brief Getter függvény - lekérdezi a test pillanatnyi állapotát leíró adatstruktúrát
     *
     * \return lpState & - a test pillanatnyi állapotát leíró adatstruktúra
     *
     */
    virtual liState *GetState() override;

    /** \brief Getter függvény - lekérdezi a test pillanatnyi állapotát tartalmazó adatstruktúrát - konstans tagfüggvény
     *
     * \return const lpState & - a test pillanatnyi állapotát tartalmazó adatstruktúra
     *
     */
    virtual const liState *GetState() const override;

    /** \brief Getter függvény - lekérdezi a test pozícióját - konstans tagfüggvény
     *
     * \return const lmVector3D & - a test pozíciója
     *
     */
    virtual const lmVector3D &GetPosition() const override;

    /** \brief Getter függvény - lekérdezi a test impulzusát - konstans tagfüggvény
     *
     * \return const lmVector3D & - a test impulzusa
     *
     */
    virtual const lmVector3D &GetMomentum() const override;

    /** \brief Getter függvény - lekérdezi a test szögelfordulását - konstans tagfüggvény
     *
     * \return lmScalar - a test szögelfordulását
     *
     */
    virtual const lmVector3D &GetOrientation() const override;

    /** \brief Getter függvény - lekérdezi a test impulzusmomentumát - konstans tagfüggvény
     *
     * \return lmScalar - a test impulzusmomentuma
     *
     */
    virtual const lmVector3D &GetAngularMomentum() const override;

    virtual bool HasBoundingVolume() override {return false;}
    virtual liBoundingVolume *GetBoundingVolume() override {return nullptr;}
};

#endif
