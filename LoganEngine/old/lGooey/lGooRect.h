#ifndef L_GOO_RECT_H
#define L_GOO_RECT_H

#include "lGooObject.h"

/** \brief Egy kirajzolható téglalapot modellező osztály.
 *
 * \author Seres Lóránt
 *
 */
class lGooRect : public lGooObject ,public liGooRect
{
public:

    typedef void (lGooRect::* AttribSetter) (lmScalar value);
    typedef lmScalar (lGooRect::* AttribGetter) () const;

    struct lGooRectAttrib
    {
    	AttribSetter SetValue;
    	AttribSetter IncValue;
    	AttribGetter GetValue;

    	lGooRectAttrib(AttribSetter set_value,AttribSetter inc_value,AttribGetter get_value)
			:SetValue(set_value),IncValue(inc_value),GetValue(get_value)
    	{}
    };

private:
    lmScalar Width; /**< A téglalap szélessége */
    lmScalar Height; /**< A téglalap hosszúsága */
public:

    lGooRect(liGuiElement *polygon)
		:lGooObject(polygon)
    {

    }

    /** \brief Konstruktor
     *
     * \param const lmVector2D &pos:    A téglalap pozíciója
     * \param const lmVector2D &scale:  A téglalap nyújtása
     * \param lmScalar orientation:     A téglalap elfordulása
     * \param bool outline:             A téglalap körvonala
     * \param const std::string &image: A téglalap textúrájának a neve
     * \param lmScalar width:           A téglalap szélessége
     * \param lmScalar height:          A téglalap hosszúsága
     *
     */
    lGooRect(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,liGuiElement *polygon,lmScalar width,lmScalar height);

    /**< Destruktor - nem csinál semmit */
    ~lGooRect();

    void IncreaseWidth(lmScalar dw)
    {
        Width += dw;
    }

    void IncreaseHeight(lmScalar dh)
    {
        Height += dh;
    }

    /** \brief Setter függvény - beállítja a téglalap szélességét
     *
     * \param lmScalar width: a téglalap új szélessége
     * \return void
     *
     */
    virtual void SetWidth(lmScalar width) override;

    /** \brief Setter függvény - beállítja a téglalap hosszúságát
     *
     * \param lmScalar height: a téglalap új hosszúsága
     * \return void
     *
     */
    virtual void SetHeight(lmScalar height) override;

    virtual lmScalar GetWidth() const override
    {
        return Width;
    }

    virtual lmScalar GetHeight() const override
    {
        return Height;
    }

    /** \brief Getter függvény - Megadja az objektumot a helyére transzformáló mátrixot - konstans tagfüggvény
     *
     * \return lmMatrix3x3 Az objektum a helyére transzformáló mátrixa.
     *
     */
    virtual lmMatrix3x3 Transform() const override;

    virtual liGooRect *ToRect() override {return this;}
};

#endif
