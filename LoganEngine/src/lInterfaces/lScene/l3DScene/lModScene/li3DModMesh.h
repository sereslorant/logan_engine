#ifndef LI_3D_MOD_MESH_H
#define LI_3D_MOD_MESH_H

#include "../li3DMesh.h"

class li3DModStaticMesh : public li3DStaticMesh
{
public:
	//
	virtual void SetModelname(std::string &model_name) = 0;
	//
	li3DModStaticMesh()
	{}
	//
	virtual ~li3DModStaticMesh() override
	{}
};

class li3DModMeshVisitor
{
public:
	//
	virtual void VisitStaticMesh(li3DModStaticMesh &static_mesh) = 0;
	//
	li3DModMeshVisitor(){}
	virtual ~li3DModMeshVisitor(){}
};

class li3DModMesh : public li3DMesh
{
public:
	//
	virtual void SetPosition(const lmVector3D &position) = 0;
	virtual void SetOrientation(const lmQuaternion &quaternion) = 0;
	virtual void SetScale(const lmVector3D &scale) = 0;
	//
	virtual void SetMaterialLibrary(const liMaterialLibrary &material_library) = 0;
	//
	virtual void Accept(li3DModMeshVisitor &visitor) = 0;
	//
	li3DModMesh(){}
	virtual ~li3DModMesh() override{}
};

#endif // LI_3D_MOD_MESH_H
