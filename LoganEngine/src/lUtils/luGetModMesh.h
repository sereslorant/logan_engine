#ifndef LU_GET_MOD_MESH_H
#define LU_GET_MOD_MESH_H

#include "../lInterfaces/lScene/l3DScene/lModScene/li3DModScene.h"

class luGetModMesh : public li3DModElementVisitor
{
private:
	li3DModMesh *ModMesh = nullptr;

public:

	li3DModMesh *GetModMesh()
	{
		return ModMesh;
	}

	virtual void VisitMesh(li3DModMesh &mesh) override
	{
		ModMesh = &mesh;
	}

	luGetModMesh() {}
	virtual ~luGetModMesh() override {}
};

#endif // LU_GET_MOD_MESH_H
