#ifndef LI_3D_MESH_H
#define LI_3D_MESH_H

#include <lCore/lMath/lMath.h>

#include <string>

class li3DStaticMesh
{
public:
	//
	virtual const std::string &GetModelName() const = 0;
	//
	li3DStaticMesh()
	{}
	//
	virtual ~li3DStaticMesh()
	{}
};

class li3DSkeletalMesh
{
public:
	//
	virtual const std::string &GetCharacterName() const = 0;
	//
	li3DSkeletalMesh()
	{}
	//
	virtual ~li3DSkeletalMesh()
	{}
};

class li3DMeshVisitor
{
public:
	//
	virtual void VisitStaticMesh(const li3DStaticMesh &static_mesh) = 0;
	virtual void VisitSkeletalMesh(const li3DSkeletalMesh &skeletal_mesh) = 0;
	//
	li3DMeshVisitor(){}
	virtual ~li3DMeshVisitor(){}
};

#include "liMaterialLibrary.h"

class li3DMesh
{
public:
	//
	virtual const lmVector3D &GetPosition() const = 0;
	virtual const lmQuaternion &GetOrientation() const = 0;
	virtual const lmVector3D &GetScale() const = 0;
	//
	virtual const liMaterialLibrary &GetMaterialLibrary() const = 0;
	//
	virtual void Accept(li3DMeshVisitor &visitor) const = 0;
	//
	li3DMesh(){}
	virtual ~li3DMesh(){}
};

#endif // LI_3D_MESH_H
