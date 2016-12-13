#ifndef LI_3D_ELEMENT_H
#define LI_3D_ELEMENT_H

#include "li3DMesh.h"

#include "../liColor.h"

class li3DLight
{
public:
    //
    virtual const lmVector3D &GetPosition() const = 0;
    //
    virtual const liColor &GetColor() const = 0;
	//
    virtual float GetIntensity() const = 0;
    //
	//virtual float GetAmbient(lColorIndex index) const = 0;
	//
	//virtual float GetDiffuse(lColorIndex index) const = 0;
	//
	//virtual float GetSpecular(lColorIndex index) const = 0;
	//
	li3DLight(){}
	virtual ~li3DLight(){}
};

class li3DElementVisitor
{
public:
	//
	virtual void VisitMesh(const li3DMesh &mesh) = 0;
	virtual void VisitLight(const li3DLight &light) = 0;
	//
	li3DElementVisitor(){}
	virtual ~li3DElementVisitor(){}
};

class li3DElement
{
public:
	//
	virtual bool IsHidden() const = 0;
	//
	virtual void Accept(li3DElementVisitor &visitor) const = 0;
	//
	li3DElement(){}
	virtual ~li3DElement(){}
};

#endif // LI_3D_ELEMENT_H
