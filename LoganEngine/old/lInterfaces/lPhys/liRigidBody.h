#ifndef LI_RIGID_BODY_H
#define LI_RIGID_BODY_H

#include "../../lMath/lMath.h"

class liAABB
{
public:

	virtual const lmVector3D &GetMin() const = 0;
	virtual const lmVector3D &GetMax() const = 0;

	virtual void SetMin(const lmVector3D &aabb_min) = 0;
	virtual void SetMax(const lmVector3D &aabb_max) = 0;

	virtual lmVector3D GetCenter() = 0;

	liAABB()
	{}

	virtual ~liAABB()
	{}
};

#include "liSphere.h"
#include "liPlane.h"
#include "liMesh.h"

class liTriangleCache
{
public:

	virtual bool IsValid() = 0;
	virtual void Invalidate() = 0;

	virtual void Update(liMesh *mesh,const lmVector3D &position,const lmQuaternion &orientation) = 0;

	virtual void SetTriangle(const liMesh::liTriangle &t) = 0;

	virtual const lmVector3D &GetNormal() = 0;
	virtual const lmVector3D &GetVertex(unsigned int i) = 0;

	liTriangleCache(){}
	virtual ~liTriangleCache(){}
};

class liBoundingVolume
{
public:

	enum lpBvType
	{
		LP_SPHERE_TREE,
		LP_AABB_TREE,
	};

	virtual lpBvType GetType() = 0;

	virtual void ResizeData(unsigned int size) = 0;
	virtual void InsertData(const liMesh::liTriangle &data,unsigned int i) = 0;

	virtual unsigned int GetDataSize() = 0;
	virtual liTriangleCache *GetData(unsigned int i) = 0;

	virtual void InvalidateCache() = 0;

	virtual void SetParent(liBoundingVolume *parent) = 0;
	virtual liBoundingVolume *GetParent() = 0;

	virtual unsigned int GetNumChildren() = 0;
	virtual void AddChild(liBoundingVolume *child,unsigned int i) = 0;
	virtual liBoundingVolume *GetChild(unsigned int i) = 0;

	virtual liSphere *ToSphere() = 0;
	virtual liAABB *ToAABB() = 0;

	liBoundingVolume(){}
	virtual ~liBoundingVolume(){}
};

#include <vector>
#include <list>

class liBoundingVolumeFactory
{
public:

	virtual liBoundingVolume *CreateBoundingVolume(const std::vector<lmVector3D> &vertices,const std::list<liMesh::liTriangle *> triangle_list,const lmVector3D &position) = 0;

	liBoundingVolumeFactory(){}
	virtual ~liBoundingVolumeFactory(){}
};

class liRigidBody
{
public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
	liRigidBody(const liRigidBody &r) = delete;
	liRigidBody &operator=(const liRigidBody &r) = delete;

	/** \brief A test geometriai típusa
     */
    enum lpType
    {
        LP_PARTICLE,/**< Pontszerű test */
        LP_SPHERE,	/**< Gömb */
        LP_PLANE,	/**< Sík */
        LP_MESH,	/**< Mesh */
        LP_BOX,		/**< Doboz */
    };

    /** \brief A test deriváltjait tartalmazó adatstruktúra, amelyből a következő ciklusbeli állapotot integráljuk.
     *
     * \author Seres Lóránt Gábor
     *
     */
    class liDerivative
    {
	public:
        virtual const lmVector3D &GetMomentum() const = 0;
        virtual const lmVector3D &GetNetForce() const = 0;
        virtual const lmVector3D &GetAngularMomentum() const = 0;
        virtual const lmVector3D &GetNetTorque() const = 0;


        virtual void SetMomentum(const lmVector3D &) = 0;
		virtual void SetNetForce(const lmVector3D &) = 0;
		virtual void SetAngularMomentum(const lmVector3D &) = 0;
		virtual void SetNetTorque(const lmVector3D &) = 0;


        virtual void IncMomentum(const lmVector3D &) = 0;
		virtual void IncNetForce(const lmVector3D &) = 0;
		virtual void IncAngularMomentum(const lmVector3D &) = 0;
		virtual void IncNetTorque(const lmVector3D &) = 0;


        /** \brief Default konstruktor, ami nullával tölti fel az adatokat.
         *
         */
        liDerivative()
        {
			//Üres függvénytörzs
        }

        virtual ~liDerivative()
        {
			//Üres függvénytörzs
        }
    };

    class liState
    {
	public:
    	virtual void SetPosition(const lmVector3D &position) = 0;
    	virtual void SetMomentum(const lmVector3D &momentum) = 0;
    	virtual void SetOrientation(const lmVector3D &orientation) = 0;
    	//virtual void SetOrientation(const lmQuaternion &orientation) = 0;
    	virtual void SetAngularMomentum(const lmVector3D &angular_momentum) = 0;

		virtual void SetMovable(bool movable) = 0;
    	virtual void SetRotatable(bool rotatable) = 0;

    	virtual void IncPosition(const lmVector3D &delta_position) = 0;
    	virtual void IncMomentum(const lmVector3D &delta_momentum) = 0;
    	virtual void IncOrientation(const lmVector3D &delta_orientation) = 0;
    	virtual void IncAngularMomentum(const lmVector3D &delta_angular_momentum) = 0;

