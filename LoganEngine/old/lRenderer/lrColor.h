#ifndef LR_COLOR_H
#define LR_COLOR_H

class liColor
{
public:

    virtual float GetRed() const = 0;
    virtual float GetGreen() const = 0;
    virtual float GetBlue() const = 0;
    virtual float GetAlpha() const = 0;

    virtual void SetRed(float red) = 0;
    virtual void SetGreen(float green) = 0;
    virtual void SetBlue(float blue) = 0;
    virtual void SetAlpha(float alpha) = 0;

    liColor()
    {

    }
    virtual ~liColor()
    {

    }
};

/** \todo Dokumentálni kéne.
 *
 * \author Seres Lóránt Gábor
 *
 */
struct lrColor : public liColor
{
    float Red;
    float Green;
    float Blue;
    float Alpha;

    virtual float GetRed() const override {return Red;}
    virtual float GetGreen() const override {return Green;}
    virtual float GetBlue() const override {return Blue;}
    virtual float GetAlpha() const override {return Alpha;}

    virtual void SetRed(float red) override {Red = red;}
    virtual void SetGreen(float green) override {Green = green;}
    virtual void SetBlue(float blue) override {Blue = blue;}
    virtual void SetAlpha(float alpha) override {Alpha = alpha;}

    lrColor(float red,float green,float blue,float alpha)
        :Red(red),Green(green),Blue(blue),Alpha(alpha)
    {

    }

    virtual ~lrColor() override
    {

    }
};

#endif // LR_COLOR_H
