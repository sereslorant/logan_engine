#ifndef LI_3D_ELEMENT_H
#define LI_3D_ELEMENT_H

#include "li3DMesh.h"

class li3DElementVisitor
{
public:
	//
	virtual void VisitMesh(const li3DMesh &mesh) = 0;
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
