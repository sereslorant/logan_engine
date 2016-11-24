#ifndef LP_PLANE_H
#define LP_PLANE_H

#include "lpRigidBody.h"

/** \brief Egy végtlen síkot modellező objektum Az lpRigidBody-ból származtatott
 *
 * \author Seres Lóránt Gábor
 *
 */
class lpPlane : public lpRigidBody,public liPlane
{
protected:
    lmVector3D Normal;/**< A sík normálvektora */
public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
    lpPlane(const lpPlane &r);
    lpPlane &operator=(const lpPlane &r);

    //Konstruktor

    /** \brief Síknak létrehozott konstruktor - a geometriai típust automatikusan LP_PLANE-re állítja
     *
     * \param mass lmScalar:                    A tömeg új étéke. Ha nulla, akkor a testnek végtelen lesz a tömege.
     * \param stiffness lmScalar:               A rugóállandó új értéke
     * \param damping_coefficient lmScalar:     A rugóerő csillapításának új értéke
     * \param state lpRigidBody::lpState *:     A test új állapota. (pozíció,impulzus,szögelfordulás,impulzusmomentum) Dinamikusan allokált kell, hogy legyen, felszabadításáról az lpRigidBody class gondoskodik.
     * \param normal const lmVector3D &:        A sík normálvektora
     *
     */
    lpPlane(bool ghost,lmScalar mass,lpRigidBody::liState *state,const lmVector3D &normal);

    virtual ~lpPlane() override {}

    //Getter függvények

	virtual const lmVector3D &GetPosition() const override
	{
		return State->GetPosition();
	}

	virtual void SetPosition(const lmVector3D &position) override
	{
		State->SetPosition(position);
	}

    /** \brief Getter függvény - lekérdezi a sík normálvektorát - konstans tagfüggvény
     *
     * \return const lmVector3D & - a sík normálvektora
     *
     */
    const lmVector3D &GetNormal() const;

	virtual void SetNormal(const lmVector3D &normal) override
	{
		Normal = normal;
	}
};

#endif
