#ifndef LR_SKELETAL_MESH_H
#define LR_SKELETAL_MESH_H

#include "../lInterfaces/lRenderer/liSkeletalMesh.h"

/** \brief Egy skeletal mesht modellező osztály. Egy skeletal meshnek egy materialja van. Több material hasznlatára való az lrCharacter osztály.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrSkeletalMesh : public liSkeletalMesh
{
protected:
    std::string Material; /**< A skeletal mesh materialjának a kulcsa */

public:

    /** \brief Getter függvény, ami visszaadja a skeletal mesh materialjának a kulcsát.
     *
     * \return std::string & - a material kulcsa
     *
     */
    virtual std::string &GetMaterial() override;

    /** \brief Getter függvény, ami visszaadja a skeletal mesh materialjának a kulcsát. Konstans tagfüggvény
     *
     * \return const std::string & - a material kulcsa
     *
     */
    virtual const std::string &GetMaterial() const override;

    /** \brief Konstruktor, amely inicializálja a material kulcsot.
     *
     * \param material const std::string &: a material kulcsa.
     *
     */
    lrSkeletalMesh(const std::string &material);

    /** \brief Virtuális destruktor - nem csinál semmit.
     *
     */
    virtual ~lrSkeletalMesh() override;
};

#endif // LR_SKELETAL_MESH_H
