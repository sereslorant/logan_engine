#ifndef LR_FRUSTUM_H
#define LR_FRUSTUM_H

#include "../lInterfaces/lRenderer/liFrustum.h"

/** \brief A játékos által látott térrészt leíró class. Arra való, hogy visszaadjon egy vetítőmátrixot.
 *
 * \todo Rendesen dokumentálni kéne
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrFrustum : public liFrustum
{
private:
    float Top;
    float Bottom;
    float Left;
    float Right;
    float Far;
    float Near;

public:

    virtual float GetTop() override
    {return Top;}

    virtual float GetBottom() override
    {return Bottom;}

    virtual float GetLeft() override
    {return Left;};

    virtual float GetRight() override
    {return Right;}

    virtual float GetFar() override
    {return Far;}

    virtual float GetNear() override
    {return Near;}

    virtual void SetTop(float top) override
    {Top = top;}

    virtual void SetBottom(float bottom) override
    {Bottom = bottom;}

    virtual void SetLeft(float left) override
    {Left = left;}

    virtual void SetRight(float right) override
    {Right = right;}

    virtual void SetFar(float far) override
    {Far = far;}

    virtual void SetNear(float near) override
    {Near = near;}

    virtual void GetProjectionMatrix(lmMatrix4x4 &Target) const
    {
        lmFrustum4x4(Target,Top,Bottom,Left,Right,Far,Near);
    }

    lrFrustum(float top,float bottom,float left,float right,float far,float near)
        :Top(top),Bottom(bottom),Left(left),Right(right),Far(far),Near(near)
    {

    }

    virtual ~lrFrustum() override
    {

    }
};

#endif // LR_FRUSTUM_H
