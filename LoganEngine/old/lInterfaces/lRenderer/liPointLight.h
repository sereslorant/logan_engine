#ifndef LI_POINT_LIGHT_H
#define LI_POINT_LIGHT_H

class liPointLight
{
public:

	virtual float *GetAmbient() = 0;
	virtual float *GetDiffuse() = 0;
	virtual float *GetSpecular() = 0;

	virtual float *GetAttenuation() = 0;

	virtual const lmVector3D &GetPosition() = 0;
	virtual void SetPosition(const lmVector3D &position) = 0;

	virtual void Enable() = 0;
	virtual void Disable() = 0;

	virtual int IsEnabled() = 0;

	virtual void MarkRemovable() = 0;
	virtual bool IsRemovable() = 0;

	liPointLight(){}
	virtual ~liPointLight(){}
};

#endif // LI_POINT_LIGHT_H
