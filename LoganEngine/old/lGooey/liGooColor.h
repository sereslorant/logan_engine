#ifndef LI_GOO_COLOR_H
#define LI_GOO_COLOR_H

#include "../lRenderer/lrColor.h"

class liGooColor : public liColor
{
public:

    /** \brief Getter függvény - visszaadja a szín vörös komponensét - konstans tagfüggvény
     *
     * \return lGooFloat - a szín vörös komponense
     *
     */
    //virtual lGooFloat GetR() const = 0;

    /** \brief Getter függvény - visszaadja a szín zöld komponensét - konstans tagfüggvény
     *
     * \return lGooFloat - a szín zöld komponense
     *
     */
    //virtual lGooFloat GetG() const = 0;

    /** \brief Getter függvény - visszaadja a szín kék komponensét - konstans tagfüggvény
     *
     * \return lGooFloat - a szín kék komponense
     *
     */
    //virtual lGooFloat GetB() const = 0;

    /** \brief Getter függvény - visszaadja a szín alfa komponensét - konstans tagfüggvény
     *
     * \return lGooFloat - a szín alfa komponense
     *
     */
    //virtual lGooFloat GetA() const = 0;

    /** \brief Setter függvény - beállítja a színünk vörös komponensét
     *
     * \param lGooFloat r: az új vörös komponens
     * \return void
     *
     */
    //virtual void SetR(lGooFloat r) = 0;

    /** \brief Setter függvény - beállítja a színünk zöld komponensét
     *
     * \param lGooFloat g: az új zöld komponens
     * \return void
     *
     */
    //virtual void SetG(lGooFloat g) = 0;

    /** \brief Setter függvény - beállítja a színünk kék komponensét
     *
     * \param lGooFloat b: az új kék komponens
     * \return void
     *
     */
    //virtual void SetB(lGooFloat b) = 0;

    /** \brief Setter függvény - beállítja a színünk alfa komponensét
     *
     * \param lGooFloat a: az új alfa komponens
     * \return void
     *
     */
    //virtual void SetA(lGooFloat a) = 0;
    virtual void Copy(const liColor &color) = 0;

    /** \brief Színtranszformáció - Addition transformation (nem tudom, hogy mi a magyar neve)
     *
     * \param lGooColor &T: a szín, amivel transzformáljuk a színünket.
     * \return void
     *
     */
    virtual void AddTransform(const liGooColor *T) = 0;

    /** \brief Színtranszformáció - Multiplication transformation (nem tudom, hogy mi a magyar neve)
     *
     * \param liGooColor &T: a szín, amivel transzformáljuk a színünket.
     * \return void
     *
     */
    virtual void MltTransform(const liGooColor *T) = 0;

    /** \brief Színtranszformáció - Combined transformation (nem tudom, hogy mi a magyar neve)
     *
     * \param liGooColor &aT: a szín, amivel az addition transformationt végezzük.
     * \param liGooColor &mT: a szín, amivel a multiplication transformationt végezzük.
     * \return void
     *
     */
    virtual void CmbTransform(const liGooColor *aT,const liGooColor *mT) = 0;

	liGooColor(){}
    virtual ~liGooColor() {};
};

#endif
