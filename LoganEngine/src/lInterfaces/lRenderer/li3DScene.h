#ifndef LI_3D_SCENE_H
#define LI_3D_SCENE_H

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

    //virtual void GetProjectionMatrix(lmMatrix4x4 &Target) const = 0;

    liFrustum()
    {
    	//
    }

    virtual ~liFrustum()
    {
    	//
    }
};

class li3DCamera
{
public:

    //virtual void GetViewMatrix(lmMatrix4x4 &Target) const = 0;

    //virtual void MoveForward(float d) = 0;

    virtual void SetPosition(const lmVector3D &position) = 0;
    virtual void SetYaw(float yaw) = 0;
    virtual void SetPitch(float pitch) = 0;

    virtual const lmVector3D &GetPosition() = 0;
    virtual lmVector3D GetDirection() const = 0;
    virtual float GetYaw() = 0;
    virtual float GetPitch() = 0;

	li3DCamera(){}
	virtual ~li3DCamera(){}
};

#endif // LI_3D_SCENE_H
