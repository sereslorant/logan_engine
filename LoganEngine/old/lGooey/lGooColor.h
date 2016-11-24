#ifndef L_GOO_COLOR_H
#define L_GOO_COLOR_H

#include "liGooColor.h"
#include <cmath>

/** \class lGooColor
 * \brief Egy RGBA színnégyest tartalmazó adatstruktúra
 *
 * \author Seres Lóránt
 */
class lGooColor : public liGooColor
{
private:
    float R; /**< A piros komponens */
    float G; /**< A kék komponens */
    float B; /**< A zöld komponens */
    float A; /**< Az alfa komponens */

public:
	/*
    virtual float GetRed() const override {return Red;}
    virtual float GetGreen() const override {return Green;}
    virtual float GetBlue() const override {return Blue;}
    virtual float GetAlpha() const override {return Alpha;}

    virtual void SetRed(float red) override {Red = red;}
    virtual void SetGreen(float green) override {Green = green;}
    virtual void SetBlue(float blue) override {Blue = blue;}
    virtual void SetAlpha(float alpha) override {Alpha = alpha;}
	*/

    /** \brief Getter függvény - visszaadja a szín vörös komponensét - konstans tagfüggvény
     *
     * \return float - a szín vörös komponense
     *
     */
    virtual float GetRed() const override
    {
		return R;
	}

    /** \brief Getter függvény - visszaadja a szín zöld komponensét - konstans tagfüggvény
     *
     * \return float - a szín zöld komponense
     *
     */
    virtual float GetGreen() const override
    {
		return G;
	}

    /** \brief Getter függvény - visszaadja a szín kék komponensét - konstans tagfüggvény
     *
     * \return float - a szín kék komponense
     *
     */
    virtual float GetBlue() const override
    {
		return B;
	}

    /** \brief Getter függvény - visszaadja a szín alfa komponensét - konstans tagfüggvény
     *
     * \return float - a szín alfa komponense
     *
     */
    virtual float GetAlpha() const override
    {
		return A;
	}

    /** \brief Setter függvény - beállítja a színünk vörös komponensét
     *
     * \param float r: az új vörös komponens
     * \return void
     *
     */
    virtual void SetRed(float r) override
    {
		R = r;
	}

    /** \brief Setter függvény - beállítja a színünk zöld komponensét
     *
     * \param float g: az új zöld komponens
     * \return void
     *
     */
    virtual void SetGreen(float g) override
    {
		G = g;
	}

    /** \brief Setter függvény - beállítja a színünk kék komponensét
     *
     * \param float b: az új kék komponens
     * \return void
     *
     */
    virtual void SetBlue(float b) override
	{
		B = b;
	}

    /** \brief Setter függvény - beállítja a színünk alfa komponensét
     *
     * \param float a: az új alfa komponens
     * \return void
     *
     */
    virtual void SetAlpha(float a) override
	{
		A = a;
	}

    virtual void Copy(const liColor &color) override
    {
    	R = color.GetRed();
    	G = color.GetGreen();
    	B = color.GetBlue();
    	A = color.GetAlpha();
    }

    /** \brief Színtranszformáció - Addition transformation (nem tudom, hogy mi a magyar neve)
     *
     * \param lGooColor &T: a szín, amivel transzformáljuk a színünket.
     * \return void
     *
     */
    virtual void AddTransform(const liGooColor *T) override
    {
		R	= std::max((float)0.0, std::min(R + T->GetRed(), (float)1.0));
		G	= std::max((float)0.0, std::min(G + T->GetGreen(), (float)1.0));
		B	= std::max((float)0.0, std::min(B + T->GetBlue(), (float)1.0));
		A	= std::max((float)0.0, std::min(A + T->GetAlpha(), (float)1.0));
	}

    /** \brief Színtranszformáció - Multiplication transformation (nem tudom, hogy mi a magyar neve)
     *
     * \param lGooColor &T: a szín, amivel transzformáljuk a színünket.
     * \return void
     *
     */
    virtual void MltTransform(const liGooColor *T) override
    {
		R *= T->GetRed();
		G *= T->GetGreen();
		B *= T->GetBlue();
		A *= T->GetAlpha();
	}

    /** \brief Színtranszformáció - Combined transformation (nem tudom, hogy mi a magyar neve)
     *
     * \param lGooColor &aT: a szín, amivel az addition transformationt végezzük.
     * \param lGooColor &mT: a szín, amivel a multiplication transformationt végezzük.
     * \return void
     *
     */
    virtual void CmbTransform(const liGooColor *aT,const liGooColor *mT) override
    {
		R	= std::max((float)0.0, std::min(R * mT->GetRed() + aT->GetRed(), (float)1.0));
		G	= std::max((float)0.0, std::min(G * mT->GetGreen() + aT->GetGreen(), (float)1.0));
		B	= std::max((float)0.0, std::min(B * mT->GetBlue() + aT->GetBlue(), (float)1.0));
		A	= std::max((float)0.0, std::min(A * mT->GetAlpha() + aT->GetAlpha(), (float)1.0));
	}

	lGooColor()
	{
		R = 0.0;
		G = 0.0;
		B = 0.0;
		A = 0.0;
	}

    /** \brief Konstruktor
     *
     * \param lGoo_Real r: az új vörös komponens
     * \param lGoo_Real g: az új zöld komponens
     * \param lGoo_Real b: az új kék komponens
     * \param lGoo_Real a: az új alfa komponens
     *
     */
    lGooColor(float r,float g,float b,float a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}


    lGooColor(const liGooColor &c)
    {
		R = c.GetRed();
		G = c.GetGreen();
		B = c.GetBlue();
		A = c.GetAlpha();
	}

    /**< Destruktor - nem csinál semmit */
    virtual ~lGooColor() override
    {};
};

#endif
