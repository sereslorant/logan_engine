#ifndef LRS_STATIC_MESH_NODE_H
#define LRS_STATIC_MESH_NODE_H

#include "lrsSceneNode.h"

#include "../../lInterfaces/lRenderer/liStaticMesh.h"

/** \class  lrsSkeletalMeshNode
 *
 * \brief Egy static mesht tartalmazó színtérgráf node.
 *
 */
class lrsStaticMeshNode : public lrsSceneNode, public liStaticMeshNode
{
protected:
    liStaticMesh    *Mesh;      /**< A kirajzolandó static meshre mutató pointer */

public:

    /** \brief Getter függvény, elkéri a static meshre mutató pointert.
     *
     * \return lrStaticMesh * - a static meshre mutató pointer
     *
     */
    virtual liStaticMesh *GetMesh() override;

    /** \brief Getter függvény, elkéri a static meshre mutató pointert.
     *
     * \return lrStaticMesh * - a static meshre mutató pointer
     *
     */
    virtual const liStaticMesh *GetMesh() const override;

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
     * \param mesh lrStaticMesh *:               A kirajzolandó static meshre mutató pointer
     * \param material lrMultiMaterial *:        A kirajzolandó mesh materialja
     *
     */
    lrsStaticMeshNode(const lmVector3D &position,const lmVector3D &scale,const lmQuaternion &orientation,liStaticMesh *mesh,liMultiMaterial *material);

    /** \brief  Destruktor, nem csinál semmit.
     *
     */
    virtual ~lrsStaticMeshNode() override;
};

#endif // LRS_STATIC_MESH_NODE_H
