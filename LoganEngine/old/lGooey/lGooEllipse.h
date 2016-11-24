#ifndef L_GOO_ELLIPSE_H
#define L_GOO_ELLIPSE_H

#include "lGooObject.h"

class lGooEllipse : public lGooObject, public liGooEllipse
{
public:

    typedef void (lGooEllipse::* AttribSetter) (lmScalar value);
    typedef lmScalar (lGooEllipse::* AttribGetter) () const;

    struct lGooEllipseAttrib
    {
    	AttribSetter SetValue;
    	AttribSetter IncValue;
    	AttribGetter GetValue;

    	lGooEllipseAttrib(AttribSetter set_value,AttribSetter inc_value,AttribGetter get_value)
			:SetValue(set_value),IncValue(inc_value),GetValue(get_value)
    	{}
    };

protected:
    lmScalar RadiusX;
    lmScalar RadiusY;

public:

	lGooEllipse(liGuiElement *polygon)
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
    lGooEllipse(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,liGuiElement *polygon,lmScalar radius_x,lmScalar radius_y)
		:lGooObject(pos,scale,orientation,polygon),RadiusX(radius_x),RadiusY(radius_y)
	{

	}

    /**< Destruktor - nem csinál semmit */
    ~lGooEllipse()
    {

    }

    void IncreaseRadiusX(lmScalar dr)
    {
        RadiusX += dr;
    }

    void IncreaseRadiusY(lmScalar dr)
    {
        RadiusY += dr;
    }

    virtual lmScalar GetRadiusX() const override
    {
    	return RadiusX;
    }

    virtual lmScalar GetRadiusY() const override
    {
    	return RadiusY;
    }

    /** \brief Setter függvény - beállítja a kör sugarát
     *
     * \param lmScalar width: a kör új sugara
     * \return void
     *
     */
    virtual void SetRadiusX(lmScalar radius_x) override
    {
		RadiusX = radius_x;
	}

    virtual void SetRadiusY(lmScalar radius_y) override
    {
		RadiusY = radius_y;
	}

    /** \brief Getter függvény - Megadja az objektumot a helyére transzformáló mátrixot - konstans tagfüggvény
     *
     * \return lmMatrix3x3 Az objektum a helyére transzformáló mátrixa.
     *
     */
    virtual lmMatrix3x3 Transform() const override
	{
		lmMatrix3x3 mTranslate  = lmTranslate3x3(Position);
		lmMatrix3x3 mRotate     = lmRotate3x3(lmDegToRad(Orientation));
		lmMatrix3x3 mScale      = lmScale3x3(Scale[0]*RadiusX,Scale[1]*RadiusY);

		if(Parent != nullptr)
		{
			return Parent->Transform() * mTranslate * mRotate * mScale;
		}
		else
		{
			return mTranslate * mRotate * mScale;
		}
	}

	virtual liGooEllipse *ToEllipse() override
	{return this;}
};

#endif // L_GOO_ELLIPSE_H
