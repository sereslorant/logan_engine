#ifndef LI_COLOR_H
#define LI_COLOR_H

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

#endif // LI_COLOR_H
