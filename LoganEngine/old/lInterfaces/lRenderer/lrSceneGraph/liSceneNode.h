#ifndef LI_SCENE_NODE_H
#define LI_SCENE_NODE_H

#include "../../../lMath/lMath.h"

class liRenderer;

#include "../liMultiMaterial.h"

/** \class lrsSceneNode
 *
 * \brief A színtérgráf egy node-ja.
 *
 */
class liSceneNode
{
public:

    /** \brief Getter függvény, ami visszaadja a node pozícióját.
     *
     * \return lmVector3D & - a node pozíciója
     *
     */
    virtual lmVector3D &GetPosition() = 0;

    /** \brief Getter függvény, ami visszaadja a node nagyítását.
     *
     * \return lmVector3D & - a node nagyítása
     *
     */
    virtual lmVector3D &GetScale() = 0;

    /** \brief Getter függvény, ami visszaadja a node orientációját.
     *
     * \return lmVector3D & - a node orientációja
     *
     */
    virtual lmQuaternion &GetOrientation() = 0;

    /** \brief Getter függvény, elkéri a skeletal mesh multi materialjára mutató pointert.
     *
     * \return lrMultiMaterial * - a skeletal mesh multi materialja
     *
     */
    virtual liMultiMaterial *GetMaterial() = 0;

    /** \brief Getter függvény, ami visszaadja a node pozícióját. Konstans tagfüggvény
     *
     * \return const lmVector3D & - a node pozíciója
     *
     */
    virtual const lmVector3D &GetPosition() const = 0;

    /** \brief Getter függvény, ami visszaadja a node nagyítását. Konstans tagfüggvény
     *
     * \return const lmVector3D & - a node nagyítása
     *
     */
    virtual const lmVector3D &GetScale() const = 0;

    /** \brief Getter függvény, ami visszaadja a node orientációját. Konstans tagfüggvény
     *
     * \return const lmVector3D & - a node orientációja
     *
     */
    virtual const lmQuaternion &GetOrientation() const = 0;

    /** \brief Getter függvény, elkéri a skeletal mesh multi materialjára mutató pointert.
     *
     * \return lrMultiMaterial * - a skeletal mesh multi materialja
     *
     */
    virtual const liMultiMaterial *GetMaterial() const = 0;

    /** \brief Setter függvény, beállítja a node pozícióját
     *
     * \param position const lmVector3D &: a node új pozíciója
     * \return void
     *
     */
    virtual void SetPosition(const lmVector3D &position) = 0;

    /** \brief Setter függvény, beállítja a node nagyítását
     *
     * \param scale const lmVector3D &: a node új nagyítása
     * \return void
     *
     */
    virtual void SetScale(const lmVector3D &scale) = 0;

    /** \brief Setter függvény, beállítja a node orientációját
     *
     * \param orientation const lmQuaternion &: a node új orientációja
     * \return void
     *
     */
    virtual void SetOrientation(const lmQuaternion &orientation) = 0;

    /** \brief Ez a függvény beszorozza a paraméterben kapott mátrixot a test model mátrixával. A model mátrix helyezi át a node-ot body space-ből world space-be.
     *
     * \param Target lmMatrix4x4 &: a beszorzandó mátrix.
     * \return void
     *
     */
    virtual void GetModelMatrix(lmMatrix4x4 &Target) = 0;

    /** \brief Ezzel a függvénnyel adja hozzá a node magát a paraméterben kapott rendererhez.
     *
     * \param Renderer lrRenderer *: a renderer, amivel ki akarjuk rajzoltatni a node által tartalmazott modellt.
     * \return void
     *
     */
    virtual void Add(liRenderer *Renderer) = 0;

    /** \brief Konstruktor, amely beállítja az objektum összes tagváltozóját.
     *
     * \param position const lmVector3D &:       A node által tartalmazott elem pozíciója
     * \param scale const lmVector3D &:          A node által tartalmazott elem x y és z tengely menti nagyítása
     * \param orientation const lmQuaternion &:  A node által tartalmazott elem orientációja
     *
     */
    liSceneNode(){};

    /** \brief Virtuális destruktor. Nem csinál semmit.
     *
     */
    virtual ~liSceneNode(){};
};

#include "../liStaticMesh.h"

class liStaticMeshNode
{
public:

    /** \brief Getter függvény, elkéri a static meshre mutató pointert.
     *
     * \return lrStaticMesh * - a static meshre mutató pointer
     *
     */
    virtual liStaticMesh *GetMesh() = 0;

    /** \brief Getter függvény, elkéri a static meshre mutató pointert.
     *
     * \return lrStaticMesh * - a static meshre mutató pointer
     *
     */
    virtual const liStaticMesh *GetMesh() const = 0;

    /** \brief Konstruktor, amely beállítja az objektum összes tagváltozóját.
     *
     * \param position const lmVector3D &:       A node által tartalmazott elem pozíciója
     * \param scale const lmVector3D &:          A node által tartalmazott elem x y és z tengely menti nagyítása
     * \param orientation const lmQuaternion &:  A node által tartalmazott elem orientációja
     * \param mesh lrStaticMesh *:               A kirajzolandó static meshre mutató pointer
     * \param material lrMultiMaterial *:        A kirajzolandó mesh materialja
     *
     */
    liStaticMeshNode() {};

    /** \brief  Destruktor, nem csinál semmit.
     *
     */
    virtual ~liStaticMeshNode() {};
};

#include "../lrCharacterAnim/liCharacter.h"

class liSkeletalMeshNode
{
public:

    /** \brief Getter függvény, elkéri a skeletal meshre mutató pointert.
     *
     * \return lrCharacter * - a skeletal meshre mutató pointer
     *
     */
    virtual liCharacter *GetCharacter() = 0;

    /** \brief Getter függvény, elkéri a skeletal mesh skeletonját
     *
     * \return lrSkeleton & - a skeletal mesh skeletonja
     *
     */
    virtual liSkeleton &GetSkeleton() = 0;

    virtual const liCharacter *GetCharacter() const = 0;

    virtual const liSkeleton &GetSkeleton() const = 0;

    /** \brief Setter függvény, beállítja a mesh skeletonját.
     *
     * \param skeleton const lrSkeleton &: az új skeleton
     * \return void
     *
     */
    virtual void SetSkeleton(const liSkeleton &skeleton) = 0;

    /** \brief Konstruktor, amely beállítja az objektum összes tagváltozóját.
     *
     * \param position const lmVector3D &:       A node által tartalmazott elem pozíciója
     * \param scale const lmVector3D &:          A node által tartalmazott elem x y és z tengely menti nagyítása
     * \param orientation const lmQuaternion &:  A node által tartalmazott elem orientációja
     * \param character lrCharacter *:           A kirajzolandó skeletal meshre mutató pointer
     * \param material lrMultiMaterial *:        A kirajzolandó mesh materialja
     * \param skeleton lrSkeleton &:             A skeletal mesh skeletonja
     *
     */
    liSkeletalMeshNode() {};

    /** \brief  Destruktor, nem csinál semmit.
     *
     */
    virtual ~liSkeletalMeshNode() {};
};

#include "../liRenderer.h"

#endif // LI_SCENE_NODE_H
