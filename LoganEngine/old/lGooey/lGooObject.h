#ifndef L_GOO_OBJECT_H
#define L_GOO_OBJECT_H

#include "../lMath/lMath.h"

#include "lGooColor.h"
#include "../lRenderer/liGuiElement.h"


#include <string>
#include <list>
#include <map>

#include "liGooEvent.h"


#include "liGooObject.h"

//class lGooRenderer;

class lGooSolidColorData : public liGuiSolidColor
{
private:
	lGooColor Color;

public:

	virtual void SetColor(const liColor &color) {Color.Copy(color);}
	virtual const liColor &GetColor() const {return Color;}

	lGooSolidColorData(){}
	virtual ~lGooSolidColorData(){}
};

/** \class lGooObject
 * \brief Egy kirajzolható objektumot modellező osztály
 *
 * \author Seres Lóránt
 *
 */
class lGooObject : public liGooObject
{
protected:

    //lGooObjectType Type; /**< Típus - Pl. téglalap, gömb, stb.*/

    lmVector2D Position		= {0.0,0.0};	/**< Pozíció */
    lmVector2D Scale		= {1.0,1.0};	/**< Nyújtás az X-Y tengely mentén */
    lmScalar Orientation	=  0.0;			/**< Elfordulás */

    lGooColor Color = {0.0,0.0,0.0,0.0}; /**< Szín */

    bool Textured = false;
    std::string Image; /**< Textúra neve */

    bool Outline = false; /**< Körvonal */
    float OutlineWidth =  0.0;
    lGooColor OutlineColor = {0.0,0.0,0.0,0.0}; /**< Körvonal színe */

    lGooSolidColorData SolidColor;
    lGooSolidColorData SolidOutline;

    liGooObject *Parent = nullptr;

    std::list<lGooAnimation *> Animations;
    std::map<std::string,lGooAnimation *> AnimIds;

    // !!!!!!!!!!!!!!!!!!!!!!!!!TMP!!!!!!!!!!!!!!!!!!!!!!!!!
    liGuiElement *Polygon = nullptr;

    liGuiBrush *FillBrush	= nullptr;
    liGuiBrush *StrokeBrush	= nullptr;
    // !!!!!!!!!!!!!!!!!!!!!!!!!TMP!!!!!!!!!!!!!!!!!!!!!!!!!

public:

	lGooObject(liGuiElement *polygon)
		:Polygon(polygon)
	{

	}

    /** \brief Konstruktor
     *
     * \param const lmVector2D &pos:    A téglalap pozíciója
     * \param const lmVector2D &scale:  A téglalap nyújtása
     * \param lmScalar orientation:     A téglalap elfordulása
     * \param bool outline:             A téglalap körvonala
     * \param const std::string &image: A téglalap textúrájának a neve
     * \param lGooObjectType type:      Az objektum típusa
     *
     */
    lGooObject(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,liGuiElement *polygon);

    /**< Virtuális destruktor */
    virtual ~lGooObject();

	virtual const lmVector2D &GetPosition() const override
	{return Position;}

	virtual const lmVector2D &GetScale() const override
	{return Scale;}

	virtual float GetOrientation() const override
	{return Orientation;}

    virtual void SetPosition(const lmVector2D &position) override
    {Position = position;}

    virtual void SetScale(const lmVector2D &scale) override
    {Scale = scale;}

    virtual void SetOrientation(float orientation) override
    {Orientation = orientation;}

    //void MoveV(const lmVector2D &d);
    //void ScaleV(const lmVector2D &scale);

    /** \brief Elmozdítja az objektumunkat az X tengely mentén
     *
     * \param lmScalar X: Az elmozdulás nagysága
     * \return void
     *
     */
    void MoveX(lmScalar dX);

     /** \brief Elmozdítja az objektumunkat az Y tengely mentén
     *
     * \param lmScalar Y: Az elmozdulás nagysága
     * \return void
     *
     */
    void MoveY(lmScalar dY);

    /** \brief Megnyújtja az objektumunkat az X tengely mentén
     *
     * \param lmScalar X: A megnyújtás nagysága
     * \return void
     *
     */
    void IncreaseScaleX(lmScalar X);

    /** \brief Megnyújtja az objektumunkat az Y tengely mentén
     *
     * \param lmScalar Y: A megnyújtás nagysága
     * \return void
     *
     */
    void IncreaseScaleY(lmScalar Y);

    /** \brief Elforgatja az objektumot
     *
     * \param lmScalar Alpha: Az elfordulás szöge
     * \return void
     *
     */
    //void Rotate(lmScalar Alpha);

    /** \brief Setter függvény - beállítja az objektum pozíciójának X koordinátáját.
     *
     * \param lmScalar X: Az új X koordinátája
     * \return void
     *
     */
    void SetX(lmScalar X);

    /** \brief Setter függvény - beállítja az objektum pozíciójának Y koordinátáját.
     *
     * \param lmScalar Y: Az új Y koordinátája
     * \return void
     *
     */
    void SetY(lmScalar Y);

    /** \brief Setter függvény - beállítja az objektum X tengely menti nyújtását.
     *
     * \param lmScalar X: Az új X menti nyújtás
     * \return void
     *
     */
    void SetScaleX(lmScalar X);

