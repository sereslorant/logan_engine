#ifndef LP_SIMULATION_WORLD
#define LP_SIMULATION_WORLD

#include <iostream>
#include <list>


#include "../lInterfaces/lPhys/liSimulationWorld.h"

#include "lpIntegrator/liPhys2Integrator.h"

/** \brief A szimulációnk világát modellező objektum.
 *
 * Tartalmazza az összes szimulációhoz hozzáadott testet.
 *
 * A Simulate tagfüggvény minden ciklusban lehívja az integrátor Step(lmScalar dt) függvényét,
 * ami az adott dt lépésközzel leellenőrzi az objektumokra ható erőket, és ezek ismeretében
 * numerikusan integrálja őket az idő szerint.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lpSimulationWorld : public liPhys2SimulationWorld,public liBodyFactory
{
private:
    std::list<lpRigidBody *> Bodies;  /**< A testeket tartalmazó tároló
                                             TODO:  lehet, hogy ki kéne majd cserélni egy fagráf-szerű tárolóra,
                                                    hogy több merev testből összeálló pontrendszert is lehessen
                                                    vele modellezni.
                                                    (pl. autót, aminek vannak kerekei, amik az autóval együtt mozognak, stb.)
                                                    (VAGY Magát az lpRigidBodyt átalakítani úgy, hogy maga legyen a gráf csúcsa,
													a tároló meg legyen szimpla láncolt lista.)
                                        */
    liPhys2Integrator *Integrator;

