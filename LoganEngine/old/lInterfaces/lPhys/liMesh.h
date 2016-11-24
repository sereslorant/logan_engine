#ifndef LI_MESH_H
#define LI_MESH_H

#include "../../lMath/lMath.h"

class liMesh
{
public:
	class liTriangle
	{
	public:
		virtual unsigned int GetVertex(unsigned int i) const = 0;
		virtual unsigned int GetNormal() const = 0;

		liTriangle(){}
		virtual ~liTriangle(){}
	};

    virtual unsigned int GetVerticesSize() const = 0;
    virtual const lmVector3D &GetVertex(unsigned int i) const = 0;

    virtual unsigned int GetNormalsSize() const = 0;
    virtual const lmVector3D &GetNormal(unsigned int i) const = 0;

    virtual unsigned int GetTrianglesSize() const = 0;
    virtual const liTriangle &GetTriangle(unsigned int i) const = 0;

	virtual const lmVector3D &GetPosition() const = 0;
	virtual void SetPosition(const lmVector3D &position) = 0;

	virtual const lmVector3D &GetOrientation() const = 0;
	virtual void SetOrientation(const lmVector3D &orientation) = 0;

	liMesh(){}
	virtual ~liMesh() {}
};

#endif // LI_MESH_H
