#ifndef LR_POINT_LIGHT_H
#define LR_POINT_LIGHT_H

#include "../lInterfaces/lRenderer/liPointLight.h"

class lrPointLight : public liPointLight
{
private:
	float Ambient[3];
	float Diffuse[3];
	float Specular[3];

	// Attenuation [0] is the constant
	// Attenuation [1] is the linear
	// Attenuation [2] is the quadratic
	float Attenuation[3];

	lmVector3D Position;

	int Enabled;
	bool Removable;

public:

	virtual float *GetAmbient() override
	{
		return Ambient;
	}

	virtual float *GetDiffuse() override
	{
		return Diffuse;
	}

	virtual float *GetSpecular() override
	{
		return Specular;
	}

	virtual float *GetAttenuation() override
	{
		return Attenuation;
	}

	virtual const lmVector3D &GetPosition() override
	{
		return Position;
	}

	virtual void SetPosition(const lmVector3D &position) override
	{
		Position = position;
	}

	virtual void Enable() override
	{
		Enabled = 1;
	}
	virtual void Disable() override
	{
		Enabled = 0;
	}

	virtual int IsEnabled() override
	{
		return Enabled;
	}



	virtual void MarkRemovable() override {Removable = true;}
	virtual bool IsRemovable() override {return Removable;}

	void Recreate(float ambient[],float diffuse[],float specular[],float attenuation[],const lmVector3D &position)
	{
		Position = position;

		for(int i=0;i < 3;i++)
		{
			Ambient[i]	= ambient[i];
			Diffuse[i]	= diffuse[i];
			Specular[i]	= specular[i];
			Attenuation[i] = attenuation[i];
		}

		Enabled = 1;
		Removable = false;
	}

	lrPointLight(const lrPointLight &p)
		:Position(p.Position)
	{
		for(int i=0;i < 3;i++)
		{
			Ambient[i]	= p.Ambient[i];
			Diffuse[i]	= p.Diffuse[i];
			Specular[i]	= p.Specular[i];
			Attenuation[i] = p.Attenuation[i];
		}
	}

	lrPointLight(float ambient[],float diffuse[],float specular[],float attenuation[],const lmVector3D &position)
		:Position(position)
	{
		/*
		for(int i=0;i < 3;i++)
		{
			Ambient[i]	= ambient[i];
			Diffuse[i]	= diffuse[i];
			Specular[i]	= specular[i];
			Attenuation[i] = attenuation[i];
		}
		*/
		Recreate(ambient,diffuse,specular,attenuation,position);
	}

	virtual ~lrPointLight() override
	{

	}
};

#endif // LR_POINT_LIGHT_H
