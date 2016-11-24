#ifndef LI_FRUSTUM_H
#define LI_FRUSTUM_H

#include "../../lMath/lMath.h"

class liFrustum
{
public:

    virtual float GetTop() = 0;
    virtual float GetBottom() = 0;
    virtual float GetLeft() = 0;
    virtual float GetRight() = 0;
    virtual float GetFar() = 0;
    virtual float GetNear() = 0;

    virtual void SetTop(float top) = 0;
    virtual void SetBottom(float bottom) = 0;
    virtual void SetLeft(float left) = 0;
    virtual void SetRight(float right) = 0;
    virtual void SetFar(float far) = 0;
    virtual void SetNear(float near) = 0;

    virtual void GetProjectionMatrix(lmMatrix4x4 &Target) const = 0;

    liFrustum()
    {

    }

    virtual ~liFrustum()
    {

    }
};

#endif // LI_FRUSTUM_H
