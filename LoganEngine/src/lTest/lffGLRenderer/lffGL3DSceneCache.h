#ifndef LFF_GL_3D_SCENE_CACHE_H
#define LFF_GL_3D_SCENE_CACHE_H

#include "lGLIncludes.h"
#include "lffGLResourceLoader.h"

struct lffGL3DMeshInstances
{
	bool MeshInitialized = false;
	lffGLStaticMesh::lffGLMaterialGroup MtlGroup;
	/*
	 * Instance-ökhöz tartozó adatok.
	 */
	unsigned int Next = 0;
	std::vector<lffGLMaterial> Materials;
	std::vector<lmMatrix4x4> MvMatrices;
	//
	void AssignMtlGroup(lffGLStaticMesh &static_mesh,unsigned int index)
	{
		if(!MeshInitialized)
		{
			MtlGroup = static_mesh.GetMtlGroup(index);
			MeshInitialized = true;
		}
	}
	//
	void Resize(unsigned int size)
	{
		Materials.resize(size);
		MvMatrices.resize(size);
		Next = 0;
	}
	//
	void FillNext(const liMaterial &material,const lmMatrix4x4 &mv_matrix)
	{
		if(Next < Materials.size())
		{
			Materials[Next].SetMaterial(material);
			MvMatrices[Next] = mv_matrix;
			Next++;
		}
	}
};

struct lffGL3DTextureList
{
	std::string DiffuseMap;
	//
	std::map<std::string,lffGL3DMeshInstances *> Szercsy;
	std::vector<lffGL3DMeshInstances> MeshInstances;
};

struct lffGL3DSceneCache
{
	std::map<std::string,lffGL3DTextureList *> Puszcsy;
	std::vector<lffGL3DTextureList> TextureLists;
};

class lr3DSceneReader : public li3DSceneDrawer, public li3DElementVisitor
{
public:
	//
	class lr3DMeshReader : public li3DMeshVisitor
	{
	private:
		lffGLResourceLoader &ResourceLoader;
		//
	public:
		//
		std::map<std::string,std::map<std::string,unsigned int> > StaticMeshCount;
		const li3DMesh *Mesh = nullptr;
		//
		virtual void VisitStaticMesh(const li3DStaticMesh &static_mesh) override
		{
			lffGLStaticMesh *StaticMesh = ResourceLoader.GetStaticMesh(static_mesh.GetModelName());
			for(unsigned int i=0;i < StaticMesh->GetNumMtlGroups();i++)
			{
				lffGLStaticMesh::lffGLMaterialGroup MtlGroup = StaticMesh->GetMtlGroup(i);
				//
				std::string Texture		= Mesh->GetMaterialLibrary().GetMaterial(MtlGroup.GetMaterial()).GetDiffuseMap();
				std::string ModelName	= static_mesh.GetModelName() + "_" + std::to_string(i);
				//
				StaticMeshCount[Texture][ModelName]++;
			}
		}
		//
		lr3DMeshReader(lffGLResourceLoader &resource_loader)
			:ResourceLoader(resource_loader)
		{}
		//
		virtual ~lr3DMeshReader() override
		{}
	};
	//
	lr3DMeshReader MeshReader;
	//
	virtual void VisitMesh(const li3DMesh &mesh) override
	{
		MeshReader.Mesh = &mesh;
		//
		mesh.Accept(MeshReader);
	}
	//
	virtual void Draw(const li3DElement &element) override
	{
		element.Accept(*this);
	}
	//
	void Print()
	{
		std::cout << "StaticMeshes:" << std::endl;
		for(auto I : MeshReader.StaticMeshCount)
		{
			std::cout << "Texture: " << I.first << "\n";
			for(auto J : I.second)
			{
				std::cout << "\tMesh: " << J.first << "\n\tCount: " << J.second << "\n\n";
			}
		}
		std::cout << std::endl;
	}
	//
	lr3DSceneReader(lffGLResourceLoader &resource_loader)
		:MeshReader(resource_loader)
	{}
	//
	virtual ~lr3DSceneReader() override
	{}
};

class lr3DSceneCacher : public li3DSceneDrawer, public li3DElementVisitor
{
public:
	//
	lr3DSceneCacher(){}
	//
	virtual ~lr3DSceneCacher() override {}
};

#endif // LFF_GL_3D_SCENE_CACHE_H
