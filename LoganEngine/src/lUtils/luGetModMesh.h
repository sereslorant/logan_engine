#ifndef LU_GET_MOD_MESH_H
#define LU_GET_MOD_MESH_H

#include <lScene/l3DScene/lModScene/li3DModScene.h>

class luGetModLight : public li3DModElementVisitor
{
private:
	li3DModLight *ModLight = nullptr;

public:

	li3DModLight *GetModLight()
	{
		return ModLight;
	}

	virtual void VisitLight(li3DModLight &light) override
	{
		ModLight = &light;
	}

	virtual void VisitMesh(li3DModMesh &mesh) override
	{
		//
	}

	luGetModLight() {}
	virtual ~luGetModLight() override {}
};

class luGetModMesh : public li3DModElementVisitor
{
private:
	li3DModMesh *ModMesh = nullptr;

public:

	li3DModMesh *GetModMesh()
	{
		return ModMesh;
	}

	virtual void VisitLight(li3DModLight &light) override
	{
		//
	}

	virtual void VisitMesh(li3DModMesh &mesh) override
	{
		ModMesh = &mesh;
	}

	luGetModMesh() {}
	virtual ~luGetModMesh() override {}
};

#endif // LU_GET_MOD_MESH_H
