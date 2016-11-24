#ifndef LRS_SKELETAL_MESH_NODE_H
#define LRS_SKELETAL_MESH_NODE_H

#include "lrsSceneNode.h"

#include "../../lInterfaces/lRenderer/lrCharacterAnim/liCharacter.h"
#include "../lrCharacterAnim/lrSkeleton.h"

/** \class  lrsSkeletalMeshNode
 *
 * \brief Egy skeletal mesht tartalmazó színtérgráf node.
 *
 */
class lrsSkeletalMeshNode : public lrsSceneNode, public liSkeletalMeshNode
{
protected:
    liCharacter     *Character; /**< A kirajzolandó skeletal meshre mutató pointer */
    lrSkeleton      Skeleton;   /**< A skeletal mesh pozícióját meghatározó csontváz */

public:

    /** \brief Getter függvény, elkéri a skeletal meshre mutató pointert.
     *
     * \return lrCharacter * - a skeletal meshre mutató pointer
     *
     */
    virtual liCharacter *GetCharacter() override;

    /** \brief Getter függvény, elkéri a skeletal mesh skeletonját
     *
     * \return lrSkeleton & - a skeletal mesh skeletonja
     *
     */
    virtual liSkeleton &GetSkeleton() override;

    virtual const liCharacter *GetCharacter() const override;

    virtual const liSkeleton &GetSkeleton() const override;

    /** \brief Setter függvény, beállítja a mesh skeletonját.
     *
     * \param skeleton const lrSkeleton &: az új skeleton
     * \return void
     *
     */
    virtual void SetSkeleton(const liSkeleton &skeleton) override;

    /** \brief Ezzel a függvénnyel adja hozzá a node magát a paraméterben kapott rendererhez.
     *
     * \param Renderer lrRenderer *: a renderer, amivel ki akarjuk rajzoltatni a node által tartalmazott modellt.
     * \return void
     *
     */
    virtual void Add(liRenderer *Renderer) override;

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
    lrsSkeletalMeshNode(const lmVector3D &position,const lmVector3D &scale,const lmQuaternion &orientation,liCharacter *character,liMultiMaterial *material,liSkeleton &skeleton);

    /** \brief  Destruktor, nem csinál semmit.
     *
     */
    virtual ~lrsSkeletalMeshNode() override;
};

#endif // LRS_SKELETAL_MESH_NODE_H