public:

    virtual void RemoveBody(const liRigidBody *Body) override
    {
    	for(std::list<lpRigidBody *>::iterator I = Bodies.begin();I != Bodies.end();I++)
		{
			if(*I == Body)
			{
				delete *I;

				Bodies.erase(I);
				break;
			}
		}
    }

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
    lpSimulationWorld(const lpSimulationWorld &s) = delete;
    lpSimulationWorld &operator=(const lpSimulationWorld &s) = delete;

    //Konstruktor, destruktor

    /** \brief Konstruktor
     *
     * \param integrator lpIntegrator*: A szimulációban használt integrátor
     *
     */
    lpSimulationWorld(liIntegrator *integrator);

    /**< Destruktor */
    virtual ~lpSimulationWorld() override;

    //Interfész, amivel a keretprogram vezérelni tudja a szimulációt

	virtual liRigidBody::liState *CreateState(const lmVector3D &position,const lmVector3D &momentum,const lmVector3D &orientation,const lmVector3D &angular_momentum,bool movable,bool rotatable) override
	{
		return new lpRigidBody::lpState(position,momentum,orientation,angular_momentum,movable,rotatable);
	}

	virtual void AddDistanceConstraint(liRigidBody *bi,liRigidBody *bj,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance) override
	{
		Integrator->AddDistanceConstraint(bi,bj,pi,pj,distance);
	}

	virtual void AddSpring(liRigidBody *bi,liRigidBody *bj,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance,lmScalar stiffness,lmScalar damping_coefficient) override
	{
		Integrator->AddSpring(bi,bj,pi,pj,distance,stiffness,damping_coefficient);
	}

    /** \brief Létrehoz egy pontszerű testet, és hozzáadja a szimulációnk világához.
     *
     * VIGYÁZAT! A szimulációnk világa még nincs felkészítve kiterjedés nélküli pontszerű
     * testekkel való ütközésellenőrzésre!
     * Nem szabad a világ jelenlegi formájában ilyet hozzáadni
     * a világhoz, mert minden falon/objektumon keresztül fog esni!
     *
     * Az lpCheckCollision függvény felelős az ütközésellenőrzésért. Ha ilyet szeretnénk hozzáadni a világhoz,
     * akkor itt le kell kódolni az ütközésellenőrzést.
     *
     * \param mass lmScalar:                a létrehozott testünk tömege.
     * \param stiffness lmScalar:           a létrehozott testünk rugóállandója.
     * \param damping_coefficient lmScalar: a létrehozott testünk rugóerő-csillapítása.
     * \param State lpRigidBody::lpState *:  a létrehozott testünk állapota. Dinamikusan allokált, a test felszabadulásakor ő is felszabadul.
     *
     * \return lpRigidBody * - a létrehozott testre mutató pointer. A keretprogram ezen keresztül tudja módosítani a test paramétereit/tud mindenféle dolgot csinálni vele.
     *
     */
    virtual liRigidBody *AddParticle(bool ghost,lmScalar mass,lpRigidBody::liState *State) override;

    /** \brief Létrehoz egy gömböt, és hozzáadja a szimulációnk világához.
     *
     * \param mass lmScalar:                a létrehozott gömbünk tömege.
     * \param stiffness lmScalar:           a létrehozott gömbünk rugóállandója.
     * \param damping_coefficient lmScalar: a létrehozott gömbünk rugóerő-csillapítása.
     * \param State lpRigidBody::lpState *: a létrehozott testünk állapota. Dinamikusan allokált, a test felszabadulásakor ő is felszabadul.
     * \param radius lmScalar:              a létrehozott gömbünk sugara.
     *
     * \return liRigidBody * - a létrehozott gömbre mutató pointer. A keretprogram ezen keresztül tudja módosítani a test paramétereit/tud mindenféle dolgot csinálni vele.
     *
     */
    virtual liRigidBody *AddSphere(bool ghost,lmScalar mass,lpRigidBody::liState *State,lmScalar radius) override;

    /** \brief Létrehoz egy végtelen síkot, és hozzáadja a szimulációnk világához.
     *
     * \param mass lmScalar:                a létrehozott sík tömege.
     * \param stiffness lmScalar:           a létrehozott sík rugóállandója.
     * \param damping_coefficient lmScalar: a létrehozott sík rugóerő-csillapítása.
     * \param State lpRigidBody::lpState *: a létrehozott testünk állapota. Dinamikusan allokált, a test felszabadulásakor ő is felszabadul.
     * \param normal const lmVector3D &:    a létrehozott sík normálvektora.
     *
     * \return liRigidBody * - a létrehozott síkra mutató pointer. A keretprogram ezen keresztül tudja módosítani a test paramétereit/tud mindenféle dolgot csinálni vele.
     *
     */
    virtual liRigidBody *AddPlane(bool ghost,lmScalar mass,lpRigidBody::liState *State,const lmVector3D &normal) override;

    /** \brief Létrehoz egy triangle mesht, és hozzáadja a szimulációnk világához.
     *
     * \param mass lmScalar:                a létrehozott sík tömege.
     * \param stiffness lmScalar:           a létrehozott sík rugóállandója.
     * \param damping_coefficient lmScalar: a létrehozott sík rugóerő-csillapítása.
     * \param state lpRigidBody::lpState *: a létrehozott testünk állapota. Dinamikusan allokált, a test felszabadulásakor ő is felszabadul.
     * \param PhMesh const lrmPhMesh *:     a létrehozott meshünk háromszöghálója.
     *
     * \return liRigidBody * - a létrehozott meshre mutató pointer. A keretprogram ezen keresztül tudja módosítani a test paramétereit/tud mindenféle dolgot csinálni vele.
     *
     */
    virtual liRigidBody *AddMesh(bool ghost,lmScalar mass,lpRigidBody::liState *state,const liPhMesh *PhMesh,bool bounding_sphere,bool quadtree,unsigned int bv_elements_size) override;

    virtual liRigidBody *AddBox(bool ghost,lmScalar mass,liRigidBody::liState *state,lmScalar width,lmScalar height,lmScalar depth) override;

	virtual liBodyFactory *GetBodyFactory() override
	{
		return this;
	}

	virtual liInteractionModel *GetInteractionModel() override
	{
		return Integrator->GetInteractionModel();
	}

    /** \brief Előremozdítja a világunkat az idő szerint.
     *
     * \param dt lmScalar: az időbeli lépésköz
     * \return void
     *
     */
    virtual void Simulate(lmScalar dt) override;
};

#endif
