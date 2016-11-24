#ifndef LI_CHARACTER_H
#define LI_CHARACTER_H

#include "liSkeleton.h"
#include "../liSkeletalMesh.h"

/** \brief Több, azonos skeletonú skeletal mesht magába foglaló class.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liCharacter
{
public:

	virtual liSkeleton &GetBindPoseSkeleton() = 0;

    /** \brief Beilleszti a paraméterben kapott material groupot az i-edik helyre.
     *
     * \warning VIGYÁZAT! A függvény nem hülyebiztos! Ha az i-edik helyre már előzőleg illesztettünk be material groupot, annak a pointere felülíródik, nem szabadul fel, így memóriaszivárgás lép fel!
     *
     * \param i unsigned int:               a material group indexe
     * \param newMesh lrSkeletalMesh*:      a beillesztendő material group.
     * \return bool - ha i kimutat a tömbből, akkor false, egyéb esetben true.
     *
     */
    virtual bool Add(unsigned int i,liSkeletalMesh *newMesh) = 0;

    /** \brief Getter függvény, amely visszaadja a skeletal meshek pointereit tartalmazó tömb hosszát.
     *
     * \return unsigned int - a skeletal meshek tömbjének a hossza.
     *
     */
    virtual unsigned int Size() = 0;

    /** \brief Operátor, mellyel az i-edik skeletal mesht lehet elkérni.
     *
     * \param i unsigned int: a skeletal mesh indexe.
     * \return lrSkeletalMesh* - a skeletal meshre mutató pointer. Ha i kimutat a tömbből, akkor nullptr.
     *
     */
    virtual liSkeletalMesh *operator[](unsigned int i) = 0;

    /** \brief Operátor, mellyel az i-edik skeletal mesht lehet elkérni. Konstans tagfüggvény.
     *
     * \param i unsigned int: a skeletal mesh indexe.
     * \return const lrSkeletalMesh* - a skeletal meshre mutató pointer. Ha i kimutat a tömbből, akkor nullptr.
     *
     */
    virtual const liSkeletalMesh *operator[](unsigned int i) const = 0;

    /** \brief Konstruktor, amely beállítja a Meshes vektor méretét.
     *
     * \param Size unsigned int: a Meshes mérete.
     *
     */
    liCharacter(){};

    /** \brief Virtuális destruktor, nem csinál semmit.
     *
     */
    virtual ~liCharacter(){};
};

#endif // LI_CHARACTER_H