    /** \brief Setter függvény - beállítja az objektum Y tengely menti nyújtását.
     *
     * \param lmScalar Y: Az új Y menti nyújtás
     * \return void
     *
     */
    void SetScaleY(lmScalar Y);

    /** \brief Setter függvény - beállítja az objektum elfordulását.
     *
     * \param lmScalar Alpha: Az új elfordulás
     * \return void
     *
     */
    //void SetOrientation(lmScalar Alpha);

	virtual const liGooColor &GetColor() const override
	{
		return Color;
	}

    virtual void SetColor(const liGooColor &color) override
    {
		Color = color;
	}

	virtual void SetTextured(bool textured) override
	{
        Textured = textured;
	}

	virtual bool IsTextured() const override
	{
        return Textured;
	}

	virtual void SetTexImage(const std::string &image) override
	{
        Image = image;
	}

    virtual const std::string &GetTexImage() const override
    {
		return Image;
	}

    virtual bool HasOutline() const override
    {
		return Outline;
	}

    virtual const liGooColor &GetOutlineColor() const override
    {
		return OutlineColor;
	}

	virtual float GetOutlineWidth() const override
    {
    	return OutlineWidth;
    }

	virtual void SetOutline(bool outline) override
	{
        Outline = outline;
	}

    virtual void SetOutlineColor(const liGooColor &outline_color) override
    {
		OutlineColor = outline_color;
	}

    virtual void SetOutlineWidth(float outline_width) override
    {
    	OutlineWidth = outline_width;
    }

    // /*
    lmScalar GetX() const
    {
        return Position[0];
    }

    lmScalar GetY() const
    {
        return Position[1];
    }

    lmScalar GetScaleX()
    {
        return Scale[0];
    }

    lmScalar GetScaleY()
    {
        return Scale[1];
    }

    lmScalar GetOrientation()
    {
        return Orientation;
    }
    // */


    /** \brief Getter függvény - Visszaadja az objektum textúrájának nevét - konstans tagfüggvény
     *
     * \return const std::string & - Az objektum textúrájának a neve.
     *
     */
    //const std::string &GetImage() const;

    /** \brief Getter függvény - megadja, hogy az objektumnak van-e körvonala - konstans tagfüggvény
     *
     * \return bool - true, ha az objektumnak be van kapcsolva a körvonal, false egyébként.
     *
     */
	/*
    bool GetOutline() const;

    const lGooColor &GetOutlineColor() const
    {
		return OutlineColor;
	}

	lGooFloat GetOutlineWidth()
    {
    	return OutlineWidth;
    }
    */


    virtual liGuiSolidColor &GetSolidColor() override
    {
    	return SolidColor;
    }

    virtual liGuiSolidColor &GetSolidOutline() override
    {
    	return SolidOutline;
    }

    virtual liGuiElement *GetPolygon() override
    {
        return Polygon;
    }

    virtual liGuiBrush *GetFillBrush() override
    {
    	return FillBrush;
    }

    virtual void SetFillBrush(liGuiBrush *fill_brush) override
    {
    	FillBrush = fill_brush;
    }

    virtual liGuiBrush *GetStrokeBrush() override
    {
    	return StrokeBrush;
    }

    virtual void SetStrokeBrush(liGuiBrush *stroke_brush) override
    {
    	StrokeBrush = stroke_brush;
    }

    virtual void AddToGUI(liGooGUI *gui) override
    {
		gui->AddObject(this);
    }


    //virtual void AddToRenderer(lGooRenderer *Renderer);

    /** \brief Getter függvény - Megadja az objektumot a helyére transzformáló mátrixot - konstans tagfüggvény
     * Tisztán virtuális tagfüggvény
     *
     * \return lmMatrix3x3 Az objektumot a helyére transzformáló mátrixa.
     *
     */
    //virtual lmMatrix3x3 Transform() const = 0;

    virtual void SetParent(liGooObject *new_parent) override
    {
    	Parent = new_parent;
    }

    virtual void AddAnimation(lGooAnimation *animation) override
    {
    	Animations.push_back(animation);
    }

    virtual void AddAnimationId(const std::string &id,lGooAnimation *animation) override
    {
    	auto I = AnimIds.find(id);

    	if(I == AnimIds.end())
		{
			AnimIds[id] = animation;
		}
    }

    virtual lGooAnimation *GetAnimation(const std::string &id) override
    {
    	auto I = AnimIds.find(id);

    	if(I == AnimIds.end())
		{
			return nullptr;
		}

		return I->second;
    }

    virtual const lGooAnimation *GetAnimation(const std::string &id) const override
    {
    	auto I = AnimIds.find(id);

    	if(I == AnimIds.end())
		{
			return nullptr;
		}

		return I->second;
    }

    virtual void Step(lmScalar dt) override
    {
    	SolidColor.SetColor(Color);
    	SolidOutline.SetColor(OutlineColor);

    	for(auto I : Animations)
		{
			I->Step(dt);
		}
    }

    virtual liGooContainer *ToContainer() override
    {return nullptr;}

    virtual liGooGUI *ToGUI() override
    {return nullptr;}

    virtual liGooRect *ToRect() override
    {return nullptr;}

    virtual liGooCircle *ToCircle() override
    {return nullptr;}

    virtual liGooLine *ToLine() override
    {return nullptr;}

    virtual liGooEllipse *ToEllipse() override
    {return nullptr;}
};

#endif
