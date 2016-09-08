#ifndef LI_3D_CAMERA_H
#define LI_3D_CAMERA_H

class liFrustum
{
public:
	//
    virtual float GetTop() const = 0;
    virtual float GetBottom() const = 0;
    virtual float GetLeft() const = 0;
    virtual float GetRight() const = 0;
    virtual float GetFar() const = 0;
    virtual float GetNear() const = 0;
    //
    liFrustum()
    {
    	//
    }
    //
    virtual ~liFrustum()
    {
    	//
    }
};

class li3DCamera
{
public:
    //
    virtual const lmVector3D &GetPosition() const = 0;
    virtual lmVector3D GetDirection() const = 0;
    virtual float GetYaw() const = 0;
    virtual float GetPitch() const = 0;
    //
	li3DCamera(){}
	virtual ~li3DCamera(){}
};

#endif // LI_3D_MOD_CAMERA_H
