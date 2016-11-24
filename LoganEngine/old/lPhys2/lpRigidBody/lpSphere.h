#ifndef LP_SPHERE_H
#define LP_SPHERE_H

#include "lpRigidBody.h"

/** \brief Egy gömb alakú merev testet modellező objektum.
 * Az lpRigidBody-ból származtatott.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lpSphere : public lpRigidBody, public liSphere
{
protected:
    lmScalar Radius;/**< A gömb sugara */
public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
    lpSphere(const lpSphere &r) = delete;
    lpSphere &operator=(const lpSphere &r) = delete;

    //Konstruktor

    /** \brief Gömbnek létrehozott konstruktor - a geometriai típust automatikusan LP_SPHERE-re állítja
     *
     * \param mass lmScalar:                    A tömeg új étéke. Ha nulla, akkor a testnek végtelen lesz a tömege.
     * \param stiffness lmScalar:               A rugóállandó új értéke
     * \param damping_coefficient lmScalar:     A rugóerő csillapításának új értéke
     * \param state lpRigidBody::lpState *:     A test új állapota. (pozíció,impulzus,szögelfordulás,impulzusmomentum) Dinamikusan allokált kell, hogy legyen, felszabadításáról az lpRigidBody class gondoskodik.
     * \param radius lmScalar:               A gömb sugara
     *
     */
    lpSphere(bool ghost,lmScalar mass,liRigidBody::liState *state,lmScalar radius);

    virtual ~lpSphere() override {}

    //Getter függvények

	virtual const lmVector3D &GetPosition() const override
	{
		return State->GetPosition();
	}

	virtual void SetPosition(const lmVector3D &position) override
	{
		State->SetPosition(position);
	}

    /** \brief Getter függvény - lekérdezi a gömb sugarát - konstans tagfüggvény
     *
     * \return lmScalar - a gömb sugara
     *
     */
    virtual lmScalar GetRadius() const override;

	virtual void SetRadius(lmScalar radius) override
	{
		Radius = radius;
	}
};

#endif
