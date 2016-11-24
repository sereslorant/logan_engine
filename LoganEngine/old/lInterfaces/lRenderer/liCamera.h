#ifndef LI_CAMERA_H
#define LI_CAMERA_H

#include "../../lMath/lMath.h"

class liCamera
{
public:

    virtual void GetViewMatrix(lmMatrix4x4 &Target) const = 0;

    virtual lmVector3D GetDirection() const = 0;

    virtual void MoveForward(float d) = 0;

    virtual void SetPosition(const lmVector3D &position) = 0;
    virtual void SetYaw(float yaw) = 0;
    virtual void SetPitch(float pitch) = 0;

    virtual const lmVector3D &GetPosition() = 0;
    virtual float GetYaw() = 0;
    virtual float GetPitch() = 0;

	liCamera(){}
	virtual ~liCamera(){}
};

#endif // LI_CAMERA_H