        virtual const lmVector3D &GetPosition() const = 0;
        virtual const lmVector3D &GetMomentum() const = 0;
        virtual const lmVector3D &GetOrientation() const = 0;
        virtual const lmVector3D &GetAngularMomentum() const = 0;

        virtual bool IsMovable() const = 0;
    	virtual bool IsRotatable() const = 0;

        /** \brief Kvaternió, amelyből ki lehet számolni a test elfordulását megadó mátrixot.
         *
         * \return lmQuaternion - a test elfordulását megadó kvaternió
         *
         */
        virtual lmQuaternion GetOrientationQuaternion() const = 0;

        liState()
        {

        }

        virtual ~liState()
        {

        }
    };

	class liCollisionCallback
	{
	public:
		virtual void Callback(const liRigidBody *other_body,const liState *other_state,const lmVector3D &intersection,unsigned int collision_data_index) = 0;

		liCollisionCallback(){}
		virtual ~liCollisionCallback() {}
	};

    liRigidBody()
    {

    }

    virtual ~liRigidBody()
    {

    }

    virtual bool IsRemovable() = 0;

    /** \brief Megjelöli a testet, hogy a következő iterációban törölni lehet.
     *
     * \warning VIGYÁZAT! Olyan testet removolni, amire constraint hat, szegmentálási hibát okozhat!
     * Először a constrainteket kell törölni, utána a testet.
     *
     * \return void
     *
     */
    virtual void MarkRemovable() = 0;

    virtual void AddCollisionCallback(liCollisionCallback *collision_callback) = 0;

    virtual void CollisionCallback(const liRigidBody *other_body,const liState *other_state,const lmVector3D &intersection,unsigned int collision_data_index) = 0;


    virtual void* GetUserData() = 0;
    virtual void SetUserData(void *user_data) = 0;

    /** \brief Getter függvény - lekérdezi a test típusát - konstans tagfüggvény
     *
     * \return lpRigidBody::lpType - a test típusa
     *
     */
    virtual lpType GetType() const = 0;

    virtual void SetGhost(bool ghost) = 0;
    virtual bool IsGhost() const = 0;

    /** \brief Getter függvény - lekérdezi a test tömegét - konstans tagfüggvény
     *
     * \return lmScalar - a test tömege
     *
     */
    virtual lmScalar GetMass() const = 0;

    /** \brief Getter függvény - lekérdezi a test tömegének reciprokát - konstans tagfüggvény
     *
     * \return lmScalar - a test tömegének reciproka
     *
     */
    virtual lmScalar GetInvMass() const = 0;

    /** \brief Getter függvény - lekérdezi a test tehetetlenségi tenzorát - konstans tagfüggvény
     *
     * \return const lmMatrix3x3 & - a test tehetetlenségi tenzora. Ha a test tömege "végtelen" vagy nulla, kkor nullmátrixszal tér vissza.
     *
     */
    virtual const lmMatrix3x3 &GetInertiaTensor() const = 0;

    /** \brief Getter függvény - lekérdezi a test tehetetlenségi tenzorának inverzét - konstans tagfüggvény
     *
     * \return const lmMatrix3x3 & - a test tehetetlenségi tenzorának inverze. Ha a test tömege "végtelen" vagy nulla, kkor nullmátrixszal tér vissza.
     *
     */
    virtual const lmMatrix3x3 &GetInvInertiaTensor() const = 0;

    /** \brief Getter függvény - lekérdezi a test pillanatnyi állapotát leíró adatstruktúrát
     *
     * \return lpState & - a test pillanatnyi állapotát leíró adatstruktúra
     *
     */
    virtual liState *GetState() = 0;

    /** \brief Getter függvény - lekérdezi a test pillanatnyi állapotát tartalmazó adatstruktúrát - konstans tagfüggvény
     *
     * \return const lpState & - a test pillanatnyi állapotát tartalmazó adatstruktúra
     *
     */
    virtual const liState *GetState() const = 0;

    /** \brief Getter függvény - lekérdezi a test pozícióját - konstans tagfüggvény
     *
     * \return const lmVector3D & - a test pozíciója
     *
     */
    virtual const lmVector3D &GetPosition() const = 0;

    /** \brief Getter függvény - lekérdezi a test impulzusát - konstans tagfüggvény
     *
     * \return const lmVector3D & - a test impulzusa
     *
     */
    virtual const lmVector3D &GetMomentum() const = 0;

    /** \brief Getter függvény - lekérdezi a test szögelfordulását - konstans tagfüggvény
     *
     * \return lmScalar - a test szögelfordulását
     *
     */
    virtual const lmVector3D &GetOrientation() const = 0;

    /** \brief Getter függvény - lekérdezi a test impulzusmomentumát - konstans tagfüggvény
     *
     * \return lmScalar - a test impulzusmomentuma
     *
     */
    virtual const lmVector3D &GetAngularMomentum()const  = 0;

    virtual bool HasBoundingVolume() = 0;
    virtual liBoundingVolume *GetBoundingVolume() = 0;
};

#endif // LI_RIGID_BODY_H
