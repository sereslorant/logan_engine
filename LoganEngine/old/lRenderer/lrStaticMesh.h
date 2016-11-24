#ifndef LR_MESH_H
#define LR_MESH_H

#include "../lInterfaces/lRenderer/liStaticMesh.h"

#include <vector>

/** \brief Egy statikus mesht modellező class. A statikus mesh material groupokból áll elő.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrStaticMesh : public liStaticMesh
{
public:

    /** \brief Egy material groupot modellező class. A material group háromszögek halmaza,
     * melyek ugyanazt a materialt használják.
     *
     */
    class lrMaterialGroup : public liMaterialGroup
    {
    protected:
        std::string Material; /**< A material kulcsa. */

    public:

        /** \brief Getter függvény, amely visszaadja a material kulcsát.
         *
         * \return std::string - a material adatok kulcsa
         *
         */
        virtual std::string &GetMaterial() override;

        /** \brief Getter függvény, amely visszaadja a material kulcsát. Konstans tagfüggvény.
         *
         * \return std::string - a material adatok kulcsa
         *
         */
        virtual const std::string &GetMaterial() const override;

        /** \brief Konstruktor, mely inicializálja a material kulcsot.
         *
         * \param material const std::string &: a material kulcs.
         *
         */
        lrMaterialGroup(const std::string &material);

        /** \brief Virtuális destruktor
         *
         */
        virtual ~lrMaterialGroup();
    };

protected:
    std::vector<liMaterialGroup *> MaterialGroups; /**< A mesht alkotó material groupokra mutató pointereket tartalmazó tömb */

public:

    /** \brief Getter függvény, amely visszaadja a material groupok pointereit tartalmazó tömb hosszát.
     *
     * \return unsigned int - a material groupok tömbjének a hossza.
     *
     */
    virtual unsigned int GetSize() const override;

    /** \brief Beilleszti a paraméterben kapott material groupot az i-edik helyre.
     *
     * \warning VIGYÁZAT! A függvény nem hülyebiztos! Ha az i-edik helyre már előzőleg illesztettünk be material groupot, annak a pointere felülíródik, nem szabadul fel, így memóriaszivárgás lép fel!
     *
     * \param i unsigned int:               a material group indexe
     * \param MatGroup lrMaterialGroup *:   a beillesztendő material group.
     * \return bool - ha i kimutat a tömbből, akkor false, egyéb esetben true.
     *
     */
    virtual bool Add(unsigned int i,liMaterialGroup *MatGroup) override;

    /** \brief Operátor, mellyel az i-edik material groupot lehet elkérni.
     *
     * \param i unsigned int: a material group indexe.
     * \return lrMaterialGroup * - a material groupra mutató pointer. Ha i kimutat a tömbből, akkor nullptr.
     *
     */
    virtual liMaterialGroup *operator[] (unsigned int i) override;

    /** \brief Operátor, mellyel az i-edik material groupot lehet elkérni. Konstans tagfüggvény.
     *
     * \param i unsigned int: a material group indexe.
     * \return const lrMaterialGroup * - a material groupra mutató pointer. Ha i kimutat a tömbből, akkor nullptr.
     *
     */
    virtual const liMaterialGroup *operator[] (unsigned int i) const override;

    /** \brief Konstruktor, amely beállítja a MaterialGroups méretét.
     *
     * \param Size unsigned int: a MaterialGroups mérete.
     *
     */
    lrStaticMesh(unsigned int Size);

    /** \brief Virtuális destruktor, felszabadítja az összes material groupot.
     *
     */
    virtual ~lrStaticMesh() override;
};

#endif // LR_MESH_H
