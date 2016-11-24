#ifndef LP_COLLISION
#define LP_COLLISION

#include "liRigidBody.h"
#include "liInteractionModel.h"

/** \brief A collision detector absztrakt ősosztálya. A collision detector feladata, hogy megállapítsa két
 * testről, hogy ütköznek-e.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liCollisionDetector
{
public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
	liCollisionDetector(const liCollisionDetector &c) = delete;
	liCollisionDetector &operator=(const liCollisionDetector &c) = delete;

    /** \brief Konstruktor - nem csinál semmit.
     */
    liCollisionDetector(){}

    /** \brief Virtuális destruktor - nem csinál semmit.
     */
    virtual ~liCollisionDetector(){}

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
    virtual bool CheckCollision(liRigidBody *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,liInteractionModel *InteractionModel) = 0;
};

#endif
