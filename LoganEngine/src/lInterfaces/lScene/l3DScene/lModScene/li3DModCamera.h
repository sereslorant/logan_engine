#ifndef LI_3D_MOD_CAMERA_H
#define LI_3D_MOD_CAMERA_H

#include "../li3DCamera.h"

class liModFrustum : public liFrustum
{
public:
    //
    virtual void SetTop(float top) = 0;
    virtual void SetBottom(float bottom) = 0;
    virtual void SetLeft(float left) = 0;
    virtual void SetRight(float right) = 0;
    virtual void SetFar(float far) = 0;
    virtual void SetNear(float near) = 0;
    //
    liModFrustum()
    {
    	//
    }
    //
    virtual ~liModFrustum() override
    {
    	//
    }
};

class li3DModCamera : public li3DCamera
{
public:
	//
    virtual void SetPosition(const lmVector3D &position) = 0;
    virtual void SetYaw(float yaw) = 0;
    virtual void SetPitch(float pitch) = 0;
    //
	li3DModCamera(){}
	virtual ~li3DModCamera() override
	{
		//
	}
};

#endif // LI_3D_MOD_CAMERA_H
