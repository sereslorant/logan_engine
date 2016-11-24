#ifndef LI_SIMULATION_WORLD
#define LI_SIMULATION_WORLD

#include "liPhMesh.h"

#include "liRigidBody.h"

class liBodyFactory
{
public:

	virtual liRigidBody::liState *CreateState(const lmVector3D &position,const lmVector3D &momentum,const lmVector3D &orientation,const lmVector3D &angular_momentum,bool movable,bool rotatable) = 0;

	virtual void AddDistanceConstraint(liRigidBody *bi,liRigidBody *bj,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance) = 0;
	virtual void AddSpring(liRigidBody *bi,liRigidBody *bj,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance,lmScalar stiffness,lmScalar damping_coefficient) = 0;

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
     * \return liRigidBody * - a létrehozott testre mutató pointer. A keretprogram ezen keresztül tudja módosítani a test paramétereit/tud mindenféle dolgot csinálni vele.
     *
     */
    virtual liRigidBody *AddParticle(bool ghost,lmScalar mass,liRigidBody::liState *State) = 0;

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
    virtual liRigidBody *AddSphere(bool ghost,lmScalar mass,liRigidBody::liState *State,lmScalar radius) = 0;

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
    virtual liRigidBody *AddPlane(bool ghost,lmScalar mass,liRigidBody::liState *State,const lmVector3D &normal) = 0;

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
    virtual liRigidBody *AddMesh(bool ghost,lmScalar mass,liRigidBody::liState *state,const liPhMesh *PhMesh,bool bounding_sphere,bool quadtree,unsigned int bv_elements_size) = 0;

    virtual liRigidBody *AddBox(bool ghost,lmScalar mass,liRigidBody::liState *state,lmScalar width,lmScalar height,lmScalar depth) = 0;

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
	liBodyFactory(const liBodyFactory &s) = delete;
	liBodyFactory &operator=(const liBodyFactory &s) = delete;

	liBodyFactory(){}
	virtual ~liBodyFactory(){}
};

#include "liInteractionModel.h"

class liSimulationWorld
{
public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
	liSimulationWorld(const liSimulationWorld &s) = delete;
	liSimulationWorld &operator=(const liSimulationWorld &s) = delete;

	virtual liBodyFactory *GetBodyFactory() = 0;
	virtual liInteractionModel *GetInteractionModel() = 0;

    /** \brief Előremozdítja a világunkat az idő szerint.
     *
     * \param dt lmScalar: az időbeli lépésköz
     * \return void
     *
     */
    virtual void Simulate(lmScalar dt) = 0;

	liSimulationWorld()
	{

	}

	virtual ~liSimulationWorld()
	{

	}
};

#endif

