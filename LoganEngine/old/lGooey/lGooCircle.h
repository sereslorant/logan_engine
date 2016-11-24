#ifndef L_GOO_CIRCLE_H
#define L_GOO_CIRCLE_H

#include "lGooObject.h"

/** \brief Egy kirajzolható kört modellező osztály.
 *
 * \author Seres Lóránt
 *
 */
class lGooCircle : public lGooObject, public liGooCircle
{
public:

    typedef void (lGooCircle::* AttribSetter) (lmScalar value);
    typedef lmScalar (lGooCircle::* AttribGetter) () const;

    struct lGooCircleAttrib
    {
    	AttribSetter SetValue;
    	AttribSetter IncValue;
    	AttribGetter GetValue;

    	lGooCircleAttrib(AttribSetter set_value,AttribSetter inc_value,AttribGetter get_value)
			:SetValue(set_value),IncValue(inc_value),GetValue(get_value)
    	{}
    };

protected:
    lmScalar Radius; /**< A kör sugara */

    static const std::map<std::string,lGooCircleAttrib> CircleAttribs;

public:

    lGooCircle(liGuiElement *polygon)
		:lGooObject(polygon)
    {

    }

    /** \brief Konstruktor
     *
     * \param const lmVector2D &pos:    A kör pozíciója
     * \param const lmVector2D &scale:  A kör nyújtása
     * \param lmScalar orientation:     A kör elfordulása
     * \param bool outline:             A kör körvonala
     * \param const std::string &image: A kör textúrájának a neve
     * \param lmScalar sugara:           A kör sugara
     *
     */
    lGooCircle(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,liGuiElement *polygon,lmScalar radius);

    /**< Destruktor - nem csinál semmit */
    virtual ~lGooCircle() override;

    void IncreaseRadius(lmScalar dr)
    {
        Radius += dr;
    }

    virtual lmScalar GetRadius() const override
    {
    	return Radius;
    }

    /** \brief Setter függvény - beállítja a kör sugarát
     *
     * \param lmScalar width: a kör új sugara
     * \return void
     *
     */
    virtual void SetRadius(lmScalar radius) override;

    /** \brief Getter függvény - Megadja az objektumot a helyére transzformáló mátrixot - konstans tagfüggvény
     *
     * \return lmMatrix3x3 Az objektum a helyére transzformáló mátrixa.
     *
     */
    virtual lmMatrix3x3 Transform() const override;

    virtual liGooCircle *ToCircle() override
    {return this;}
};

#endif
