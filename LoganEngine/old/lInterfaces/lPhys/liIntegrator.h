#ifndef LP_INTEGRATOR
#define LP_INTEGRATOR

/* 8================================D
 * TMP!!!!!!!!!!!!!!!!!!!!!
 * 8================================D
 */
#include "liRigidBody.h"
/* 8================================D
 * TMP!!!!!!!!!!!!!!!!!!!!!
 * 8================================D
 */


/** \brief Az integrátor class absztrakt ősosztálya. Az integrátor felelős azért, hogy
 * numerikus integrálással a szimuláció elemeinek állapotát előre mozdítsa az időben.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liIntegrator
{
public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
	liIntegrator(const liIntegrator &i) = delete;
	liIntegrator &operator=(const liIntegrator &i) = delete;

	virtual void AddDistanceConstraint(liRigidBody *bi,liRigidBody *bj,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance) = 0;
	virtual void AddSpring(liRigidBody *bi,liRigidBody *bj,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance,lmScalar stiffness,lmScalar damping_coefficient) = 0;

    /** \brief Default konstruktor, nem csinál semmit.
     */
    liIntegrator()
    {

    }

    /** \brief Virtuális destruktor, nem csinál semmit.
     */
    virtual ~liIntegrator()
    {

    }

    /** \brief Új test hozzáadása a szimulációhoz
     *
     * \param Body lpRigidBody &: a hozzáadandó test
     * \return void
     *
     */
    virtual void AddBody(liRigidBody &Body) = 0;

    /** \brief A szimuláció előreléptetése
     *
     * \param dt lmScalar: az időbeli lépésköz
     * \return void
     *
     */
    virtual void Step(lmScalar dt) = 0;
};

#include "liSimulationWorld.h"

#endif
