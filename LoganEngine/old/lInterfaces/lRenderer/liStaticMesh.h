#ifndef LI_STATIC_MESH_H
#define LI_STATIC_MESH_H

#include <string>

class liStaticMesh
{
public:

    /** \brief Egy material groupot modellező class. A material group háromszögek halmaza,
     * melyek ugyanazt a materialt használják.
     *
     */
    class liMaterialGroup
    {
    public:

        /** \brief Getter függvény, amely visszaadja a material kulcsát.
         *
         * \return std::string - a material adatok kulcsa
         *
         */
        virtual std::string &GetMaterial() = 0;

        /** \brief Getter függvény, amely visszaadja a material kulcsát. Konstans tagfüggvény.
         *
         * \return std::string - a material adatok kulcsa
         *
         */
        virtual const std::string &GetMaterial() const = 0;

        /** \brief A renderer kirajzoláskor ezzel a függvénnyel aktiválja a material groupot.
         *
         * \return void
         *
         */
        virtual void Enable() = 0;

        /** \brief A renderer kirajzolás után ezzel a függvénnyel deaktiválja a material groupot.
         *
         * \return void
         *
         */
        virtual void Disable() = 0;

        /** \brief Konstruktor, mely inicializálja a material kulcsot.
         *
         */
        liMaterialGroup(){}

        /** \brief Virtuális destruktor
         *
         */
        virtual ~liMaterialGroup(){}
    };

    /** \brief Getter függvény, amely visszaadja a material groupok pointereit tartalmazó tömb hosszát.
     *
     * \return unsigned int - a material groupok tömbjének a hossza.
     *
     */
    virtual unsigned int GetSize() const = 0;

    /** \brief Beilleszti a paraméterben kapott material groupot az i-edik helyre.
     *
     * \warning VIGYÁZAT! A függvény nem hülyebiztos! Ha az i-edik helyre már előzőleg illesztettünk be material groupot, annak a pointere felülíródik, nem szabadul fel, így memóriaszivárgás lép fel!
     *
     * \param i unsigned int:               a material group indexe
     * \param MatGroup lrMaterialGroup *:   a beillesztendő material group.
     * \return bool - ha i kimutat a tömbből, akkor false, egyéb esetben true.
     *
     */
    virtual bool Add(unsigned int i,liMaterialGroup *MatGroup) = 0;

    /** \brief Operátor, mellyel az i-edik material groupot lehet elkérni.
     *
     * \param i unsigned int: a material group indexe.
     * \return lrMaterialGroup * - a material groupra mutató pointer. Ha i kimutat a tömbből, akkor nullptr.
     *
     */
    virtual liMaterialGroup *operator[] (unsigned int i) = 0;

    /** \brief Operátor, mellyel az i-edik material groupot lehet elkérni. Konstans tagfüggvény.
     *
     * \param i unsigned int: a material group indexe.
     * \return const lrMaterialGroup * - a material groupra mutató pointer. Ha i kimutat a tömbből, akkor nullptr.
     *
     */
    virtual const liMaterialGroup *operator[] (unsigned int i) const = 0;

    /** \brief Konstruktor, amely beállítja a MaterialGroups méretét.
     *
     * \param Size unsigned int: a MaterialGroups mérete.
     *
     */
    liStaticMesh(){}

    /** \brief Virtuális destruktor, felszabadítja az összes material groupot.
     *
     */
    virtual ~liStaticMesh(){}
};

#endif // LI_MESH_H
