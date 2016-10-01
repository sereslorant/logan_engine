#ifndef LFF_GL_3D_SCENE_CACHE_H
#define LFF_GL_3D_SCENE_CACHE_H

#include "lGLIncludes.h"

#include "lffGLUtils.h"

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
	unsigned int NumInstances()
	{
		return MvMatrices.size();
	}
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
		if(Next < NumInstances())
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
	std::map<std::string,lffGL3DMeshInstances *> ModelDictionary;
	std::vector<lffGL3DMeshInstances> MeshInstances;
};

struct lffGL3DSceneCache
{
	std::map<std::string,lffGL3DTextureList *> TextureDictionary;
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
				std::string ModelName;
				lffGLUtils::ComputeMatGroupName(static_mesh.GetModelName(),i,ModelName);
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
	void InitializeSceneCache(lffGL3DSceneCache &scene_cache)
	{
		scene_cache.TextureLists.resize(MeshReader.StaticMeshCount.size());
		unsigned int i=0;
		//
		for(auto I : MeshReader.StaticMeshCount)
		{
			lffGL3DTextureList &TextureList = scene_cache.TextureLists[i];
			//
			TextureList.DiffuseMap = I.first;
			TextureList.MeshInstances.resize(I.second.size());
			unsigned int j=0;
			//
			for(auto J : I.second)
			{
				lffGL3DMeshInstances &MeshInstance = TextureList.MeshInstances[j];
				MeshInstance.Resize(J.second);
				TextureList.ModelDictionary[J.first] = &MeshInstance;
				j++;
			}
			scene_cache.TextureDictionary[I.first] = &TextureList;
			i++;
		}
	}
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
	/*
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
	*/
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
private:
	lffGLResourceLoader &ResourceLoader;
	lffGL3DSceneCache &SceneCache;
	//
	class lr3DMeshCacher : public li3DMeshVisitor
	{
	private:
		lffGLResourceLoader &ResourceLoader;
		lffGL3DSceneCache &SceneCache;
		//
		const li3DMesh &Mesh;
		//
	public:
		//
		virtual void VisitStaticMesh(const li3DStaticMesh &static_mesh) override
		{
			lffGLStaticMesh *StaticMesh = ResourceLoader.GetStaticMesh(static_mesh.GetModelName());
			for(unsigned int i=0;i < StaticMesh->GetNumMtlGroups();i++)
			{
				lffGLStaticMesh::lffGLMaterialGroup MtlGroup = StaticMesh->GetMtlGroup(i);
				//
				const liMaterial &Material = Mesh.GetMaterialLibrary().GetMaterial(MtlGroup.GetMaterial());
				lmMatrix4x4 MvMatrix(lmMatrix4x4::IDENTITY);
				lffGLUtils::GetModelMatrix(Mesh,MvMatrix);
				//
				std::string ModelName;
				lffGLUtils::ComputeMatGroupName(static_mesh.GetModelName(),i,ModelName);
				//
				lffGL3DMeshInstances &MeshInstances = *SceneCache.TextureDictionary[Material.GetDiffuseMap()]->ModelDictionary[ModelName];
				//
				if(!MeshInstances.MeshInitialized)
				{
					MeshInstances.MtlGroup = MtlGroup;
					MeshInstances.MeshInitialized = true;
				}
				MeshInstances.FillNext(Material,MvMatrix);
			}
		}
		//
		lr3DMeshCacher(lffGLResourceLoader &resource_loader,lffGL3DSceneCache &scene_cache,const li3DMesh &mesh)
			:ResourceLoader(resource_loader),SceneCache(scene_cache),Mesh(mesh)
		{
			//
		}
	};
	//
public:
	//
	virtual void VisitMesh(const li3DMesh &mesh) override
	{
		lr3DMeshCacher MeshCacher(ResourceLoader,SceneCache,mesh);
		//
		mesh.Accept(MeshCacher);
	}
	//
	virtual void Draw(const li3DElement &element) override
	{
		element.Accept(*this);
	}
	//
	//*
	void Print()
	{
		std::cout << "StaticMeshes:" << std::endl;
		for(auto I : SceneCache.TextureDictionary)
		{
			std::cout << "Texture: " << I.first << "\n";
			lffGL3DTextureList &TextureList = *I.second;
			for(auto J : TextureList.ModelDictionary)
			{
				std::cout << "\tMesh: " << J.first << "\n";
				lffGL3DMeshInstances &MeshInstances = *J.second;
				for(int i=0;i < MeshInstances.Materials.size();i++)
				{
					std::cout << "\t\tMaterial:\n";
					std::cout << "\t\t" << MeshInstances.Materials[i].Diffuse[0] << ";" << MeshInstances.Materials[i].Diffuse[1] << ";" <<MeshInstances.Materials[i].Diffuse[2] << "\n";
					std::cout << "\t\t" << MeshInstances.Materials[i].Specular[0] << ";" << MeshInstances.Materials[i].Specular[1] << ";" <<MeshInstances.Materials[i].Specular[2] << "\n";
					std::cout << "\t\t" << MeshInstances.Materials[i].Shininess << "\n";
					std::cout << "\t\tModel matrix:\n";
					for(int j=0;j < 4;j++)
					{
						std::cout << "\t\t";
						for(int k=0;k < 4;k++)
						{
							std::cout << MeshInstances.MvMatrices[i][j][k] << ";";
						}
						std::cout << "\n";
					}
					std::cout << "\n";
				}
				std::cout << "\n";
			}
		}
		std::cout << std::endl;
	}
	// */
	//
	lr3DSceneCacher(lffGLResourceLoader &resource_loader,lffGL3DSceneCache &scene_cache)
		:ResourceLoader(resource_loader),SceneCache(scene_cache)
	{}
	//
	virtual ~lr3DSceneCacher() override {}
};

#endif // LFF_GL_3D_SCENE_CACHE_H
