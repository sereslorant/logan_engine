#ifndef LR_COLOR_H
#define LR_COLOR_H

#include "../lInterfaces/lScene/liColor.h"

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

    virtual void operator = (const liColor &color) //override
    {
    	Red		= color.GetRed();
    	Green	= color.GetGreen();
    	Blue	= color.GetBlue();
    	Alpha	= color.GetAlpha();
    }

    lrColor(float red,float green,float blue,float alpha)
        :Red(red),Green(green),Blue(blue),Alpha(alpha)
    {
		//Üres függvénytörzs
    }

    lrColor(const liColor &c)
		:Red(c.GetRed()),Green(c.GetGreen()),Blue(c.GetBlue()),Alpha(c.GetAlpha())
    {
		//Üres függvénytörzs
    }

    virtual ~lrColor() override
    {
		//Üres függvénytörzs
    }
};

#endif
