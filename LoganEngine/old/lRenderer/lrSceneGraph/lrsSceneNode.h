#ifndef LRS_SCENE_NODE_H
#define LRS_SCENE_NODE_H

#include "../../lInterfaces/lRenderer/lrSceneGraph/liSceneNode.h"

/** \class lrsSceneNode
 *
 * \brief A színtérgráf egy node-ja.
 *
 */
class lrsSceneNode : public liSceneNode
{
protected:
    lmVector3D Position;        /**< A node által tartalmazott elem pozíciója */
    lmVector3D Scale;           /**< A node által tartalmazott elem x y és z tengely menti nagyítása */
    lmQuaternion Orientation;   /**< A node által tartalmazott elem orientációja */

    liMultiMaterial *Material;  /**< A skeletal mesh multi materialjára mutató pointer */

public:

    /** \brief Getter függvény, ami visszaadja a node pozícióját.
     *
     * \return lmVector3D & - a node pozíciója
     *
     */
    virtual lmVector3D &GetPosition() override;

    /** \brief Getter függvény, ami visszaadja a node nagyítását.
     *
     * \return lmVector3D & - a node nagyítása
     *
     */
    virtual lmVector3D &GetScale() override;

    /** \brief Getter függvény, ami visszaadja a node orientációját.
     *
     * \return lmVector3D & - a node orientációja
     *
     */
    virtual lmQuaternion &GetOrientation() override;

    /** \brief Getter függvény, elkéri a skeletal mesh multi materialjára mutató pointert.
     *
     * \return lrMultiMaterial * - a skeletal mesh multi materialja
     *
     */
    virtual liMultiMaterial *GetMaterial() override
    {
		return Material;
	}

    /** \brief Getter függvény, ami visszaadja a node pozícióját. Konstans tagfüggvény
     *
     * \return const lmVector3D & - a node pozíciója
     *
     */
    virtual const lmVector3D &GetPosition() const override;

    /** \brief Getter függvény, ami visszaadja a node nagyítását. Konstans tagfüggvény
     *
     * \return const lmVector3D & - a node nagyítása
     *
     */
    virtual const lmVector3D &GetScale() const override;

    /** \brief Getter függvény, ami visszaadja a node orientációját. Konstans tagfüggvény
     *
     * \return const lmVector3D & - a node orientációja
     *
     */
    virtual const lmQuaternion &GetOrientation() const override;

    /** \brief Getter függvény, elkéri a skeletal mesh multi materialjára mutató pointert.
     *
     * \return lrMultiMaterial * - a skeletal mesh multi materialja
     *
     */
    virtual const liMultiMaterial *GetMaterial() const override
    {
		return Material;
	}

    /** \brief Setter függvény, beállítja a node pozícióját
     *
     * \param position const lmVector3D &: a node új pozíciója
     * \return void
     *
     */
    virtual void SetPosition(const lmVector3D &position) override;

    /** \brief Setter függvény, beállítja a node nagyítását
     *
     * \param scale const lmVector3D &: a node új nagyítása
     * \return void
     *
     */
    virtual void SetScale(const lmVector3D &scale) override;

    /** \brief Setter függvény, beállítja a node orientációját
     *
     * \param orientation const lmQuaternion &: a node új orientációja
     * \return void
     *
     */
    virtual void SetOrientation(const lmQuaternion &orientation) override;

    /** \brief Ez a függvény beszorozza a paraméterben kapott mátrixot a test model mátrixával. A model mátrix helyezi át a node-ot body space-ből world space-be.
     *
     * \param Target lmMatrix4x4 &: a beszorzandó mátrix.
     * \return void
     *
     */
    virtual void GetModelMatrix(lmMatrix4x4 &Target) override;

    /** \brief Konstruktor, amely beállítja az objektum összes tagváltozóját.
     *
     * \param position const lmVector3D &:       A node által tartalmazott elem pozíciója
     * \param scale const lmVector3D &:          A node által tartalmazott elem x y és z tengely menti nagyítása
     * \param orientation const lmQuaternion &:  A node által tartalmazott elem orientációja
     *
     */
    lrsSceneNode(const lmVector3D &position,const lmVector3D &scale,const lmQuaternion &orientation,liMultiMaterial *material);

    /** \brief Virtuális destruktor. Nem csinál semmit.
     *
     */
    virtual ~lrsSceneNode() override;
};

#endif // LRS_SCENE_NODE_H
