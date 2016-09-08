#ifndef LI_3D_MOD_ELEMENT_H
#define LI_3D_MOD_ELEMENT_H

#include "../li3DElement.h"

#include "li3DModMesh.h"

class li3DModElementVisitor
{
public:
	//
	virtual void VisitMesh(li3DModMesh &mesh) = 0;
	//
	li3DModElementVisitor(){}
	virtual ~li3DModElementVisitor(){}
};

class li3DModElement : public li3DElement
{
public:
	//
	virtual void Show() = 0;
	virtual void Hide() = 0;
	//
	virtual void Accept(li3DModElementVisitor &visitor) = 0;
	//
	li3DModElement(){}
	virtual ~li3DModElement() override{}
};

#endif // LI_3D_MOD_ELEMENT_H
