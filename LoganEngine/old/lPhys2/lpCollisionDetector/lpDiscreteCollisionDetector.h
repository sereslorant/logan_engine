#ifndef LP_DISCRETE_COLLISION_DETECTOR
#define LP_DISCRETE_COLLISION_DETECTOR

#include "../../lInterfaces/lPhys/liCollisionDetector.h"

#include "../lpRigidBody/lpRigidBody.h"
#include "../lpRigidBody/lpSphere.h"
#include "../lpRigidBody/lpPlane.h"
#include "../lpRigidBody/lpMesh.h"
#include "../lpRigidBody/lpBox.h"

struct lpCollisionData
{
	lmVector3D	Intersection1;
	lmVector3D	Intersection2;
	lmVector3D	Normal;
	lmScalar	PenetrationDepth;

	lpCollisionData(const lmVector3D &intersection1,const lmVector3D &intersection2,const lmVector3D &normal,lmScalar penetration_depth)
		:Intersection1(intersection1),Intersection2(intersection2),Normal(normal),PenetrationDepth(penetration_depth)
	{}
};

struct lpContactData
{
	liRigidBody *Body1 = nullptr;
	liRigidBody *Body2 = nullptr;
	liRigidBody::liState *State1 = nullptr;
	liRigidBody::liState *State2 = nullptr;
	liRigidBody::liDerivative *Derivative1 = nullptr;
	liRigidBody::liDerivative *Derivative2 = nullptr;

	std::list<lpCollisionData> CollisionData;

	lpContactData()
		//:Body1(nullptr),Body2(nullptr),State1(nullptr),State2(nullptr),Derivative1(nullptr),Derivative2(nullptr)
	{}
};

/** \brief Diszkrét ütközésellenőrző osztály.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lpDiscreteCollisionDetector : public liCollisionDetector
{
private:

	bool MeshMeshCollision(lpMesh *Body_i,lpMesh *Body_j,liRigidBody::liState &State_i,liRigidBody::liState &State_j,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData);

	bool MeshPlaneCollision(lpMesh *Mesh,lpPlane *Plane,liRigidBody::liState &MeshState,liRigidBody::liState &PlaneState,liRigidBody::liDerivative &MeshDerivative,liRigidBody::liDerivative &PlaneDerivative,lpContactData &ContactData);

	bool MeshSphereCollision(lpMesh *Mesh,lpSphere *Sphere,liRigidBody::liState &MeshState,liRigidBody::liState &SphereState,liRigidBody::liDerivative &MeshDerivative,liRigidBody::liDerivative &SphereDerivative,lpContactData &ContactData);

	bool MeshParticleCollision(lpMesh *Mesh,lpRigidBody *Particle,liRigidBody::liState &MeshState,liRigidBody::liState &ParticleState,liRigidBody::liDerivative &MeshDerivative,liRigidBody::liDerivative &ParticleDerivative,lmScalar dt,lpContactData &ContactData);



	bool ParticleCollision(lpRigidBody *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData);

	bool SphereCollision(lpSphere *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData);

	bool PlaneCollision(lpPlane *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData);

	bool MeshCollision(lpMesh *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData);

	bool BoxCollision(lpBox *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData);

public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
    lpDiscreteCollisionDetector(const lpDiscreteCollisionDetector &d) = delete;
    lpDiscreteCollisionDetector &operator=(const lpDiscreteCollisionDetector &d) = delete;

    /** \brief Konstruktor - nem csinál semmit
     */
    lpDiscreteCollisionDetector()
    {
    	//Üres függvénytörzs
    }

    /** \brief Destruktor - nem csinál semmit
     */
    virtual ~lpDiscreteCollisionDetector() override
    {
    	//Üres függvénytörzs
    }

    /** \brief Ez a függvény felelős azért, hogy eldöntse, hogy két test ütközik-e, vagy sem.
     *
     * \param Body_i lpRigidBody*                       Az i-edik test
     * \param State_i lpRigidBody::lpState&             Az i-edik test állapota
     * \param Body_j lpRigidBody*                       A j-edik test
     * \param State_j lpRigidBody::lpState&             A j-edik test állapota
     * \param dt lmScalar                               A szimuláció lépésköze. Folytonos ütközésdetektálás esetén lehet szükséges.
     * \param Derivative_i lpRigidBody::lpDerivative&   Az i-edik test állapotának deriváltja
     * \param Derivative_j lpRigidBody::lpDerivative&   A j-edik test állapotának deriváltja
     * \param InteractionModel lpInteractionModel*      Két test kölcsönhatását definiáló objektum. Ő tartja nyilván, hogy melyik test melyik testtel ütközött, és milyen erők hatnak köztük.
     *
     * \return virtual bool - true, ha a két test ütközik, false, ha nem.
     *
     */
    virtual bool CheckCollision(liRigidBody *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,lpRigidBody::liDerivative &Derivative_j,liInteractionModel *InteractionModel) override;
};

#endif
